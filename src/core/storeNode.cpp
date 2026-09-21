#include "core/storeNode.h"
#include "ast/ast_manager.h"

StoreNode::StoreNode(){}
StoreNode::~StoreNode(){}

void StoreNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
}

ProcessEntry* StoreNode::getattachable(NodeDependencies& dependencyList){
    auto process = [this, &dependencyList](){};
    return new ProcessEntry(this, dependencyList, process);
}

bool StoreNode::storeItem(const std::string& item) noexcept {
    return true;
}

constexpr std::string StoreNode::determine_default_value()
{
    return std::string();
}
