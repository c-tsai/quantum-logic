#include "synthesizer.h"
#include <stdexcept>
//#include <iostream>
//#include "table.h"


// Using ways: ./a.out  spec.pla  alg_type  if_controled_min  direction  cost_type  
int main(int argc, char** argv) {
	Table* t = pla_reader((argv[1]));
	/*for (int i = 0; i < argc; i++) {
		if (i == 2) { std::cout << std::stoi(argv[i]) << ' '; }
		std::cout << ',' << (argv[i]) << ',' <<  std::endl;
	}
	std::cout << t->len() << std::endl;*/
	QCSynthesizer* qcs = new QCSynthesizer(t, t->len());
	if ((argv[3])[0] == 'T' || (argv[3])[0] == 't') {
		//std::cout << "wwwwwwwwwwwwwwwwww0" << std::endl;
		qcs->algorithm_selector(std::stoi((argv[2])), 0, true, (argv[4])[0], (argv[5])[0]);}
	else if ((argv[3])[0] == 'F' || (argv[3])[0] == 'f') {
		//std::cout << "wwwwwwwwwwwwwwwwww1" << std::endl;
		qcs->algorithm_selector(std::stoi((argv[2])), 0, false, (argv[4])[0], (argv[5])[0]);
		//std::cout << std::stoi(argv[2]) << ' ' << (argv[4])[0] << ' ' << (argv[5])[0] << std::endl;
	}
	else { /*std::cout << "wwwwwwwwwwwwwwwwww2" << std::endl; */throw std::invalid_argument("the 3rd argument (indicating using control min or not) should be booleon"); }
	QCircuit* qc = qcs->output();
	std::cout << "length: " <<qc->cost('l') << "   qcost(NCV-111): " << qc->cost('q') << std::endl;
}