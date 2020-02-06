#include "traverse_map.h"
#include "table.h"

std::set<int>* Node::traverse_add(Control_generator* c_g, bool rm_frm_clines) {
	set_traversed();
	if (rm_frm_clines) { remove_from_clines(c_g); }
	std::set<int>* neww = new std::set<int>;
	for (auto i = de->begin(); i != de->end(); i++) {
		if ((*i)->if_traversed()) { continue; }
		bool block = true;
		for (auto j = (*i)->pre_begin(); j != (*i)->pre_end(); j++) {
			if (!((*j)->if_traversed())) { block = false; break; }
		}
		if (block) { neww->insert((*i)->id); }
	}return neww;
}
void Node::remove_from_clines(Control_generator* c_g) {
	c_line_removed = true; c_g->remove(id);
	for (auto i = pre_begin(); i != pre_end(); i++) {
		if (!((*i)->c_line_removed)) { (*i)->remove_from_clines(c_g); }
	}
}
void Node::add_all_pre(std::unordered_map<int, Node*>* m) {
	int point = 1;
	for (int i = 0; i < bit_len; i++) {
		if ((point | id) == id) {
			int aim = (point | id) - point;
			
			if (m->find(aim) == m->end()) {
				Node* n = new Node(aim, bit_len);
				m->insert(std::pair<int,Node*>(aim,n));
				n->add_all_pre(m);
			}
			m->find(aim)->second->add_de(this);
		} point = point << 1;
	}
}
void Map::traverse(int idx, Control_generator* c_g, bool rm_frm_clines) {
	int point = 1;
	
	for (int i = 0; i < b_len; i++) {
		
		if (((point & idx) != point) && ((nodes->find(point|idx)) == (nodes->end()))) {
			Node* n = new Node(point | idx, b_len);
			(*nodes)[point | idx] = n;
			n->add_all_pre(nodes);
		}
		point = point << 1;
	}

	std::set<int>* neww =(*nodes)[idx]->traverse_add(c_g, rm_frm_clines);
	if (!(neww->empty())) { able->unioning(neww, Hamming_Dist(idx, 0, b_len) + 1); }
	able->pop(idx);
}