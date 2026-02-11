#include "ast_manager.h"
#include <iostream>

ASTManager::ASTManager()
{
}

ASTManager::~ASTManager()
{
}

std::filesystem::path ASTManager::mainPath;
std::shared_ptr<ASTreeNode>ASTManager::rootNode; //root node of the AST

//build the tree from the first node list
// the first tag must be a server tag
std::shared_ptr<ASTreeNode> ASTManager::buildTree(std::filesystem::path htmlPath) {
	mainPath = htmlPath.parent_path(); //set the main path to the html path
	std::string resolvedPath = std::filesystem::weakly_canonical(htmlPath).string();
	std::vector<HTMLTagData> firstNodeList = FileParser::determineParseType(resolvedPath);
	//create a tree of the html text

	for (auto& tag_data : firstNodeList) {
		//create a node for the root tag
		// search for the server tag
		if (tag_data.tag == "server") {
			//ensure that the root tag is the server tag
			//any tag not under the server tag would be ignored

			//create a node for the server tag
			std::shared_ptr<ASTreeNode> serverNode = ASTNodeFactory::getInstance().create("server");
			rootNode = serverNode; //set the root node to the server node
			if (serverNode == nullptr) {
				std::cerr << "Server was not successfully created" << std::endl;
				return nullptr;
			}
			NodeDependencies transformedDependencies = ASTManager::transformNodeDependencies(serverNode->getRawDependencies());
			serverNode->registernode(tag_data.tag, tag_data.attributes, tag_data.content);
			CelProcess::getInstance().attachProcess(
				serverNode->getattachable(transformedDependencies)
			);
			return serverNode;
		}
	}
	return nullptr;
}

//this function needs proper error handling as well as fixing
void ASTManager::addNodeChildrenFromContent(std::string& content, ASTreeNode* node) {
	if (!node || node == nullptr) {
		if (ASTManager::rootNode == nullptr) {
			return; //No root node available
		}
		node = ASTManager::rootNode.get(); //Start from the root node if no start node is provided
	}
	TagDataList parsed_content = FileParser::parse_html_content(content);

	for (auto& tag_data : parsed_content) {

		std::shared_ptr<ASTreeNode> astInstance = ASTNodeFactory::getInstance().create(tag_data.tag);
		if (astInstance != nullptr) {
			//use the file parser to change the content based on an attribute called `sref`
			//the sref is given more priority than the content directly placed in the tag
			astInstance->registernode(tag_data.tag, tag_data.attributes, tag_data.content);
			std::string sref = astInstance->nodeAttributes["sref"];
			if (!sref.empty()) {
				//TODO there is a small issue here that i need to fix soonest
				std::filesystem::path p(ASTManager::getInstance().getMainPath());
				tag_data.content = FileParser::readHtmlFile(std::filesystem::weakly_canonical(p/sref).string());
			} 
			//ASTManager::addNodeChildrenFromContent(tag_data.content, astInstance.get());
			NodeDependencies transformedDependencies = ASTManager::transformNodeDependencies(astInstance->getRawDependencies());
			CelProcess::getInstance().attachProcess(
				astInstance->getattachable(transformedDependencies)
			);
			node->AddChild(astInstance);
			//all classes should be derived from ASTreeNode
		}
	}
}

//Find a route node with a specific endpoint
[[nodiscard]]
std::shared_ptr<ASTreeNode> ASTManager::findRouteNodeWithEndpoint(const std::string& endpoint, std::shared_ptr<ASTreeNode> startnode = ASTManager::rootNode) {
	if (!startnode || startnode == nullptr) {
		if (ASTManager::rootNode == nullptr) {
			return nullptr; //No root node available
		}
		startnode = ASTManager::rootNode; //Start from the root node if no start node is provided
	}

	//Check if current node is the matching route node
	if (startnode->getTagName() == "route" && startnode->nodeAttributes["endpoint"] == endpoint) return startnode;

	//Recursively search children
	for (auto& child : startnode->GetChildren()) {
		std::shared_ptr<ASTreeNode> result = findRouteNodeWithEndpoint(endpoint, child);
		if (result) return result; 
	}

	return nullptr; //Not found in this branch
}

