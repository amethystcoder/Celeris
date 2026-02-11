#include "storeNode.h"
#include "ast/ast_manager.h"


ProcessEntry* StoreNode::getattachable(NodeDependencies& dependencyList){
    auto process = [this, &dependencyList](){};
    return new ProcessEntry(this, dependencyList, process);
}


std::string* StoreNode::getStoreItemData(const std::string& name) noexcept {
    //go through the children
    auto itemNode = ASTManager::getInstance().findNodeWithTagandName("item", name, static_cast<std::shared_ptr<ASTreeNode>>(this));
    if (itemNode != nullptr){
        return &(itemNode->nodeAttributes["data"]);
    }
    //not found
    return new std::string("");
}

bool StoreNode::storeItem(const std::string& data, const std::string& item) noexcept {
    return true;
}