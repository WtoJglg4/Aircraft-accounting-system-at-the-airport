#ifndef PLANELIST_H
#define PLANELIST_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct plane{
    int id;
    string name;     
    int capacity;   //вместимость
    plane *next, *prev;
};

class planeList{
    protected:
        
        int count;
    public:
        plane *head, *tail;

        planeList();
        
        int length();
        bool empty();

        void append(int index, string data, int capacity);
        plane* getPlane(int id);
        void init(const char *path);
        int remove(string name);
        void print();
        // string removeById(int id);
        // int removeByData(string data);
};

#endif