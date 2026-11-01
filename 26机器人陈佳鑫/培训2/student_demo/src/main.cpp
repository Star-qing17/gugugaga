#include <iostream>
#include <windows.h>
#include <string>
#include "student.hpp"
using namespace std;
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    string a;int b,c;
    cout << "请输入学生名字，学号，分数："<<endl;
    cin>>a>>b>>c;
    student s1(a,b,c);
    s1.print();
    int new_s;
    cout << "请输入你要修改的分数："<<endl;
    cin>>new_s;
    s1.setscore(new_s);
    s1.print();
    return 0;
}