#ifndef AIRPORTLIST_H
#define AIRPORTLIST_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//аэропорт
struct airport{
    int id;                 //id
    string name;            //название аэропорта
    airport *next, *prev;   //следующий, предыдущий элементы
};

//список аэропортов
class airportList{
    protected:
        airport *head, *tail;   //начало, конец списка
        int count;              //количество элементов
    public:
        airportList();                          //конструктор класса airportList
        void init(const char *path);            //инициализация из файла
        void append(int index, string data);    //добавление аэропорта в конец списка
        int remove(string name);                //удаление элемента
        int length();                           //длина списка
        bool empty();                           //проверка на пустоту
        void print();                           //печать списка
        string getData(int id);                 //получение названия по id
};

#endif