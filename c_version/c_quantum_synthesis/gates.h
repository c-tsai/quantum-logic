#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>


//remember to check if dict could be implement into an array
class Gate {
public:
	Gate(size_t b1, size_t b2, int len) :length(len), bit1(b1), bit2(b2){}
	virtual size_t inf(size_t bit) { return -1; }
	size_t get_bit1() { return bit1; }
	size_t get_bit2() { return bit2; }
	int get_length() { return length; }
	virtual int control_num() { return 0; }
	char get_bit1_symb() { return bit1_symb(); }
	char get_bit2_symb() { return bit2_symb(); }
	virtual char bit1_symb() { return 0; }
	virtual char bit2_symb() { return 0; }
	virtual std::string text(){return "";}

	
private:
	int length;
	size_t bit1;
	size_t bit2;
};

class TofoliGate: public Gate {
public:
	TofoliGate(size_t control, size_t invert, int len) : Gate(control, invert, len) { cont_n = -1; }
	size_t inf(size_t bit) ;
	int control_num();
	virtual char bit1_symb() { return '.'; }
	virtual char bit2_symb() { return '+'; }
	std::string text() ;
private: 
	int cont_n;
};

class SwapGate : public Gate {
public:
	SwapGate(size_t swap1, size_t swap2, int len) : Gate(swap1, swap2, len) {}
	size_t inf(size_t bit);
	virtual char bit1_symb() { return 's'; }
	virtual char bit2_symb() { return 's'; }
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
	QCircuit* copy();
	std::vector<Gate*>::iterator vec_begin() { return q_vec->begin(); }
	std::vector<Gate*>::iterator vec_end() { return q_vec->end(); }
	std::vector<Gate*>::reverse_iterator vec_rbegin() { return q_vec->rbegin(); }
	std::vector<Gate*>::reverse_iterator vec_rend() { return q_vec->rend(); }
	std::unordered_map<int, int>::iterator dict_begin() { return dict->begin(); }
	std::unordered_map<int, int>::iterator dict_end() { return dict->end(); }
	size_t inf(size_t bit);
	size_t get_targ() { return targ; }
	int size() { return q_vec->size(); }
	int cost(char c_typ);
	void add(QCircuit* q_cir, char typ);
	void set_typ(char c) { typ = c; }
	void set_targ(size_t t) { targ = t; }
	char get_typ() { return typ; }
	std::string text();


private:
	std::vector<Gate*>* q_vec;
	std::unordered_map<int, int>* dict;
	char typ;
	size_t targ;

};

inline std::ostream &operator<<(std::ostream &os, Gate* g) {
	os << "--";
	size_t c = g->get_bit1(); size_t inv = g->get_bit2();
	//os << c << ' ' << inv << std::endl;
	for (int i = 0; i < g->get_length(); i++) {
		//g->get_bit1_symb();
		if (c % 2 == 1) {  os << ".--"; }
		else if (inv % 2 == 1) {  os << "+--"; }
		else { os << "---"; }
		//os << i << std::endl;
		c = c / 2; inv = inv / 2;
	}
	os << std::endl << "--";
	for (int i = 0; i < g->get_length(); i++) { os << "|--"; }
	return os;
}

inline std::ostream &operator<<(std::ostream &os, QCircuit* c) { 
	//int count =0;
	for (auto it = c->vec_begin(); it != c->vec_end(); it++)
		{//os << count << "***********"<< std::endl; count++ ;
		 os << (*it) << std::endl; }return os;}
 
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



