#include "table.h"
#include "gates.h"
#include "traverse_map.h"
#include <stdexcept>
#include <unordered_set>


/*given order=-1;DFS=0; BFS=1; Dym =2; Dym_DFS=3*/

class QCSynthesizer {
public:
	QCSynthesizer(Table* t, int b_l, Table* t_b = 0) {
		table_f = t->new_copy();
		length = t->len(); b_len = b_l;
		out_f = new QCircuit(); out_b = new QCircuit();
		table_h = 0;
		
		if (t_b == 0) { table_b = 0;  update_table_b();}
		else { table_b= t_b->new_copy(); }
		order = new std::vector<int>;
		c_g = new Control_generator(b_len);
	}
	~QCSynthesizer() {
		delete table_f, table_b, table_h;
		delete out_f, out_b;
		delete order;
		delete c_g;
	}
	int hamming_cost() {
		if (table_h == 0) { update_table_h(); }
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
		else { given_order_alg(ord, length, cont_m, direction, c_typ); }
	}
	void DFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(0, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(0, cont_m, direction, c_typ); }
	}
	void BFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(1, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(1, cont_m, direction, c_typ); }
	}
	void Dym_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(2, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(2, cont_m, direction, c_typ); }
	}
	void DymDFS_alg(bool permute, bool cont_m, char direction, char c_typ) {
		if (permute) { permuting(3, 0, cont_m, direction, c_typ); }
		else { dynamic_proto(3, cont_m, direction, c_typ); }
	}

private:
	QCircuit* gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ);
	QCircuit* gate_syns_simp(int i_b, int f_b);
	QCircuit* select_b_f(int targ, bool cont_m, char c_typ);
	QCircuit* BFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* Dym(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DymDFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	void update_table_b();
	void update_table_f();
	void update_table_h();
	void traverse(int targ);
	void given_order_alg(int* ord, int len, bool cont_m, char direction, char c_typ);
	void dynamic_proto(int alg, bool cont_m, char direction, char c_typ);
	void permuting(int alg, int* ord, bool cont_m, char direction, char c_typ);



	Table* table_f;
	Table* table_b;
	Table* table_h;
	int length;
	int b_len;
	QCircuit* out_f;
	QCircuit* out_b;
	std::vector<int>* order;
	Control_generator* c_g;
};
/*
int main() {
	Table* t = new Table(8);
	int array[5] = {1,6,5,7,2};
	int order[8] = { 0,1,2,3,4,5,6,7 };
	for (int i = 0; i != 8; i++) { std::cout << array[i]; t->set_value(i, array[i]); }
	//for (auto i = t->begin(); i != t->end(); i++) { std::cout <<std::endl << i->first << ' ' << i->second << std::endl; }
	QCSynthesizer* q = new QCSynthesizer(t, 3);
	q->algorithm_selector(3, 0, false, 'b', 'q');
	QCircuit* qc = q->output();
	for (int i = 0; i < 5; i++) {
		if (qc->inf(i) != array[i]) { throw std::invalid_argument("wrong result!!"); }
	}
}
*/
