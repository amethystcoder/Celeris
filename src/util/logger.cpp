#include "logger.h"
#include <vector>

void Logger::log(const std::string& message, LogLevel level) {
	switch (level) {
	case LogLevel::INFO:
		std::cout << "[INFO]: " << message << "\n";
		break;
	case LogLevel::WARNING:
		std::cout << "[WARNING]: " << message << "\n";
		break;
	case LogLevel::ERR:
		std::cerr << "[ERROR]: " << message << "\n";
		break;
	}
}

// Parses a message like:
// "Hello {user.name}, id={id}"
// Returns the fully resolved string
std::string Logger::parseTemplateMessage(
    const std::string& message,
    std::shared_ptr<ASTreeNode> startNode,
    std::shared_ptr<ASTreeNode> self
) {
    std::string result;
    size_t pos = 0;
    while (pos < message.length()) {
        size_t open = message.find('{', pos);

        if (open == std::string::npos) {
            result.append(message.substr(pos,message.length()));
            break;
        }

        result.append(message.substr(pos, open - pos));

        size_t close = message.find('}', open + 1);
        if (close == std::string::npos) {
            
            result.append(message.substr(open));
            break;
        }

        //work on what is in the '{'
        std::string token = message.substr(open + 1, close - open - 1);
        auto parts = FileParser::splitString(token, '.');

        std::string replacement;

        if (parts.size() == 1) {
            // {attribute}
            auto it = startNode->nodeAttributes.find(parts[0]);
            if (it != startNode->nodeAttributes.end())
                replacement = it->second;
        }
        else if (parts.size() == 2) {
            // {node.attribute}
             std::shared_ptr<ASTreeNode> node = nullptr;
            if (parts[0] == "self") {
                node = self;
            }
            else{
                node = ASTManager::getInstance()
                    .findNodeWithTagName(parts[0], startNode);
            }

            if (node) {
                auto it = node->nodeAttributes.find(parts[1]);
                if (it != node->nodeAttributes.end())
                    replacement = it->second;
            }
        }
        else if (parts.size() >= 3) {
            // {tag.name.attribute}
            std::shared_ptr<ASTreeNode> node = nullptr;
            if (parts[0] == "self") {
                //this condenses a {self.name.attribute} to {self.attribute}
                node = self;
            }
            node = ASTManager::getInstance()
                .findNodeWithTagandName(parts[0], parts[1], startNode);

            if (node) {
                auto it = node->nodeAttributes.find(parts[2]); 
                //NOTE: If you use self in this case, i.e {self.name.attribute}, 
                //it would be condensed down to {self.attribute}
                if (it != node->nodeAttributes.end())
                    replacement = it->second;
            }
        }

        // Fallback: keep original if unresolved
        if (replacement.empty()) {
            result.append("{").append(token).append("}");
        }
        else {
            result.append(replacement);
        }

        pos = close + 1;
    }

    return result;
}

bool Logger::startLoggerParse(std::shared_ptr<ASTreeNode> startNode, std::shared_ptr<ASTreeNode> currNode) {
    if (currNode->nodeAttributes.count("message")) {
        std::string parsed =
            parseTemplateMessage(currNode->nodeAttributes["message"], startNode, currNode);

        printQueue.push_back(parsed);
    }

    for (auto& node : currNode->GetChildren())
        startLoggerParse(startNode, node);

}


void Logger::PrintLogQueues() {
	for (auto& logData : printQueue) {
		Logger::log(logData);
	}
}

Logger::Logger(){}