#include <iostream>
#include "net/cleansocket.h"
#include "core/ServerNode.h"
#include "core/databaseNode.h"
#include <vector>
#include "util/fileParser.h"
#include "core/RouteNode.h"
#include "core/APINode.h"
#include "core/RatelimitNode.h"
#include "core/FileSystem.h"
#include "ast/ast_factory.h"
#include <filesystem>
#include "ast/ast_manager.h"


// Helper function to register classes
static void registerClasses() {
	ASTNodeFactory::getInstance().registerClass("server", []() { return std::make_shared<ServerNode>(); });
	ASTNodeFactory::getInstance().registerClass("route", []() { return std::make_shared<RouteNode>(); });
	ASTNodeFactory::getInstance().registerClass("api", []() { return std::make_shared<APINode>(); });
	ASTNodeFactory::getInstance().registerClass("database", []() { return std::make_shared<DatabaseNode>(); });
	ASTNodeFactory::getInstance().registerClass("ratelimit", []() { return std::make_shared<RateLimitNode>(); });
	ASTNodeFactory::getInstance().registerClass("filesystem", []() { return std::make_shared<FileSystem>(); });
}

int main(int argc, char** argv) {

	AmthSocket::SocketImpl sockimpl{};
	registerClasses();

	//find the current working directory
	std::filesystem::path currentPath = std::filesystem::current_path();

	std::string html_text = "../../../src/resources/serverFile.html"; 
	//keep this for now, in the finished application, this would be set to the absolute path of the html file to be parsed
	//there could also be a command line argument to specify the path to the html file and something to determine the location of the file

	std::filesystem::path htmlPath = currentPath / html_text;

	ASTreeNode* rootNode = ASTManager::getInstance().buildTree(htmlPath);
}

