#include "synthesizer.h"
#include <stdexcept>
#include <ctime>
//#include <iostream>
//#include "table.h"


// Using ways: ./a.exe  spec.pla  alg_type  if_controled_min  direction  cost_type  
// Alg Type:      given order=-1;DFS=0; BFS=1; Dym =2; Dym_DFS=3

int main(int argc, char** argv) {
	Table* t = pla_reader((argv[1]));
	/*for (int i = 0; i < argc; i++) {
		if (i == 2) { std::cout << std::stoi(argv[i]) << ' '; }
		std::cout << ',' << (argv[i]) << ',' <<  std::endl;
	}
	std::cout << t->len() << std::endl;*/
	QCSynthesizer* qcs = new QCSynthesizer(t, t->len());
	std::clock_t time;
	time = std::clock();
	if ((argv[3])[0] == 'T' || (argv[3])[0] == 't') {
		//std::cout << "wwwwwwwwwwwwwwwwww0" << std::endl;
		qcs->algorithm_selector(std::stoi((argv[2])), 0, true, (argv[4])[0], (argv[5])[0]);}
	else if ((argv[3])[0] == 'F' || (argv[3])[0] == 'f') {
		//std::cout << "wwwwwwwwwwwwwwwwww1" << std::endl;
		qcs->algorithm_selector(std::stoi((argv[2])), 0, false, (argv[4])[0], (argv[5])[0]);
		//std::cout << std::stoi(argv[2]) << ' ' << (argv[4])[0] << ' ' << (argv[5])[0] << std::endl;
	}
	else { std::cout << "the 3rd argument (indicating using control min or not) should be booleon" << std::endl; throw std::invalid_argument("the 3rd argument (indicating using control min or not) should be booleon"); }
	time = std::clock() - time;
	QCircuit* qc = qcs->output();
	bool right_res = true;
	int c = 0;
	//std::cout << "complete" << std::endl;
	for (auto i = t->begin(); i != t->end(); i++) { 
		if (qc->inf(i->first) != i->second) {
			std::cout << qc << std::endl;
			std::cout << c << " wrong result( need " << i->second << " for " << i->first << " but got " << qc->inf(i->first) << " instead)"; 
			right_res = false;  break;
		}
		else { /*std::cout << i->first << " checked " << std::endl;*/ }
		c++;
	}
	if (right_res) {
		std::cout << "length: " << qc->cost('l') << "   qcost(NCV-111): " << qc->cost('q') << "    time: " << ((float)time / CLOCKS_PER_SEC) << " (sec)" << std::endl;
	}
}