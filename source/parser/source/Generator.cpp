#include "Generator.h"

int RefelactionGenerator::Generate(const std::vector<std::shared_ptr<RClass>>& m_class)
{
    for (auto& m_class_item : m_class)
    {
        std::string generate_file_name = out_path + "/" + m_class_item->GetNmae() + ".gen.hpp";
        std::string out_string = "";
        out_string += "#pragma once\n";
        out_string += "#include \"reflection/reflection_type.h\"\n";
        out_string += "class " + m_class_item->GetNmae() + "RefelcationType {\n";
        for (auto tmp : m_class_item->GetField())
        {
            out_string += FieldGenerate(tmp);
        }

        out_string += "\n};\n";

        FileUtils::SaveFile(out_string, generate_file_name);

        std::cout << "///////////////////" << std::endl;
        std::cout << out_string;
    }
    return 0;
}


std::string RefelactionGenerator::FieldGenerate(std::shared_ptr<RField> field)
{
    std::string filed_name = field->GetNmae();
    std::string filed_type_name = field->GetFiledTypeName();
    RClass* filed_parent_class_ptr = field->GetParentClass();
    std::string out_string = "";
    out_string += "static const char* GetFiledName_" + filed_name + "(){return " + StringUtils::DoubleQuote(filed_name)
        + ";}\n";
    out_string += "static void Set_" + filed_name + "(){return " + StringUtils::DoubleQuote(filed_type_name)
        + ";}\n";

    out_string += "static const char* GetFiledName_" + filed_name + "(void* instance, void* value){" + "static_cast<" +
        filed_parent_class_ptr->GetNmae() + "*>(instance)->x = *static_cast<" + filed_type_name + "*>(value)"
        + ";}\n";
    return out_string;
}
