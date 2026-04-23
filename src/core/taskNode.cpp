#include "core/taskNode.h"

TaskNode::TaskNode(){}

TaskNode::~TaskNode(){}

void TaskNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
}

bool TaskNode::startTask(){
    //start with empty dependency
    std::vector<std::shared_ptr<ASTreeNode>> dataDependencies = { };
    for (auto &child: this->GetChildren()){
        //just run the task of each child
        auto functionalDependency = child->getattachable(dataDependencies);//empty for now
        //execute each process.
        functionalDependency->process();
    }
    return true;
}