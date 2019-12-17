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
	std::unordered_set<Node*>* traverse_add();
	void set_traversed() { traversed = true; }
	void add_pre(Node * n) { pre->insert(n); }
	void add_de(Node* n) { pre->insert(n); }
	void add_all_pre(std::unordered_map<int,Node*>* m);

private:
	int id;
	bool traversed;
	std::unordered_set<Node*>* pre;
	std::unordered_set<Node*>* de;

};

class Map {
public:
	Map(int bit_len) {
		b_len = bit_len; n = new Node(0);
		nodes = new std::unordered_map<int, Node*>;
		able = new std::unordered_set<int>;
		(*nodes)[0] = n; able->insert(0);
	}
	~Map() {
		for (auto i = nodes->begin(); i != nodes->end()) { delete (*i); }
		delete nodes;
		delete able;
	}
	std::unordered_set<int>* available() { return able; }
	void traverse(int idx);
private:
	int b_len;
	std::unordered_map<int, Node*>* nodes;
	std::unordered_set<int>* able;
};
