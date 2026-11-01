#include "student.hpp"
#include <iostream>


    student::student(string n,int i,int s){
        name=n;score=s;id=i;
    }
   
    void student:: setscore(int new_s){
        score=new_s;
        cout << name<<"的分数修改完毕。"<< endl;
    }
    void student::print(){
        cout <<"学生信息如下："<<endl;
        cout <<"姓名："<<name<<endl;
        cout <<"学号："<<id<<endl;
        cout <<"分数："<<score<<endl;
    }
   