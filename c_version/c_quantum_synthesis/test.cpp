#include <vector>
#include <iostream>

using namespace std;

vector<int>a = { 1,2,3 };
vector<int>b = { 4,5,6 };
int main(int argc, char** argv) {
	vector<int>a = { 1,2,3 };
	vector<int>b = { 4,5,6 };
	a.insert(a.begin(), b.begin(), b.end());
	b[0] = 10;
	for (int i = 0; i < 6; i++) { std:cout << a[i]; }
};
