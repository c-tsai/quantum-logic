#include <unordered_set>
#include "control_lines.h"
//#include <iostream> //for testing

class Node {
public:
	Node(size_t i, int len) {
		id = i; bit_len = len; traversed = false;
		c_line_removed = false;
		pre = new std::unordered_set<Node*>;
		de = new std::unordered_set<Node*>;
	}
	~Node() { delete pre, de; }
	size_t get_id() { return id; }
	bool if_traversed() { return traversed; }
	std::unordered_set<Node*>::iterator pre_begin() { return pre->begin(); }
	std::unordered_set<Node*>::iterator pre_end(){return pre->end();}
	std::set<size_t>* traverse_add(Control_generator* c_g, bool rm_frm_clines);
	void remove_from_clines(Control_generator* c_g);
	void set_traversed() { traversed = true; }
	void add_pre(Node* n) { pre->insert(n); }
	void add_de(Node* n) { de->insert(n);  n->add_pre(this); }
	void add_all_pre(std::unordered_map<size_t, Node*>* m);
	//std::unordered_set<Node*>* de;//for testing

private:
	size_t id;
	int bit_len;
	bool traversed;
	bool c_line_removed;
	std::unordered_set<Node*>* pre;
	std::unordered_set<Node*>* de;//real one

};

class Map {
public:
	Map(int bit_len) {
		b_len = bit_len; Node* n = new Node(0, b_len);
		nodes = new std::unordered_map<size_t, Node*>;
		able = new Control_lines(bit_len);
		traversed = new std::unordered_set<size_t>;
		(*nodes)[0] = n;
	}
	~Map() {
		for (auto i = nodes->begin(); i != nodes->end(); i++) { delete i->second; }
		delete nodes;
		delete able;
		delete traversed;
	}
	Control_lines* available() { return able; }
	std::unordered_set<size_t>::iterator traversed_end() {return traversed->end();}
	std::unordered_set<size_t>::iterator traversed_begin() {return traversed->begin();}
	void traverse(size_t idx, Control_generator* c_g, bool rm_frm_clines=true);
	//Control_lines* able;// for testing
	//std::unordered_map<int, Node*>* nodes;//for testing
private:
	int b_len;
	Control_lines* able;// real one
	std::unordered_set<size_t>* traversed;
	std::unordered_map<size_t, Node*>* nodes;//real one
};


/*int main() {
	Map* m = new Map(3);
	Control_generator* cg = new Control_generator(3);
	int s = 0;
	while (true) {
		if (s > 10) { break; }
		std::cout << '{';
		for (auto i = m->nodes->begin(); i != m->nodes->end(); i++) {
			std::cout << i->first << ':';
			for (auto j = i->second->de->begin(); j != i->second->de->end(); j++) {
				std::cout << (*j)->get_id() << '.';
			}
			std::cout << ',';
		}
		std::cout << '}'<< std::endl;
		m->traverse((m->able->begin()).element(),cg);
		std::cout << "able traverse term:[";
		for (auto i = m->available()->begin(); i != m->available()->end(); i++) {
			std::cout << i.element() << ',';
		}
		std::cout << ']' << std::endl;
		s++;
	}
}*/
