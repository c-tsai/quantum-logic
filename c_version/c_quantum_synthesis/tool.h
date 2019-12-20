#include <iostream>


int Hamming_dist(int b1, int b2, int b_len){
	int diff = b1 ^ b2;
	int point = 1, result =0;
	//std::cout << diff << std::endl;
	for (int i = 0; i < b_len; i++) {
		if ((point & diff) != 0) { result++; }
		point = point << 1;
	}
	return result;
}
