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
#include "utils.h"

namespace ReflectionProperty
{
	const auto Class_AnnotateAttr = "reflect-class";

	const auto All = "All";

	const auto Fields = "Fields";
	const auto Methods = "Methods";

	const auto Enable = "Enable";
	const auto Disable = "Disable";

	const auto WhiteListFields = "WhiteListFields";
	const auto WhiteListMethods = "WhiteListMethods";

} // namespace NativeProperty


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

	static  std::vector<CXCursor> GetChildCursor(CXCursor current_cursor) {
		std::vector<CXCursor> res;
		clang_visitChildren(
			current_cursor,
			[](CXCursor current_cursor, CXCursor parent, CXClientData data) {
				auto res = static_cast<std::vector<CXCursor>*>(data);

				res->emplace_back(current_cursor);

				if (current_cursor.kind == CXCursor_LastPreprocessing)
					return CXChildVisit_Break;
				return CXChildVisit_Continue;
			},
			&res
		);
		return res;
	};

	static bool CursorAnnotateAttrCamp(CXCursor current_cursor, std::string camp_AnnoateAttr, std::vector<std::string>& propertyArray) {
		for (auto& child : GetChildCursor(current_cursor)) {
			if (child.kind != CXCursor_AnnotateAttr) {
				continue;
			}
			else {
				std::string property = ClangCursorUtils::GetDisplayName(child);
				if (StringUtils::contain(property, camp_AnnoateAttr)) {
					propertyArray = StringUtils::Split(StringUtils::Replace(property, camp_AnnoateAttr + ":", ""), ",");
					return true;
				}

			}
		}
		return false;
	}
};