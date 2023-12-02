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
        void find(event* Event);

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

void list::find(event* Event){
    if(empty()){
        cout << "Событий не найдено\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Найденные события:\n";
    do{
        bool equals = true;
        if(Event->type != -1 && Event->type != curr->type) equals = false;
        if(Event->depAirport.compare("-") && Event->depAirport.compare(curr->depAirport)) equals = false;
        if(Event->arrAirport.compare("-") && Event->arrAirport.compare(curr->arrAirport)) equals = false;
        if(Event->depTime != -1 && Event->depTime != curr->depTime) equals = false;
        if(Event->arrTime != -1 && Event->arrTime != curr->arrTime) equals = false;
        if(Event->flightNumber.compare("-") && Event->flightNumber.compare(curr->flightNumber)) equals = false;
        if(Event->planeBrand.compare("-") && Event->planeBrand.compare(curr->planeBrand)) equals = false;
        if(Event->distance.compare("-") && Event->distance.compare(curr->distance)) equals = false;
        if(Event->onBoard != -1 && Event->onBoard != curr->onBoard) equals = false;
        if(Event->sold != -1 && Event->sold != curr->sold) equals = false;
        if(Event->ticketPrice != -1 && Event->ticketPrice != curr->ticketPrice) equals = false;

        if(equals) {
            printEvent(curr);
            equalsCount++;
        }

        curr = curr->next;
    }
    while(curr != head);  
    
    if(equalsCount == 0){
        cout << "Собыйтий не найдено\n";
    } else cout << "Событий найдено " << equalsCount << endl;
}

int main(){
    list list;
    cout << list.empty() << endl;
    list.print();


    list.init(path);
    // list.print();
    cout << list.empty() << " " << list.len() << endl;

    event* custom = list.getEventFromConsole();
    list.printEvent(custom);

    list.find(custom);
   

}