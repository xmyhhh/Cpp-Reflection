
#include "reflection/reflection_type.h"




CLASS() User
{
    PROPERTY()
    int id;
};


class Goods
{
   
   int id;
};

//Class const * GetClassImpl(ClassTag<User>)
//{
//    static Class clazz;
//   
//    return &clazz;
//}
