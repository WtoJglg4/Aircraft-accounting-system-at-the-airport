/********************************************************************
*                КАФЕДРА № 304 2 КУРС  1 СЕМ                        *
*-------------------------------------------------------------------*
* File Name     : airportList.cpp                                   *
* Language      : C++                                               *
* Programmer(s) : Глазов В.В.                                       *
* Modifyed By   :                                                   *
* Created       : 20/11/23                                          *
* Last Revision : 18/12/23                                          *
* Comment(s)    : Курсовая работа                                   *
*********************************************************************/
#include <iostream>
#include <string>
#include "airportList.h"

//конструктор класса airportList
airportList::airportList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

//инициализация из файла
void airportList::init(const char *path){
    ifstream file(path);

    string data;
    while(!file.eof()){
        int index;
        file >> index;
        getline(file, data);
        data.erase(0, 1);

        append(index, data);
    }
    file.close();

    printf("Добавлено %d элементов\n", count);
}

//добавление аэропорта в конец списка
void airportList::append(int index, string data){
    airport* newAirport = new airport;
    newAirport->id = index;
    newAirport->name = data;
    newAirport->next = nullptr;
    newAirport->prev = nullptr;

    if(!head){
        head = newAirport;
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        newAirport->next = head;
        newAirport->prev = tail;
        tail->next = newAirport;
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

//удаление элемента
int airportList::remove(string name){
    airport *curr = head;
    do{
        if(!name.compare(curr->name)) break;
        curr = curr->next;
    }
    while(curr != head);

    if(curr == head){
        if(name.compare(curr->name)) return -1; //не найден
        head = head->next;
    }
    if(curr == tail) tail = tail->prev;

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    
    int id = curr->id;
    airport *tmp = curr;
    curr = curr->next;
    delete tmp;
    count--;
    return id;
}

//длина списка
int airportList::length(){
    return count;
}

//проверка на пустоту
bool airportList::empty(){
    return count == 0;
}

//печать списка
void airportList::print(){
    
    cout << "\nairportsList:\n";
    airport* curr = head;
    do{
        cout << curr->id << " " << curr->name << endl;
        curr = curr->next;
    }
    while(curr != head);
    cout << endl << endl;
}

//получение названия по id
airport* airportList::getAirport(int id){
    if(id < 0 || id >= count){
        cerr << "в списке нет элемента с таким id " << id;
        return nullptr;
    }

    airport* curr = head;
    do{
        if(curr->id == id) return curr;
        curr = curr->next;
    }
    while(curr != head);

    cerr << "в списке нет элемента с таким id " << id;
    return nullptr;
}