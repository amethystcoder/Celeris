#ifndef AST_H
#define AST_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <utility> //for std::move

struct RawDependency {
	std::string depNodeName; //name of the dependency
	std::string depName;
};

using RepProcess = std::function<void()>; //repeatable process type, a function that takes a list of dependencies and does something with them

//smart pointer containing the repeatable process
using RepProcessPtr = std::shared_ptr<RepProcess>;

class ASTreeNode; //forward declaration of the ASTreeNode class
typedef std::vector<std::shared_ptr<ASTreeNode>>  NodeDependencies;


struct ProcessEntry {
	ASTreeNode* node;
	NodeDependencies deps;
	RepProcess process;

	ProcessEntry(ASTreeNode* n, NodeDependencies d, RepProcess p)
		: node(n), deps(std::move(d)), process(std::move(p)) {}
};

class ASTreeNode
{

	//A tree node for the abstract syntax tree
	// It is more a server node tree, than an AST, but still works in this case 
	//it would contain children and they would also be of type ASTreeNode
	//other classes in the server html tree would be derived from this class
public:
	ASTreeNode();

	~ASTreeNode();

	typedef std::vector<std::shared_ptr<ASTreeNode>> NodeChildren;

	//add a child to the node
	void AddChild(std::shared_ptr<ASTreeNode> child);

	virtual void registernode(const std::string& name, const std::string& attributes, std::string& content);

	void RemoveChild(std::shared_ptr<ASTreeNode> child);

	NodeChildren GetChildren();

	static void setNodeAttributes(std::map<std::string, std::string> attributes, ASTreeNode* node);


	static void addTagName(const std::string& tagname, ASTreeNode* node);


	std::string getTagName() const noexcept;

	std::map<std::string, std::string> nodeAttributes;

	ASTreeNode* getParent() const noexcept;


	//attachable is a function that is called during the processs
	//a process is a loop that runs during the lifetime of the application
	//the loop checks all the nodes for their 'attachables' and runs each of them
	virtual ProcessEntry* getattachable(NodeDependencies& dependencyList);

	ASTreeNode* getDependency(RawDependency* rawdep) const noexcept;

	virtual std::vector<RawDependency*> getRawDependencies() const noexcept;
private:

	//use vector to store children
	NodeChildren children;

	//use a vector to store raw dependencies
	std::vector<RawDependency*> rawDependencies;

	//use a vector to store dependencies
	NodeDependencies dependencies; //TODO: make this a set to avoid duplicates, or a priority queue to order them by priority

	bool determinedDependencies = false; //if the dependencies are determined or not

	//use a pointer to the parent node
	ASTreeNode* parent = nullptr;

	std::string name;
};


#endif // !AST_H

