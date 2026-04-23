#include "core/requestNode.h"

RequestNode::RequestNode(){}

RequestNode::~RequestNode(){}

void RequestNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
}

/* ProcessEntry* RequestNode::getattachable(NodeDependencies& dependencyList)
{
	RepProcess process = [this, &dependencyList]() {};
	return new ProcessEntry(this, dependencyList, process);
}
 */