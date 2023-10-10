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
	ClangParser(std::string _target_parse_json_file, std::string _temp_folder, std::string _clang_include_folder);
	int parse();
	int generateFiles();

private:
	bool GenerateParseHeader();

private:
	std::string target_parse_json_file_path;
	std::string temp_folder_path;
	std::string parse_header_path;
	std::string clang_include_folder;

};


class ClangCursorUtils {
public:
	static void ToString(const CXString& str, std::string& output)
	{
		auto cstr = clang_getCString(str);

		output = cstr;

		clang_disposeString(str);
	}

	static std::string GetDisplayName(CXCursor cursor)
	{
		std::string display_name;

		ToString(clang_getCursorDisplayName(cursor), display_name);

		return display_name;
	}

	static std::string GetSourceFile(CXCursor cursor)
	{
		auto range = clang_Cursor_getSpellingNameRange(cursor, 0, 0);

		auto start = clang_getRangeStart(range);

		CXFile   file;
		unsigned line, column, offset;

		clang_getFileLocation(start, &file, &line, &column, &offset);

		std::string filename;

		ToString(clang_getFileName(file), filename);

		return filename;
	}
};