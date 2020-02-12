#include "synthesizer.h"
//#include "traverse_map.h"
#include <limits>
#include <iostream>


void QCSynthesizer::add(QCircuit* cir, char typ, Table* t_b, Table* t_f, Table* t_h) {
	//std::cout << typ;
	if (t_f != 0) { table_f = t_f->new_copy(); }
	if (t_b != 0) { table_b= t_b->new_copy(); }
	else if (typ == 'f') {
		Table* temp = new Table(length);
		for (auto i = table_f->begin(); i != table_f->end(); i++) {
			//std::cout << i->first << i->second << cir->inf(i->second);
			temp->set_value(i->first, cir->inf(i->second));
		}delete table_f; table_f = temp; update_table_b();
	}
	else {
		for (auto q = cir->vec_rbegin(); q != cir->vec_rend(); q++) {
			Table* temp = new Table(length);
			for (auto i = table_b->begin(); i != table_b->end(); i++) {
				temp->set_value(i->first, (*q)->inf(i->second));
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
	//std::cout << typ<< " target_bit " << f_b << std::endl;
	if (!cont_m) { QCircuit* c = gate_syns_simp(i_b, f_b); c->set_typ(typ); return c; }
	//std::cout << "starting" << std::endl; 
	QCircuit* res = new QCircuit(); int b = i_b;
	if ((i_b == -1) || (f_b == -1)) { return res; }
	//std::cout << "starting" << std::endl;
	while (b != f_b) {
		int diff = (b ^ f_b); int point = 1; int cost_q = INT16_MAX; int cost_h = INT16_MAX;
		//std::cout << "current bit: " << b << " target bit: " << f_b << " diff: " << diff << std::endl;
		QCircuit* res_sub = 0;
		for (int i = 0; i < b_len; i++) {
			//std::cout << point << "----------" << std::endl;
			if ((diff & point) != 0) {
				//std::cout << "start a none zero point" << std::endl;
				std::vector<int>* lib = c_g->best_clines(b, point);
				for (auto it = lib->begin(); it != lib->end(); it++) {
					//std::cout << *it << std::endl;
					Gate* q = new TofoliGate(*it, point, b_len);
					std::vector<Gate*>* vec = new std::vector<Gate*>;
					vec->push_back(q);
					QCircuit* one_cir = new QCircuit(vec);
					QCSynthesizer* syns_temp = new QCSynthesizer(table_f, b_len, table_b);
					QCircuit* cir_temp = res->copy();
					cir_temp->add(one_cir, 'f');
					syns_temp->add(cir_temp, typ);
					int t_h = syns_temp->hamming_cost();
					int t_q = cir_temp->cost(c_typ);
					if (t_h == 0) { res_sub = cir_temp; cost_q = t_q; cost_h = t_h; break; }
					else if (t_q < cost_q) {
						res_sub = cir_temp; cost_q = t_q; cost_h = t_h;
						if (t_q == 0) { break; }
					}
					else if (t_q == cost_q && t_h < cost_h) {
						res_sub = cir_temp; cost_q = t_q; cost_h = t_h;
						if (t_h == 0) { break; }
					}
					else { delete cir_temp; } delete syns_temp;
				}
			}if (cost_h == 0 or cost_q == 0) { break; } point = point << 1; //std::cout << "end a bit" << std::endl;
		} delete res;//std::cout << "result address" << '*' << std::endl;
		res = res_sub; b = res->inf(i_b);
	}/*std::cout << res << std::endl;*/ return res;
}
QCircuit* QCSynthesizer::gate_syns_simp(int i_b, int f_b) {
	QCircuit* res = new QCircuit();
	if (i_b == -1 | f_b == -1) { return res; }

	//std::cout << i_b << ' ' << f_b  << std::endl;
	//std::cout << table_f << std::endl;
	int diff_1 = (i_b ^ f_b) & i_b;
	int diff_0 = (i_b ^ f_b) & f_b;
	int point = 1;
	for (int i = 0; i != b_len; i++) {
		//std::cout << point << std::endl;
		if ((diff_0 & point) != 0) {
			Gate* t = new TofoliGate(i_b, point, b_len); std::vector < Gate* > t_vec;
			t_vec.push_back(t);
			QCircuit* t_cir = new QCircuit(&t_vec);
			res->add(t_cir, 'f');
		}point = point << 1;
	}point = 1;
	for (int i = 0; i != b_len; i++) {
		//std::cout << point << std::endl;
		if ((diff_1 & point) != 0) {
			Gate* t = new TofoliGate(f_b, point, b_len); std::vector < Gate* > t_vec;
			//std::cout << "generated gate" << std::endl;
			t_vec.push_back(t);
			QCircuit* t_cir = new QCircuit(&t_vec);
			//std::cout << "generated qcircuit" << std::endl;
			res->add(t_cir, 'f');
			//std::cout << "add circuit to existing result" << std::endl;
		}point = point << 1;
	}//std::cout << res << std::endl;
	return res;
}

QCircuit* QCSynthesizer::select_b_f(int targ, bool cont_m, char c_typ) {
	//std::cout << "table_b: " << table_b->get_value(targ) << " table_f: " << table_f->get_value(targ) << std::endl;
	if ((table_b->get_value(targ)) == -1) {
		//std::cout << "partial specify " << std::endl;
		QCircuit* c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);
		c->set_typ('f'); return c;}
	else if ((table_f->get_value(targ)) == -1) {
		//std::cout << "partial specify " << std::endl;
		QCircuit* c = gate_syns(targ, table_b->get_value(targ), 'b', cont_m, c_typ);
		c->set_typ('b'); return c;
	}
	QCircuit* c_f = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);
	QCircuit* c_b = gate_syns(targ, table_b->get_value(targ), 'b', cont_m, c_typ);
	if (c_b->cost(c_typ) < c_f->cost(c_typ)) { 
		c_b->set_typ('b'); c_b->set_targ(targ); 
	//std::cout << "b " << targ << ' ' << table_f;
		delete c_f; return c_b; }
	else { c_f->set_typ('f'); c_f->set_targ(targ); delete c_b; return c_f; }
}

QCircuit* QCSynthesizer::BFS(Control_lines* candi, bool cont_m, char direction, char c_typ) {
	int targ = *(candi->min_group()->begin());
	//std::cout << targ << std::endl;
	QCircuit* c;
	if (direction == 'b') { c =select_b_f(targ, cont_m, c_typ);}
	else { c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);}
	c->set_targ(targ);
	return c;
}
QCircuit* QCSynthesizer::DFS(Control_lines* candi, bool cont_m, char direction, char c_typ) {
	int targ = *(candi->max_group()->begin());
	QCircuit* c;
	if (direction == 'b') { c =select_b_f(targ, cont_m, c_typ); }
	else { c = gate_syns(table_f->get_value(targ), targ, 'f', cont_m, c_typ);  }
	c->set_targ(targ);
	return c;
}
QCircuit* QCSynthesizer::Dym(Control_lines* candi, bool cont_m, char direction, char c_typ) {
	int cost = INT16_MAX; QCircuit* c = 0; int h = INT16_MAX; 
	bool fin = false; int targ_u = -1;
	for (auto i = candi->begin(); i != candi->end(); i++) {
		int t = i.element();
		if (table_b->get_value(t) == -1 && table_f->get_value(t) == -1) { targ_u = t; continue; }
		QCircuit* c_temp = 0;
		if (direction == 'b') { c_temp = select_b_f((t), cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(t), (t), 'f', cont_m, c_typ); }
		int cost_temp = c_temp->cost(c_typ);
		c_temp->set_targ((t));
		QCSynthesizer* h_calc = new QCSynthesizer(table_f, b_len, table_b);
		h_calc->add(c_temp, c_temp->get_typ());
		int h_temp = h_calc->hamming_cost(); delete h_calc;

		if (fin && h != 0) { delete c_temp; continue; }
		if (!fin && h == 0) { cost = cost_temp; h = h_temp; delete c; c = c_temp; fin = true; }
		else if (cost_temp < cost) { 
			cost = cost_temp; h = h_temp; delete c; c = c_temp;
			if (cost == 0) { break; }}
		else if (
			cost_temp == cost && h_temp <h) { cost = cost_temp; h = h_temp; delete c; c = c_temp; 
			if (cost == 0) { break; }}
		else { delete c_temp; }
	}
	if (c == 0) {
		c = gate_syns(table_f->get_value(targ_u), targ_u, 'f', cont_m, c_typ);
		c->set_typ('f'); c->set_targ(targ_u);
	}return c;
}
QCircuit* QCSynthesizer::DymDFS(Control_lines* candi, bool cont_m, char direction, char c_typ) {
	int cost = INT16_MAX; QCircuit* c = 0; int h = INT16_MAX;
	bool fin = false; int targ_u = -1;
	//std::cout << '{' << std::endl;
	for (auto i = candi->max_group()->begin(); i != candi->max_group()->end(); i++) {
		int t = *i;
		//std::cout << t ;
		if (table_b->get_value(t) == -1 && table_f->get_value(t) == -1) {
			//std::cout << "--not specified terms" << std::endl;
			targ_u = t; continue; }
		QCircuit* c_temp = 0;
		if (direction == 'b') { c_temp = select_b_f((t), cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(t), (t), 'f', cont_m, c_typ); }
		int cost_temp = c_temp->cost(c_typ);
		c_temp->set_targ((t));
		QCSynthesizer* h_calc = new QCSynthesizer(table_f, b_len, table_b);
		h_calc->add(c_temp, c_temp->get_typ());
		int h_temp = h_calc->hamming_cost(); delete h_calc;

		if (fin && h != 0) {
			//std::cout << "--hamming!=0" << std::endl; 
			delete c_temp; continue; }
		if (!fin && h == 0) { 
			//std::cout << "--find a term with hamming cost=0" << std::endl; 
			cost = cost_temp; h = h_temp; delete c; c = c_temp; fin = true; }
		else if (cost_temp < cost) {
			//std::cout << "--find a term with smaller cost" << std::endl;
			cost = cost_temp; h = h_temp; delete c; c = c_temp;
			if (cost == 0) { break; }
		}
		else if (
			cost_temp == cost && h_temp < h) {
			//std::cout << "--fina a term with smaller hamming" << std::endl;
			cost = cost_temp; h = h_temp; delete c; c = c_temp;
			if (cost == 0) { break; }
		}
		else { 
			/*std::cout << "--a term nothing special " << cost_temp << ' ' << INT8_MAX <<std::endl;*/ delete c_temp; }
	}
	if (c == 0) {
		c = gate_syns(table_f->get_value(targ_u), targ_u, 'f', cont_m, c_typ);
		c->set_typ('f'); c->set_targ(targ_u);
	}//std::cout << '}' << std::endl; 
	return c;
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
	c_g->remove(targ);
	order->push_back(targ);
	table_b->traverse_pop(targ);
	table_f->traverse_pop(targ);
	table_h->traverse_pop(targ);
}
void QCSynthesizer::given_order_alg(int* ord, int len, bool cont_m, char direction, char c_typ) {
	for (int i = 0; i!=len; i++){
		//std::cout << i << std::endl;
		QCircuit* c_temp = 0;
		if (direction == 'b') { c_temp = select_b_f(ord[i], cont_m, c_typ); }
		else { c_temp = gate_syns(table_f->get_value(ord[i]), ord[i], 'f', cont_m, c_typ); }
		add(c_temp, c_temp->get_typ());
		traverse(ord[i]);
	}
}
void QCSynthesizer::dynamic_proto(int alg, bool cont_m, char direction, char c_typ) {
	QCircuit* c_temp = 0;
	//std::cout << "table_f: " << table_f << std::endl;
	//std::cout << "table_b: " << table_b << std::endl;
	//std::cout << "started" << std::endl;
	if (direction == 'b') { c_temp = select_b_f(0, cont_m, c_typ); }
	else { c_temp = gate_syns(table_f->get_value(0), 0, 'f', cont_m, c_typ); }
	//std::cout << c_temp->get_targ() << "------" << std::endl;
	//std::cout << c_temp << std::endl;
	//std::cout << "generated circuit" << std::endl;
	add(c_temp, c_temp->get_typ());
	delete c_temp;
	//std::cout << '1' << std::endl;
	traverse(0);
	Map* t_map = new Map(b_len);
	t_map->traverse(0, c_g);
	bool conti = true;
	//int count = 0;
	//std::cout << "table_f: " << table_f << std::endl;
	//std::cout << "table_b: " << table_b << std::endl;
	while (!(t_map->available()->empty()) && conti ) {
		QCircuit* cir = 0;
		//count++;
		switch (alg) {
		case(0):cir = DFS(t_map->available(), cont_m, direction, c_typ); break;
		case(1):cir = BFS(t_map->available(), cont_m, direction, c_typ); break;
		case(2):cir = Dym(t_map->available(), cont_m, direction, c_typ); break;
		case(3):cir = DymDFS(t_map->available(), cont_m, direction, c_typ); break;
		}
		//std::cout << cir->get_targ() << std::endl;
		//std::cout << cir << std::endl;
		//std::cout << "table_f: " << table_f << std::endl;
		//std::cout << "table_b: " << table_b << std::endl;
		add(cir, cir->get_typ());
		traverse(cir->get_targ());
		int targ = cir->get_targ();
		t_map->traverse(targ,c_g,!(table_b->get_value(targ)==-1 && table_f->get_value(targ) == -1));
		conti = false;
		delete cir;
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