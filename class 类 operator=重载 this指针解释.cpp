#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;

class Person
{
public:
	Person(int age)
	{
		m_Age = new int(age);      //new -- p1 p2 的m_Age 都在堆区
	}
	~Person()
	{
		if (m_Age != NULL)
		{
			delete m_Age;
			m_Age = NULL;
		}
	}
	Person& operator=(Person& p)
	{
		//编译器提供的浅拷贝
		//m_Age = p.m_Age;

		//应该先判断 是否有属性开辟在堆区，如果有，先释放干净，然后再进行深拷贝;
		if (m_Age != NULL)
		{
			delete m_Age;
			m_Age = NULL;
		}
		//深拷贝
		m_Age = new int(*p.m_Age);
		//连等操作，返回对象本身;
		return *this;
		//return *this的作用  是返回当前对象的引用，这是为了支持链式赋值（例如 p3 = p2 = p1;）
		//为了理解为什么不能直接写 return this;   我们需要区分      this指针与它所指向的对象：
		//this 是一个指针（Pointer），存储的是当前对象的内存地址（类型为 Person* ）
		//*this 是对象本身（Object），是通过对指针进行解引用操作获取的当前对象实例（类型为 Person& ）
		//1
		//如果写 return this;  实际返回的是 Person*（指针）,这与函数定义的返回类型（引用）不匹配, 编译器会报错。
		//写 return *this; 返回的是对象本身，符合返回类型 Person&.

		//2. 支持链式赋值操作
		//  赋值运算符需要支持像 p3 = p2 = p1; 这样的连续赋值操作：
		//	首先执行 p2 = p1; ，完成赋值后，需要返回一个对象本身（即 p2 的引用）供下一次操作使用;
		//	然后执行 p3 = (p2 的引用) 完成赋值。
		//	如果返回的是指针，就* 无法 *进行这种标准的链式操作。
	}
	int* m_Age;
};


void test01()
{
	Person p1(18);
	Person p2(20);
	Person p3(21);
	p3 = p2 = p1;
	//会出现浅拷贝问题，造成堆区内存重复释放;m_Age 指向的内存 会被重复释放 两次; 
	//解决方法：自己写一个等号赋值操作[重载 等号] operator=   防止编译器自己造成浅拷贝;
	cout << "p1 的年龄: " << *p1.m_Age << endl;
	cout << "p2 的年龄: " << *p2.m_Age << endl;
	cout << "p3 的年龄: " << *p3.m_Age << endl;
}

int main()
{
	test01();
	return 0;
}
