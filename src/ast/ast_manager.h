#ifndef AST_MANAGER_H
#define AST_MANAGER_H

#include "../util/fileParser.h"
#include "ast.h"
#include "ast_factory.h"
#include "../process/process.h"
#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <memory>

class ASTManager
{
public:

	static void addNodeChildrenFromContent(std::string& content, ASTreeNode* node);

	static std::shared_ptr<ASTreeNode> findNodeWithTagName(const std::string& name, std::shared_ptr<ASTreeNode> startnode);

	static std::shared_ptr<ASTreeNode> findRouteNodeWithEndpoint(const std::string& endpoint, std::shared_ptr<ASTreeNode> startnode);

	static std::map<std::string, std::string> parseattributes(const std::string& attributes);

	static std::shared_ptr<ASTreeNode> findNodeWithName(const std::string& name, std::shared_ptr<ASTreeNode> startnode);
	
	static std::shared_ptr<ASTreeNode> findNodeWithTagandName(const std::string& Tagname, const std::string& name, std::shared_ptr<ASTreeNode> startnode = ASTManager::rootNode);
	
	static NodeDependencies transformNodeDependencies(std::vector<RawDependency*> rawDep);

	std::shared_ptr<ASTreeNode> buildTree(std::filesystem::path htmlPath);

	static ASTManager& getInstance() {
		static ASTManager instance;
		return instance;
	}

	std::filesystem::path getMainPath() const noexcept;
	void setMainPath(std::string& path) noexcept;

	~ASTManager();

private:
	static std::filesystem::path mainPath;
	ASTManager(const ASTManager&) = delete;
	ASTManager& operator=(const ASTManager&) = delete;

	static std::shared_ptr<ASTreeNode> rootNode; //root node of the AST

	//global access for process
	CelProcess process;
	ASTManager();
};

#endif // !AST_MANAGER_H