#ifndef TASK_H
#define TASK_H

#include "ast/ast.h"

class TaskNode : public ASTreeNode {

public:
	TaskNode();
	~TaskNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content);

	ProcessEntry* getattachable(NodeDependencies& dependencies) override;

    bool startTask();

private:
	std::vector<RawDependency*> rawDependencies = {};
	

};
#endif // !TASK_H
