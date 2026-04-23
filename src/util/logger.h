#include <iostream>
#include <unordered_map>
#include "fileParser.h"
#include "ast/ast.h"
#include "ast/ast_manager.h"


#ifndef LOGGER_H
#define LOGGER_H

#include <vector>

class Logger {
public:
	enum class LogLevel {
		INFO,
		WARNING,
		ERR
	};

	Logger();
	static void log(const std::string& message, LogLevel level = LogLevel::INFO);

	Logger operator=(const Logger&) = delete;

	std::string parseTemplateMessage(const std::string& message,
		std::shared_ptr<ASTreeNode> startNode, std::shared_ptr<ASTreeNode> self = nullptr);

	bool startLoggerParse(std::shared_ptr<ASTreeNode> startNode, std::shared_ptr<ASTreeNode> thisNode);

	void PrintLogQueues();


private:
	std::vector<std::string> printQueue;
};
#endif // LOGGER_H