#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int strSize = 20;
const int arrival = 0, departure = 1, shortDist = 0, longDist = 1;
const char *path = "input/input.txt";

struct event{
    int id;             //индекс в списке
    int type;           //тип события
    string depAirport;   //аэропорт вылета
    string arrAirport;   //аэропорт прибытия
    int depTime;      //время вылета
    int arrTime;      //время прибытия
    string flightNumber;   //номер рейса
    string planeBrand;  //марка самолета
    string distance;       //ближний/дальний рейс
    int onBoard;        //пассажиров на борту
    int sold;           //проданных билетов
    float ticketPrice;  //цена билета

    event *next, *prev; //следующий, предыдущий элементы
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
}

void list::append(event* newEvent){
    if(!head){
        head = newEvent;
        newEvent->prev = newEvent;
        newEvent->next = newEvent;
    } else{
        event *curr = head, *prev;
        while(curr->next != head){
            prev = curr;
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

void list::print(){
    if(empty()){
        cerr << "list is empty\n";
        return; 
    }

    event* curr = head;
    do{
        //печать элемента
        cout << "id: " << curr->id << "\ntype: " << curr->type << "\ndepair: " << curr->depAirport << "\narrAirpot: " << curr->arrAirport << "\ndepTime: " << curr->depTime << "\narrTime: " << curr->arrTime << "\nflightNumber: ";
        cout << curr->flightNumber << "\nplaneBrand: " << curr->planeBrand << "\ndistance: " << curr->distance << "\nonBoard: " << curr->onBoard << "\nsold: " << curr->sold << "\nticketPrice: " << curr->ticketPrice << "\n\n";
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

int main(){
    list list;
    cout << list.empty() << endl;
    list.print();


    list.init(path);
    list.print();
    cout << list.empty() << " " << list.len() << endl;


   

}