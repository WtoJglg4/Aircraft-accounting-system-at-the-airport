#include <iostream>
#include <string>
#include <fstream>
#include "eventList.h"
#include "planeList.h"
#include "airportList.h"
#include <iomanip>

eventList::eventList(airportList airport, planeList plane){
    airports = airport;
    planes = plane;
    head = nullptr;
    tail = nullptr;
    count = 0;
}

event* eventList::getEvent(ifstream& file)
{
    string typeStr, airportStr, dataStr;          
    string flightNumber, planeBrand, distance, onBoardStr, soldStr, ticketPriceStr;
    int type, data, onBoard, sold; 
    float ticketPrice;

    getline(file, typeStr);    //пробел
    getline(file, typeStr);
    if(typeStr.compare("Dep")){
        type = arrival;
    } else type = departure;

    getline(file, airportStr);
    int airportId = atoi(airportStr.c_str());

    getline(file, dataStr, '\n');
    data = dataToInt(dataStr);

    getline(file, flightNumber, '\n');

    getline(file, planeBrand, '\n');
    int planeBrandId = atoi(planeBrand.c_str());

    getline(file, distance, '\n');
    
    getline(file, onBoardStr, '\n');
    onBoard = atoi(onBoardStr.c_str());

    getline(file, soldStr, '\n');
    sold = atoi(soldStr.c_str());

    getline(file, ticketPriceStr, '\n');
    ticketPrice = atof(ticketPriceStr.c_str());
    
    event *newEvent = new event;
    newEvent->id = 0;
    newEvent->type = type;
    newEvent->airportId = airportId;
    newEvent->dataStr = dataStr;
    newEvent->data = data;
    newEvent->flightNumber = flightNumber;
    newEvent->planeBrandId = planeBrandId;
    newEvent->distance = distance;
    newEvent->onBoard = onBoard;
    newEvent->sold = sold;
    newEvent->ticketPrice = ticketPrice;
    newEvent->next = nullptr;
    newEvent->prev = nullptr;
    
    return newEvent;
}

void eventList::init(const char *path){
    ifstream file(path);
    string eventsCountString;

    getline(file, eventsCountString);
    int eventsCount = atoi(eventsCountString.c_str());
    event* newEvent ;
    for(int i = 0; i < eventsCount; i++){
        newEvent = getEvent(file);
        append(newEvent);
    }

    file.close();
    printf("Добавлено %d событий\n", eventsCount);
}

void eventList::append(event* newEvent){
    if(!head){
        head = newEvent;
        tail = newEvent;
        newEvent->prev = newEvent;
        newEvent->next = newEvent;        
    } else{
        tail->next = newEvent;
        newEvent->next = head;
        newEvent->prev = tail;
        head->prev = newEvent;
        tail = newEvent;
    }
    newEvent->id = count;
    count++;
}

//печать события
void eventList::printEvent(event* curr){
    string etype;
    plane *plane = planes.getPlane(curr->planeBrandId);
    if(curr->type){
        etype = "Dep";
    } else etype = "Arr";
    cout << "id: " << curr->id << "\ntype: " << etype << "\nairport: " << airports.getData(curr->airportId) << "\ndata: " << curr->dataStr << "\nflightNumber: ";
    cout << curr->flightNumber << "\nplaneBrand: " << plane->name << "\ndistance: " << curr->distance << "\nonBoard: " << curr->onBoard << "\nsold: " << curr->sold << "\nticketPrice: " << curr->ticketPrice << "\n\n";
}

void eventList::print(){
    if(empty()){
        cerr << "eventList пуст\n";
        return; 
    }

    cout << "Всего событий: " << count << endl;
    printTableHead();
    event* curr = head;
    do{
        // printEvent(curr);
        printEventRow(curr);
        curr = curr->next;
    }
    while(curr != head);
}

bool eventList::empty(){
    return count == 0;
}

int eventList::len(){
    return count;
}

void eventList::removePlane(string name){
    int rmId = planes.remove(name); //удалить из списка самолетов

    event *curr = head;
    do{
        if(rmId == curr->planeBrandId){
            if(curr == head) head = head->next;
            if(curr == tail) tail = tail->prev; 

            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;

            event *tmp = curr;
            curr = curr->next;
            delete tmp;
            count--;
        }
        curr = curr->next;
    }
    while(curr != head);     
}

