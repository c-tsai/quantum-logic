#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


int TofoliGate::inf(int bit) {
	if (bit == -1) { return -1; }
	if (bit&get_bit1() == get_bit1()) { return bit^get_bit2(); }
	return bit;
}

int SwapGate::inf(int bit) {
	if (bit == -1) { return -1; }
	int b = bit | get_bit1() | get_bit2() - get_bit1() - get_bit2();
	if (bit&get_bit1() == get_bit1()) { return b += get_bit2(); }
	if (bit&get_bit2() == get_bit2()) { return b += get_bit1(); }
	return b;
}

QCircuit* QCircuit::reverse() {
	std::reverse(q_vec->begin(), q_vec->end()); vector<Gate*>* q_rev = q_vec;
	std::reverse(q_vec->begin(), q_vec->end());
	return &QCircuit(q_rev);
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