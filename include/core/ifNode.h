#include "ast/ast.h"
#include "util/decisions/ifDecTree.h"
#include "ast/ast_manager.h"

class IfNode: public ASTreeNode
{
public:
	IfNode ();
	~IfNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content) override;

	ASTreeNode::NodeChildren GetChildren() override;

private:
	std::string expression;
	ifDecTree treeRef;
};