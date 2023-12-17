#ifndef EVENTLIST_H
#define EVENTLIST_H
#include <iostream>
#include <string>
#include <fstream>
#include "planeList.h"
#include "airportList.h"
using namespace std;

const int arrival = 0, departure = 1;

struct event{
    int id;                 //id индекс в списке
    int airportId;          //id аэропорт
    int planeBrandId;       //id марка самолета
    int type;               //тип события(вылет/прилет)
    
    int time;               //время
    string flightNumber;    //номер рейса
    
    string distance;        //ближний/дальний рейс
    int onBoard;            //пассажиров на борту
    int sold;               //проданных билетов
    float ticketPrice;      //цена билета

    event *next, *prev;    //следующий, предыдущий элементы
};

class eventList
{
    protected:
        event *head, *tail;
        
        
        int count;

    public:
        airportList airports;
        planeList planes;

        eventList(airportList airport, planeList plane);

        void init(const char *path);
        void append(event* newEvent);
        void print();
        bool empty();
        int len();
        event* getEvent(ifstream& file);
        event* getEventFromConsole();
        void select(event* Event);
        void remove(event* Event);
        bool satisfyPatternEvent(event* pattern, event* event);
        void moveForward(int eventId, int steps);
        void moveBack(int eventId, int steps);

        void printEvent(event* Event);

        void removePlane(string name);
        void removeAirport(string name);

};

#endif