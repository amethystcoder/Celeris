#ifndef SHAPE_H
#define SHAPE_H

#include "ast/ast.h"

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
class ShapeNode
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
	/// : What kind of tree? like a B+ tree? cannot be a binary type of tree.
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

};

#endif // !SHAPE_H
