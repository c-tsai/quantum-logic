#include "control_lines.h"
#include "table.h" //removed temporarily for testing 
#include <algorithm>
#include <iterator>


void Control_lines::pop(int line, int b_num) {
	int b = (b_num == -1) ? Hamming_Dist(line, 0, bit_len) : b_num;
	auto got = lib[b]->find(line);
	if (got != lib[b]->end()) { lib[b]->erase(got); }
	if (b == mn && lib[b]->empty()) { 
		for (int i = (mn + 1); i <= mx; i++) {
			if (!(lib[i]->empty())) { mn = i; break; }}}
	else if (b == mx && lib[b]->empty()) {
		for (int i = (mx -1); i >= mn; i--) {
			if (!(lib[i]->empty())) { mx = i; break; }
		}
	}
}
void Control_lines::unioning(std::set<int>* other, int b_num ) {
		std::set<int>* neww = new std::set<int>; int b = b_num;
		std::set_union(lib[b]->begin(), lib[b]->end(), 
			other->begin(), other->end(), std::inserter(*neww,neww->end()));
		if (b > mx) { mx = b; }
		if (b < mn) { mn = b; }
		delete lib[b]; lib[b] = neww;	
}
void Control_lines::add(int line, int b_num) {
	int b = (b_num == -1) ? Hamming_Dist(line, 0, bit_len) : b_num;
	//for (auto i = lib[b]->begin(); i != lib[b]->end(); i++) { std::cout << *i; }//for testing
	//std::cout << "if the set empty " << lib[b]->empty() << std::endl;
	lib[b]->insert(line);
	//std::cout << "inserted" << std::endl;
	if (lib[mn]->empty()) { mn = b; }
	if (lib[mx]->empty()) { mx = b; }
	if (b > mx) { mx = b; }
	if (b < mn) { mn = b; }
	//std::cout << line << " added, mx= " << mx << " mn= " << mn << std::endl;
}
bool Control_lines::contain(int line, int b_num) {
	int b = (b_num == -1) ? Hamming_Dist(line, 0, bit_len) : b_num;
	return ((lib[b]->find(line)) != (lib[b]->end()));
}




void Control_lib::pop(int line, int b_num) {
	int b = (b_num == -1) ? Hamming_Dist(line, 0, bit_len) : b_num;
	auto got = lib[b]->find(line);
	if (got != lib[b]->end()) { lib[b]->erase(got); }
	if (b == mn && lib[b]->empty()) {
		for (int i = (mn + 1); i <= mx; i++) {
			if (!(lib[i]->empty())) { mn = i; break; }
		}
	}
	else if (b == mx && lib[b]->empty()) {
		for (int i = (mx - 1); i >= mn; i--) {
			if (!(lib[i]->empty())) { mx = i; break; }
		}
	}
}
void Control_lib::add(int key, std::vector<int>* clines, int b_num) {
	std::pair<int, std::vector<int>*> targ(key, clines);
	int b = (b_num == -1) ? Hamming_Dist(key, 0, bit_len) : b_num;
	//std::cout << "inserted" << std::endl;
	lib[b]->insert(targ);
	if (lib[mn]->empty()) { mn = b; }
	if (lib[mx]->empty()) { mx = b; }
	if (b > mx) { mx = b; }
	if (b < mn) { mn = b; }
	//std::cout << key << " added, mx= " << mx << " mn= " << mn << std::endl;
}
std::vector<int>* Control_lib::get(int targ, int b_num ) {
	int b = (b_num == -1) ? Hamming_Dist(targ, 0, bit_len) : b_num;
	return lib[b]->find(targ)->second;
}
bool Control_lib::contain(int line, int b_num) {
	int b = (b_num == -1) ? Hamming_Dist(line, 0, bit_len) : b_num;
	return ((lib[b]->find(line)) != (lib[b]->end()));
}



void Control_generator::remove(int targ) {
	int b_num = Hamming_Dist(targ, 0, bit_len);
	unable->add(targ, b_num);
	each_num[b_num]++;
	if (b_num == smllst_b && each_num[b_num] == allowed_num) {
		smllst_b++;
		allowed_num *= (bit_len - smllst_b + 1);
		allowed_num = allowed_num / smllst_b;
	}
}
void Control_generator::combine(std::vector<int>* b_list, int targ, int num) {
	//std::cout << "combine with num=" << num << " targ=" << targ << " b_list size=" << b_list->size() << std::endl;
	if (num == 0) {
		//std::cout << "detect num=0" << std::endl;
		std::vector<int>* zero = new std::vector<int>; 
		zero->push_back(0); lib->add(targ, zero, 0);
	}
	if (b_list->size() == num) {
		std::vector<int>* t_list = new std::vector<int>;
		t_list->push_back(targ); lib->add(targ, t_list, num); 
	}
	if (lib->contain(targ, num)) { /*std::cout << "must be returning" << std::endl;*/ return; }
	int b = b_list->back();
	std::vector<int>* sub1 = new std::vector<int>(*b_list);
	sub1->pop_back();
	std::vector<int>* sub2 = new std::vector<int>(*sub1);
	combine(sub1, targ - b, num - 1);
	combine(sub2, targ - b, num);
	delete sub1, sub2;
	std::vector<int>* res = new std::vector<int>(*(lib->get(targ-b,num)));
	for (auto i = lib->get(targ - b, num - 1)->begin(); i != lib->get(targ - b, num - 1)->end(); i++) {
		res->push_back((*i) + b);
		//std::cout << "adding " << ((*i) + b);
	}lib->add(targ, res, num);
}
std::vector<int>* Control_generator::best_clines(int bit1, int controled) {
	int aim = (bit1 | controled) - controled;
	//std::cout << "aim= " << aim << std::endl;
	std::vector<int>* res = new std::vector<int>;
	if (aim == 0) {
		//std::cout << "aim = 0" << std::endl;
		if (!(unable->contain(0, 0))) { res->push_back(0); }
		return res;
	}
	std::vector<int>* list = bit_list(aim, bit_len);
	int b_num = list->size();
	for (int i = smllst_b; i <= b_num; i++) {
		//std::cout << "b_num for now: " << b_num << std::endl;
		combine(list, aim, i);
		//for (auto iter = lib->begin(); iter != lib->end(); iter++) { std::cout << iter.first(); } //for testing
		//std::cout << std::endl;
		for (auto iter = lib->get(aim, i)->begin(); iter != lib->get(aim, i)->end(); iter++) {
			if (!(unable->contain(*iter, i))) { res->push_back(*iter); }
		}if (!(res->empty())) { delete list; return res; }
	}delete list; return res;
}