void eventList::removeAirport(string name){
    int rmId = airports.remove(name); //удалить из списка самолетов

    event *curr = head;
    do{
        if(rmId == curr->airportId){
            if(curr == head) head = head->next;
            if(curr == tail) tail = tail->prev; 

            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;

            event *tmp = curr;
            curr = curr->next;
            delete tmp;
            count--;
        }
        curr = curr->next;
    }
    while(curr != head);
}

event* eventList::getEventFromConsole(){
    string typeStr = "-1", airport = "-", dataStr = "-1";          
    string flightNumber = "-", planeBrand = "-", distance = "-", onBoardStr = "-1", soldStr = "-1", ticketPriceStr = "-1";
    int type = -1, depdata = -1, arrdata = -1, onBoard = -1, sold = -1; 
    float ticketPrice = -1;

    event *newEvent = new event;
    newEvent->id = -1;
    newEvent->type = -1;
    newEvent->airportId = -1;
    newEvent->data = -1;
    newEvent->dataEnd = -1;
    newEvent->flightNumber = "-";
    newEvent->planeBrandId = -1;
    newEvent->distance = "-";
    newEvent->onBoard = -1;
    newEvent->sold = -1;
    newEvent->ticketPrice = -1;

    printf("Введите поля для события\n0. ID элемента\n1. Тип события\n2. ID аэропорта\n");
    printf("3. Диапазон дат\n4. Бортовой номер\n5. ID марки самолета\n6. Дистанция\n");
    printf("7. Пассажиров на борту\n8. Продано билетов\n9. Цена билета\n10. Выйти из добавления фильтра\n");

    int num;
    string filter;
    do{
        getline(cin, filter);
       
        cout << "Операция: ";
        getline(cin, filter);
        num = atoi(filter.c_str());
        // cin >> num;
        // cout <<"NUM " << num << endl;

        switch (num)
        {
        case 0:
            cout << "ID: ";
            getline(cin, filter);
            newEvent->id = atoi(filter.c_str());
            break;
        case 1: 
            cout << "Type: ";
            getline(cin, filter);
            if(!filter.compare("Dep")){
                newEvent->type = 1;
            } else if(!filter.compare("Arr")) newEvent->type = 0;
            break;
        case 2:
            cout << "Airport ID: ";
            getline(cin, filter);
            newEvent->airportId = atoi(filter.c_str());
            break;
        case 3:
            cout << "Дата начала: ";
            getline(cin, filter);
            newEvent->data = dataToInt(filter);
            newEvent->dataStr = filter;

            cout << "Дата конца: "; 
            getline(cin, filter);
            newEvent->dataEnd = dataToInt(filter);
            newEvent->dataStrEnd = filter;
            
            break;
        case 4:
            cout << "Бортовой номер: ";
            newEvent->flightNumber = filter;
            break;
        case 5:
            cout << "ID марки самолета: ";
            getline(cin, filter);
            newEvent->planeBrandId = atoi(filter.c_str());;
            break;
        case 6:
            cout << "Дистанция: ";
            getline(cin, filter);
            newEvent->distance = filter;
            break;
        case 7:
            cout << "Пассажиры: ";
            getline(cin, filter);
            newEvent->onBoard = atoi(filter.c_str());
            break;
        case 8:
            cout << "Проданных билетов: ";
            getline(cin, filter);
            newEvent->sold = atoi(filter.c_str());
            break;
        case 9:
            cout << "Цена билета: ";
            getline(cin, filter);
            newEvent->ticketPrice = atof(filter.c_str());
            break;
        default:
            break;
        }
    } while(num != 10);

    return newEvent;
}

bool eventList::satisfyPatternEvent(event* pattern, event* event){
    if(pattern->id != -1 && pattern->id != event->id) return false;
    if(pattern->type != -1 && pattern->type != event->type) return false;
    if(pattern->airportId != -1 && pattern->airportId != event->airportId) return false;
    // if(pattern->data != -1 && pattern->data != event->data) return false;
    if(pattern->flightNumber.compare("-") && pattern->flightNumber.compare(event->flightNumber)) return false;
    if(pattern->planeBrandId != -1 && pattern->planeBrandId != event->planeBrandId) return false;
    if(pattern->distance.compare("-") && pattern->distance.compare(event->distance)) return false;
    if(pattern->onBoard != -1 && pattern->onBoard != event->onBoard) return false;
    if(pattern->sold != -1 && pattern->sold != event->sold) return false;
    if(pattern->ticketPrice != -1 && pattern->ticketPrice != event->ticketPrice) return false;
    if(pattern->data != -1 && pattern->dataEnd != -1 && (pattern->data > event->data || pattern->dataEnd < event->data)) return false;
    return true;
}

