
#include "reflection/reflection_type.h"




CLASS(auto) User
{
	PROPERTY()
		int id;
	FUNCTION()
		void add() {

	}

};

CLASS(auto) Shop
{
	PROPERTY()
		int id;
	PROPERTY()
		int Goods_num;
	FUNCTION()
	void b() {

	}
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
