#include "ast/ast_factory.h"


ASTNodeFactory& ASTNodeFactory::getInstance() {
    static ASTNodeFactory instance;
    return instance;
}

void ASTNodeFactory::registerClass(const std::string& name, CreatorFunc creator) {
    registry[name] = std::move(creator);
}


std::shared_ptr<ASTreeNode> ASTNodeFactory::create(const std::string& name) {
    auto it = registry.find(name);
    if (it != registry.end()) {
        return it->second();
    }
    return nullptr; // Return nullptr if the class name is not found
}