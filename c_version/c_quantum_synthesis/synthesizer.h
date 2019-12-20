#include "table.h"
#include "gates.h"
#include <unordered_set>


/*given order=-1;DFS=0; BFS=1; Dym =2; Dym_DFS=3*/

class QCSynthesizer {
public:
	QCSynthesizer(Table t, int b_l, Table t_b = 0) {
		table_f = &t;
		length = t.len(); b_len = b_l;
		out_f = new QCircuit(); out_b = new QCircuit();
		table_h = 0;
		if (t_b == 0) { update_table_b(); }
		else { table_b = &t_b; }
		//c_g = Control_line_generator(b_len)
	}
	~QCSynthesizer() {
		delete table_f, table_b, table_h;
		delete out_f, out_b;
		//delete c_g
	}
	int hamming_cost() {
		if (table_h == 0) { update_table_h; }
		return table_h->summ();
	}
	QCircuit* output() { 
		QCircuit* res = out_b->reverse(); res->add(out_f->reverse(), 'f');
		return res;
	}
	void add(QCircuit* cir, char typ, Table* t_b=0, Table* t_f=0, Table* t_h=0);
	void algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ);

	void order_alg(int* ord, bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(-1, ord, cont_m, direction, c_typ); }
		else { given_order_alg(ord, cont_m, direction, c_typ); }
	}
	void DFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(0, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(DFS, cont_m, direction, c_typ); }
	}
	void BFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(1, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(BFS, cont_m, direction, c_typ); }
	}
	void Dym_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(2, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(Dym, cont_m, direction, c_typ); }
	}
	void DymDFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(3, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(DymDFS, cont_m, direction, c_typ); }
	}

private:
	QCircuit* gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ);
	QCircuit* gate_syns_simp(int i_b, int f_b, char typ);
	QCircuit* select_b_f(int targ, bool cont_m, char c_typ);
	QCircuit* BFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ);
	QCircuit* Dym(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DymDFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ);
	void update_table_b();
	void update_table_f();
	void update_table_h();
	void traverse(int targ);
	void given_order_alg(int* ord, bool cont_m, char direction, char c_typ);
	void dynamic_proto(QCircuit* (*f)(std::unordered_set<int>*, bool, char, char), bool cont_m, char direction, char c_typ);
	void permuting(int alg, int* ord, bool cont_m, char direction, char c_typ);
	void algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ);



	Table* table_f;
	Table* table_b;
	Table* table_h;
	int length;
	int b_len;
	QCircuit* out_f;
	QCircuit* out_b;
	//Control_lines_generator* c_g
};
