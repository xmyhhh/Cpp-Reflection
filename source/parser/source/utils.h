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


class StringUtils {
public:

    static bool contain(std::string input, std::string sub) {
        return input.find(sub) != std::string::npos;
    }

    static bool contain_attr(std::vector<std::string> input, std::string sub) {
        for (const auto & i : input) {
            if (i == sub) {
                return true;
            }
        }
        return false;
    }

   static std::vector<std::string> Split(std::string input, std::string pat)
    {
        std::vector<std::string> ret_list;
        while (true)
        {
            size_t      index = input.find(pat);
            std::string sub_list = input.substr(0, index);
            if (!sub_list.empty())
            {

                ret_list.push_back(sub_list);
            }
            input.erase(0, index + pat.size());
            if (index == -1)
            {
                break;
            }
        }
        return ret_list;
    }

   static std::string Replace(std::string& source_string, std::string sub_string, const std::string new_string)
   {
       std::string::size_type pos = 0;
       while ((pos = source_string.find(sub_string)) != std::string::npos)
       {
           source_string.replace(pos, sub_string.length(), new_string);
       }
       return source_string;
   }

   static std::string Replace(std::string& source_string, char taget_char, const char new_char)
   {
       std::replace(source_string.begin(), source_string.end(), taget_char, new_char);
       return source_string;
   }

   static  std::string ToUpper(std::string& source_string)
   {
       transform(source_string.begin(), source_string.end(), source_string.begin(), ::toupper);
       return source_string;
   }

   static std::string Remove(std::string& source_string, char taget_char)
   {
       source_string.erase(remove(source_string.begin(), source_string.end(), taget_char), source_string.end());
       return source_string;
   }


};

class FileUtils {
public:
   static std::string GetFileName(std::string path)
    {
        if (path.size() < 1)
        {
            return std::string();
        }
        std::vector<std::string> result = StringUtils::Split(path, "/");
        if (result.size() < 1)
        {
            return std::string();
        }
        return result[result.size() - 1];
    }


   static std::string GetFilePath(std::string path)
   {
       if (path.size() < 1)
       {
           return std::string();
       }

       std::vector<std::string> result = StringUtils::Split(path, "/");

       if (result.size() < 1)
       {
           return std::string();
       }

       std::string  ret_string = result[0];
       for (int index = 1; index < result.size()-1; ++index)
       {
           ret_string += "/" + result[index];
       }

       return ret_string;
   }
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
