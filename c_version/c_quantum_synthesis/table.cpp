#include "table.h"
#include <fstream>


int Table::summ() {
	int result = 0;
	for (auto it = lib->begin(); it != lib->end(); it ++) {
		result += (it->second);}
	return result;
}



std::vector<int>* bit_list(int targ, int bit_len) {
	std::vector<int>* result= new std::vector<int>;
	int point = 1;
	for (int i = 0; i != bit_len; i++) {
		if ((point & targ) != 0) { result->push_back(point); }
		//std::cout << (point& targ);
		point = point << 1;
	}
	return result;
}
Table* pla_reader(const std::string file_name) {
	std::ifstream file;
	file.open(file_name);
	int b_len, b;
	file >> b_len >> b;
	Table* res_p = new Table(b_len);
	std::string in_s, out_s;
	while (file >> in_s >> out_s) {
		int in_int = std::stoi(in_s, 0, 2);
		int out_int = std::stoi(out_s, 0, 2);
		res_p->set_value(in_int, out_int);
	}
	return res_p;
}
int Hamming_Dist(int bit1, int bit2, int b_len) {
	int point = 1;
	int dist = 0;
	int targ = bit1 ^ bit2;
	for (int i = 0; i != b_len; i++) {
		if ((point & targ) != 0) { dist++; }
		point = point << 1;
		//std::cout << point;
	}
	return dist;
}

