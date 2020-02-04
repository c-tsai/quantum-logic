#include "control_lines.h"
#include "table.h"
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
void Control_lines::unioning(Control_lines* other, int b_num ) {
	if (b_num != -1) {
		std::set<int>* neww = new std::set<int>; int b = b_num;
		std::set_union(lib[b]->begin(), lib[b]->end(), 
			other->get_group(b)->begin(), other->get_group(b)->end(), std::inserter(*neww,neww->end()));
		if (b > mx) { mx = b; }
		if (b < mn) { mn = b; }
		delete lib[b]; lib[b] = neww;
	}
	else {
		for (int i = other->mn; i <= other->mx; i++) {
			std::set<int>* neww = new std::set<int>;
			std::set_union(lib[i]->begin(), lib[i]->end(),
				other->get_group(i)->begin(), other->get_group(i)->end(), std::inserter(*neww,neww->end()));
			delete lib[i]; lib[i] = neww;
		}
		if (other->mn < mn) { mn = other->mn; }
		if (other->mx > mx) { mx = other->mx; }
	}delete other;
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
bool Control_lines::contain(int line, int b_num) {}
Control_lines* Control_lines::copy() {}

void Control_lib::pop(int line, int b_num) {}
void Control_lib::add(int line, int b_num) {}
int Control_lib::get(int targ, int b_num ) {}

void Control_generator::remove(int targ) {}
void Control_generator::combine(std::set<int>* b_list, int targ, int num) {}
std::set<int>* Control_generator::best_clines(int bit1, int controled) {}