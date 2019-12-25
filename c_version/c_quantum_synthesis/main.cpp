#include "synthesizer.h"
#include "table.h"

int main(int argc, char** argv) {
	Table* t = pla_reader(*(argv[1]));
	QCSynthesizer* qc = new QCSynthesizer(t, t->len());
	if (*(argv[3]) == "True" || *(argv[3]) == "true") {
		qc->algorithm_selector(std::stoi(*(argv[2])), 0, true, *(argv[4])[0], *(argv[5])[0]);}
	else if (*(argv[3]) == "False" || *(argv[3]) == "false") {
		qc->algorithm_selector(std::stoi(*(argv[2])), 0, false, *(argv[4])[0], *(argv[5])[0]);
	}
	else { throw std::invalid_argument("the 3rd argument (indicating using control min or not) should be booleon") }
}