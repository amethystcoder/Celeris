#include "core/itemNode.h"


ItemNode::ItemNode()
{
}

ItemNode::~ItemNode()
{
}

void ItemNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
}