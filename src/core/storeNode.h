#ifndef STORE_H
#define STORE_H

#include "ast/ast.h"

class StoreNode : public ASTreeNode {

public:
	StoreNode();
	~StoreNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content);

	ProcessEntry* getattachable(NodeDependencies& dependencies) override;

	std::string* getStoreItemData(const std::string& name) noexcept;

    bool storeItem(const std::string &data, const std::string &item) noexcept;

private:
	std::vector<RawDependency*> rawDependencies = {};
};
#endif // !STORE_H
