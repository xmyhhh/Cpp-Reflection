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

class RFunction;
class RClass;
class RField;

class TypeBase {

};

class RClass :public TypeBase {
public:
	RClass(CXCursor current_cursor) {

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