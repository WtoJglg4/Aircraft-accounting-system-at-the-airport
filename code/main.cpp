/********************************************************************
*                КАФЕДРА № 304 2 КУРС  1 СЕМ                        *
*-------------------------------------------------------------------*
* Project Type  : Win64 Console Application                         *
* Project Name  : Coursework                                        *
* File Name     : main.cpp                                          *
* Language      : C++                                               *
* Programmer(s) : Глазов В.В.                                       *
* Modifyed By   :                                                   *
* Created       : 20/11/23                                          *
* Last Revision : 18/12/23                                          *
* Comment(s)    : Курсовая работа                                   *
*********************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "eventList.h"
#include "planeList.h"
#include "airportList.h"
using namespace std;

//путь до списка взлетов/посадок, списка марок самолетов, списка аэропортов
const char *eventsPath = "input/input.txt", *planesPath = "input/planes.txt", *airoportsPath = "input/airoports.txt";

//g++ main.cpp eventlist.cpp planelist.cpp airportlist.cpp -o output.exe
int main(){
    //инициализация списка марок самолетов и списка аэропортов
    planeList planes;
    airportList airports;
    planes.init(planesPath);
    airports.init(airoportsPath);

    //инициализация списка взлетов/посадок
    eventList events(eventsPath, airports, planes);
    
    int command;
    while(true){
        cout << "\n----------MENU----------\n";
        cout << "1. Добавление события\n2. Удаление\n3. Поиск\n4. Печать списка\n5. Перемещение в прямом направлении\n6. Перемещение в прямом направлении\n7. Выход\n\n";
        cout << "Операция: ";
        cin >> command;
        event* custom;
        int eventId, steps;
        switch (command){
            case 1:
                cout << "\nВведите событие для добавления:\n";
                custom = events.getEventFromConsole();
                events.append(custom);
                break;
            case 2:
                cout << "\nВведите событие для удаления:\n";
                custom = events.getEventFromConsole();
                events.remove(custom);
                break;
            case 3:
                // cout << "\nВведите запрос:\n";
                custom = events.getEventFromConsole();
                events.select(custom);
                break;
            case 4:
                events.print();
                break;
            case 5:
                cout << "Введите индекс перемещаемого элемента: ";
                cin >> eventId;
                cout << "Введите кол-во шагов: ";
                cin >> steps;
                cout << endl;
                cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. вперед\n\n" ;
                events.moveForward(eventId, steps);
                break;
            case 6:
                cout << "Введите индекс перемещаемого элемента: ";
                cin >> eventId;
                cout << "Введите кол-во шагов: ";
                cin >> steps;
                cout << endl;
                cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. назад\n\n" ;
                events.moveBack(eventId, steps);
                break;
            default:
                return 0;
        }        
    }
}