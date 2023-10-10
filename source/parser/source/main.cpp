
#include<string>
#include<iostream>
#include<chrono>
#include <fstream>

#include "ClangParser.h"
#include "utils.h"

int parse(std::string, std::string, std::string);


int main(int argc, char* argv[])
{

	auto start_time = std::chrono::system_clock::now();
	int  result = 0;

	if (argv[1] != nullptr)
	{
		if (argv[3] == nullptr) {
			result = parse(argv[1], argv[2], FileUtils::GetFilePath(argv[1]));
		}
		else {
			result = parse(argv[1], argv[2], argv[3]);
		}


		auto duration_time = std::chrono::system_clock::now() - start_time;
		std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration_time).count()
			<< "ms" << std::endl;
		return result;
	}
	else
	{
		std::cerr << "Arguments parse error!" << std::endl
			<< "Please call the tool like this:" << std::endl
			<< "meta_parser  project_file_name  include_file_name_to_generate  project_base_directory "
			"sys_include_directory module_name showErrors(0 or 1)"
			<< std::endl
			<< std::endl;
		return -1;
	}

	return 0;
}

int parse(std::string target_parse_json_file,
	std::string clang_include_folder,
	std::string temp_folder
)
{
	ClangParser parser(target_parse_json_file, temp_folder, clang_include_folder);


	int result = parser.Parse();
	if (0 != result)
	{
		return result;
	}

	parser.GenerateFiles();

	return 0;
}
