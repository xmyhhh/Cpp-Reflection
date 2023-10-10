#include <vector>


#ifdef __REFLECT__
#define CLASS(...) class __attribute__((annotate("reflect-class:" #__VA_ARGS__)))
#define UNION(...) union __attribute__((annotate("reflect-class:" #__VA_ARGS__)))
#define PROPERTY(...) __attribute__((annotate("reflect-property:" #__VA_ARGS__)))
#define FUNCTION(...) __attribute__((annotate("reflect-function:" #__VA_ARGS__)))

#else 
#define CLASS(...) class
#define UNION(...) union
#define PROPERTY(...)
#define FUNCTION(...)

#endif 

/* ========================================================================= */
/* Public API                                                                */
/* ========================================================================= */
class Class;
struct Type;
struct Field;
struct Function;

template<class T>
struct TypeTag {};

template<class T>
struct ClassTag {};


template<class T>
Class const*
GetClassImpl(ClassTag<T>);

template<class T>
Type const*
GetTypeImpl(TypeTag<T>);

class Type
{
public:
    size_t m_size;
    char const* m_name;

};

struct Field{
public:
    Type const *m_type; 
    unsigned m_offset;
    char const *m_name;
};

struct Function{
public:
    Field const *m_returnType;
    Field const *m_parameters;
    Field const *m_parametersEnd;
    char const *m_name;
};

class Class: public Type{
public:
    Class *m_baseClass;
    Field *m_fields;
    Field *m_fieldsEnd;
    Function *m_functions;
    Function *m_functionsEnd;
    char const *m_name;

};



template<class T>
Type const * GetType() 
{
    return GetTypeImpl(TypeTag<T>{});
}


template<class T>
Class const* GetClass() noexcept
{
    return GetClassImpl(ClassTag<T>{});
}