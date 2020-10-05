#include "table.h"
#include "gates.h"
#include "traverse_map.h"
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>


/*given order=-1;DFS=0; BFS=1; Dym =2; Dym_DFS=3*/

class QCSynthesizer {
public:
	QCSynthesizer(Table* t, int b_l, Table* t_b = 0) {
		table_f = t->new_copy();
		length = t->len(); 
		b_len = b_l;
		out_f = new QCircuit(); 
		out_b = new QCircuit();
		table_h = 0;
		
		if (t_b == 0) { table_b = 0;  update_table_b();}
		else { table_b= t_b->new_copy(); }
		order = new std::vector<size_t>;
		c_g = new Control_generator(b_len);
		t_map= new Map(b_len);
		prefered_cont= new std::unordered_map<size_t, int>;
		size_t point=1;
		for(int i=0; i<b_len; i++){
			prefered_cont->insert({point, 0}); point = point << 1;}
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
	void algorithm_selector(int alg, size_t* ord, bool cont_m, char direction, char c_typ);

	void order_alg(size_t* ord, bool permute, bool cont_m, char direction, char c_typ) {
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
	QCircuit* gate_syns(size_t i_b, size_t f_b, char typ, bool cont_m, char c_typ);
	QCircuit* gate_syns_simp(size_t i_b, size_t f_b);
	QCircuit* gate_syns_simp_cont(size_t i_b, size_t f_b);
	QCircuit* select_b_f(size_t targ, bool cont_m, char c_typ);
	QCircuit* BFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* Dym(Control_lines* candi, bool cont_m, char direction, char c_typ);
	QCircuit* DymDFS(Control_lines* candi, bool cont_m, char direction, char c_typ);
	void update_table_b();
	void update_table_f();
	void update_table_h();
	void traverse(size_t targ, bool no_care);
	void given_order_alg(size_t* ord, int len, bool cont_m, char direction, char c_typ);
	void dynamic_proto(int alg, bool cont_m, char direction, char c_typ);
	void permuting(int alg, size_t* ord, bool cont_m, char direction, char c_typ);
	std::vector<size_t>* prefered_cont_order(size_t targ);
	bool check(QCircuit* q_cir);




	Table* table_f;
	Table* table_b;
	Table* table_h;
	int length;
	int b_len;
	QCircuit* out_f;
	QCircuit* out_b;
	std::vector<size_t>* order;
	std::unordered_map<size_t, int>* prefered_cont;//for the gate_syns_simp_cont alg
	Control_generator* c_g;
	Map* t_map;
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
