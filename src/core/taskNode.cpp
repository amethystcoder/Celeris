#include "taskNode.h"

TaskNode::TaskNode(){}

TaskNode::~TaskNode(){}

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