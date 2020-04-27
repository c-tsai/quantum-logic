#include "synthesizer.h"
#include <fstream>
#include <stdexcept>
#include <ctime>


/* Using ways: ./a.exe  spec.pla  alg_type  if_controled_min  direction  cost_type  
   
   alg_type          :given order=-1/ DFS=0/ BFS=1/ Dym =2/ Dym_DFS=3
   if_controled_min  :True/ False
   directiona        :bi-/ uni- direction
   cost_typ          :length/ qcost
*/
 

int main(int argc, char** argv) {
	Table* t = pla_reader((argv[1]));
	//std::cout << t << std::endl;
	switch (std::stoi((argv[2])))
	{
	case(0):std::cout << "DFS"; break;
	case(1):std::cout << "BFS"; break;
	case(2):std::cout << "Dym"; break;
	case(3):std::cout << "DymDFS"; break;
	}
	if ((argv[5])[0] == 'q') { std::cout << "_QCost"; }
	else { std::cout << "_length"; }
	/*for (int i = 0; i < argc; i++) {
		if (i == 2) { std::cout << std::stoi(argv[i]) << ' '; }
		std::cout << ',' << (argv[i]) << ',' <<  std::endl;
	}
	std::cout << t->len() << std::endl;*/
	QCSynthesizer* qcs = new QCSynthesizer(t, t->len());
	std::clock_t time;
	if ((argv[3])[0] == 'T' || (argv[3])[0] == 't') {
		std::cout << "_cont" << std::endl;
		time = std::clock();
		qcs->algorithm_selector(std::stoi((argv[2])), 0, true, (argv[4])[0], (argv[5])[0]);}
	else if ((argv[3])[0] == 'F' || (argv[3])[0] == 'f') {
		std::cout << std::endl;
		time = std::clock();
		qcs->algorithm_selector(std::stoi((argv[2])), 0, false, (argv[4])[0], (argv[5])[0]);
		//std::cout << std::stoi(argv[2]) << ' ' << (argv[4])[0] << ' ' << (argv[5])[0] << std::endl;
	}
	else { std::cout << "the 3rd argument (indicating using control min or not) should be booleon" << std::endl; throw std::invalid_argument("the 3rd argument (indicating using control min or not) should be booleon"); }
	time = std::clock() - time;

	QCircuit* qc = qcs->output();
	bool right_res = true;
	//int c = 0;
	//std::cout << "complete" << std::endl;
	for (auto i = t->begin(); i != t->end(); i++) { 
		//std::cout << i->first << std::endl;
		if (qc->inf(i->first) != i->second) {
			std::cout << qc << std::endl;
			std::cout << "wrong result( need " << i->second << " for " << i->first << " but got " << qc->inf(i->first) << " instead)"; 
			right_res = false;  break;
		}
		else { /*std::cout << i->first << " checked " << std::endl;*/ }
		//c++;
	}
	if (right_res) {
		std::ofstream res;
		std::cout << "length: " << qc->cost('l') << "   qcost(NCV-111): " << qc->cost('q') << "    time: " << ((float)time / CLOCKS_PER_SEC) << " (sec)" << std::endl;
		res.open("result.txt");
		res << qc->text();
		res.close();
	}
}
