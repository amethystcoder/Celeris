#include "net/cleansocket.h"

#include "core/RouteNode.h"
#include "core/APINode.h"
#include "core/RatelimitNode.h"
#include "core/ServerNode.h"
#include "core/databaseNode.h"
#include "core/FileSystem.h"
#include "core/storeNode.h"
#include "core/requestNode.h"
#include "core/itemNode.h"

#include "process/process.h"

#include "ast/ast_factory.h"
#include <filesystem>
#include "ast/ast_manager.h"
#include "util/logger.h"


// Helper function to register classes
static void registerClasses() {
	ASTNodeFactory::getInstance().registerClass("server", []() { return std::make_shared<ServerNode>(); });
	ASTNodeFactory::getInstance().registerClass("route", []() { return std::make_shared<RouteNode>(); });
	ASTNodeFactory::getInstance().registerClass("api", []() { return std::make_shared<APINode>(); });
	ASTNodeFactory::getInstance().registerClass("database", []() { return std::make_shared<DatabaseNode>(); });
	ASTNodeFactory::getInstance().registerClass("ratelimit", []() { return std::make_shared<RateLimitNode>(); });
	ASTNodeFactory::getInstance().registerClass("filesystem", []() { return std::make_shared<FileSystem>(); });
	ASTNodeFactory::getInstance().registerClass("request", []() { return std::make_shared<RequestNode>(); });
	ASTNodeFactory::getInstance().registerClass("store", []() { return std::make_shared<StoreNode>(); });
	ASTNodeFactory::getInstance().registerClass("item", []() { return std::make_shared<ItemNode>(); });
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

	auto rootNode = ASTManager::getInstance().buildTree(htmlPath);
	
	Logger log{};

	log.startLoggerParse(rootNode, rootNode);
	log.PrintLogQueues();

	CelProcess::getInstance().beginprocess();

}


