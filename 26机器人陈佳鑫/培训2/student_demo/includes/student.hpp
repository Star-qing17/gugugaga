#pragma once
#include <string>
using namespace std;
class student
{
    public:
    student(string n,int i,int s);
   
    void setscore(int new_s);
    void print();
    private:
    string name;
    int score;
    int id;
};
