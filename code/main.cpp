#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int strSize = 20;
const int arrival = 0, departure = 1, shortDist = 0, longDist = 1;
const char *path = "input/input.txt";

struct event{
    int id;                 //индекс в списке
    int type;               //тип события
    string depAirport;      //аэропорт вылета
    string arrAirport;      //аэропорт прибытия
    int depTime;            //время вылета
    int arrTime;            //время прибытия
    string flightNumber;    //номер рейса
    string planeBrand;      //марка самолета
    string distance;        //ближний/дальний рейс
    int onBoard;            //пассажиров на борту
    int sold;               //проданных билетов
    float ticketPrice;      //цена билета

    event *next, *prev;     //следующий, предыдущий элементы
};

class list{
    event *head;
    event* getEvent(ifstream& file);
    

    int count;
    
    

    public:
        list();

        void init(const char *path);
        void append(event* newEvent);
        void print();
        bool empty();
        int len();
        event* getEventFromConsole();
        void select(event* Event);
        void remove(event* Event);
        bool satisfyPatternEvent(event* pattern, event* event);
        void moveForward(int eventId, int steps);
        void moveBack(int eventId, int steps);

        void printEvent(event* Event);

};

list::list(){
    head = nullptr;
    count = 0;
}

event* list::getEvent(ifstream& file){
    string typeStr, depAirport, arrAirport, depTimeStr, arrTimeStr;          
    string flightNumber, planeBrand, distance, onBoardStr, soldStr, ticketPriceStr;
    int type, depTime, arrTime, onBoard, sold; 
    float ticketPrice;

    getline(file, typeStr);    //пробел
    getline(file, typeStr);
    getline(file, depAirport);
    getline(file, arrAirport);
    getline(file, depTimeStr, '\n');
    depTime = atoi(depTimeStr.c_str());
    getline(file, arrTimeStr, '\n');
    if(!arrTimeStr.compare("-")){
        arrTime = -1;
    } else arrTime = atoi(arrTimeStr.c_str());
    getline(file, flightNumber, '\n');
    getline(file, planeBrand, '\n');
    getline(file, distance, '\n');
    getline(file, onBoardStr, '\n');
    getline(file, soldStr, '\n');
    getline(file, ticketPriceStr, '\n');
    onBoard = atoi(onBoardStr.c_str());
    sold = atoi(soldStr.c_str());
    ticketPrice = atof(ticketPriceStr.c_str());
    if(typeStr.compare("Dep")){
        type = 0;
    } else type = 1;

    event* newEvent = new event{0, type, depAirport, arrAirport, 
                                depTime, arrTime, flightNumber, planeBrand,
                                distance, onBoard, sold, ticketPrice,
                                nullptr, nullptr};

    return newEvent;
}

void list::init(const char *path){
    ifstream file("input/input.txt");
    string eventsCountString;
    getline(file, eventsCountString);
    int eventsCount = atoi(eventsCountString.c_str());

    for(int i = 0; i < eventsCount; i++){
        event* newEvent = getEvent(file);
        append(newEvent);
    }

    file.close();
    printf("Добавлено %d событий\n", eventsCount);
}

void list::append(event* newEvent){
    if(!head){
        head = newEvent;
        newEvent->prev = newEvent;
        newEvent->next = newEvent;
    } else{
        event *curr = head;
        while(curr->next != head){
            curr = curr->next;
        }
        curr->next = newEvent;
        newEvent->next = head;
        newEvent->prev = curr;
        head->prev = newEvent;
    }
    newEvent->id = count;
    count++;
}

//печать события
void list::printEvent(event* curr){
    cout << "id: " << curr->id << "\ntype: " << curr->type << "\ndepair: " << curr->depAirport << "\narrAirpot: " << curr->arrAirport << "\ndepTime: " << curr->depTime << "\narrTime: " << curr->arrTime << "\nflightNumber: ";
    cout << curr->flightNumber << "\nplaneBrand: " << curr->planeBrand << "\ndistance: " << curr->distance << "\nonBoard: " << curr->onBoard << "\nsold: " << curr->sold << "\nticketPrice: " << curr->ticketPrice << "\n\n";
}

void list::print(){
    if(empty()){
        cerr << "list is empty\n";
        return; 
    }

    event* curr = head;
    do{
        printEvent(curr);
        curr = curr->next;
    }
    while(curr != head);
}

bool list::empty(){
    if(!head) return true;
    return false;
}

int list::len(){
    return count;
}

