#include <algorithm>
#include "table.h"
#include "gates.h"
//#include <iostream>//for testing

size_t TofoliGate::inf(size_t bit) {
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

std::string TofoliGate::text(){
	int point = 1;
	std::string res = "t";
	res = res + std::to_string(control_num() +1 );
	res = res + " x";
	for (int i =0; i<get_length(); i++){
		//std::cout << res << i << std::endl;
		if((point&get_bit2())!=0){res = res + std::to_string(i);break;}
		point = point <<1;}
	point =1;
	for (int i =0; i<get_length(); i++){
		///std::cout << res << i << std::endl;
		if((point&get_bit1())!=0){
			res = res + " x";
			res = res + std::to_string(i);}
		point = point <<1;}
	return res;
}

size_t SwapGate::inf(size_t bit) {
	//std::cout << bit<< 's' << endl;
	if (bit == -1) { return -1; }
	size_t b = (bit | get_bit1() |get_bit2());
	if ((bit&get_bit1()) == 0) { return b -= get_bit2(); }
	if ((bit&get_bit2()) == 0) { return b -= get_bit1(); }
	return b;
}

size_t QCircuit::inf(size_t bit) {
	size_t i = bit;
	for (auto it = q_vec->begin(); it != q_vec->end(); it++) {
		i = (*it)->inf(i);
	}
	return i;
}

void QCircuit::add(QCircuit* q_cir, char typ) {
	if (typ == 'f') {q_vec->insert(q_vec->end(), q_cir->q_vec->begin(), q_cir->q_vec->end());}
	else { q_vec->insert(q_vec->begin(), q_cir->q_vec->begin(), q_cir->q_vec->end()); }
	for (auto i = q_cir->dict_begin(); i != q_cir->dict_end(); i++) {
		//std::cout << i->first << std::endl;
		auto got = dict->find(i->first);
		if (got == dict->end()) { 
			//std::cout << "new pair: " << i->first << ' ' << i->second << std::endl;
			dict->insert({ {i->first,i->second} });
			//std::cout << "new pair: " << i->first << ' ' << i->second << std::endl;
		}
		else { (got->second) += i->second; }
	}
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
	std::vector<Gate*>* n_vec= new std::vector<Gate*>(*q_vec);
	std::unordered_map<int, int>* n_dict=new std::unordered_map<int, int>(*dict);
	QCircuit* res = new QCircuit(n_vec, n_dict);
	return res;
}


std::string QCircuit::text(){
	std::string res = ".version 2.0\n.numvars ";
	int len = (*(q_vec->begin()))->get_length();
	res = res + std::to_string(len);
	res = res + "\n.variables";
	for (int i =0; i!= len; i++) {
		res = res + " x";
		res = res + std::to_string(i);
	}
	res = res + "\n.inputs";
	for (int i =0; i!= len; i++) {
		res = res + " x";
		res = res + std::to_string(i);
	}
	res = res + "\n.outputs";
	for (int i =0; i!= len; i++) {
		res = res + " y";
		res = res + std::to_string(i);
	}
	res = res + "\n.constants ";
	for (int i =0; i!= len; i++) {
		res = res + "-";
	}
	res = res + "\n.garbage ";
	for (int i =0; i!= len; i++) {
		res = res + "-";
	}
	res = res + "\n.begin\n";


	for (auto i = q_vec->begin(); i != q_vec->end(); i++){
		res = res + (*i)->text();
		res = res + "\n";}
	return res;


	res = res + "\n.end\n";
}
