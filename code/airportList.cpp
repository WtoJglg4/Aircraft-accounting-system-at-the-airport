#include <iostream>
#include <string>
#include "airportList.h"

airportList::airportList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

int airportList::length(){
    return count;
}


bool airportList::empty(){
    return count == 0;
}


void airportList::append(int index, string data){
    if(!head){
        head = new airport{index, data, nullptr, nullptr};
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        tail->next = new airport{index, data, head, tail};
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

void airportList::print(){
    
    cout << "airportsList:\n";
    airport* curr = head;
    do{
        cout << curr->id << " " << curr->name << endl;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    cout << endl << endl;
}

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

int airportList::getId(string data){
    airport* curr = head;
    do{
        if(!data.compare(curr->name)) return curr->id;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    
    cerr << "в списке нет элемента " << data << endl;
    exit(1);
}

string airportList::getData(int id){
    if(id < 0 || id >= count){
        cerr << "в списке нет элемента с таким id " << id;
        exit(1); 
    }

    airport* curr = head;
    do{
        if(curr->id == id) return curr->name;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    cerr << "в списке нет элемента с таким id " << id;
    exit(1); 
}

int airportList::remove(string name){
    airport *curr = head;
    do{
        if(!name.compare(curr->name)) break;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

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