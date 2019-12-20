#include "table.h"
#include "gates.h"

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
private:
	QCircuit* gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ);
	QCircuit* gate_syns_simp(int i_b, int f_b, char typ);
	select_b_f()

	Table* table_f;
	Table* table_b;
	Table* table_h;
	int length;
	int b_len;
	QCircuit* out_f;
	QCircuit* out_b;
	//Control_lines_generator* c_g
};
