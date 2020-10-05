#include "table.h"
#include <fstream>


size_t Table::summ() {
	size_t result = 0;
	for (auto it = lib->begin(); it != lib->end(); it ++) {
		result += (it->second);}
	return result;
}



std::vector<size_t>* bit_list(size_t targ, int bit_len) {
	std::vector<size_t>* result= new std::vector<size_t>;
	size_t point = 1;
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
		size_t in_int = 0;
		size_t out_int = 0;
		size_t point = 1;
		for (int i= b_len-1; i >= 0 ; i--){
			if(in_s[i]== '0') in_int += point;
			if(out_s[i]== '0') out_int += point;
			point *= 2;
		}
		res_p->set_value(in_int, out_int);
		//std::cout << in_s << out_s << std::endl;
	}
        file.close();
	return res_p;
}
int Hamming_Dist(size_t bit1, size_t bit2, int b_len) {
	size_t point = 1;
	int dist = 0;
	size_t targ = bit1 ^ bit2;
	for (int i = 0; i != b_len; i++) {
		if ((point & targ) != 0) { dist++; }
		point = point << 1;
		//std::cout << point;
	}
	return dist;
}

