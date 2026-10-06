#include "core/storeNode.h"
#include "ast/ast_manager.h"


//////////////////////////////////////////////////////////////////////////////////////
// Resolvers
/////////////////////////////////////////////////////////////////////////////////////


bool Celeris::ResolveInt::check_type_correctness(const std::string& item) {
    //check that this item is an integer or even possibly support floats
    //NOTE: will start with a brute force approach then figure out a better way after
    //Brute force approach would involve going through the string until we reach a non number
    //We do not use functions like std::stoi because we are going to include extra functionality
    //And robustness

    int i = 0;
    //check the string is empty
    if (item.size() == 0) return false;

    //try ignore leading whitespace
    while (item[i] == ' ' && i < item.size()) i++;

    //check the first character
    const char starting_char = item[i];

    const auto starting_pos = (starting_char == '-' || starting_char == '+') ? 1 : 0;

    //catch both - and + only strings. 
    if (item.size() == 1 && starting_pos == 1) return false;

    auto stop_pos = item.find_first_not_of("0123456789", starting_pos); //check for the first non digit

    return stop_pos == std::string::npos;
}


bool Celeris::ResolveString::check_type_correctness(const std::string& item){
    //strings are not strict. Might remove this to prevent the cycle moving into this function
    //no op
    return true;
}

bool Celeris::ResolveBoolean::check_type_correctness(const std::string& item) {
    //item is falsey if {0, false, empty} else it is truthy
    return !(item == "0" || item == "false" || item.size() == 0);
}




/////////////////////////////////////////////////////////////////
// Store Node
/////////////////////////////////////////////////////////////////
StoreNode::StoreNode(){}
StoreNode::~StoreNode(){}

void StoreNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
    //custom, set the type
    setStoreType(nodeAttributes["type"]);
}

ProcessEntry* StoreNode::getattachable(NodeDependencies& dependencyList){
    auto process = [this, &dependencyList](){};
    return new ProcessEntry(this, dependencyList, process);
}

Celeris::TypeStoreResult StoreNode::setStoreType(const std::string& type) {
    //check the type... else default to a string type.
    std::unordered_map < std::string, Celeris::Types > typeRelInfo{
        {"string", Celeris::Types::String},
        {"int", Celeris::Types::Integer},
        {"bool", Celeris::Types::Boolean},
        {"shape", Celeris::Types::Shape}
    };
    if (typeRelInfo.find(type) == typeRelInfo.end())
    {
        //set to a string as default
        this->store_type = Celeris::Types::String;
        return Celeris::TypeStoreResult::Error;
    }
    this->store_type = typeRelInfo[type];
    return Celeris::TypeStoreResult::Success;
}

bool StoreNode::confirmStoreType(const std::string& input) const {
    if (store_type == Celeris::Types::Boolean) return Celeris::ResolveBoolean::check_type_correctness(input);
    if (store_type == Celeris::Types::Integer) return Celeris::ResolveInt::check_type_correctness(input);
    if (store_type == Celeris::Types::String) return Celeris::ResolveString::check_type_correctness(input);
    //Will work on shape soon
    //if (store_type == Celeris::Types::Shape) return Celeris::ResolveBoolean::check_type_correctness(input);
}

bool StoreNode::storeItem(const std::string& item) noexcept {
    //check that the item is of the right type
    if (confirmStoreType(item)) {
        //just store in the value
        value = item;
    }
    else value = determine_default_value();
    return true;
}

constexpr std::string StoreNode::determine_default_value()
{

    return std::string();
}
