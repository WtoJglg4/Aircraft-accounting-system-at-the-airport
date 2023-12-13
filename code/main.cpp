#include <iostream>
#include <string>
#include <fstream>
#include "eventList.h"
#include "planeList.h"
#include "airportList.h"
using namespace std;


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
    
    cout << "ПЕРВИЧНАЯ ИНИЦИАЛИЗАЦИЯ:\n";
    events.init(eventsPath);
    events.print();

    // cout << "ДОБАВЛЕНИЕ НОВОГО ЭЛЕМЕНТА В СПИСОК:\n";
    // events.append(events.getEventFromConsole());
    // events.print();

    cout << "ПОИСК ЭЛЕМЕНТОВ:\n";
    event* custom = events.getEventFromConsole();
    events.select(custom);
    

    cout << "УДАЛЕНИЕ ЭЛЕМЕНТОВ:\n";
    cout << "\nВведите событие для удаления:\n";
    custom = events.getEventFromConsole();
    events.remove(custom);
    events.print();

    cout << "ПЕРЕМЕЩЕНИЕ В ПРЯМОМ НАПРАВЛЕНИИ:\n";
    int eventId, steps;
    cout << "Введите индекс перемещаемого элемента: ";
    cin >> eventId;
    cout << "Введите кол-во шагов: ";
    cin >> steps;
    cout << endl;
    events.moveForward(eventId, steps);
    events.print();

    cout << "ПЕРЕМЕЩЕНИЕ В ОБРАТНОМ НАПРАВЛЕНИИ:\n";
    cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. назад\n\n" ;
    events.moveBack(eventId, steps);
    events.print();

    system("pause");
}