[[nodiscard]]
std::shared_ptr<ASTreeNode> ASTManager::findNodeWithName(const std::string& name, std::shared_ptr<ASTreeNode> startnode = ASTManager::rootNode) {
	if (!startnode || startnode == nullptr) {
		if (ASTManager::rootNode == nullptr) {
			return nullptr; //No root node available
		}
		startnode = ASTManager::rootNode; //Start from the root node if no start node is provided
	}

	//work on this tomorrow 

	if (startnode->nodeAttributes.find("name") != startnode->nodeAttributes.end()) {
		if (startnode->nodeAttributes["name"] == name) return startnode;
	}

	//Recursively search children
	for (auto& child : startnode->GetChildren()) {
		std::shared_ptr<ASTreeNode> result = findNodeWithName(name, child);
		if (result) return result;
	}

	return nullptr; //Not found in this branch
}

[[nodiscard]]
std::shared_ptr<ASTreeNode> ASTManager::findNodeWithTagName(const std::string& Tagname, std::shared_ptr<ASTreeNode> startnode = ASTManager::rootNode) {
	if (!startnode || startnode == nullptr) {
		if (ASTManager::rootNode == nullptr) {
			return nullptr; //No root node available
		}
		startnode = ASTManager::rootNode; //Start from the root node if no start node is provided
	}

	if (startnode->getTagName() == Tagname) return startnode;

	//Recursively search children
	for (auto& child : startnode->GetChildren()) {
		std::shared_ptr<ASTreeNode> result = findNodeWithTagName(Tagname, child);
		if (result) return result;
	}

	return nullptr; //Not found in this branch
}

[[nodiscard]]
std::shared_ptr<ASTreeNode> ASTManager::findNodeWithTagandName(const std::string& Tagname, const std::string& name, std::shared_ptr<ASTreeNode> startnode) {
	if (!startnode || startnode == nullptr) {
		if (ASTManager::rootNode == nullptr) {
			return nullptr; //No root node available
		}
		startnode = ASTManager::rootNode; //Start from the root node if no start node is provided
	}

	if (startnode->nodeAttributes.find("name") != startnode->nodeAttributes.end()) {
		if (startnode->nodeAttributes["name"] == name && startnode->getTagName() == Tagname) return startnode;
	}

	//Recursively search children
	for (auto& child : startnode->GetChildren()) {
		std::shared_ptr<ASTreeNode> result = findNodeWithTagandName(Tagname, name ,child);
		if (result) return result;
	}

	return nullptr; //Not found in this branch
}

std::map<std::string, std::string> ASTManager::parseattributes(const std::string& attributes) {
	return FileParser::parseAttributes(attributes);
}

std::filesystem::path ASTManager::getMainPath() const noexcept {
	return mainPath;
}

void ASTManager::setMainPath(std::string& path) noexcept {
	//parse path and remove text behind slashes that might contain an extension
	std::filesystem::path p(path);
	if (p.has_extension()) {
		p = p.parent_path();
	}
	mainPath = p.string();
}

NodeDependencies ASTManager::transformNodeDependencies(std::vector<RawDependency*> rawDep)
{
	NodeDependencies depList;
	depList.reserve(rawDep.size());

	std::transform(rawDep.begin(), rawDep.end(), std::back_inserter(depList), [](RawDependency* dep) {
		std::shared_ptr<ASTreeNode> node;
		if (dep->depName.empty())
			node = ASTManager::findNodeWithTagName(dep->depNodeName);
		else
			node = ASTManager::findNodeWithName(dep->depName); // ideally improve to match both
		return node;
	});

	return depList;
}
