#include "control_lines.h"


void Control_lines::pop(int line, int b_num = -1) {}
void Control_lines::unioning(Control_lines* n, int b_num ) {}
void Control_lines::add(int line, int b_num) {}
void Control_lines::contain(int line, int b_num) {}
void Control_lines::copy() {}

void Control_lib::pop(int line, int b_num) {}
void Control_lib::add(int line, int b_num) {}
int Control_lib::get(int targ, int b_num ) {}

void Control_generator::remove(int targ);
void Control_generator::combine(std::unordered_set<int>* b_list, int targ, int num);
std::unordered_set<int>* Control_generator::best_clines(int bit1, int controled);