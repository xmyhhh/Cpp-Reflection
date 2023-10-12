#include "ClangParser.h"





ClangParser::ClangParser(std::string _target_parse_json_file, std::string _temp_folder, std::string _clang_include_folder) :
	target_parse_json_file_path(_target_parse_json_file),
	temp_folder_path(_temp_folder),
	clang_include_folder(_clang_include_folder)
{

}

int ClangParser::Parse()
{
	if (GenerateParseHeader() != true) {
		return -1;
	}


	std::vector<const char*>clang_arg = { {"-x",
									   "c++",
									   "-std=c++11",
									   "-D__REFLECT__",
									   "-DNDEBUG",
									   "-D__clang__",
									   "-w",
									   "-MG",
									   "-M",
									   "-ferror-limit=0",
									   "-o clangLog.txt"} };


	std::string pre_include = "-I";
	auto m_work_paths = StringUtils::Split(clang_include_folder, ";");
	std::string sys_include_temp;
	for (int index = 0; index < m_work_paths.size(); ++index)
	{
		//TODO: fix sys_include_temp release problem
		sys_include_temp = pre_include + m_work_paths[index];
		clang_arg.push_back(sys_include_temp.c_str());
	}

	CXIndex m_index = clang_createIndex(true, 0);

	CXTranslationUnit m_translation_unit = clang_createTranslationUnitFromSourceFile(
		m_index, parse_header_path.c_str(), static_cast<int>(clang_arg.size()), clang_arg.data(), 0, nullptr);

	if (m_translation_unit == nullptr) {
		std::cerr << "Unable to parse translation unit. Quitting.\n";
		return -1;
	}

	CXCursor cursor = clang_getTranslationUnitCursor(m_translation_unit);


	clang_visitChildren(
		cursor,
		[](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {

			switch (clang_getCursorKind(current_cursor))
			{
			case CXCursor_ClassDecl://如果找到class定义
			{
				std::vector<std::string> propertyArray;
				if (ClangCursorUtils::CursorAnnotateAttrCamp(current_cursor, ReflectionProperty::Class_AnnotateAttr, propertyArray)) {
					//如果该class有AnnotateAttr，并且是class AnnotateAttr
					std::cerr << "find class: " << ClangCursorUtils::GetDisplayName(current_cursor) << std::endl;
					std::unique_ptr<TypeBase> type = std::make_unique<RClass>(current_cursor, propertyArray);
					
				}
				else {

				}

				break;
			}
			default:
				break;
			}

			return CXChildVisit_Recurse;
		},
		nullptr
	);

	clang_disposeTranslationUnit(m_translation_unit);
	clang_disposeIndex(m_index);

	return 0;
}

int ClangParser::GenerateFiles()
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

	parse_header_path = temp_folder_path + "/parser_header_tmp.h";

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
