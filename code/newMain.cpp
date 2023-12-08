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
        plane *head, *tail;
        int count;
    public:
        planeList();
        
        int length();
        bool empty();

        void append(int index, string data, int capacity);
        plane* getPlane(int id);
        void init(const char *path);
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
    
    cout << "list:\n";
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
        int getId(string data);
        string getData(int id);
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
    
    cout << "list:\n";
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
        airportList airports;
        planeList planes;
        
        int count;

    public:
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
    
    
    //event->

    return new event{0, type, airportId, time, flightNumber, planeBrandId, distance, onBoard, sold, ticketPrice, nullptr, nullptr};
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

// event* eventList::getEventFromConsole(){
//     string typeStr = "-1", airport = "-", timeStr = "-1";          
//     string flightNumber = "-", planeBrand = "-", distance = "-", onBoardStr = "-1", soldStr = "-1", ticketPriceStr = "-1";
//     int type = -1, depTime = -1, arrTime = -1, onBoard = -1, sold = -1; 
//     float ticketPrice = -1;
//     event* newEvent = new event{-1, type, depAirport, arrAirport, 
//                                 depTime, arrTime, flightNumber, planeBrand,
//                                 distance, onBoard, sold, ticketPrice,
//                                 nullptr, nullptr};

//     printf("Введите поля для события\n0. ID элемента\n1. Тип события\n2. Аэропорт вылета\n3. Аэропорт прибытия\n");
//     printf("4. Время вылета\n5. Время прибытия\n6. Бортовой номер\n7. Марка самолета\n8. Дистанция\n");
//     printf("9. Пассажиров на борту\n10. Продано билетов\n11. Цена билета\n12. Выйти из добавления фильтра\n");

//     int num;
//     string filter;
//     do{
//         getline(cin, filter);
//         num = atoi(filter.c_str());
//         getline(cin, filter);
//         if(num != 12) cout << num << " " << filter << endl;        
//         switch (num)
//         {
//         case 0:
//             newEvent->id = atoi(filter.c_str());
//             break;
//         case 1: 
//             if(!filter.compare("Dep")){
//                 newEvent->type = 1;
//             } else if(!filter.compare("Arr")) newEvent->type = 0;
//             break;
//         case 2:
//             newEvent->depAirport = filter;
//             break;
//         case 3:
//             newEvent->arrAirport = filter;
//             break;
//         case 4:
//             newEvent->depTime = atoi(filter.c_str());
//             break;
//         case 5:
//             newEvent->arrTime = atoi(filter.c_str());
//             break;
//         case 6:
//             newEvent->flightNumber = filter;
//             break;
//         case 7:
//             newEvent->planeBrand = filter;
//             break;
//         case 8:
//             newEvent->distance = filter;
//             break;
//         case 9:
//             newEvent->onBoard = atoi(filter.c_str());
//             break;
//         case 10:
//             newEvent->sold = atoi(filter.c_str());
//             break;
//         case 11:
//             newEvent->ticketPrice = atof(filter.c_str());
//             break;
//         default:
//             break;
//         }
//     } while(num != 12);

//     return newEvent;
// }

// bool eventList::satisfyPatternEvent(event* pattern, event* event){
//     bool equals = true;
//         if(pattern->id != -1 && pattern->id != event->id) equals = false;
//         if(pattern->type != -1 && pattern->type != event->type) equals = false;
//         if(pattern->depAirport.compare("-") && pattern->depAirport.compare(event->depAirport)) equals = false;
//         if(pattern->arrAirport.compare("-") && pattern->arrAirport.compare(event->arrAirport)) equals = false;
//         if(pattern->depTime != -1 && pattern->depTime != event->depTime) equals = false;
//         if(pattern->arrTime != -1 && pattern->arrTime != event->arrTime) equals = false;
//         if(pattern->flightNumber.compare("-") && pattern->flightNumber.compare(event->flightNumber)) equals = false;
//         if(pattern->planeBrand.compare("-") && pattern->planeBrand.compare(event->planeBrand)) equals = false;
//         if(pattern->distance.compare("-") && pattern->distance.compare(event->distance)) equals = false;
//         if(pattern->onBoard != -1 && pattern->onBoard != event->onBoard) equals = false;
//         if(pattern->sold != -1 && pattern->sold != event->sold) equals = false;
//         if(pattern->ticketPrice != -1 && pattern->ticketPrice != event->ticketPrice) equals = false;
//     return equals;
// }

