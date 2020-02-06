#include <algorithm>
#include "table.h"
#include "gates.h"


int TofoliGate::inf(int bit) {
	//std::cout << bit<< 't' << endl;
	if (bit == -1) { return -1; }
	if ((bit&get_bit1()) == get_bit1()) { return bit^get_bit2(); }
	return bit;
}

int TofoliGate::control_num() {
	if (cont_n == -1) {
		cont_n = Hamming_Dist(get_bit1(), 0, get_length());}
	//std::cout << cont_n;
	return cont_n;
}

int SwapGate::inf(int bit) {
	//std::cout << bit<< 's' << endl;
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
		if (got == dict->end()) { dict->insert(std::pair<int,int>(i->first,i->second)); }
		else { (got->second) += i->second; }
	}
	delete q_cir;
}

int QCircuit::cost(char c_typ) {
	if (c_typ == 'l') { return size(); }
	else {
		int table[10] = { 1,1,5,13,29,61,125,253,509,1021 };
		int result = 0;
		for (auto i = dict_begin(); i != dict_end(); i++) {
			if ((i->first) < 10) { result += (table[i->first] * (i->second)); }
			else { result += (((1 << ((i->first) + 1)) - 3)*(i->second)); }
		}return result;
	}
}

QCircuit* QCircuit::reverse() {
	std::vector<Gate*>* q_rev= new std::vector<Gate*>(q_vec->rbegin(), q_vec->rend());
	QCircuit* q_p = new QCircuit(q_rev, dict);
	return q_p;
}

QCircuit* QCircuit::copy() {
	std::vector<Gate*>* n_vec= new std::vector<Gate*>(q_vec);
	std::unordered_map<int, int>* n_dict=new std::unordered_map<int, int>(dict);
	QCircuit* res = new QCircuit(n_vec, n_dict);
	return res;
}


