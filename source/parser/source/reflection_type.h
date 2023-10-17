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


class TypeBase
{
public :
    TypeBase(CXCursor _current_cursor)
    {
        m_name = ClangCursorUtils::GetCXCursorName(_current_cursor);
    }

    const std::string GetNmae()
    {
        return m_name;
    }

protected:
    std::string m_name;
};

class RField : public TypeBase
{
public:
    RField(CXCursor _current_cursor): TypeBase(_current_cursor)
    {
        field_type_name = ClangCursorUtils::GetCXTypeName(clang_getCursorType(_current_cursor));
    }

    std::string GetFiledTypeName()
    {
        return field_type_name;
    }

private:
    std::string field_type_name;
};

class RFunction : public TypeBase
{
public:
    RFunction(CXCursor _current_cursor): TypeBase(_current_cursor)
    {
    }
};

class RClass : public TypeBase
{
public:
    RClass(CXCursor _current_cursor): TypeBase(_current_cursor)
    {
        std::vector<std::string> propertyArray;
        ClangCursorUtils::GetCursorAnnotateAttrProperty(_current_cursor, ReflectionProperty::Class_AnnotateAttr,
                                                        propertyArray);
        if (propertyArray[0] == ReflectionProperty::Class_AnnotateAttrProperty::ReflectAll)
        {
            clang_visitChildren(
                _current_cursor,
                [](CXCursor current_cursor, CXCursor parent, CXClientData data)
                {
                    switch (clang_getCursorKind(current_cursor))
                    {
                    case CXCursor_FieldDecl:
                        {
                            std::cerr << "find Field: " << ClangCursorUtils::GetCXCursorName(current_cursor) <<
                                std::endl;
                            auto res = static_cast<RClass*>(data);
                            res->AddField(std::make_shared<RField>(current_cursor));
                            break;
                        }
                    case CXCursor_CXXMethod:
                        {
                            // std::cerr << "find Function: " << ClangCursorUtils::GetDisplayName(current_cursor) <<
                            //     std::endl;
                            // auto res = static_cast<RClass*>(data);
                            // res->AddFunction(std::make_shared<RFunction>(current_cursor));
                            break;
                        }
                    }

                    return CXChildVisit_Recurse;
                },
                this
            );
        }
    }

    void AddField(std::shared_ptr<RField> field)
    {
        m_fields.push_back(field);
    }

    void AddFunction(std::shared_ptr<RFunction> function)
    {
        m_functions.push_back(function);
    }


    const std::vector<std::shared_ptr<RField>> GetField()
    {
        return m_fields;
    }

    const std::vector<std::shared_ptr<RFunction>> GetFunction()
    {
        return m_functions;
    }

private:
    std::vector<std::shared_ptr<RField>> m_fields;
    std::vector<std::shared_ptr<RFunction>> m_functions;
};
