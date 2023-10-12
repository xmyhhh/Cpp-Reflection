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
				[](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
					switch (clang_getCursorKind(current_cursor)) {
						case CXCursor_FieldDecl: {
							std::cerr << "find Field: " << ClangCursorUtils::GetDisplayName(current_cursor) << std::endl;
							break;
						}
						case CXCursor_FunctionDecl: {
							std::cerr << "find Function: " << ClangCursorUtils::GetDisplayName(current_cursor) << std::endl;
							break;
						}
					}

					return CXChildVisit_Recurse;
				},
				nullptr
			);
		}
	}

	void AddField(RField const* field)
	{
		m_fields.push_back(field);
	}

	void
		AddFunction(RFunction const* function)
	{
		m_functions.push_back(function);
	}
private:
	std::vector<RField const*> m_fields;
	std::vector<RFunction const*> m_functions;
};

class RFunction :public TypeBase {

};

class RField :public TypeBase {

};