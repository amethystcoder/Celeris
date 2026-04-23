#ifndef REQUEST_H
#define REQUEST_H

#include "ast/ast.h"
#include "net/clientsock.h"
#include "ast/ast_manager.h"

class RequestNode : public ASTreeNode {

public:
	RequestNode();
	~RequestNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content);

	//ProcessEntry* getattachable(NodeDependencies& dependencies) override;

private:
	std::vector<RawDependency*> rawDependencies = {};
	

};
#endif // !STORE_H
