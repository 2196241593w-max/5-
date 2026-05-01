//创建了两个文件 .hpp(模板)和 源.cpp
//此处 合并了二者，可以直接在vs上创建.hpp和.cpp 在进行复制

/////文件名 MyArray.hpp
#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <iostream>
using namespace std;

template <class T>
class MyArray
{
public:
	//有参构造函数
	MyArray(int capacity)//容量
	{
		//cout << "myarray 有参构造调用" << "\n";
		this->m_Capacity = capacity;
		this->m_Size = 0;
		this->pAddress = new T[this->m_Capacity];//堆区开辟数组
	}

	//拷贝构造函数
	MyArray(const MyArray& arr)
	{
		//cout << "myarray 拷贝构造调用" << "\n";
		this->m_Capacity = arr.m_Capacity;
		this->m_Size = arr.m_Size;
		//this->pAddress = arr.pAddress;  
		// 编译器自动提供这样的 浅拷贝，为了防止 堆区内存重复释放，需要自己提供 深拷贝
		this->pAddress = new T[arr.m_Capacity];
		for (int i = 0; i < this->m_Size; i++)
		{
			this->pAddress[i] = arr.pAddress[i];  //将arr数组中的数据拷入
		}
	}

	//operator= 重载 ，也是为了防止浅拷贝的问题
	MyArray& operator=(const MyArray& arr)
	{
		//cout << "myarray operator= 构造调用" << "\n";
		//先判断 堆区 是否 有数据，如果有 先释放
		if (this->pAddress != NULL)
		{
			delete[]this->pAddress;
			this->pAddress = NULL;
			this->m_Capacity = 0;
			this->m_Size = 0;
		}
		//深拷贝操作
		this->m_Capacity = arr.m_Capacity;
		this->m_Size = arr.m_Size;
		for (int i = 0; i < this->m_Size; i++)
		{
			this->pAddress[i] = arr.pAddress[i];
		}
		return *this;  //返回对象本身
	}

	//尾插法
	bool Push_Back(const T& val)
	{
		if (this->m_Capacity == this->m_Size)
		{
			return false;
		}
		this->pAddress[this->m_Size] = val;//在数组末尾插入数据 val;
		this->m_Size++;    //数组大小＋1;
		return true;
	}

	//尾删法
	bool Pop_Back()
	{
		if (this->m_Size == 0)
			return false;
		this->m_Size = this->m_Size - 1;
		return true;
	}
	//通过下标 访问数组中的元素  重载[]
	T& operator[](int index)
	{
		//引用返回（Return by Reference）与 左值（value），通常出现在*运算符重载*的场景中;
		
		//1. 什么是左值（Lvalue）和右值（Rvalue）？
		//左值：可以放在赋值操作符 = 左边的表达式。它代表一个 *占据内存中* *某个确定位置的对象* 例如：arr[0] = 100; 中，arr[0] 就是一个左值;
		//右值：只能放在赋值操作符 = 右边的表达式。它通常是一个*临时数据或字面量*;
		
		//2. 为什么返回引用（T&）可以作为左值？
		//返回值为普通类型（值传递 T）-- 
		// 如果函数返回的是一个普通类型，C++ 会在内存中创建一个临时副本来存储结果,
		// 因为临时对象是*只读*的（即右值）,所以你无法对其赋值（比如无法执行 arr[0] = 100，编译器会报错）;
		//返回值为引用（T &）：当函数返回引用时，它不会创建临时副本，而是直接返回该元素在堆内存中的本体（别名）;
		
		//返回 T（传值）：return 产生新的一块临时内存，修改它不会影响原数组，因此禁止作为左值;
		//返回 T& （传引用）：return 返回原数据的内存引用，直接操作那块内存;
		return this->pAddress[index];
	}
	//返回数组容量
	int GetCapacity()
	{
		return this->m_Capacity;
	}
	//返回数组大小
	int GetSize()
	{
		return this->m_Size;
	}
	//析构函数
	~MyArray()
	{
		//cout << "myarray 析构函数调用" << "\n";
		if (this->pAddress != NULL)
		{
			delete[]this->pAddress;
			this->pAddress = NULL;// 析构作用 -- 释放堆区数据
		}
	}
private:
	T* pAddress;
	int m_Capacity;
	int m_Size;
};


