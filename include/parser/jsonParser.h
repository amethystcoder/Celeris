#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <regex> //Notes: will use later for tests
#include <variant>

/// /////////////////////////////////////////////////////
/// 
/// Parse Json text and return an internal map representation
/// 
/// 
/// 
/// 
/// ///////////////////////////////////////////////////////
class JsonParser
{
public:

	static JsonParser& Instance() {
		static JsonParser instance;
		return instance;
	}

	/// /////////////////////////////////////////////////////
	/// 
	/// Steps for parsing the json
	///		: json would possibly be parsed recursively
	///		: 
	/// 
	/// /////////////////////////////////////////////////////
	

	static void parse() const noexcept;

	static void parseShape() const noexcept;

	static bool confirmTypes() const; 

private:
	JsonParser();
	~JsonParser();
};

#endif // !JSON_PARSER_H
