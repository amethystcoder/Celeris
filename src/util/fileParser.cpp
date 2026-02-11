#include "fileParser.h"
#include <iostream>


FileParser::FileParser(const char* filename)
{
	fileRead.open("", std::ios::binary | std::ios::ate);
	if (fileRead.fail()) return;

	std::streamsize size = fileRead.tellg();
	if (size == -1) return;
	fileRead.seekg(0, std::ios::beg);

	std::string content(size, 0);
	fileRead.read(&content[0], size);
	fileContent = content;
}

FileParser::~FileParser()
{
	fileRead.close();
}

bool FileParser::check_is_html(const std::string& filename) {
	//check if the file is a valid html file
	//valid files have the .html extension
	std::string file = filename;
	if (file.find(".html") == std::string::npos) return false;
	return true;
}

bool FileParser::check_is_file(const std::string& filename) {
	std::filesystem::path path(filename);
	auto ext = path.extension();

	// Check if extension is non-empty and longer than just '.'
	return !ext.empty() && ext.string() != ".";
}

std::map<std::string, std::string> FileParser::parseAttributes(const std::string& input) {
	std::map<std::string, std::string> result;
	std::regex pattern(R"((\w+)='([^']*)'|(\w+))");
	std::smatch match;
	std::string str = input;

	while (std::regex_search(str, match, pattern)) {
		if (match[1].matched) { // Matched key='value' pattern
			result[match[1]] = match[2];
		}
		else if (match[3].matched) { // Matched standalone key
			result[match[3]] = ""; // Assign empty value
		}
		str = match.suffix();
	}

	return result;
}

void FileParser::parseStringTemplateLiteral(const std::string& input) {
	//parse the string template literal
	//this is a simple implementation that does not handle all cases
	//it is just a placeholder for now
	//instead of using a string, use a map
	//this is just a note to myself
	std::vector<std::vector<std::string>> result;
	std::regex pattern(R"(\{([^{}]+)\})");
	std::smatch match;
	std::string str = input;

	while (std::regex_search(str, match, pattern)) {
		std::string content = match[1];
		//get the string between the curly braces
		content.replace(content.find("{"), 1, "");
		content.replace(content.find("}"), 1, "");
	}
}

//delimiter is char to ensure that one character is used i.e '.'.
//There would be another function like this that would deal with two or more characters
std::vector<std::string> FileParser::splitString(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

TagDataList FileParser::parse_html_content(std::string& html_text) {
	html_text.erase(std::remove(html_text.begin(), html_text.end(), '\r'), html_text.end());
	html_text.erase(std::remove(html_text.begin(), html_text.end(), '\n'), html_text.end());

	std::stringstream ss(html_text);
	TagDataList tag_list;
	short depth = 0;
	bool in_tag = false;

	char c;
	std::string nameAndAttributes;
	std::string content;

	while (ss.get(c)) {
		if (depth == 0 && c != OPENING_TAG) continue;

		if (c == OPENING_TAG && ss.peek() != '/') {
			// Start of new tag
			depth = 1;
			in_tag = true;

			// Read tag name and attributes until '>'
			nameAndAttributes.clear();
			bool isSelfClosing = false;
			while (ss.get(c) && c != CLOSING_TAG) {
				if (c == '/' && ss.peek() == CLOSING_TAG) {
					isSelfClosing = true;
					ss.get();
					break;
				}
				else nameAndAttributes += c;
			}

			// Clean trailing '/' from nameAndAttributes if self-closing
			if (isSelfClosing && !nameAndAttributes.empty() && nameAndAttributes.back() == '/') {
				nameAndAttributes.pop_back();
			}

			// Parse tag name and attributes
			HTMLTagData tagData;
			auto space_pos = nameAndAttributes.find(' ');
			if (space_pos == std::string::npos) {
				tagData.tag = nameAndAttributes;
				tagData.attributes = "";
			}
			else {
				tagData.tag = nameAndAttributes.substr(0, space_pos);
				tagData.attributes = nameAndAttributes.substr(space_pos + 1);
			}

			// Handle content only if not self-closing
			if (!isSelfClosing) {
				content.clear();
				std::string endTagName = tagData.tag;

				while (ss.get(c)) {
					if (c == OPENING_TAG && ss.peek() == '/') {
						ss.get(); // consume '/'
						std::string maybeEndTag;
						while (ss.get(c) && c != CLOSING_TAG) {
							maybeEndTag += c;
						}

						if (maybeEndTag == endTagName) break;
						else content += "</" + maybeEndTag + ">";
					}
					else {
						content += c;
					}
				}

				tagData.content = content;
			}
			else {
				tagData.content = ""; // self-closing tags have no content
			}

			tag_list.emplace_back(tagData);

			// Reset state
			in_tag = false;
			depth = 0;
		}
	}

	return tag_list;
}

std::map<std::string, std::string> FileParser::parseJSON(const std::string& raw_json) {
	std::map<std::string, std::string> json_map;
	std::regex pattern(R"(\s*\"([^\"]+)\"\s*:\s*\"([^\"]+)\"\s*)");
	std::smatch match;
	std::string str = raw_json;

	while (std::regex_search(str, match, pattern)) {
		json_map[match[1]] = match[2];
		str = match.suffix();
	}

	return json_map;
}

std::string FileParser::readHtmlFile(const std::string& html_file){
	if (!check_is_html(html_file)) return "";
	std::ifstream file(html_file);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

std::string FileParser::readFile(const std::string& file_name) {
	std::ifstream file(file_name);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

std::stringstream FileParser::readHtmlFileAsBuffer(const std::string& html_file) {
	std::ifstream file(html_file);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer;
}

//html file like index.html
TagDataList FileParser::parse_html_file(const std::string& html_file)
{
	std::string html_text = readHtmlFile(html_file);
	if (html_text == "") return TagDataList();
	return parse_html_content(html_text);
}

TagDataList FileParser::determineParseType(std::string& input) {
	//check if the input is a file or a string
	//if the input is a file then return the parsed file content
	//if the input is a string then return the parsed string content
	//if the input is neither a file nor a string then return an empty string
	return input.size() >= 5 && input.compare(input.size() - 5, 5, ".html") == 0
		? parse_html_file(input)
		: parse_html_content(input);
}