event* list::getEventFromConsole(){
    string typeStr = "-1", depAirport = "-", arrAirport = "-", depTimeStr = "-1", arrTimeStr = "-1";          
    string flightNumber = "-", planeBrand = "-", distance = "-", onBoardStr = "-1", soldStr = "-1", ticketPriceStr = "-1";
    int type = -1, depTime = -1, arrTime = -1, onBoard = -1, sold = -1; 
    float ticketPrice = -1;
    event* newEvent = new event{-1, type, depAirport, arrAirport, 
                                depTime, arrTime, flightNumber, planeBrand,
                                distance, onBoard, sold, ticketPrice,
                                nullptr, nullptr};

    printf("Добавьте фильтры для события\n1. Тип события\n2. Аэропорт вылета\n3. Аэропорт прибытия\n");
    printf("4. Время вылета\n5. Время прибытия\n6. Бортовой номер\n7. Марка самолета\n8. Дистанция\n");
    printf("9. Пассажиров на борту\n10. Продано билетов\n11. Цена билета\n12. Выйти из добавления фильтра\n");

    int num;
    string filter;
    do{
        getline(cin, filter);
        num = atoi(filter.c_str());
        getline(cin, filter);
        cout << num << " " << filter << endl;
        switch (num)
        {
        case 1: 
            if(!filter.compare("Dep")){
                newEvent->type = 1;
            } else if(!filter.compare("Arr")) newEvent->type = 0;
            break;
        case 2:
            newEvent->depAirport = filter;
            break;
        case 3:
            newEvent->arrAirport = filter;
            break;
        case 4:
            newEvent->depTime = atoi(depTimeStr.c_str());
            break;
        case 5:
            newEvent->arrTime = atoi(arrTimeStr.c_str());
            break;
        case 6:
            newEvent->flightNumber = filter;
            break;
        case 7:
            newEvent->planeBrand = filter;
            break;
        case 8:
            newEvent->distance = filter;
            break;
        case 9:
            newEvent->onBoard = atoi(onBoardStr.c_str());
            break;
        case 10:
            newEvent->sold = atoi(soldStr.c_str());
            break;
        case 11:
            newEvent->ticketPrice = atof(ticketPriceStr.c_str());
            break;
        default:
            break;
        }
    } while(num != 12);

    return newEvent;
}

bool list::satisfyPatternEvent(event* pattern, event* event){
    bool equals = true;
        if(pattern->type != -1 && pattern->type != event->type) equals = false;
        if(pattern->depAirport.compare("-") && pattern->depAirport.compare(event->depAirport)) equals = false;
        if(pattern->arrAirport.compare("-") && pattern->arrAirport.compare(event->arrAirport)) equals = false;
        if(pattern->depTime != -1 && pattern->depTime != event->depTime) equals = false;
        if(pattern->arrTime != -1 && pattern->arrTime != event->arrTime) equals = false;
        if(pattern->flightNumber.compare("-") && pattern->flightNumber.compare(event->flightNumber)) equals = false;
        if(pattern->planeBrand.compare("-") && pattern->planeBrand.compare(event->planeBrand)) equals = false;
        if(pattern->distance.compare("-") && pattern->distance.compare(event->distance)) equals = false;
        if(pattern->onBoard != -1 && pattern->onBoard != event->onBoard) equals = false;
        if(pattern->sold != -1 && pattern->sold != event->sold) equals = false;
        if(pattern->ticketPrice != -1 && pattern->ticketPrice != event->ticketPrice) equals = false;
    return equals;
}

void list::select(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Найденные события:\n";
    do{
        if(satisfyPatternEvent(Event, curr)) {
            printEvent(curr);
            equalsCount++;
        }

        curr = curr->next;
    }
    while(curr != head);  
    
    if(equalsCount == 0){
        cout << "Собыйтий не найдено\n";
    } else cout << "Событий найдено: " << equalsCount << endl << endl;
}

void list::remove(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Удалённые события:\n";

    while(curr->next != head){
        if(satisfyPatternEvent(Event, curr)){
            printEvent(curr);
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
    if(satisfyPatternEvent(Event, curr)){
            printEvent(curr);
            equalsCount++;

            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            if(curr == head) head = curr->next;
            event* tmp = curr;
            curr = curr->next;
            delete tmp;
            count--;
        } else curr = curr->next;
    
    if(equalsCount == 0){
        cout << "Собыйтий не найдено\n";
    } else cout << "Событий удалено: " << equalsCount << endl << endl;

    curr = head;
    int newId = 0;
    do{
        curr->id = newId;
        newId++;
        curr = curr->next;
    }
    while(curr != head);
}

void list::moveForward(int eventId, int steps){
    if(eventId >= count) {
        cout << "Элемента с таким id нет\n";
        return;
    }
    event* curr = head;
    while(curr->id != eventId) curr = curr->next;
    event* left = curr, *right, *tmp;
    for(int i = 0; i < steps; i++) curr = curr->next;
    right = curr;

    if(left == head) head = left->next;
    // tmp = right;
    
    cout << left->id << " " << right->id << " " << tmp->id << endl;
    // left->prev->next = tmp;
    // right->prev = left->prev;
    // right->next= left->next;
    // left->next->prev = tmp;

    // tmp->prev->next = left;
    // left->prev = tmp->prev;
    // left->next= tmp->next;
    // tmp->next->prev = left;

    tmp = left->prev;
    left->prev->next = left->next;
    left->next->prev = left->prev;

    right->next->prev = left;
    left->next = right->next;
    left->prev = right;
    right->next = left;

    cout << left->prev->id << " " << left->id << " " << left->next->id << endl;
    cout << right->prev->id << " " << right->id << " " << right->next->id << endl;
    cout << tmp->prev->id << " " << tmp->id << " " << tmp->next->id << endl;

    int newId = (tmp->id + 1)%count;
    tmp = tmp->next;
    do{
        tmp->id = newId;
        newId = (newId + 1)%count;
        tmp = tmp->next;
    }
    while(tmp != left->next);

}

void list::moveBack(int eventId, int steps){
    moveForward(eventId, (count - steps - 1)%count);
}

int main(){
    list list;
    cout << list.empty() << endl;
    list.print();


    list.init(path);
    // list.print();
    cout << list.empty() << " " << list.len() << endl;
    list.print();

    // event* custom = list.getEventFromConsole();
    // list.printEvent(custom);

    // list.select(custom);
    // list.remove(custom);
    // list.print();
   
    list.moveForward(0, 5);
    list.print();
    list.moveForward(5, 5);
    list.print();
}