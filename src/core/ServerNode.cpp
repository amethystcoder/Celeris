#include "ServerNode.h"

ServerNode::ServerNode()
{
	this->port = 8080;
	this->cleanSocket = new CleanSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->serverSock = AmthSocket::ServerSocket();
}

ServerNode::~ServerNode()
{
	delete this->cleanSocket;
}

void ServerNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
	this->setPort(this->nodeAttributes["port"]);
	this->serverSock.listenforConnections(this->cleanSocket, this->address, this->port);//start up the server immediately
}

//this would or should not throw any execptions 
//it would return false if the port is not a number
bool ServerNode::setPort(const std::string& port) noexcept {
	//check that the string port actually has numbers in it
	if (port.find_first_not_of("0123456789") != std::string::npos) {
		return false;
	}
	int minPort = std::stoi(port);
	if (minPort < 0 || minPort > 65535) {
		return false;
	}
	this->port = minPort;
	return true;
}

//todos:
//generate a better regex pattern to handle self closing tags, comments, and double quotes...
//the current one does not properly handle self closing tags and double quotes

void ServerNode::sendResponse(const char* response) {
	SOCKET clientSocket = ConnectionRequest::getInstance().getSocket();
	this->serverSock.sendData(clientSocket, response);
}

ProcessEntry* ServerNode::getattachable(NodeDependencies& dependencyList)
{
	RepProcess process = [this, &dependencyList]() {
		ConnectionRequest& conReq = ConnectionRequest::getInstance();
		SOCKET clientSocket = this->serverSock.acceptConnection(this->cleanSocket);
		std::string request_data = this->serverSock.receiveData(clientSocket);
		//there should be a sort of cache to store the headers with the request data
		//there should be a proper documented comment on this function
		HTTPHeaderMap headers = HTTPTextParser::ParseRequest(request_data);
		//set connection request 
		conReq.setSocket(clientSocket);
		conReq.setHeaders(headers);
		conReq.setRoute(headers["path"]);
		conReq.setRequestMethod(headers["method"]);
		conReq.setContent(HTTPTextParser::GetRequestBody(request_data));
	};
	return new ProcessEntry(this, dependencyList, process);
}
