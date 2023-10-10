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


class StringUtils {
public:

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