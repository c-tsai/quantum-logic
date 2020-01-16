#include "synthesizer.h"
#include "traverse_map.h"
#include <iostream>


void QCSynthesizer::add(QCircuit* cir, char typ, Table* t_b, Table* t_f, Table* t_h) {
	if (t_f != 0) { table_f = t_f->new_copy(); }
	if (t_b != 0) { table_b= t_b->new_copy(); }
	else if (typ == 'f') {
		Table* temp = new Table(length);
		for (auto i = table_f->begin(); i != table_f->end(); i++) {
			temp->set_value(i->first, cir->inf(i->second));
		}delete table_f; table_f = temp; update_table_b();
	}
	else {
		for (auto i = cir->vec_rbegin(); i != cir->vec_rend(); i++) {
			Table* temp = new Table(length);
			for (auto i = table_b->begin(); i != table_b->end(); i++) {
				temp->set_value(i->first, cir->inf(i->second));
			}delete table_b; table_b = temp;
		} update_table_f();
	}
	if (typ == 'f') { out_f->add(cir, typ); }
	else { out_b->add(cir, typ); }
	if (t_h != 0) { table_h = t_h->new_copy(); }
	else { update_table_h(); }
}
void QCSynthesizer::algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ) {
	switch (alg)
	{
	case(-1):given_order_alg(ord, length, cont_m, direction, c_typ); break;
	case(0):dynamic_proto(0, cont_m, direction, c_typ); break;
	case(1):dynamic_proto(1, cont_m, direction, c_typ); break;
	case(2):dynamic_proto(2, cont_m, direction, c_typ); break;
	case(3):dynamic_proto(3, cont_m, direction, c_typ); break;
	}
}
QCircuit* QCSynthesizer::gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ) {
	std::cout << typ;
	if (!cont_m) { QCircuit* c = gate_syns_simp(i_b, f_b); c->set_typ(typ); }/*to be continue*/
}
QCircuit* QCSynthesizer::gate_syns_simp(int i_b, int f_b) {
	QCircuit* res = new QCircuit();
	if (i_b == -1 | f_b == -1) { return res; }

	std::cout << i_b << ' ' << f_b  << std::endl;
	int diff_1 = (i_b ^ f_b) & i_b;
	int diff_0 = (i_b ^ f_b) & f_b;
	int point = 1;
	for (int i = 0; i != b_len; i++) {
		if ((diff_0 & point) != 0) {
			Gate* t = new TofoliGate(i_b, point, b_len); std::vector < Gate* > t_vec;
			t_vec.push_back(t);
			QCircuit* t_cir = new QCircuit(&t_vec);
			res->add(t_cir, 'f');
		}point = point << 1;
	}point = 1;
	for (int i = 0; i != b_len; i++) {
		if ((diff_1 & point) != 0) {
			Gate* t = new TofoliGate(f_b, point, b_len); std::vector < Gate* > t_vec;
			t_vec.push_back(t);
			QCircuit* t_cir = new QCircuit(&t_vec);
			res->add(t_cir, 'f');
		}point = point << 1;
	}
	return res;
}

