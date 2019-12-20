#include "synthesizer.h"


void QCSynthesizer::add(QCircuit* cir, char typ, Table* t_b = 0, Table* t_f = 0, Table* t_h = 0) {
	if (t_f != 0) { Table t_n = *t_f; table_f = &t_n; }
	if (t_b != 0) { Table t_n = *t_f; table_b = &t_n; }
	else if (typ == 'f') {
		Table* temp = new Table();
		for (auto i = table_f->begin(); i != table_f->end(); i++) {
			temp->set_value(i->first, cir->inf(i->second));
		}delete table_f; table_f = temp; update_table_b();
	}
	else {
		for (auto i = cir->vec_rbegin(); i != cir->vec_rend(); i++) {
			Table* temp = new Table();
			for (auto i = table_b->begin(); i != table_b->end(); i++) {
				temp->set_value(i->first, cir->inf(i->second));
			}delete table_b; table_b = temp;
		} update_table_f();
	}
	if (typ == 'f') { out_f->add(cir, typ); }
	else { out_b->add(cir, typ); }
	if (t_h != 0) { Table t_n = *t_h; table_h = &t_n; }
	else { update_table_h(); }
}
void QCSynthesizer::algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ) {
	switch (alg)
	{case(-1):given_order_alg(ord, cont_m, direction, c_typ);
	case(0):dynamic_proto(DFS, cont_m, direction, c_typ);
	case(1):dynamic_proto(BFS, cont_m, direction, c_typ);
	case(2):dynamic_proto(Dym, cont_m, direction, c_typ);
	case(3):dynamic_proto(DymDFS, cont_m, direction, c_typ);
	}
}
QCircuit* QCSynthesizer::gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ) {}
QCircuit* QCSynthesizer::gate_syns_simp(int i_b, int f_b, char typ) {}
QCircuit* QCSynthesizer::select_b_f(int targ, bool cont_m, char c_typ) {}
QCircuit* QCSynthesizer::BFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::DFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::Dym(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::DymDFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}



void QCSynthesizer::update_table_b() {}
void QCSynthesizer::update_table_f() {}
void QCSynthesizer::update_table_h() {}
void QCSynthesizer::traverse(int targ) {}
void QCSynthesizer::given_order_alg(int* ord, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::dynamic_proto(QCircuit* (*f)(std::unordered_set<int>*, bool, char, char), bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::permuting(int alg, int* order, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ) {}