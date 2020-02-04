#include <unordered_map>
#include <set>
#include <iostream>// for testing


class Control_lines {
public:
	Control_lines(int len) {
		bit_len = len;
		lib = new std::set<int>*[len+1];
		for (int i = 0; i <= len; i++) { lib[i] = new std::set<int>; }
		mn = 0; mx = 0; 
	}
	~Control_lines() { 
		for (int i = 0; i < bit_len; i++) { delete lib[i]; }
		delete[] lib; }
	friend class Control_generator;


	class iterator{
	public:
		iterator(const std::set<int>::iterator& i, Control_lines* c, int id){
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
			//std::cout << "comparing iterid: " << iterid << " origin: "<< origin <<  std::endl;
			//std::cout << "and iterid: " << other.iterid << " origin: " << other.origin << std::endl;
			return ((iter != other.iter) || (origin != other.origin) || (iterid != other.iterid)); }
		int element() {
			//std::cout << *iter << std::endl;
			return *iter; }
		virtual int first() { return 0; }
		virtual Control_lines* second() { return 0; }
	protected:
		Control_lines* origin;
		std::set<int>::iterator iter;
		std::set<int>::iterator end;
		int iterid;
		int mx;
	};
	iterator begin() {
		//std::cout << "begin created " << std::endl;
		//std::unordered_set<int>::iterator i = lib[mn]->begin(); // for testing
		return iterator(lib[mn]->begin(),this,mn); }
	iterator end() {
		//std::cout << "end created " << std::endl;
		return iterator(lib[mx]->end(),this,mx); }



	void pop(int line, int b_num = -1);
	void unioning(Control_lines* other, int b_num = -1);
	void add(int line, int b_num = -1);// for testing put in public
	std::set<int>* max_group() { return lib[mx]; };
	std::set<int>* min_group() { return lib[mn]; };
	std::set<int>* get_group(int key) { return lib[key]; };
	int get_max() { return mx; }

private:
	//void add(int line, int b_num = -1); the real private add
	bool contain(int targ, int b_num = -1);
	Control_lines* copy();
	
	std::set<int>** lib;
	int bit_len;
	int mn;
	int mx;
};




class Control_lib: public Control_lines{
public:
	Control_lib(int len): Control_lines(len) {
		lib = new std::unordered_map<int, Control_lines*>* [len + 1];
		for (int i = 0; i <= len; i++) { 
			lib[i] = new std::unordered_map<int, Control_lines*>;
		}
	}
	class iterator : public Control_lines::iterator {
	public:
		iterator(const std::unordered_map<int, Control_lines*>::iterator& i, Control_lib* c, int id):Control_lines::iterator(id){
			iter = i;  origin = c; mx = c->get_max();
			end = c->get_group(id)->end();
		}
		int first() { return iter->first; }
		Control_lines* second() { return iter->second; }
	private:
		std::unordered_map<int, Control_lines*>::iterator iter;
	};
	friend class Control_generator;

	
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
	void combine(std::set<int>* b_list, int targ, int num);
	std::set<int>* best_clines(int bit1, int controled);

private:
	Control_lines* unable;
	Control_lib* lib;
	int* each_num;
	int smllst_b;
	int allowed_num;
	int bit_len;
};

int main(int argc, char** argv) {
	Control_lines* c = new Control_lines(3);
	Control_lines* d = new Control_lines(3);
	d->add(0);
	c->add(1);
	//std::cout << "before add 7" << std::endl;
	d->add(4);
	c->add(7);
	c->unioning(d);
	for (auto i = c->begin(); i != c->end(); i++) {
		std::cout << i.element() << std::endl;
	}
	std::cout <<"max group: " << c->get_max() << std::endl;
}
