// Ŭnicode please 
#include "stdafx.h"
#include "draw_attribute_list_mixin.h"

void DrawAttributeListMixin_Node::pop_back() 
{
	auto node = nodeList.back();
	if(node) {
		node->remove();
		node->drop();
	}
	nodeList.pop_back();
}
void DrawAttributeListMixin_Node::clear() 
{
	auto it = nodeList.begin();
	auto endit = nodeList.end();
	for( ; it != endit ; ++it) {
		auto node = *it;
		if(node) {
			node->remove();
			node->drop();
		}
	}
	nodeList.clear(); 
}