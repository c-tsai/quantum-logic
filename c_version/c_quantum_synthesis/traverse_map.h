#include <unordered_set>
#include <unordered_map>

class Node {
public:
	Node(int i) {
		id = i; traversed = false;
		pre = new std::unordered_set<Node*>;
		de = new std::unordered_set<Node*>;
	}
	~Node() { delete pre, de; }
	int get_id() { return id; }
	bool if_traversed() { return traversed; }
	std::unordered_set<Node*>::iterator pre_begin() { return pre->begin(); }
	std::unordered_set<Node*>::iterator pre_end(){return pre->end();}
	void traverse_add(std::unordered_set<int>* m_p);
	void set_traversed() { traversed = true; }
	void add_pre(Node* n) { pre->insert(n); }
	void add_de(Node* n) { de->insert(n);  n->add_pre(this); }
	void add_all_pre(std::unordered_map<int, Node*>* m, int b_len);

private:
	int id;
	bool traversed;
	std::unordered_set<Node*>* pre;
	std::unordered_set<Node*>* de;

};

class Map {
public:
	Map(int bit_len) {
		b_len = bit_len; Node* n = new Node(0);
		nodes = new std::unordered_map<int, Node*>;
		able = new std::unordered_set<int>;
		(*nodes)[0] = n; able->insert(0);
	}
	~Map() {
		for (auto i = nodes->begin(); i != nodes->end(); i++) { delete i->second; }
		delete nodes;
		delete able;
	}
	std::unordered_set<int>* available() { return able; }
	void traverse(int idx);
private:
	int b_len;
	std::unordered_set<int>* able;
	std::unordered_map<int, Node*>* nodes;
};

/*
int main() {
	Map* m = new Map(3);
	while (!(m->able->empty())) {
		std::cout << '{';
		for (auto i = m->nodes->begin(); i != m->nodes->end(); i++) {
			std::cout << i->first << ':';
			for (auto j = i->second->de->begin(); j != i->second->de->end(); j++) {
				std::cout << (*j)->get_id() << '.';
			}
			std::cout << ',';
		}
		std::cout << '}'<< std::endl;
		m->traverse(*(m->able->begin()));
	}
}*/