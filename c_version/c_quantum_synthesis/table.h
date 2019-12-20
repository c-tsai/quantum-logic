#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>



class Table {
public:
	//int length;
	Table(int l) { length = l; 
		lib = new std::unordered_map<int, int>; }
	~Table() { delete lib; }
	const int operator [] (int) const;
	int& operator [] (int i) { return (*lib)[i]; }
	void set_value(int b1, int b2) { (*lib)[b1] = b2; }
	int summ();
	int len() { return length; }
	std::unordered_map<int,int>::iterator begin() { return lib->begin(); }
	std::unordered_map<int, int>::iterator end() { return lib->end(); }

private:
	int length;
	std::unordered_map<int,int>* lib;


};

std::vector<int>* bit_list(int targ, int bit_len);
Table* pla_reader(std::string file_name);
int Hamming_Dist(int bit1, int bit2, int b_len);
std::ostream& operator<<(std::ostream& os, Table t);


/*
int main(){
	Table* t = pla_reader("add6.pla");
	std::cout << (*t);
}
*/
