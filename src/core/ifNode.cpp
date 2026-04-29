#include "core/ifNode.h"

IfNode::IfNode()
{}

IfNode ::~IfNode()
{
}

void IfNode::registernode(const std::string& name, const std::string& attributes, std::string& content) {
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
	expression = nodeAttributes["expr"];//make a copy of expression... for now
}

ASTreeNode::NodeChildren IfNode::GetChildren()
{
	//return the node children if the expression evaluates to true, else return nothing
	if (expression.length() <= 0)
	{
		//TODO: check node attributes as well
		if (nodeAttributes.find("expr") == nodeAttributes.end()) return {};

		//we found node attributes 
	}
	else {
		//work on expression
		treeRef.split_eq(expression);
		bool evaluation_result = treeRef.evaluate();
		std::cout << "eval result" << evaluation_result << std::endl;
		if (evaluation_result) return children;
		else return {};
	}
}
