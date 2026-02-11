#ifndef FILEPARSER_H
#define FILEPARSER_H

//some tag closing definitions
#define CLOSING_TAG '>'
#define OPENING_TAG '<'

#include <map>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>

struct HTMLTagData {
	std::string tag;
	std::string attributes;
	std::string content;
};

typedef std::vector<HTMLTagData> TagDataList;


//utility function
class FileParser
{
public:
	FileParser(const char* filename);
	~FileParser();

	FileParser& operator = (const FileParser&) = delete;
	FileParser(const FileParser&) = delete;

	static bool check_is_html(const std::string& filename);

	static bool check_is_file(const std::string& filename);

	static std::map<std::string, std::string> parseAttributes(const std::string& input);

	static TagDataList parse_html_content(std::string& html_text);

	//html file like index.html
	static TagDataList parse_html_file(const std::string& html_file);
	static TagDataList determineParseType(std::string& input);

	static std::string readHtmlFile(const std::string& html_file);

	static std::string readFile(const std::string& file_name);

	static std::stringstream readHtmlFileAsBuffer(const std::string& html_file);

	static void parseStringTemplateLiteral(const std::string& input);

	static std::map<std::string, std::string> parseJSON(const std::string& raw_json);

	static std::vector<std::string> splitString(const std::string& str, char delimiter);
	

private:
	std::fstream fileRead;
	std::string fileContent;
	//std::vector<std::string> allowed_extensions = { ".html", ".htm" };
};


#endif // FILEPARSER_H