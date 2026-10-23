#ifndef SHAPE_H
#define SHAPE_H

#include "ast/ast.h"
#include <vector>
#include <ranges>


///////////////////////////////////////////////////////////
///
/// 
/// About Verifiers: 
///		: Verifiers are checks on internal types 
///		: They check the validity of the type 
///		: They also prevent the shape from completing
/// 
/// Need to note the structure of a verifier
///		int : [in 1 to 20]
///			: [than 1]
///			: [less 10]
///			: [without 5] 
///	
///		string : [maxlength 10] 
///			   : [contains @,$,9]
///			   : [length 10 : contains @,$,9]
///
/// //////////////////////////////////////////////////////
struct IntVerifier {
	enum class restraints{
		in, than, less, without
	};

	//TODO: Might need to check for verification for two constraints that might go against each other
	bool in(int num, int max, int min) { return num <= max && num >= min; }

	bool than(int num, int min) { return num > min; }

	bool less(int num, int max) { return num < max; }

	bool without(int num, int except) { return num != except; }
};
struct StringVerifier{
	enum class restraints {
		length, contains
	};

	//restraint functions
	bool maxlength(std::string_view item, int length) const { return item.length() <= length; }

	bool minlength(std::string_view item, int length) const { return item.length() >= length; }

	bool length(std::string_view item, int length) const { return item.length() == length; }
	//stream verifiers through restraint functions

	std::optional<std::string_view> find_missing_verifier(std::string_view item, std::span<const std::string> verifiers)
	{
		for (const auto& verifier : verifiers) {
			if (item.find(verifier) == std::string_view::npos) {
				return verifier;
			}
		}
		return std::nullopt;
	}

	bool contains(std::string_view item, std::span<const std::string> verifiers) {
		//TODO: possibly tell what verifier is needed
		return find_missing_verifier(item, verifiers) == std::nullopt;
	}

};

/// ////////////////////////////////////////////////////////
/// 
/// Internal representation of a shape.
/// Parses from json, xml, toml
/// 
/// Example :
///		: {
///			name: string
///			age: int
///		}
/// ////////////////////////////////////////////////////////
class InternalShapeRep {

	InternalShapeRep();

	//TODO: Add delete functionality for children
	~InternalShapeRep();

	using ShapeRepChildren = std::vector<std::unique_ptr<InternalShapeRep>>;

	//reverse

	//delete copy constructor
	//InternalShapeRep(InternalShapeRep& second) = delete;

	//copy operators and move operators
	InternalShapeRep operator=(InternalShapeRep& second) = delete;
	InternalShapeRep operator=(InternalShapeRep&& second) = delete;

	ShapeRepChildren children;

	Celeris::Types inner_type;

public:

	//TODO: find out if we would need to keep this a singleton
	static InternalShapeRep& getInstance() {
		static InternalShapeRep instance;
		return instance;
	}

	Celeris::Types get_type() const noexcept { return inner_type; }

	bool confirm_type() noexcept { return false; }
};

/// //////////////////////////////////////////////////////////////////
/// 
/// : The shape node stores and validates the shape
///	: of any object type stored in it
/// 
/// the structure of the object can be in any form
///		: xml, json, yaml and possibly toml as support comes for them
/// 
/// In future versions, algorithms would be built to determine
/// the kind of shape entered, but for now, 
/// a "structure" parameter would need to be included
/// 
/// /////////////////////////////////////////////////////////////////
class ShapeNode: public ASTreeNode
{
public:
	ShapeNode();
	~ShapeNode();

private:
	// Need to figure out what the shape node would use as an internal representation 
	// of the structure validator.

	/// /////////////////////////////////////////////////////////////
	/// Notes regarding the internal representation
	/// 
	/// : Since the representation might usually be in a hierachy,
	///		a tree like structure might be the best bet in this scenario
	/// 
	/// : What kind of tree? like a shema tree? cannot be a binary type of tree.
	/// : Like the type of tree found in asts?
	/// 
	/// : How do we easily validate?
	/// : Do we validate on the spot? or create a tree from the date,
	///		: and then compare two trees?
	/// 
	/// //////////////////////////////////////////////////////////////

	/// ////////////////////////////////////////////////////////////////
	/// : Stores the structure or format inside of the shape, as it appears
	///		in the html. This structure can be any of the supported formats.
	/// 
	/// : Currently supported
	///		: xml
	///		: json
	///		: yaml
	///		: toml
	/// /////////////////////////////////////////////////////////////////
	std::string structure;

	/// //////////////////////////////////////////////////////////
	/// Prints the raw structure of the shape onto the terminal
	/// ////////////////////////////////////////////////////////
	void display_structure() const noexcept;

	bool verify() const;

	void registernode(const std::string& name, const std::string& attributes, std::string& content) override;

	ProcessEntry* getattachable(NodeDependencies& dependencyList) override;

};

#endif // !SHAPE_H
