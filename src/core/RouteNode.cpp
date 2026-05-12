#include "core/RouteNode.h"


RouteNode::RouteNode()
{
}


RouteNode::~RouteNode()
{
}

void RouteNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
	setParams();
}

inline void RouteNode::setParams() noexcept {
	this->endpoint = endpoint = this->nodeAttributes["endpoint"];
	this->rescontenttype = this->nodeAttributes["rescontenttype"];
	this->method = this->nodeAttributes["method"];
	this->response = this->setEndpointContent(this->nodeAttributes["response"]);
}

std::string RouteNode::getFullResponse() noexcept {
	return this->determineResponseStatus() + this->determineContentType() + this->determineResponse();
}

std::string RouteNode::determineResponseStatus() noexcept {
	this->method = this->nodeAttributes["method"];
	if (this->method == "GET") return "HTTP/1.1 200 OK\n";
	else if (this->method == "POST") return "HTTP/1.1 201 Created\n";
	else if (this->method == "PUT") return "HTTP/1.1 204 No Content\n";
	else if (this->method == "DELETE") return "HTTP/1.1 204 No Content\n";
	else return "HTTP/1.1 200 OK\n";
}

std::string RouteNode::determineResponse() {
	//check that response is not an html file
	return this->response;
}

std::string RouteNode::determineContentType() noexcept {
	this->rescontenttype = this->nodeAttributes["rescontenttype"];
	return "Content-Type: " + MimeTypes::getInstance().getMimeType(this->rescontenttype);
}

std::string RouteNode::getEndpoint() {
	return this->endpoint;
}

std::string RouteNode::getMethod() {
	return this->method;
}

std::string RouteNode::getResponse() {
	return this->response;
}

std::string RouteNode::getResContentType() {
	return this->rescontenttype;
}

std::string RouteNode::setEndpointContent(const std::string& content) {
	std::string filecontent = content;
	bool isFile = FileParser::check_is_file(content);
	if (isFile) {
		std::filesystem::path responseFilePath = ASTManager::getInstance().getMainPath() / content;
		std::filesystem::path responsePath = std::filesystem::weakly_canonical(responseFilePath);
		this->nodeAttributes["response"] = responsePath.string();
		filecontent = FileParser::readFile(content);
		if (!std::filesystem::exists(responsePath)) {
			std::cout << "File not found: " << responsePath.string() << std::endl;
			filecontent = "<html><body><h1>404 Not Found</h1></body></html>";
		}
	}
	return filecontent;
}

ProcessEntry* RouteNode::getattachable(NodeDependencies& dependencyList)
{
	RepProcess process = [this,&dependencyList]() {
		// Default implementation does nothing
		ConnectionRequest& conReq = ConnectionRequest::getInstance();

		if (conReq.getRoute() == this->getEndpoint() && conReq.getRequestMethod() == this->getMethod()) {

			struct RawDependency rawdep { "server", "" };
			ServerNode* ServerApplication = static_cast<ServerNode*>(this->getDependency(&rawdep));
			
			//check if the route has a rate limit
			if (this->nodeAttributes.find("rateLimit") != this->nodeAttributes.end()) {
				//we need to give a proper reference to a rate limit node in a route node by giving it a more appropriate attribute
				struct RawDependency rateLimitDep { "ratelimit", this->nodeAttributes["rateLimit"] };
				RateLimitNode* rateLimitNode = static_cast<RateLimitNode*>(this->getDependency(&rateLimitDep));
				if (rateLimitNode == nullptr) {
					ServerApplication->sendResponse("HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\n\n<html><body><h1>500 Internal Server Error</h1></body></html>");
					return;
				}
				rateLimitNode->addNewIpaddress(conReq.getIpAddress());

				if (rateLimitNode->isRateLimited(conReq.getIpAddress())) {
					//response to the client if the ip address is in the rate limit
					ServerApplication->sendResponse("HTTP/1.1 429 Too Many Requests\nContent-Type: text/html\n\n<html><body><h1>429 Too Many Requests</h1></body></html>");
					//continue;
				}
			}
			//Send the response
			ServerApplication->sendResponse(this->getFullResponse().c_str());
			//else this->serverSock.sendData(clientSocket, "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body><h1>404 Not Found</h1></body></html>");//send a 404 response
		}
	};
	return new ProcessEntry(this, dependencyList, process);
}
