#include <iostream>
#include <string>
#include "planeList.h"

planeList::planeList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

int planeList::length(){
    return count;
}


bool planeList::empty(){
    return count == 0;
}


void planeList::append(int index, string data, int capacity){
    if(!head){
        head = new plane{index, data, capacity, nullptr, nullptr};
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        tail->next = new plane{index, data, capacity, head, tail};
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

void planeList::print(){
    
    cout << "planesList:\n";
    plane* curr = head;
    do{
        cout << curr->id << " " << curr->name << " " << curr->capacity << endl;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    cout << endl << endl;
}

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
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    cerr << "в списке нет элемента с таким id " << id;
    exit(1); 
}

int planeList::remove(string name){
    plane *curr = head;
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
    plane *tmp = curr;
    curr = curr->next;
    delete tmp;
    count--;
    return id;
}