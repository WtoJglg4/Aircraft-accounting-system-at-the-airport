#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int arrival = 0, departure = 1;
const char *eventsPath = "input/input.txt", *planesPath = "input/planes.txt", *airoportsPath = "input/aeroports.txt";

struct plane{
    int id;
    string name;     
    int capacity;   //вместимость
    plane *next, *prev;
};

class planeList{
    protected:
        
        int count;
    public:
        plane *head, *tail;

        planeList();
        
        int length();
        bool empty();

        void append(int index, string data, int capacity);
        plane* getPlane(int id);
        void init(const char *path);
        int remove(string name);
        void print();
        // string removeById(int id);
        // int removeByData(string data);
};

planeList::planeList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

int planeList::length(){
    return count;
}


bool planeList::empty(){
    return count == 0;
}


void planeList::append(int index, string data, int capacity){
    if(!head){
        head = new plane{index, data, capacity, nullptr, nullptr};
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        tail->next = new plane{index, data, capacity, head, tail};
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

void planeList::print(){
    
    cout << "planesList:\n";
    plane* curr = head;
    do{
        cout << curr->id << " " << curr->name << " " << curr->capacity << endl;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    cout << endl << endl;
}

void planeList::init(const char *path){
    ifstream file(path);

    string data;
    int capacity;
    while(!file.eof()){
        int index, capacity;
        file >> index;
        getline(file, data);
        data.erase(0, 1);
        file >> capacity;
        append(index, data, capacity);
    }
    file.close();

    printf("Добавлено %d элементов\n", count);
}

plane* planeList::getPlane(int id){
    if(id < 0 || id >= count){
        cerr << "в списке нет элемента с таким id " << id;
        exit(1); 
    }

    plane* curr = head;
    do{
        if(curr->id == id) return curr;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    cerr << "в списке нет элемента с таким id " << id;
    exit(1); 
}

int planeList::remove(string name){
    plane *curr = head;
    do{
        if(!name.compare(curr->name)) break;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    if(curr == head){
        if(name.compare(curr->name)) return -1; //не найден
        head = head->next;
    }
    if(curr == tail) tail = tail->prev;

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    
    int id = curr->id;
    plane *tmp = curr;
    curr = curr->next;
    delete tmp;
    count--;
    return id;
}


struct airport{
    int id;
    string name;
    airport *next, *prev;
};

class airportList{
    protected:
        airport *head, *tail;
        int count;
    public:
        airportList();
        
        int length();
        bool empty();

        void append(int index, string data);
        int getId(string name);
        string getData(int id);
        int remove(string name);
        void init(const char *path);
        void print();
        // string removeById(int id);
        // int removeByData(string data);
};

airportList::airportList(){
    head = nullptr;
    tail = nullptr;
    count = 0;
}

int airportList::length(){
    return count;
}


bool airportList::empty(){
    return count == 0;
}


void airportList::append(int index, string data){
    if(!head){
        head = new airport{index, data, nullptr, nullptr};
        head->prev = head;
        head->next = head;
        tail = head;
    } else{
        tail->next = new airport{index, data, head, tail};
        head->prev = tail->next;
        tail = tail->next;
    }
    count++;
}

void airportList::print(){
    
    cout << "airportsList:\n";
    airport* curr = head;
    do{
        cout << curr->id << " " << curr->name << endl;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    cout << endl << endl;
}

void airportList::init(const char *path){
    ifstream file(path);

    string data;
    while(!file.eof()){
        int index;
        file >> index;
        getline(file, data);
        data.erase(0, 1);

        append(index, data);
    }
    file.close();

    printf("Добавлено %d элементов\n", count);
}

int airportList::getId(string data){
    airport* curr = head;
    do{
        if(!data.compare(curr->name)) return curr->id;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
    
    cerr << "в списке нет элемента " << data << endl;
    exit(1);
}

string airportList::getData(int id){
    if(id < 0 || id >= count){
        cerr << "в списке нет элемента с таким id " << id;
        exit(1); 
    }

    airport* curr = head;
    do{
        if(curr->id == id) return curr->name;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    cerr << "в списке нет элемента с таким id " << id;
    exit(1); 
}

int airportList::remove(string name){
    airport *curr = head;
    do{
        if(!name.compare(curr->name)) break;
        curr = curr->next;
    }
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию

    if(curr == head){
        if(name.compare(curr->name)) return -1; //не найден
        head = head->next;
    }
    if(curr == tail) tail = tail->prev;

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    
    int id = curr->id;
    airport *tmp = curr;
    curr = curr->next;
    delete tmp;
    count--;
    return id;
}



struct event{
    int id;                 //индекс в списке
    int type;               //тип события(вылет/прилет)
    int airportId;          //аэропорт
    int time;               //время
    string flightNumber;    //номер рейса
    int planeBrandId;       //марка самолета
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

eventList::eventList(airportList airport, planeList plane){
    airports = airport;
    planes = plane;
    head = nullptr;
    tail = nullptr;
    count = 0;
}

event* eventList::getEvent(ifstream& file)
{
    string typeStr, airportStr, timeStr;          
    string flightNumber, planeBrand, distance, onBoardStr, soldStr, ticketPriceStr;
    int type, time, onBoard, sold; 
    float ticketPrice;

    getline(file, typeStr);    //пробел
    getline(file, typeStr);
    if(typeStr.compare("Dep")){
        type = arrival;
    } else type = departure;

    getline(file, airportStr);
    int airportId = atoi(airportStr.c_str());

    getline(file, timeStr, '\n');
    time = atoi(timeStr.c_str());

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
    newEvent->time = time;
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

    for(int i = 0; i < eventsCount; i++){
        event* newEvent = getEvent(file);
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
    cout << "id: " << curr->id << "\ntype: " << etype << "\nairport: " << airports.getData(curr->airportId) << "\nTime: " << curr->time << "\nflightNumber: ";
    cout << curr->flightNumber << "\nplaneBrand: " << plane->name << "\ndistance: " << curr->distance << "\nonBoard: " << curr->onBoard << "\nsold: " << curr->sold << "\nticketPrice: " << curr->ticketPrice << "\n\n";
}

void eventList::print(){
    if(empty()){
        cerr << "eventList пуст\n";
        return; 
    }

    event* curr = head;
    cout << "Всего событий: " << count << endl;
    do{
        printEvent(curr);
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
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
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
    while(curr != head);    //если использовать обычный while, придется после него делать еще одну итерацию
}

event* eventList::getEventFromConsole(){
    string typeStr = "-1", airport = "-", timeStr = "-1";          
    string flightNumber = "-", planeBrand = "-", distance = "-", onBoardStr = "-1", soldStr = "-1", ticketPriceStr = "-1";
    int type = -1, depTime = -1, arrTime = -1, onBoard = -1, sold = -1; 
    float ticketPrice = -1;

    event *newEvent = new event;
    newEvent->id = -1;
    newEvent->type = -1;
    newEvent->airportId = -1;
    newEvent->time = -1;
    newEvent->flightNumber = "-";
    newEvent->planeBrandId = -1;
    newEvent->distance = "-";
    newEvent->onBoard = -1;
    newEvent->sold = -1;
    newEvent->ticketPrice = -1;

    printf("Введите поля для события\n0. ID элемента\n1. Тип события\n2. ID аэропорта\n");
    printf("3. Время\n4. Бортовой номер\n5. ID марки самолета\n6. Дистанция\n");
    printf("7. Пассажиров на борту\n8. Продано билетов\n9. Цена билета\n10. Выйти из добавления фильтра\n");

    int num;
    string filter;
    do{
        getline(cin, filter);
        num = atoi(filter.c_str());
        getline(cin, filter);
        switch (num)
        {
        case 0:
            newEvent->id = atoi(filter.c_str());
            break;
        case 1: 
            if(!filter.compare("Dep")){
                newEvent->type = 1;
            } else if(!filter.compare("Arr")) newEvent->type = 0;
            break;
        case 2:
            newEvent->airportId = atoi(filter.c_str());
            break;
        case 3:
            newEvent->time = atoi(filter.c_str());
            break;
        case 4:
            newEvent->flightNumber = filter;
            break;
        case 5:
            newEvent->planeBrandId = atoi(filter.c_str());;
            break;
        case 6:
            newEvent->distance = filter;
            break;
        case 7:
            newEvent->onBoard = atoi(filter.c_str());
            break;
        case 8:
            newEvent->sold = atoi(filter.c_str());
            break;
        case 9:
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
        if(pattern->time != -1 && pattern->time != event->time) return false;
        if(pattern->flightNumber.compare("-") && pattern->flightNumber.compare(event->flightNumber)) return false;
        if(pattern->planeBrandId != -1 && pattern->planeBrandId != event->planeBrandId) return false;
        if(pattern->distance.compare("-") && pattern->distance.compare(event->distance)) return false;
        if(pattern->onBoard != -1 && pattern->onBoard != event->onBoard) return false;
        if(pattern->sold != -1 && pattern->sold != event->sold) return false;
        if(pattern->ticketPrice != -1 && pattern->ticketPrice != event->ticketPrice) return false;
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

void eventList::remove(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Удалённые события:\n";

    do{
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
    steps = (count - steps - 1)%count; //????????????????????????
    if(steps < 0) steps += count;
    moveForward(eventId, steps);
}

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
}