void eventList::select(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Найденные события:\n";
    
    bool tableHead = false;
    do{
        if(satisfyPatternEvent(Event, curr)) {
            // printEvent(curr);
            if(!tableHead){
                printTableHead();
                tableHead = true;
            }
            printEventRow(curr);
            equalsCount++;
        }

        curr = curr->next;
    }
    while(curr != head);  
    
    if(equalsCount == 0){
        cout << "Собыйтий не найдено\n";
    } else cout << "Событий найдено: " << equalsCount << endl << endl;
}

void eventList::remove(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Удалённые события:\n";

    bool tableHead = false;
    do{
        if(satisfyPatternEvent(Event, curr)){
            // printEvent(curr);
            if(!tableHead){
                printTableHead();
                tableHead = true;
            }
            printEventRow(curr);
            equalsCount++;

            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            if(curr == head) head = curr->next;
            event* tmp = curr;
            curr = curr->next;
            delete tmp;
            count--;
        } else curr = curr->next;
    }
    while(curr != head);
    
    if(equalsCount == 0){
        cout << "Собыйтий не найдено\n";
    } else cout << "Событий удалено: " << equalsCount << endl << endl;

    curr = head; //обновление индексов после удаления 
    int newId = 0;
    do{
        curr->id = newId;
        newId++;
        curr = curr->next;
    }
    while(curr != head);
}

void eventList::moveForward(int eventId, int steps){
    if(eventId >= count || eventId < 0) {
        cout << "Элемента с таким id нет\n";
        return;
    }
    event* curr = head;
    while(curr->id != eventId) curr = curr->next;
    event* left = curr, *right, *tmp;
    for(int i = 0; i < steps; i++) curr = curr->next;
    right = curr;

    if(left == head) {
        head = left->next;
        tmp = left->prev;
    } else tmp = left;

    left->prev->next = left->next;
    left->next->prev = left->prev;

    right->next->prev = left;
    left->next = right->next;
    left->prev = right;
    right->next = left;

    int newId = 0;
    curr = head;
    do{
        curr->id = newId;
        newId++;
        curr = curr->next;
    }
    while(curr != head);
}

void eventList::moveBack(int eventId, int steps){
    steps = (count - steps - 1)%count;
    if(steps < 0) steps += count;
    moveForward(eventId, steps);
}

void eventList::printTableHead(){
    int spaces[] = {2, 4, 17, 10, 12, 15, 8, 7, 4, 11};
    
    cout << "+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    
    cout << "\n|id|type|     airport     |   data   |flightNumber|  planeBrand   |distance|onBoard|sold|ticketPrice|\n+";
    
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}

void eventList::printEventRow(event* curr){
    int spaces[] = {2, 4, 17, 10, 12, 15, 8, 7, 4, 11};
    string etype;
    plane *plane = planes.getPlane(curr->planeBrandId);
    if(curr->type){
        etype = "Dep";
    } else etype = "Arr";
    // cout << "id: " << curr->id << "\ntype: " << etype << "\nairport: " << airports.getData(curr->airportId) << "\ndata: " << curr->data << "\nflightNumber: ";
    // cout << curr->flightNumber << "\nplaneBrand: " << plane->name << "\ndistance: " << curr->distance << "\nonBoard: " << curr->onBoard << "\nsold: " << curr->sold << "\nticketPrice: " << curr->ticketPrice << "\n\n";
    
    cout << "|" << setw(2) << curr->id << "|" << setw(4) << etype << "|" << setw(17) << airports.getData(curr->airportId) << "|";
    cout << setw(10) << curr->dataStr << "|" << setw(12) << curr->flightNumber << "|" << setw(15) << plane->name << "|" << setw(8) << curr->distance;
    cout << "|" << setw(7) << curr->onBoard << "|" << setw(4) << curr->sold << "|" << setw(11) << curr->ticketPrice << "|\n+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}
        
int eventList::dataToInt(string data){
    int days = (data[0] - 48) * 10 + data[1] - 48;
    int months = (data[3] - 48) * 10 + data[4] - 48;
    int years = (data[6] - 48) * 10 + data[7] - 48;
    return days + 30*months + 365*years;
}


