#include<bits/stdc++.h>
using namespace std;

// 不能指使用单线程，还要加同步锁
// 加锁保证多线程环境下只创建一个实例
// 注意懒汉和饿汉，

// 解法1 静态构造函数
// 解法2 双重锁+需要时才创建实例的懒汉模式

// 懒汉式，只有get_instance时才会new新的单例对象
/*
class Singleton {
	private:
		static volatile Singleton* instance;
		static std::mutex mtx;
		Singleton() {}

	public:
        Singleton(Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
  
		static Singleton* getInstance() {
			if (instance == nullptr) {
				std::lock_guard<std::mutex> lock(mtx);
				if (instance == nullptr) {
					instance = new Singleton();
				}
			}
			return const_cast<Singleton*>(instance);
		}
};

volatile Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

*/

//这个为上面的优化
//利用了c++11的magic statoc
//变量初始化时进入声明，并发线程会阻塞等待初始化结束

class Singleton {
	private:
		Singleton() {};

	public:
		~Singleton() {};
		Singleton(Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		static Singleton& get_instance() {
			static Singleton instance;
			return instance;
		}
};

// 有种加上智能指针和锁可以保证线程安全和内存安全，但是要求用户也必须用智能指针。

int main() {
	Singleton& instance_1 = Singleton::get_instance();
	Singleton& instance_2 = Singleton::get_instance();

	return 0;
}