// void eventList::select(event* Event){
//     if(empty()){
//         cout << "Список пуст\n";
//         return;
//     }

//     int equalsCount = 0;
//     event* curr = head;
//     cout << "Найденные события:\n";
//     do{
//         if(satisfyPatternEvent(Event, curr)) {
//             printEvent(curr);
//             equalsCount++;
//         }

//         curr = curr->next;
//     }
//     while(curr != head);  
    
//     if(equalsCount == 0){
//         cout << "Собыйтий не найдено\n";
//     } else cout << "Событий найдено: " << equalsCount << endl << endl;
// }

// void eventList::remove(event* Event){
//     if(empty()){
//         cout << "Список пуст\n";
//         return;
//     }

//     int equalsCount = 0;
//     event* curr = head;
//     cout << "Удалённые события:\n";

//     while(curr->next != head){
//         if(satisfyPatternEvent(Event, curr)){
//             printEvent(curr);
//             equalsCount++;

//             curr->prev->next = curr->next;
//             curr->next->prev = curr->prev;
//             if(curr == head) head = curr->next;
//             event* tmp = curr;
//             curr = curr->next;
//             delete tmp;
//             count--;
//         } else curr = curr->next;
//     }
//     if(satisfyPatternEvent(Event, curr)){
//             printEvent(curr);
//             equalsCount++;

//             curr->prev->next = curr->next;
//             curr->next->prev = curr->prev;
//             if(curr == head) head = curr->next;
//             event* tmp = curr;
//             curr = curr->next;
//             delete tmp;
//             count--;
//         } else curr = curr->next;
    
//     if(equalsCount == 0){
//         cout << "Собыйтий не найдено\n";
//     } else cout << "Событий удалено: " << equalsCount << endl << endl;

//     curr = head;//??????????????????
//     int newId = 0;
//     do{
//         curr->id = newId;
//         newId++;
//         curr = curr->next;
//     }
//     while(curr != head);
// }

// void eventList::moveForward(int eventId, int steps){
//     if(eventId >= count) {
//         cout << "Элемента с таким id нет\n";
//         return;
//     }
//     event* curr = head;
//     while(curr->id != eventId) curr = curr->next;
//     event* left = curr, *right, *tmp;
//     for(int i = 0; i < steps; i++) curr = curr->next;
//     right = curr;

//     if(left == head) head = left->next;

//     tmp = left->prev;
//     left->prev->next = left->next;
//     left->next->prev = left->prev;

//     right->next->prev = left;
//     left->next = right->next;
//     left->prev = right;
//     right->next = left;

//     int newId = (tmp->id + 1)%count;
//     tmp = tmp->next;
//     do{
//         tmp->id = newId;
//         newId = (newId + 1)%count;
//         tmp = tmp->next;
//     }
//     while(tmp != left->next);
// }

// void eventList::moveBack(int eventId, int steps){
//     moveForward(eventId, (count - steps - 1)%count);
// }

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
    eventList eventList(airports, planes);
    
    cout << "ПЕРВИЧНАЯ ИНИЦИАЛИЗАЦИЯ:\n";
    eventList.init(eventsPath);
    eventList.print();

    // cout << "ДОБАВЛЕНИЕ НОВОГО ЭЛЕМЕНТА В СПИСОК:\n";
    // eventList.append(eventList.getEventFromConsole());
    // eventList.print();

    // cout << "ПОИСК ЭЛЕМЕНТА:\n";
    // //проверить удаление по id(селект работает)
    // event* custom = eventList.getEventFromConsole();
    // eventList.select(custom);
    

    // cout << "УДАЛЕНИЕ ЭЛЕМЕНТА:\n";
    // //проверить удаление по id(селект работает)
    // eventList.remove(custom);
    // eventList.print();

    // cout << "ПЕРЕМЕЩЕНИЕ В ПРЯМОМ НАПРАВЛЕНИИ:\n";
    // int eventId, steps;
    // cout << "Введите индекс перемещаемого элемента: ";
    // cin >> eventId;
    // cout << "Введите кол-во шагов: ";
    // cin >> steps;
    // cout << endl;
    // eventList.moveForward(eventId, steps);
    // eventList.print();

    // cout << "ПЕРЕМЕЩЕНИЕ В ОБРАТНОМ НАПРАВЛЕНИИ:\n";
    // cout << "Перемещаем " << eventId << "-й элемент на " << steps << "поз. назад\n\n" ;
    // eventList.moveForward(eventId, steps);
    // eventList.print();
}