#include "synthesizer.h"


void QCSynthesizer::add(QCircuit* cir, char typ, Table* t_b = 0, Table* t_f = 0, Table* t_h = 0) {}
void QCSynthesizer::algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::order_alg(int* ord, bool permute, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::DFS_alg(bool permute, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::BFS_alg(bool permute, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::Dym_alg(bool permute, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::DymDFS_alg(bool permute, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::gate_syns(int i_b, int f_b, char typ, bool cont_m, char c_typ) {}
QCircuit* QCSynthesizer::gate_syns_simp(int i_b, int f_b, char typ) {}
QCircuit* QCSynthesizer::select_b_f(int targ, bool cont_m, char c_typ) {}
QCircuit* QCSynthesizer::BFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::DFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::Dym(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}
QCircuit* QCSynthesizer::DymDFS(std::unordered_set<int>* candi, bool cont_m, char direction, char c_typ) {}



void QCSynthesizer::update_table_b() {}
void QCSynthesizer::update_table_f() {}
void QCSynthesizer::update_table_h() {}
void QCSynthesizer::traverse(int targ) {}
void QCSynthesizer::given_order_alg(int* ord, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::dynamic_proto(QCircuit* (*f)(std::unordered_set<int>*, bool, char, char), bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::permuting(int alg, bool cont_m, char direction, char c_typ) {}
void QCSynthesizer::algorithm_selector(int alg, int* ord, bool cont_m, char direction, char c_typ) {}