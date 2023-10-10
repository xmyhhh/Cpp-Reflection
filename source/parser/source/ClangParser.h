#pragma once


#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <clang-c/Index.h>

class ClangParser {
public:
	ClangParser(std::string _target_parse_json_file,std::string _temp_folder);
	int parse();
	int generateFiles();

private:
	bool GenerateParseHeader();
private:
	std::string target_parse_json_file_path;
	std::string temp_folder_path;
	std::string parse_header_path;

	CXIndex           m_index;
	CXTranslationUnit m_translation_unit;
};