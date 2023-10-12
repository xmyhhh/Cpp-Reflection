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

#include "reflection_type.h"



class RClass;

class ClangParser {
public:
	ClangParser(std::string _target_parse_json_file, std::string _temp_folder, std::string _clang_include_folder);
	int Parse();
	int GenerateFiles();

private:
	bool GenerateParseHeader();

private:
	std::string target_parse_json_file_path;
	std::string temp_folder_path;
	std::string parse_header_path;
	std::string clang_include_folder;

	std::vector<std::unique_ptr<RClass>> m_class;

};


