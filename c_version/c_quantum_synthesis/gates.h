#include <iostream>
#include <vector>
#include <algorithm>


class Gate {
public:
	Gate(int b1, int b2, int len) :length(len), bit1(b1), bit2(b2){}
	virtual int inf(int bit) { return -1; }
	int get_bit1() { return bit1; }
	int get_bit2() { return bit2; }
	int get_length() { return length; }
	virtual char bit1_symb() { return 0; }
	virtual char bit2_symb() { return 0; }
private:
	int length;
	int bit1;
	int bit2;
};

class TofoliGate: public Gate {
public:
	TofoliGate(int control, int invert, int len): Gate(control,invert,len) {}
	char bit1_symb() { return '.'; }
	char bit2_symb() { return '+'; }
};

class SwapGate : public Gate {
public:
	SwapGate(int swap1, int swap2, int len) : Gate(swap1, swap2, len) {}
	char bit1_symb() { return 's'; }
	char bit2_symb() { return 's'; }
};

class QCircuit {
public:
	//QCircuit(Gate* g) { q_vec.push_back(g); }
	QCircuit(vector<Gate*>* p) { q_vec = p; }
	int inf(int bit) { int i = bit; for (auto q : *q_vec) { i = q->inf(i); } return i; }
	int size() { return q_vec->size(); }
	void add(QCircuit* q_cir) { 
		q_vec->insert(q_vec->end(), q_cir->q_vec->begin(), q_cir->q_vec->end()); }
	int cost(int h_cost) { return size(); }
	vector<Gate*>* q_vec;
};

ostream &operator<<(ostream &os, Gate* g);
ostream &operator<<(ostream &os, QCircuit* c) { 
	for (auto q : *(c->q_vec)) { os << q << endl; }return os;}
 
/*
void main() {
	Gate* s = &SwapGate(1, 2, 3);
	Gate* t = &TofoliGate(1, 2, 3);
	vector<Gate*> t_vec, s_vec;
	t_vec.push_back(t); s_vec.push_back(s);
	QCircuit* c1 = &QCircuit(&t_vec);
	QCircuit* c2 = &QCircuit(&s_vec);
	c1->add(c2);
	cout << c1;
};
*/