QCircuit* QCSynthesizer::select_b_f(int targ, bool cont_m, char c_typ) {
	if (table_b->get_value(targ) == -1) {
		std::cout << "partial specify f";
		QCircuit* c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);
		c->set_typ('f'); return c;}
	else if (table_f->get_value(targ) == -1) {
		std::cout << "partial specify b";
		QCircuit* c = gate_syns(targ, table_b->get_value(targ), 'b', cont_m, c_typ);
		c->set_typ('b'); return c;
	}
	QCircuit* c_f = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);
	QCircuit* c_b = gate_syns(targ, table_b->get_value(targ), 'b', cont_m, c_typ);
	if (c_b->cost(c_typ) < c_f->cost(c_typ)) { 
		c_b->set_typ('b'); c_b->set_targ(targ); 
	//std:cout << "b " << targ << ' ' << table_f
		delete c_f; return c_b; }
	else { c_f->set_typ('f'); c_f->set_targ(targ); delete c_b; return c_f; }
}
QCircuit* QCSynthesizer::BFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {
	int weight = 100000; int targ = -1;
	for (auto i = candi->begin(); i != candi->end(); i++) { 
		int w = Hamming_Dist((*i), 0, b_len);
		//std::cout << (*i) << " : " << w << ", ";
		if (w < weight) { targ = (*i); weight = w; }}
	QCircuit* c;
	if (direction == 'b') { c =select_b_f(targ, cont_m, c_typ);}
	else { c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);}
	c->set_targ(targ);
	return c;
}
QCircuit* QCSynthesizer::DFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {
	int targ = 1000000;
	for (auto i = candi->begin(); i != candi->end(); i++) {
		if ((*i) < targ) { targ = (*i); }}
	QCircuit* c;
	if (direction == 'b') { c =select_b_f(targ, cont_m, c_typ); }
	else { c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);  }
	c->set_targ(targ);
	return c;
}
QCircuit* QCSynthesizer::Dym(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {
	int cost = 100000000; QCircuit* c = 0; int h = 10000000;
	for (auto i = candi->begin(); i != candi->end(); i++) {
		QCircuit* c_temp = 0;
		if (direction == 'b') { c_temp = select_b_f((*i), cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(*i), (*i), 'f', cont_m, c_typ); }
		int cost_temp = c_temp->cost(c_typ);
		c_temp->set_targ((*i));
		QCSynthesizer* h_calc = new QCSynthesizer(table_f, b_len, table_b);
		h_calc->add(c_temp, c_temp->get_typ());
		int h_temp = h_calc->hamming_cost(); delete h_calc;
		if (cost_temp < cost) { cost = cost_temp; h = h_temp; delete c; c = c_temp; }
		else if (cost_temp == cost && h_temp <h) { cost = cost_temp; h = h_temp; delete c; c = c_temp; }
		else { delete c_temp; }
	}return c;
}
QCircuit* QCSynthesizer::DymDFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {
	int cost = 100000000; QCircuit* c = 0; int cont_num = 0; int h = 10000000;
	for (auto i = candi->begin(); i != candi->end(); i++) {
		QCircuit* c_temp = 0;
		int cont_num_t = Hamming_Dist((*i), 0, b_len);
		if (cont_num_t < cont_num) { continue; }
		if (direction == 'b') { c_temp = select_b_f((*i), cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(*i), (*i), 'f', cont_m, c_typ); }
		int cost_temp = c_temp->cost(c_typ);
		c_temp->set_targ((*i));
		QCSynthesizer* h_calc = new QCSynthesizer(table_f, b_len, table_b);
		h_calc->add(c_temp, c_temp->get_typ());
		int h_temp = h_calc->hamming_cost(); delete h_calc;
		if (cont_num_t > cont_num) { cont_num = cont_num_t; h = h_temp; cost = cost_temp; delete c; c = c_temp; }
		else if (cost_temp < cost) { cont_num = cont_num_t; h = h_temp; cost = cost_temp; delete c; c = c_temp; }
		else if (cost_temp == cost && h_temp < h) { cont_num = cont_num_t; h = h_temp; cost = cost_temp; delete c; c = c_temp; }
		else { delete c_temp; }
	}return c;
}



void QCSynthesizer::update_table_b() {
	if (table_b != 0) { delete table_b; }
	table_b = new Table(length);
	for (auto i = table_f->begin(); i != table_f->end(); i++) {
		//std::cout << i->second << ' ' << i->first << std::endl;
		table_b->set_value(i->second, i->first);}
}
void QCSynthesizer::update_table_f() {
	if (table_f != 0) { delete table_f; }
	table_f = new Table(length);
	for (auto i = table_b->begin(); i != table_b->end(); i++) {
		table_f->set_value(i->second, i->first);}
}
void QCSynthesizer::update_table_h() {
	if (table_h != 0) { delete table_h; }
	table_h = new Table(length);
	for (auto i = table_f->begin(); i != table_f->end(); i++) {
		table_h->set_value(i->first, Hamming_Dist(i->first,i->second,b_len));}
}
void QCSynthesizer::traverse(int targ) {
	//c_g.remove(targ);
	order->push_back(targ);
	table_b->traverse_pop(targ);
	table_f->traverse_pop(targ);
	table_h->traverse_pop(targ);
}
void QCSynthesizer::given_order_alg(int* ord, int len, bool cont_m, char direction, char c_typ) {
	for (int i = 0; i!=len; i++){
		std::cout << i << std::endl;
		QCircuit* c_temp = 0;
		if (direction == 'b') { c_temp = select_b_f(ord[i], cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(ord[i]), ord[i], 'f', cont_m, c_typ); }
		add(c_temp, c_temp->get_typ());
		traverse(ord[i]);
	}
}
void QCSynthesizer::dynamic_proto(int alg, bool cont_m, char direction, char c_typ) {
	QCircuit* c_temp = 0;
	if (direction == 'b') { c_temp = select_b_f(0, cont_m, c_typ); }
	else { c_temp = gate_syns(table_f->get_value(0), 0, 'f', cont_m, c_typ); }
	add(c_temp, c_temp->get_typ());
	traverse(0);
	Map* t_map = new Map(b_len);
	t_map->traverse(0);
	bool conti = true;
	//int count = 0;
	while (!(t_map->available()->empty()) && conti /*&& (count<8)*/) {
		QCircuit* cir = 0;
		//count++;
		switch (alg) {
		case(0):cir = DFS(t_map->available(), cont_m, direction, c_typ); break;
		case(1):cir = BFS(t_map->available(), cont_m, direction, c_typ); break;
		case(2):cir = Dym(t_map->available(), cont_m, direction, c_typ); break;
		case(3):cir = DymDFS(t_map->available(), cont_m, direction, c_typ); break;
		}
		//std::cout << cir->get_targ() << std::endl;
		add(cir, cir->get_typ());
		traverse(cir->get_targ());
		t_map->traverse(cir->get_targ());
		conti = false;
		/*
		std::cout << '{';
		for (auto i = t_map->available()->begin(); i != t_map->available()->end(); i++) { std::cout << (*i); }
		std::cout <<'}' << std::endl;
		*/
		for (auto i = table_f->begin(); i != table_f->end(); i++) { 
			if (i->second != i->first) { conti = true; break; }}
	}
	delete t_map;
}
void QCSynthesizer::permuting(int alg, int* order, bool cont_m, char direction, char c_typ) {}