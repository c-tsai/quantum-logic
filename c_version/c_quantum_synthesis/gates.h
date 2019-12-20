#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;


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
	QCircuit(vector<Gate*>* p=0, unordered_map<int, int>* d=0) { 
		if (p != 0) { q_vec = p; } else { q_vec = new vector<Gate*>; }
		if (d != 0) { dict = d; }
		else {
			dict = new unordered_map<int, int>;
			for (auto i = vec_begin(); i != vec_end(); i++) {
				int id = (*i)->control_num(); auto got = dict->find(id);
				if (got == dict_end()) { (*dict)[id] = 1; }
				else { got->second++; }
			}
		}

	}
	~QCircuit() { delete q_vec, dict; }
	QCircuit* reverse();
	vector<Gate*>::iterator vec_begin() { return q_vec->begin(); }
	vector<Gate*>::iterator vec_end() { return q_vec->end(); }
	unordered_map<int, int>::iterator dict_begin() { return dict->begin(); }
	unordered_map<int, int>::iterator dict_end() { return dict->end(); }
	int inf(int bit);
	int size() { return q_vec->size(); }
	void add(QCircuit* q_cir, char typ);
	int cost(char c_typ);

private:
	vector<Gate*>* q_vec;
	unordered_map<int, int>* dict;
	char typ

};

ostream &operator<<(ostream &os, Gate* g);
ostream &operator<<(ostream &os, QCircuit* c) { 
	for (auto it = c->vec_begin(); it != c->vec_end(); it++)
		{ os << (*it) << endl; }return os;}
 

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



