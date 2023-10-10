#include "ClangParser.h"
#include "utils.h"


ClangParser::ClangParser(std::string _target_parse_json_file, std::string _temp_folder) :
	target_parse_json_file_path(_target_parse_json_file),
	temp_folder_path(_temp_folder)
{

}

int ClangParser::parse()
{
	if (GenerateParseHeader() != true) {
		return -1;
	}


	std::vector<const char*>clang_arg = { {"-x",
									   "c++",
									   "-std=c++11",
									   "-D__REFLECTION_PARSER__",
									   "-DNDEBUG",
									   "-D__clang__",
									   "-w",
									   "-MG",
									   "-M",
									   "-ferror-limit=0",
									   "-o clangLog.txt"} };

	m_index = clang_createIndex(true, 0);

	m_translation_unit = clang_createTranslationUnitFromSourceFile(
		m_index, parse_header_path.c_str(), static_cast<int>(clang_arg.size()), clang_arg.data(), 0, nullptr);
	auto cursor = clang_getTranslationUnitCursor(m_translation_unit);



	return 0;
}

int ClangParser::generateFiles()
{
	return 0;
}

bool ClangParser::GenerateParseHeader()
{

	bool result = true;

	std::fstream include_txt_file(target_parse_json_file_path, std::ios::in);

	if (include_txt_file.fail())
	{
		std::cout << "Could not load file: " << target_parse_json_file_path << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << include_txt_file.rdbuf();

	std::string context = buffer.str();

	auto         inlcude_files = StringUtils::Split(context, ";");
	std::fstream include_file;

	parse_header_path = temp_folder_path + "/parse_header_tmp.h";

	include_file.open(parse_header_path, std::ios::out);
	if (!include_file.is_open())
	{
		std::cout << "Could not craete the parse header tmp file in folder: " << temp_folder_path << std::endl;
		return false;
	}
	std::string output_filename = FileUtils::GetFileName(parse_header_path);

	std::cout << "Generate Parse Header file <" << output_filename << "> in folder : " << temp_folder_path << std::endl;

	if (output_filename.empty())
	{
		output_filename = "META_INPUT_HEADER_H";
	}
	else
	{
		StringUtils::Replace(output_filename, ".", "_");
		StringUtils::Replace(output_filename, " ", "_");
		StringUtils::ToUpper(output_filename);
	}
	include_file << "#ifndef __" << output_filename << "__" << std::endl;
	include_file << "#define __" << output_filename << "__" << std::endl;

	for (auto include_item : inlcude_files)
	{
		std::string temp_string(include_item);
		StringUtils::Replace(temp_string, '\\', '/');
		StringUtils::Remove(temp_string, '\n');
		include_file << "#include  \"" << temp_string << "\"" << std::endl;
	}

	include_file << "#endif" << std::endl;
	include_file.close();
	return result;

	return true;
}
