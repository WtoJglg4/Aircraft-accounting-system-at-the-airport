/********************************************************************
*                КАФЕДРА № 304 2 КУРС  1 СЕМ                        *
*-------------------------------------------------------------------*
* File Name     : eventList.h                                       *
* Language      : C++                                               *
* Programmer(s) : Глазов В.В.                                       *
* Modifyed By   :                                                   *
* Created       : 20/11/23                                          *
* Last Revision : 18/12/23                                          *
* Comment(s)    : Курсовая работа                                   *
*********************************************************************/

#ifndef EVENTLIST_H
#define EVENTLIST_H
#include <iostream>
#include <string>
#include <fstream>
#include "planeList.h"
#include "airportList.h"
using namespace std;


//событие вылет/прилет
struct event{
    int id;                 //id 
    int airportId;          //id аэропорта
    int planeBrandId;       //id самолета
    int type;               //тип события(вылет/прилет)
    
    int date;               //дата (дд.мм.гг)
    string dateStr;         //дни + 30*мес + 365*годы
    string flightNumber;    //номер рейса
    
    string distance;        //ближний/дальний рейс
    int onBoard;            //пассажиров на борту
    int sold;               //проданных билетов
    float ticketPrice;      //цена билета

    event *next, *prev;    //следующий, предыдущий элементы

    int dateEnd;        //конечная дата интервале
    string dateStrEnd;  //используется для поиска
};

//ошибка
struct Error {
    int code;
    string message;
};

//список событий
class eventList
{
    protected:
        event *head, *tail;                                     //начало/конец списка
        int count;                                              //количество элементов

        bool satisfyPatternEvent(event* pattern, event* event); //сравнение шаблона события с событием(для фильтрации)
        void printTableHead();                                  //печать головы таблицы
        Error* printEventRow(event* Event);                     //печать события в строке таблицы
        void removePlane(string name);                          //удаление самолета из списка
        void removeAirport(string name);                        //удаление аэропорта из списка
        int dateToInt(string date);                             //дата string(дд.мм.гг)->int
        bool isDateCorrect(string date);                        //проверка даты на корректность
        bool isDigitCorrect(string idStr);                      //проверка id корректность

    public:
        airportList airports;   //список аэропортов
        planeList planes;       //список марок самолетов

        eventList(const char *path, airportList airport, planeList plane);    //конструктор класса eventList

        void init(const char *path);                //инициализация списка из файла
        void append(event* newEvent);               //добавление события в конец
        void print();                               //печать списка в таблицу
        bool empty();                               //проверка на пустоту
        int len();                                  //длина списка
        event* getEvent(ifstream& file);            //получение события из файла
        event* getEventFromConsole();               //получение события из консоли
        void select(event* Event);                  //поиск по списку элементов, соответствующих шаблону
        void queries();                             //вызов готовых запросов на поиск
        void remove(event* Event);                  //удаление элементов, соответствующих шаблону
        void moveForward(int eventId, int steps);   //перемещение элемента вперед по списку
        void moveBack(int eventId, int steps);      //перемещение элемента назад по списку
};

#endif