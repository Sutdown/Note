#include<bits/stdc++.h>
using namespace std;

class CMyString {
	public:
		CMyString(char* pData = NULL);
		CMyString(const CMyString& str);
		~CMyString(void);

		CMyString& operator =(const CMyString &str);

	private:
		char* m_pData;
};

// 深拷贝
CMyString& CMyString::operator =(const CMyString &str) {
	if (this == &str)
		return *this;
	delete []m_pData; // 释放当前内存时注意判断两个实例是不是同一个
	m_pData = NULL;

	m_pData = new char[strlen(str.m_pData) + 1];
	strcpy(m_pData, str.m_pData);

	return *this;

	//为了避免new内存不足抛出异常，我们在局部空间中设置一个临时实例，
	//将临时实例的值和变量交换
/*	if (*this != &str) {
		CMyString strTemp(str);
		char *pTemp = strTemp.m_pData;
		strTemp.m_pData = m_pData;
		m_pData = pTemp;
	}
	return *this;
*/

}

int main() {

	return 0;
}
