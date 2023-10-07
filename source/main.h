
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name

#define REFLECTION_TYPE(class_name) \
    namespace Reflection \
    { \
        namespace TypeFieldReflectionOparator \
        { \
            class Type##class_name##Operator; \
        } \
    };

#define REFLECTION_BODY(class_name) \
    friend class Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class Serializer;


#include <vector>

REFLECTION_TYPE(BaseTest)
CLASS(BaseTest, Fields)
{
    REFLECTION_BODY(BaseTest);

public:
    int               m_int;
    std::vector<int*> m_int_vector;
};


REFLECTION_TYPE(Test1)
CLASS(Test1 : public BaseTest, WhiteListFields)
{
    REFLECTION_BODY(Test1);

public:
    META(Enable)
    char m_char;
};


REFLECTION_TYPE(Test2)
CLASS(Test2 : public BaseTest, , Fields)
{
    REFLECTION_BODY(Test2);


};
