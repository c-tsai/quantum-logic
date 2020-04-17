#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>



class Table {
public:
	//int length;
	Table(int l) {
		length = l;
		lib = new std::unordered_map<long int, long int>;
	}
	~Table() { delete lib; }
	long int get_value(int i) {
		//std::cout << i << "--------" << std::endl;
		//for (auto it = lib->begin(); it != lib->end(); it++) { std::cout << it->first <<' ' <<it->second << std::endl; }
		std::unordered_map<long int, long int>::const_iterator got = lib->find(i);
		//return 0;
		if (got == lib->end()) { return -1; }
		else { return got->second; }
	}
	void set_value(long int b1, long int b2) {
		//std::cout << b1 << b2 << std::endl;
		lib->insert(std::pair<long int, long int>(b1, b2));
	}
	long int summ();
	int len() { return length; }
	Table* new_copy() {
		Table* t = new Table(length);
		for (auto i = begin(); i != end(); i++) { t->set_value(i->first, i->second); }
		return t;
	}
	std::unordered_map<long int, long int>::iterator begin() { return lib->begin(); }
	std::unordered_map<long int, long int>::iterator end() { return lib->end(); }
	void traverse_pop(long int targ) { lib->erase(targ); }

private:
	int length;
	std::unordered_map<long int, long int>* lib;


};

std::vector<long int>* bit_list(long int targ, int bit_len);
Table* pla_reader(std::string file_name);
int Hamming_Dist(long int bit1, long int bit2, int b_len);
inline std::ostream& operator<<(std::ostream& os, Table* t) {
	os << '{';
	for (auto it = t->begin(); it != t->end(); it++) {
		os << it->first << ':' << it->second << ',';
	}
	os << '}';
	return os;
}


/*
int main(){
	Table* t = pla_reader("add6.pla");
	std::cout << (*t);
}
*/
