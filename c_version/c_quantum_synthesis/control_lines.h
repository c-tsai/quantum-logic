#include <unordered_map>
#include <unordered_set>


class Control_lines {
public:
	Control_lines(int len) {
		bit_len = len;
		lib = new std::unordered_set<int>*[len+1];
		for (int i = 0; i < len; i++) { lib[i] = new std::unordered_set<int>; }
		mn = 0; mx = 0; 
	}
	~Control_lines() { 
		for (int i = 0; i < bit_len; i++) {delete lib[i]}
		delete[] lib; }
	friend class Control_generator;


	class iterator: std::unordered_set<int>::iterator {
	public:
		iterator(std::unordered_set<int>::iterator i, int m, int id): std::unordered_set<int>::iterator(i){
			iter = i; mx = m; iterid = id;
		}
		iterator& operator ++ ();
		iterator operator ++ (int);
	private:
		int mx;
		int iterid;
	};
	iterator begin() { iterid = mn; return iterator(lib[mn]->begin(),mx,mn); }
	iterator end() { return iterator(lib[mx]->end(),mx,mx); }



	void pop(int line, int b_num = -1);
	void unioning(Control_lines* n, int b_num = -1);
	std::unordered_set<int>* max_group() { return lib[mx]; };
	std::unordered_set<int>* min_group() { return lib[mn]; };

private:
	void add(int line, int b_num = -1);
	bool contain(int targ, int b_num = -1);
	std::unordered_set<int>* get_group(int key) { return lib[key]; };
	Control_lines* copy();
	
	std::unordered_set<int>** lib;
	int bit_len;
	int mn;
	int mx;
};




class Control_lib {
public:
	Control_lib(int len) {
		bit_len = len;
		lib = new std::unordered_map<int, Control_lines*>* [len + 1];
		for (int i = 0; i < len; i++) { 
			lib[i] = new std::unordered_map<int, Control_lines*>
		}
		mn = 0; mx = 0; iterid = -1;
	}
	friend class Control_generator;

	class iterator : std::unordered_map<int, Control_lines*>::iterator {
	public:
		iterator(std::unordered_map<int, Control_lines*>::iterator i, int m, int id) : std::unordered_map<int, Control_lines*>::iterator(i) {
			iter = i; mx = m; iterid = id;
		}
		iterator& operator ++ ();
		iterator operator ++ (int);
	private:
		int mx;
		int iterid;
	};
	void pop(int line, int b_num = -1);

private:
	void add(int line, int b_num = -1);
	int get(int targ, int b_num = -1);

	std::unordered_map<int, Control_lines*>** lib;
	
};



class Control_generator {
public:
	Control_generator(int len) {
		bit_len = len; smllst_b = 0; allowed_num = 1;
		unable = new Control_lines(len);
		lib = new Control_lib(len);
		each_num = new int[len+1];
		for (int i = 0; i < len; i++) { each_num[i] = 0; }
	}
	~Control_generator() { delete lib, unable; delete[]  each_num; }
	void remove(int targ);
	void combine(std::unordered_set<int>* b_list, int targ, int num);
	std::unordered_set<int>* best_clines(int bit1, int controled);

private:
	Control_lines* unable;
	Control_lib* lib;
	int* each_num;
	int smllst_b;
	int allowed_num;
	int bit_len;
};
