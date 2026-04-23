#ifndef ITEMNODE_H
#define ITEMNODE_H

#include "ast/ast.h"
#include "ast/ast_manager.h"


class ItemNode: public ASTreeNode
{
private:
    std::string data;
    void registernode(const std::string& name, const std::string& attributes, std::string& content);
public:
    ItemNode();
    ~ItemNode();
};


#endif //!ITEMNODE_H