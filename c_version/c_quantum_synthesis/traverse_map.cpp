#include "traverse_map.h"
#include <unordered_set>
#include <unordered_map>

void Node::traverse_add(std::unordered_set<int>* m_p) {
	set_traversed();
	for (auto i = de->begin(); i != de->end(); i++) {
		if ((*i)->if_traversed()) { continue; }
		//std::cout << (*i)->get_id() << "------"<< std::endl;
		bool block = true;
		for (auto j = (*i)->pre_begin(); j != (*i)->pre_end(); j++) {
			//std::cout << (*i)->get_id() <<  std::endl;
			if (!((*j)->if_traversed())) { block = false; break; }}
		if (block) { m_p->insert((*i)->id); }
	}
}
void Node::add_all_pre(std::unordered_map<int, Node*>* m, int b_len) {
	int point = 1;
	for (int i = 0; i < b_len; i++) {
		if ((point | id) != id) {
			int aim = (point | id) - point;
			
			if (m->find(aim) == m->end()) {
				Node* n = new Node(aim);
				(*m)[aim] = n;
			}
			Node* n = new Node(0);
			(*m)[aim]->add_de(n);
		} point = point << 1;
	}
}
void Map::traverse(int idx) {
	able->erase(idx);
	int point = 1;
	
	for (int i = 0; i < b_len; i++) {
		
		if (((point & idx) != point) && (nodes->find(point|idx) == nodes->end())) {
			//std::cout << point;
			Node* n = new Node(point | idx);
			(*nodes)[point | idx];
			n->add_all_pre(nodes, b_len);
		}
		point = point << 1;
	}

	(*nodes)[idx]->traverse_add(able);
	
}