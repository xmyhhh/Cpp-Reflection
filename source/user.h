#include <string>
#include <vector>

#include "reflection/reflection_type.h"


CLASS() User
{
    int id;
    std::string name;
    std::vector<std::string> pets;
};


Class const * GetClassImpl(ClassTag<User>)
{
    static Class clazz;
   
    return &clazz;
}
