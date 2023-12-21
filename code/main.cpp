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

Error* testFile(const char *path);

//g++ main.cpp eventlist.cpp planelist.cpp airportlist.cpp -o output.exe
int main(){
    system("color F0");

    //проверка на существование и непустоту файлов
    Error* err;
    const char *files[] = {eventsPath, planesPath, airoportsPath};
    for(int i = 0; i < size(files); i++){
        err = testFile(files[i]);
        if(err){
            cout << err->message << " " << files[i] << endl;
            system("pause");
            return err->code;
        }
    }

    cout << "СИСТЕМА УЧЕТА ВОЗДУШНЫХ СУДОВ В АЭРОПОРТУ\n\n\n";

    //инициализация списка марок самолетов и списка аэропортов
    planeList planes;
    airportList airports;
    planes.init(planesPath);
    airports.init(airoportsPath);

    //инициализация списка взлетов/посадок
    eventList events(eventsPath, airports, planes);

    
    string command;
    while(true){
        cout << "\n----------MENU----------\n";
        cout << "1. Добавление события\n2. Удаление\n3. Поиск\n4. Печать списка\n5. Перемещение в прямом направлении\n6. Перемещение в прямом направлении\n7. Выход\n\n";
        while(true){
            cout << "Операция: ";
            getline(cin, command);
            if(command.length() == 1 && command[0] > 47 && command[0] < 58){
                break;
            }
            cerr << "Некорректный ввод. Введите заново.\n";
        }        
        int com = atoi(command.c_str());
        event* custom;
        int eventId, steps;
        switch (com){
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
                cout << "\n1. Воспользоваться готовыми запросами\n2. Ввести вручную\n";
                while(true){
                    cout << "Операция: ";
                    getline(cin, command);
                    if(command.length() == 1 && (command[0] == 49 || command[0] == 50)){
                        break;
                    }
                    cerr << "Некорректный ввод. Введите заново.\n";
                }        
                 com = atoi(command.c_str());
                switch (com){
                    case 1:
                        events.queries();
                        break;
                    case 2:
                        cout << "\nВведите запрос:\n";
                        custom = events.getEventFromConsole();
                        events.select(custom);
                        break;
                    deafault:
                        break;
                }
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

Error* testFile(const char *path){
    ifstream file(path);
    Error *err = new Error;
    if(!file.is_open()){
        err->code = 1;
        err->message = "файл невозможно открыть";
    } else if(file.peek() == EOF){
        err->code = 2;
        err->message = "файл пуст";
    } else err = nullptr;

    file.close();
    return err;
}