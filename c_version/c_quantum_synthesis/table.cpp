#include "table.h"
#include <fstream>


long int Table::summ() {
	long int result = 0;
	for (auto it = lib->begin(); it != lib->end(); it ++) {
		result += (it->second);}
	return result;
}



std::vector<long int>* bit_list(long int targ, int bit_len) {
	std::vector<long int>* result= new std::vector<long int>;
	long int point = 1;
	for (int i = 0; i != bit_len; i++) {
		if ((point & targ) != 0) { result->push_back(point); }
		//std::cout << (point& targ);
		point = point << 1;
	}
	return result;
}
Table* pla_reader(const std::string file_name) {
	std::ifstream file(file_name);
	int b_len, b;
	file >> b_len >> b;
	//std::cout << file.is_open() << std::endl;
	Table* res_p = new Table(b_len);
	std::string in_s, out_s;
	//std::cout << b_len << std::endl;
	while (file >> in_s >> out_s) {
		//std::cout << in_s << out_s << std::endl;
		long int in_int = std::stol(in_s, 0, 2);
		long int out_int = std::stol(out_s, 0, 2);
		res_p->set_value(in_int, out_int);
		//std::cout << in_s << out_s << std::endl;
	}
        file.close();
	return res_p;
}
int Hamming_Dist(long int bit1, long int bit2, int b_len) {
	long int point = 1;
	int dist = 0;
	long int targ = bit1 ^ bit2;
	for (int i = 0; i != b_len; i++) {
		if ((point & targ) != 0) { dist++; }
		point = point << 1;
		//std::cout << point;
	}
	return dist;
}

