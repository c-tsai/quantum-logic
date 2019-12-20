#include <algorithm>
#include "gates.h"
#include "tool.h"


int TofoliGate::inf(int bit) {
	std::cout << bit<< 't' << endl;
	if (bit == -1) { return -1; }
	if ((bit&get_bit1()) == get_bit1()) { return bit^get_bit2(); }
	return bit;
}

int TofoliGate::control_num() {
	if (cont_n == -1) {
		cont_n = Hamming_dist(get_bit1(), 0, get_length());}
	//std::cout << cont_n;
	return cont_n;
}

int SwapGate::inf(int bit) {
	std::cout << bit<< 's' << endl;
	if (bit == -1) { return -1; }
	int b = (bit | get_bit1() |get_bit2());
	if ((bit&get_bit1()) == 0) { return b -= get_bit2(); }
	if ((bit&get_bit2()) == 0) { return b -= get_bit1(); }
	return b;
}

int QCircuit::inf(int bit) {
	int i = bit;
	for (auto it = q_vec->begin(); it != q_vec->end(); it++) {
		i = (*it)->inf(i);
	}
	return i;
}

void QCircuit::add(QCircuit* q_cir, char typ) {
	if (typ == 'f') {q_vec->insert(q_vec->end(), q_cir->q_vec->begin(), q_cir->q_vec->end());}
	else { q_vec->insert(q_vec->begin(), q_cir->q_vec->begin(), q_cir->q_vec->end()); }
	for (auto i = q_cir->dict_begin(); i != q_cir->dict_end(); i++) {
		auto got = dict->find(i->first);
		if (got == dict->end()) { (*dict)[i->first] = 1; }
		else { (got->second)++; }
	}
}

int QCircuit::cost(char c_typ) {
	if (c_typ == 'l') { return size(); }
}

QCircuit* QCircuit::reverse() {
	vector<Gate*> q_rev(q_vec->rbegin(), q_vec->rend());
	QCircuit* q_p = new QCircuit(&q_rev, dict);
	return q_p;
}

ostream &operator<<(ostream &os, Gate* g) {
	os << "--";
	int c = g->get_bit1(); int in = g->get_bit2();
	for (int i = 0; i < g->get_length(); i++) {
		if (c % 2 == 1) { os << g->bit1_symb() << "--"; }
		else if (in % 2 == 1) { os << g->bit2_symb() << "--"; }
		else { os << "---"; }
		c = c / 2; in = in / 2;
	}
	os << endl << "--";
	for (int i = 0; i < g->get_length(); i++) { os << "|--"; }
	return os;
}