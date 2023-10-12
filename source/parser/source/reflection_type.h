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

#include "utils.h"

class RFunction;
class RClass;
class RField;

class TypeBase {

};


namespace ReflectionProperty
{
	const auto Class_AnnotateAttr = "reflect-class";

	namespace Class_AnnotateAttrProperty
	{
		const auto ReflectAll = "auto";
	}

	const auto All = "All";

	const auto Fields = "Fields";
	const auto Methods = "Methods";

	const auto Enable = "Enable";
	const auto Disable = "Disable";

	const auto WhiteListFields = "WhiteListFields";
	const auto WhiteListMethods = "WhiteListMethods";

} // namespace NativeProperty

class RClass :public TypeBase {
public:
	RClass(CXCursor _current_cursor, const std::vector<std::string>& propertyArray) {
		if (propertyArray[0] == ReflectionProperty::Class_AnnotateAttrProperty::ReflectAll) {

			clang_visitChildren(
				_current_cursor,
				[](CXCursor current_cursor, CXCursor parent, CXClientData data) {
					switch (clang_getCursorKind(current_cursor)) {
					case CXCursor_FieldDecl: {
						std::cerr << "find Field: " << ClangCursorUtils::GetDisplayName(current_cursor) << std::endl;
						auto res = static_cast<RClass*>(data);
						res->AddField(std::make_unique<RField>(current_cursor));
						break;
					}
					case CXCursor_CXXMethod: {
						std::cerr << "find Function: " << ClangCursorUtils::GetDisplayName(current_cursor) << std::endl;
						auto res = static_cast<RClass*>(data);
						res->AddFunction(std::make_unique<RFunction>(current_cursor));
						break;
					}
					}

					return CXChildVisit_Recurse;
				},
				this
			);
		}
	}

	void AddField(std::unique_ptr<RField> field)
	{
		m_fields.push_back(field);
	}

	void AddFunction(std::unique_ptr<RFunction> function)
	{
		m_functions.push_back(function);
	}
private:
	std::vector<std::unique_ptr<RField>> m_fields;
	std::vector< std::unique_ptr<RFunction>> m_functions;
};

class RFunction :public TypeBase {
public:
	RFunction(CXCursor _current_cursor) {

	}
};

class RField :public TypeBase {
public:
	RField(CXCursor _current_cursor) {

	}
};