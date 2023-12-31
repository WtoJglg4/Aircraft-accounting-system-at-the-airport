/********************************************************************
*                КАФЕДРА № 304 2 КУРС  1 СЕМ                        *
*-------------------------------------------------------------------*
* File Name     : planeList.h                                       *
* Language      : C++                                               *
* Programmer(s) : Глазов В.В.                                       *
* Modifyed By   :                                                   *
* Created       : 20/11/23                                          *
* Last Revision : 18/12/23                                          *
* Comment(s)    : Курсовая работа                                   *
*********************************************************************/
#ifndef PLANELIST_H
#define PLANELIST_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//марка самолета
struct plane{
    int id;             //id
    string name;        //название марки
    int capacity;       //вместимость
    plane *next, *prev; //следующий, предыдущий элементы
};

//список марок
class planeList{
    protected:
        plane *head, *tail; //начало, конец списка
        int count;          //количество элементов
    public:
        planeList();                                        //конструктор класса planeList
        void init(const char *path);                        //инициализация из файла
        void append(int index, string data, int capacity);  //добавление аэропорта в конец
        int remove(string name);                            //удаление элемента
        int length();                                       //длина списка
        bool empty();                                       //проверка на пустоту
        void print();                                       //печать списка
        plane* getPlane(int id);                  //получение названия по id
};

#endif