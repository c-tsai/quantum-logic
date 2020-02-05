#include <vector>
#include <unordered_map>
#include <iostream>


//remember to check if dict could be implement into an array
class Gate {
public:
	Gate(int b1, int b2, int len) :length(len), bit1(b1), bit2(b2){}
	virtual int inf(int bit) { return -1; }
	int get_bit1() { return bit1; }
	int get_bit2() { return bit2; }
	int get_length() { return length; }
	virtual int control_num() { return 0; }
	virtual char bit1_symb() { return 0; }
	virtual char bit2_symb() { return 0; }
	
private:
	int length;
	int bit1;
	int bit2;
};

class TofoliGate: public Gate {
public:
	TofoliGate(int control, int invert, int len) : Gate(control, invert, len) { cont_n = -1; }
	int inf(int bit) ;
	int control_num();
	char bit1_symb() { return '.'; }
	char bit2_symb() { return '+'; }
private: 
	int cont_n;
};

class SwapGate : public Gate {
public:
	SwapGate(int swap1, int swap2, int len) : Gate(swap1, swap2, len) {}
	int inf(int bit);
	char bit1_symb() { return 's'; }
	char bit2_symb() { return 's'; }
};

class QCircuit {
public:
	QCircuit(std::vector<Gate*>* p=0, std::unordered_map<int, int>* d=0) {
		if (p != 0) { q_vec = p; } else { q_vec = new std::vector<Gate*>; }
		if (d != 0) { dict = d; }
		else {
			dict = new std::unordered_map<int, int>;
			for (auto i = vec_begin(); i != vec_end(); i++) {
				int id = (*i)->control_num(); auto got = dict->find(id);
				if (got == dict_end()) { dict->insert(std::pair<int,int>(id,1)); }
				else { got->second++; }
			}
		}
		typ = 'f';

	}
	~QCircuit() { delete q_vec, dict; }
	QCircuit* reverse();
	std::vector<Gate*>::iterator vec_begin() { return q_vec->begin(); }
	std::vector<Gate*>::iterator vec_end() { return q_vec->end(); }
	std::vector<Gate*>::reverse_iterator vec_rbegin() { return q_vec->rbegin(); }
	std::vector<Gate*>::reverse_iterator vec_rend() { return q_vec->rend(); }
	std::unordered_map<int, int>::iterator dict_begin() { return dict->begin(); }
	std::unordered_map<int, int>::iterator dict_end() { return dict->end(); }
	int inf(int bit);
	int size() { return q_vec->size(); }
	void add(QCircuit* q_cir, char typ);
	void set_typ(char c) { typ = c; }
	void set_targ(int t) { targ = t; }
	char get_typ() { return typ; }
	int get_targ() { return targ; }
	int cost(char c_typ);

private:
	std::vector<Gate*>* q_vec;
	std::unordered_map<int, int>* dict;
	char typ;
	int targ;

};

inline std::ostream &operator<<(std::ostream &os, Gate* g) {
	os << "--";
	int c = g->get_bit1(); int inv = g->get_bit2();
	for (int i = 0; i < g->get_length(); i++) {
		if (c % 2 == 1) { os << g->bit1_symb() << "--"; }
		else if (inv % 2 == 1) { os << g->bit2_symb() << "--"; }
		else { os << "---"; }
		c = c / 2; inv = inv / 2;
	}
	os << std::endl << "--";
	for (int i = 0; i < g->get_length(); i++) { os << "|--"; }
	return os;
}

inline std::ostream &operator<<(std::ostream &os, QCircuit* c) { 
	for (auto it = c->vec_begin(); it != c->vec_end(); it++)
		{ os << (*it) << std::endl; }return os;}
 
/*
int main() {
	Gate* s =  new SwapGate(1, 2, 3);
	Gate* t = new TofoliGate(1, 2, 3);
	vector<Gate*> t_vec, s_vec;
	t_vec.push_back(t); s_vec.push_back(s);
	QCircuit* c1 = new QCircuit(&t_vec);
	QCircuit* c2 = new QCircuit(&s_vec);
	c1->add(c2, 'f');
	QCircuit* c3 = c1->reverse();
	cout << c3->cost('q');
};
*/



