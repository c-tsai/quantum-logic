#include <unordered_map>
#include <set>
#include <vector>
//#include <iostream>// for testing
//#include "table.h"//for testing

// remember to check if implement both unordered_set(for Control_generator and Control_lib) and 
// set(for traverse_map's available) version on the Control_lines can improve the performance
class Control_lines {
public:
	Control_lines(int len) {
		bit_len = len;
		lib = new std::set<size_t>*[len+1];
		for (int i = 0; i <= len; i++) { lib[i] = new std::set<size_t>; }
		mn = 0; mx = 0; 
	}
	~Control_lines() { 
		for (int i = 0; i < bit_len; i++) { delete lib[i]; }
		delete[] lib; }
	friend class Control_generator;


	class iterator{
	public:
		iterator(const std::set<size_t>::iterator& i, Control_lines* c, int id){
			iter = i; iterid = id; origin = c; mx = c->mx;
			end = c->get_group(id)->end();}
		iterator(int id) { iterid = id; }
		iterator operator ++ (int) {
			//std::cout << "increment" << std::endl;
			if (!(origin->get_group(iterid)->empty())) { iter++; }
			while (iter == end && iterid < mx) {
				//std::cout << "next strand" << std::endl;
				iterid++; iter = origin->get_group(iterid)->begin(); end = origin->get_group(iterid)->end();
			}
			return *this;
		};
		bool operator == (const iterator& other) { 
			return ((iter == other.iter)&&(origin==other.origin)&&(iterid==other.iterid)); }
		bool operator != (const iterator& other) {
			//std::cout << "comparing iterid: " << iterid << " origin: "<< origin <<std::endl;
			//std::cout << "and iterid: " << other.iterid << " origin: " << other.origin  << std::endl;
			return ((iter != other.iter) || (origin != other.origin) || (iterid != other.iterid)); }
		size_t element() {
			//std::cout << *iter << std::endl;
			return *iter; }
		virtual size_t first() { return 0; }
		virtual std::vector<size_t>* second() { return 0; }
		//int iterid; //for testing
	protected:
		Control_lines* origin;
		std::set<size_t>::iterator iter;
		std::set<size_t>::iterator end;
		int iterid; //the real one
		int mx;
	};
	iterator begin() {
		//std::cout << "begin created " << std::endl;
		//std::unordered_set<int>::iterator i = lib[mn]->begin(); // for testing
		return iterator(lib[mn]->begin(),this,mn); }
	iterator end() {
		//std::cout << "end created " << std::endl;
		return iterator(lib[mx]->end(),this,mx); }



	void pop(size_t line, int b_num = -1);
	void unioning(std::set<size_t>* other, int b_num);
	//void add(int line, int b_num = -1);// for testing put in public
	//bool contain(int targ, int b_num = -1);// for testing put in public
	std::set<size_t>* max_group() { return lib[mx]; };
	std::set<size_t>* min_group() { return lib[mn]; };
	std::set<size_t>* get_group(int key) { return lib[key]; };
	int get_max() { return mx; }
	bool empty() { return begin() == end(); }

protected:
	void add(size_t line, int b_num = -1); //the real private add
	bool contain(size_t targ, int b_num = -1); //the real private one
	
	
	std::set<size_t>** lib;
	int bit_len;
	int mn;
	int mx;
};




class Control_lib: public Control_lines{
public:
	Control_lib(int len): Control_lines(len) {
		lib = new std::unordered_map<size_t, std::vector<size_t>*>* [len + 1];
		for (int i = 0; i <= len; i++) { 
			lib[i] = new std::unordered_map<size_t, std::vector<size_t>*>;
		}
	}
	class iterator : public Control_lines::iterator {
	public:
		iterator(const std::unordered_map<size_t, std::vector<size_t>*>::iterator& i, Control_lib* c, int id):Control_lines::iterator(id){
			iter = i;  origin = c; mx = c->get_max();
			end = c->get_group(id)->end();
		}
		iterator operator ++ (int) {
			//std::cout << "increment" << std::endl;
			if (!(origin->get_group(iterid)->empty())) { iter++; }
			while (iter == end && iterid < mx) {
				//std::cout << "next strand" << std::endl;
				iterid++; iter = origin->get_group(iterid)->begin(); end = origin->get_group(iterid)->end();
			}
			return *this;
		};
		bool operator == (const iterator& other) {
			return ((iter == other.iter) && (origin == other.origin) && (iterid == other.iterid));
		}
		bool operator != (const iterator& other) {
			//std::cout << "comparing iterid: " << iterid << " origin: " << origin << std::endl;
			//std::cout << "and iterid: " << other.iterid << " origin: " << other.origin << std::endl;
			return ((iter != other.iter) || (origin != other.origin) || (iterid != other.iterid));
		}
		size_t first() { return iter->first; }
		std::vector<size_t>* second() { return iter->second; }
	private:
		std::unordered_map<size_t, std::vector<size_t>*>::iterator iter;
		std::unordered_map<size_t, std::vector<size_t>*>::iterator end;
		Control_lib* origin;
	};
	iterator begin() {return iterator(lib[mn]->begin(), this, mn);}
	iterator end() {return iterator(lib[mx]->end(), this, mx);}
	friend class Control_generator;

	
	void pop(size_t line, int b_num = -1);
	//void add(int key, std::vector<int>* clines, int b_num = -1);// public for testing
	//Control_lines* get(int targ, int b_num = -1);// public for testing
	std::unordered_map<size_t, std::vector<size_t>*>* get_group(int key) { return lib[key]; }

private:
	void add(size_t key, std::vector<size_t>* clines, int b_num = -1);// real private one
	std::vector<size_t>* get(size_t targ, int b_num = -1);// real private one
	bool contain(size_t line, int b_num);

	std::unordered_map<size_t, std::vector<size_t>*>** lib;
	
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
	void remove(size_t targ);
	void combine(std::vector<size_t>* b_list, size_t targ, int num);
	std::vector<size_t>* best_clines(size_t bit1, size_t controled);
	//Control_lines* unable;//for testing
	//Control_lib* lib;//for testing

private:
	Control_lines* unable; //real one
	Control_lib* lib;//real one
	int* each_num;
	int smllst_b;
	int allowed_num;
	int bit_len;
};

/*int main(int argc, char** argv) {
	Control_generator* c = new Control_generator(3);
	c->remove(0);
	c->remove(2);
	c->remove(4);
	//std::vector<int>* list = bit_list(3, 3);
	//std::cout << "the size of the original b_list: " << list->size() << std::endl;
	
	int s = 0;
	std::vector<int>* res = c->best_clines(7, 1);
	for (auto i = res->begin(); i != res->end(); i++) {
		s++;
		std::cout << *i<< std::endl;
		if (s > 7) { break; }
	}
}*/
