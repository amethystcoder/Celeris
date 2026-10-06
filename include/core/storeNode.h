#ifndef STORE_H
#define STORE_H

#include "ast/ast.h"
#include <array>
#include <ranges>

/// //////////////////////////////////////////////
/// Store node: Store an item dynamically sent by the server
/// Or gotten from a database. or set by a user
/// 
/// Main properties (Also determined in Node Attributes)
///	* type : the type of the object. 
/// 
/// * value : the value of this object that conforms to the type specified
/// 
/// * safe_value : if value does not conform to the data type, we can default to this value
///				 : Note that for each type, there is a certain default value to it
/// 
/// 
/// Possible Attributes (in NodeAttributes)
/// * should : What the node should do on any type mismatch. defaults to silent
///			 : error (Will terminal error and not add the item)
///			 : log (Will log to the terminal and add the default item)
///			 : silent (Will not log to the terminal and add the default item)
/// 
/// /////////////////////////////////////////////////

namespace Celeris {
	
	enum class Types {
		Integer, String, Boolean, Shape
	};

	/// //////////////////////////////////////////
	/// Contains all utility functionality to Resolve and check Int correctness
	/// /////////////////////////////////////////
	struct ResolveInt {
		// this size would check for the max capacity of the integer (using the length of the string)
		// if the the supplied integer is larger than the size, 
		// we treat the number as a 'chain' integer.
		static const short max_int_size{ 19 }; //related to a max 64 bit (will also test with cpp limits)
		static constexpr bool check_type_correctness(const std::string& item);
	};

	/// //////////////////////////////////////////
	/// Contains all utility functionality to Resolve and check String correctness
	/// /////////////////////////////////////////
	struct ResolveString {
		static constexpr bool check_type_correctness(const std::string& item); // This might end up not being needed, as there is no check for strings
	};

	/// //////////////////////////////////////////
	/// Contains all utility functionality to Resolve and check Boolean correctness
	/// /////////////////////////////////////////
	struct ResolveBoolean{
		static constexpr bool check_type_correctness(const std::string& item); //check for the correctness or truthyness of a value
	};


	enum class TypeStoreResult {
		Success, Error
	};
};


class StoreNode : public ASTreeNode {

public:
	StoreNode();
	~StoreNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content);

	ProcessEntry* getattachable(NodeDependencies& dependencies) override = 0;

    /// ////////////////////////////////////////////////////////////////////////
    /// Attempts to store an item using the provided data. The function is noexcept and will not throw exceptions.
	/// In the case that an item is provided and the type of this item does not conform with the type or shape provided,
	/// The store node defaults to the safe type, or the type
    /// 
    /// <param name="item">The item to store, provided as a string.</param>
    /// <returns>true if the item was stored successfully; false otherwise.</returns>
	/// //////////////////////////////////////////////////////////////////////////
    bool storeItem(const std::string &item) noexcept;

private:
	std::vector<RawDependency*> rawDependencies = {};

	std::string determine_default_value();


protected:

	/// /////////////////////////////////////////////////
	/// : The types set now would be integer (int) 
	///		   : string (regular string)
	///		   : boolean (true or false)
	///		   : a shape (possible object shape data type determined by a shape specifier)
	/// /////////////////////////////////////////////////
	Celeris::Types store_type;

	/// ////////////////////////////////////////////////
	/// 
	/// checks that the type of the item matches the type currently set in the store 
	/// returns false if it does not match
	///
	/// ///////////////////////////////////////////
	bool confirmStoreType(const std::string& input) const;

	/// //////////////////////////////////////////////////
	///	: Confirms the safe value if it exists
	///		: checks the type of the safe value for correctness
	///		: defaults to a random safe value
	///		
	///		Defaults :
	///			: String = ""
	///			: Integer = 0
	///			: Boolean = false or 0;
	///			: shape = {}
	///		
	/// ///////////////////////////////////////////////////
	bool confirmAndSetSafeValue() noexcept;

	//set the store type and default to string on incorrect type.
	//using the string version in attributes would be a waste for each check so assign to an enum
	Celeris::TypeStoreResult setStoreType(const std::string& type) noexcept;

	

	/// /////////////////////////////////////////////////
	/// : value of the item being stored
	///		: depending on the type set, it is usually checked on store,
	///		: and checked before use by the different type checkers
	///		
	///		:: Note that for shapes, the checker would need a shape type attached
	///		: else it would accept to any shape, even ones that are badly shaped
	///		: an example could be wrongly formatted json, or wrongly formatted data
	///		: on no shape attribute, this data would be given as is, effectively being a string
	/// /////////////////////////////////////////////////
	std::string value;

	/// /////////////////////////////////////////////////
	/// : safe default value of item being stored
	///		: if safe_value is wrong (i.e not conforming to the type set), safe value defaults to the default value
	/// 
	/// /////////////////////////////////////////////////
	std::string safe_value;
};


/// TODOS
/// Include array functionality to be able to store lists of items.
#endif // !STORE_H
