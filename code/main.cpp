#include <iostream>
#include <string>
#include <fstream>
#include "eventList.h"
#include "planeList.h"
#include "airportList.h"
using namespace std;

//путь до списка взлетов/посадок, списка марок самолетов, списка аэропортов
const char *eventsPath = "input/input.txt", *planesPath = "input/planes.txt", *airoportsPath = "input/airoports.txt";


//g++ main.cpp eventList.cpp planeList.cpp airportList.cpp -o output.exe
int main(){

    //инициализация списка марок самолетов и списка аэропортов
    planeList planes;
    airportList airports;
    planes.init(planesPath);
    airports.init(airoportsPath);

    //печать списков
    planes.print();
    airports.print();

    //инициализация списка взлетов/посадок
    eventList events(airports, planes);
    events.planes.print();
    

    // cout << "ПЕРВИЧНАЯ ИНИЦИАЛИЗАЦИЯ:\n";
    events.init(eventsPath);
    cout << "Список инициализирован\n";
    events.print();
    // system("pause");
    

    cout << "ДОБАВЛЕНИЕ НОВОГО ЭЛЕМЕНТА В СПИСОК:\n";
    event* custom = events.getEventFromConsole();
    events.append(custom);
    events.print();
    system("pause");


    
    cout << "ПОИСК ЭЛЕМЕНТОВ:\n";
    custom = events.getEventFromConsole();
    events.select(custom);
    

    cout << "УДАЛЕНИЕ ЭЛЕМЕНТОВ:\n";
    // cout << "\nВведите событие для удаления:\n";
    custom = events.getEventFromConsole();
    events.remove(custom);
    events.print();

    cout << "ПЕРЕМЕЩЕНИЕ В ПРЯМОМ НАПРАВЛЕНИИ:\n";
    int eventId, steps;
    cout << "Введите индекс перемещаемого элемента: ";
    cin >> eventId;
    cout << "Введите кол-во шагов: ";
    cin >> steps;
    cout << "\nПеремещаем " << eventId << "-й элемент на " << steps << "поз. вперед\n\n" ;
    events.moveForward(eventId, steps);
    events.print();

    cout << "ПЕРЕМЕЩЕНИЕ В ОБРАТНОМ НАПРАВЛЕНИИ:\n";
    cout << "Введите индекс перемещаемого элемента: ";
    cin >> eventId;
    cout << "Введите кол-во шагов: ";
    cin >> steps;
    cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. назад\n\n" ;
    events.moveBack(eventId, steps);
    events.print();
    system("pause");  

    // int command;
    // while(true){
    //     cout << "1. Добавление события\n2. Удаление\n3. Поиск\n4. Печать списка\n5. Перемещение в прямом направлении\n6. Перемещение в прямом направлении\n\n";
    //     cin >> command;
    //     event* custom;
    //     int eventId, steps;
    //     switch (command){
    //         case 1:
    //             cout << "\nВведите событие для добавления:\n";
    //             custom = events.getEventFromConsole();
    //             events.append(custom);
    //             break;
    //         case 2:
    //             cout << "\nВведите событие для удаления:\n";
    //             custom = events.getEventFromConsole();
    //             events.remove(custom);
    //             break;
    //         case 3:
    //             cout << "\nВведите запрос:\n";
    //             custom = events.getEventFromConsole();
    //             events.select(custom);
    //             break;
    //         case 4:
    //             events.print();
    //             break;
    //         case 5:
    //             cout << "Введите индекс перемещаемого элемента: ";
    //             cin >> eventId;
    //             cout << "Введите кол-во шагов: ";
    //             cin >> steps;
    //             cout << endl;
    //             cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. вперед\n\n" ;
    //             events.moveForward(eventId, steps);
    //             break;
    //         case 6:
    //             cout << "Введите индекс перемещаемого элемента: ";
    //             cin >> eventId;
    //             cout << "Введите кол-во шагов: ";
    //             cin >> steps;
    //             cout << endl;
    //             cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. назад\n\n" ;
    //             events.moveBack(eventId, steps);
    //             break;
    //         default:
    //             break;
    //     }        
    // }
}