/////.cpp
#define _CRT_SECURE_NO_WARNINGS 1
#include "MyArray.hpp"
#include <string>
void printIntArray(MyArray<int>& arr)
{
	for (int i = 0; i<arr.GetSize(); i++)
	{
		cout << arr[i] << "\n";
	}
}
void test01()
{
	MyArray <int>arr1(5);
	for (int i = 0; i < 5; i++)
	{
		arr1.Push_Back(i);
	}
	cout << "arr1 打印输出" << "\n";
	printIntArray(arr1);
	cout << "arr1 容量" << arr1.GetCapacity() << "\n";
	cout << "arr1 大小" << arr1.GetSize() << "\n";

	MyArray<int>arr2(arr1); //拷贝
	cout << "arr2 打印输出" << "\n";
	printIntArray(arr2);
	arr2.Pop_Back();
	cout << "arr2 容量" << arr2.GetCapacity() << "\n";
	cout << "arr2 大小" << arr2.GetSize() << "\n";

	//MyArray <int>arr2(arr1); 
	//MyArray <int>arr3(100);
	//arr3 = arr1;
}
class Person
{
public:
	Person() {};
	//1为什么不写默认构造函数会报错？
	/*
	在 test02() 中实例化 MyArray<Person> arr(10); 时，类模板内部通常会为开辟的数组空间初始化：
	动态内存分配：在 MyArray 的构造函数中，有类似 this->pAddress = new T[capacity] 的语句;
	默认构造函数的调用：C++ 语法要求，在使用 new 关键字为一个对象数组分配内存时，
	系统会尝试调用类 T（此处为 Person）的默认构造函数（无参构造函数）来初始化数组中的每一个元素。
	规则限制：当只写了一个有参构造函数 Person(string name, int age) 时，
	编译器就不会再自动生成默认的无参构造函数。此时如果不提供，就会因为找不到无参构造函数而引发编译错误。
	*/

	//2.为什么写了空实现却“和不写没区别”？
	/*
	满足语法要求：Person() {}
	仅仅是为了满足编译器在分配数组内存时的语法要求，它确保了数组中的每个 Person 对象都能被正确创建出来;
	为什么感觉“没区别”:
	虽然它开辟了空间并创建了空对象，但在后续的 test02() 中，通过 Push_Back(p1) 等方法把具体的 Person 数据存入数组时，
	实际上是用拷贝或赋值操作覆盖了原本的空对象.所以从最终结果看，它和不写没有区别.
	*/
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
	string m_Name;
	int m_Age;
};
void printPersonArray(MyArray<Person>& arr)
{
	for (int i = 0; i < arr.GetSize(); i++)
	{
		cout << "name" << arr[i].m_Name << "\n";
		cout << "age" << arr[i].m_Age << "\n";//因为是 person 的数据类型 故用arr[i].是可以访问到 年龄和姓名的;
	}
}

void test02()
{
	MyArray<Person>arr(10);
	Person p1("野眠1", 18);
	Person p2("野眠2", 88);
	Person p3("野眠3", 888);
	Person p4("野眠4", 999);
	Person p5("野眠5", 111);

	//将数据插入到数组中
	arr.Push_Back(p1);
	arr.Push_Back(p2);
	arr.Push_Back(p3);
	arr.Push_Back(p4);
	arr.Push_Back(p5);
    //打印数组操作
	printPersonArray(arr);
	//输出容量
	cout << "arr 容量" << arr.GetCapacity() << "\n";
	//输出大小
	cout << "arr 大小" << arr.GetSize() << "\n";
}
int main()
{
	//test01();
	test02();
	return 0;
}


