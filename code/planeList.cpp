#include <iostream>
#include <string>
#include "planeList.h"

//конструктор класса planeList
planeList::planeList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

//инициализация из файла
void planeList::init(const char *path){
    ifstream file(path);

    string data;
    int capacity;
    while(!file.eof()){
        int index, capacity;
        file >> index;
        getline(file, data);
        data.erase(0, 1);
        file >> capacity;
        append(index, data, capacity);
    }
    file.close();

    printf("Добавлено %d элементов\n", count);
}

//добавление аэропорта в конец
void planeList::append(int index, string data, int capacity){
    plane* newPlane = new plane;
    newPlane->id = index;
    newPlane->name = data;
    newPlane->capacity = capacity;
    newPlane->next = nullptr;
    newPlane->prev = nullptr;

    if(!head){
        head = newPlane;
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        newPlane->next = head;
        newPlane->prev = tail;
        tail->next = newPlane;
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

//удаление элемента
int planeList::remove(string name){
    plane *curr = head;
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
    plane *tmp = curr;
    curr = curr->next;
    delete tmp;
    count--;
    return id;
}

//печать списка
void planeList::print(){
    
    cout << "planesList:\n";
    plane* curr = head;
    do{
        cout << curr->id << " " << curr->name << " " << curr->capacity << endl;
        curr = curr->next;
    }
    while(curr != head);
    cout << endl << endl;
}

//длина списка
int planeList::length(){
    return count;
}

//проверка на пустоту
bool planeList::empty(){
    return count == 0;
}

//получение названия по id
plane* planeList::getPlane(int id){
    if(id < 0 || id >= count){
        cerr << "в списке нет элемента с таким id " << id;
        exit(1); 
    }

    plane* curr = head;
    do{
        if(curr->id == id) return curr;
        curr = curr->next;
    }
    while(curr != head);

    cerr << "в списке нет элемента с таким id " << id;
    exit(1); 
}