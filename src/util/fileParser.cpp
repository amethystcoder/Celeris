#include "util/fileParser.h"
#include <iostream>


FileParser::FileParser(const char* filename)
{
	fileRead.open(filename, std::ios::binary | std::ios::ate);
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
	size_t position_of_last_separator = filename.find_last_of(".");
	if (position_of_last_separator == std::string::npos) return false;
	std::string extension = filename.substr(position_of_last_separator + 1);
	if (MimeTypes::getInstance().mime_type_exists(extension)) return true;
	return false;
}

std::map<std::string, std::string> FileParser::parseAttributes(const std::string& input) {
    std::map<std::string, std::string> result;

	size_t pos{ 0 };
    const size_t len = input.size();

    while (pos < len) {
        while (pos < len && std::isspace(static_cast<unsigned char>(input[pos]))) {
            ++pos;
        }
        if (pos >= len) break;

        std::string key;
        while (pos < len && !std::isspace(static_cast<unsigned char>(input[pos])) && input[pos] != '=') {
            key.push_back(input[pos++]);
        }

        while (pos < len && std::isspace(static_cast<unsigned char>(input[pos]))) {
            ++pos;
        }

        std::string value;
        if (pos < len && input[pos] == '=') {
            ++pos;
            while (pos < len && std::isspace(static_cast<unsigned char>(input[pos]))) {
                ++pos;
            }

            if (pos < len && (input[pos] == '"' || input[pos] == '\'')) {
                char quote = input[pos++];
                while (pos < len && input[pos] != quote) {
                    value.push_back(input[pos++]);
                }
                if (pos < len && input[pos] == quote) {
                    ++pos;
                }
            } else {
                while (pos < len && !std::isspace(static_cast<unsigned char>(input[pos]))) {
                    value.push_back(input[pos++]);
                }
            }
        }

        if (!key.empty()) {
            result[key] = value;
        }
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
	short depth { 0u };
	bool in_tag{ false };

	bool possible_comment{ false }, in_comment{ false };

	char c;
	std::string nameAndAttributes;
	std::string content;

	while (ss.get(c)) {
		if (depth == 0 && c != OPENING_TAG) continue;

		//check for comments
		/////////////////////////////////////////////
		// Comments have the structure <!-- -->
		// 
		// text or code after <!-- opening tags and --> would be ignored and not parsed
		// 
		// What happens in the case of ill-formed comments?:
		//	In the case of an ill formed comment, if the ill-formed comment starts with <! ,
		// it would be ignored until its closing tag as if it were a comment, 
		// and a warning would be displayed in the terminal
		/////////////////////////////////////////////
		if (c == OPENING_TAG && ss.peek() == OPENING_POSSIBLE_COMMENT) {
			//we are possibly in a comment
			ss.get(); // consume the next part of the character !
			possible_comment = true;
			//since possible comment, check that the next parts of the comment are possibly valid
			ss.get(c);
			if (c == '-' && ss.peek() == '-'){
				in_comment = true; //in_comment and possible_comment are going to be used for possible future comment analysis and tracking
			}
			else {
				std::cout << "Malformed comment... code in malformed comment would be ignored until the next closing tag" << "\n"; //TODO: work on getting line numbers for warnings and errors
			}
			//keep ignoring comments
			while (ss.get(c) && in_comment)
			{
				//check for closing comment indicator '--'
				if (c == '-' && ss.peek() == '-')
				{
					ss.get(); //consume next -
					//check for >
					if (ss.peek() == CLOSING_TAG) {
						possible_comment = false;
						in_comment = false;
					}
				}
			}
		}



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
	//if the input is neither a file nor a string then return an empty 
	return input.size() >= 5 && input.compare(input.size() - 5, 5, ".html") == 0
		? parse_html_file(input)
		: parse_html_content(input);
}