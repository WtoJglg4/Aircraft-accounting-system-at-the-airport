#ifndef AIRPORTLIST_H
#define AIRPORTLIST_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct airport{
    int id;
    string name;
    airport *next, *prev;
};

class airportList{
    protected:
        airport *head, *tail;
        int count;
    public:
        airportList();
        
        int length();
        bool empty();

        void append(int index, string data);
        int getId(string name);
        string getData(int id);
        int remove(string name);
        void init(const char *path);
        void print();
        // string removeById(int id);
        // int removeByData(string data);
};

#endif