#include "shapeNode.h"
#include "core/storeNode.h"
#include "ast/ast_manager.h"

/////////////////////////////////////////////////////////////////
// Shape Node
/////////////////////////////////////////////////////////////////

void ShapeNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
    addTagName(name, this);
    setNodeAttributes(ASTManager::parseattributes(attributes), this);

    //the shape node does not add create children nodes from its content
    // instead, save it in the structure.
    //ASTManager::addNodeChildrenFromContent(content, this);
}

ProcessEntry* ShapeNode::getattachable(NodeDependencies& dependencyList) {
    auto process = [this, &dependencyList]() {};
    return new ProcessEntry(this, dependencyList, process);
}

ShapeNode::ShapeNode()
{
}

ShapeNode::~ShapeNode()
{
}

void ShapeNode::display_structure() const noexcept
{
}
