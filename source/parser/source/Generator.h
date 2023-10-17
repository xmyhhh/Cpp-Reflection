#pragma once
#include <memory>
#include <string>
#include <vector>
#include "reflection_type.h"



class IGenerator
{
public:
    virtual int Generate(const std::vector<std::shared_ptr<RClass>>& m_class) = 0;
};


class RefelactionGenerator : public IGenerator
{
public:
    RefelactionGenerator(std::string _out_path)
    {
        out_path = _out_path;
    }

    int Generate(const std::vector<std::shared_ptr<RClass>>& m_class) override;


private:
    std::string FieldGenerate(std::shared_ptr<RField> field);
private:
    std::string out_path;
};
