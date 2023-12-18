#include <iostream>
#include <string>
#include <fstream>
#include "eventList.h"
#include "planeList.h"
#include "airportList.h"
#include <iomanip>

//конструктор класса eventList
eventList::eventList(const char *path, airportList airport, planeList plane){
    airports = airport;
    planes = plane;
    head = nullptr;
    tail = nullptr;
    count = 0;
    init(path); //инициализация списка из файла
}

//получение события из файла
event* eventList::getEvent(ifstream& file)
{
    string typeStr, airportStr, dateStr;            
    string flightNumber, planeBrand, distance, onBoardStr, soldStr, ticketPriceStr;
    int type, date, onBoard, sold; 
    float ticketPrice;

    //считать необходимые поля из файла
    //конвертировать в необходимый тип
    getline(file, typeStr);     //пробел
    getline(file, typeStr);
    if(typeStr.compare("Dep")){ //установить необходимый тип события
        type = 0;               //прилет
    } else type = 1;            //вылет

    getline(file, airportStr);
    int airportId = atoi(airportStr.c_str());

    getline(file, dateStr, '\n');
    if(!isDateCorrect(dateStr)){
        cerr << "\nERROR: введена некорректная дата\n";
        system("pause");
        exit(1);   
    }
    date = dateToInt(dateStr);
    

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
    
    //инициализировать новое событие
    event *newEvent = new event;
    newEvent->id = 0;
    newEvent->type = type;
    newEvent->airportId = airportId;
    newEvent->dateStr = dateStr;
    newEvent->date = date;
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

//инициализация списка из файла
void eventList::init(const char *path){
    ifstream file(path);
    string eventsCountString;

    getline(file, eventsCountString);
    int eventsCount = atoi(eventsCountString.c_str());  //счетчик
    event* newEvent;    //создать новое событие
    for(int i = 0; i < eventsCount; i++){
        newEvent = getEvent(file);  //инициализировать из файла
        append(newEvent);           //добавить в конец списка
    }

    file.close();
    printf("Добавлено %d событий\n", eventsCount);
}

//добавление события в конец
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

//печать списка в таблицу
void eventList::print(){
    if(empty()){
        cerr << "eventList пуст\n";
        return; 
    }

    cout << "Всего событий: " << count << endl;
    printTableHead();           //печать головы таблицы
    event* curr = head;
    do{
        printEventRow(curr);    //печать элемента в строке таблицы
        curr = curr->next;
    }
    while(curr != head);
}

//проверка на пустоту
bool eventList::empty(){
    return count == 0;
}

//длина списка
int eventList::len(){
    return count;
}

//удаление самолета из списка
void eventList::removePlane(string name){
    int rmId = planes.remove(name);     //удалить из списка самолетов

    //удалить все связанные с эти самолетом записи в списке
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

//удаление аэропорта из списка
void eventList::removeAirport(string name){
    int rmId = airports.remove(name); //удалить из списка аэропортов

    //удалить все связанные с эти аэропортом записи в списке
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

//получение события из консоли
event* eventList::getEventFromConsole(){
    
    //инициализация дефолтными значениями
    string typeStr = "-1", airport = "-", dateStr = "-1";          
    string flightNumber = "-", planeBrand = "-", distance = "-", onBoardStr = "-1", soldStr = "-1", ticketPriceStr = "-1";
    int type = -1, depdate = -1, arrdate = -1, onBoard = -1, sold = -1; 
    float ticketPrice = -1;

    //заполняем дефолтными значениями
    event *newEvent = new event;
    newEvent->id = -1;
    newEvent->type = -1;
    newEvent->airportId = -1;
    newEvent->date = -1;
    newEvent->dateEnd = -1;
    newEvent->flightNumber = "-";
    newEvent->planeBrandId = -1;
    newEvent->distance = "-";
    newEvent->onBoard = -1;
    newEvent->sold = -1;
    newEvent->ticketPrice = -1;

    printf("Введите поля для события\n0. ID элемента\n1. Тип события\n2. ID аэропорта\n");
    printf("3. Диапазон дат\n4. Бортовой номер\n5. ID марки самолета\n6. Дистанция\n");
    printf("7. Пассажиров на борту\n8. Продано билетов\n9. Цена билета\n10. Выйти из добавления фильтра\n");

    //юзер заполняет необходимые ему поля, создавая шаблон(некоторые поля могут остаться дефолтными)
    int num;
    string filter;
    do{
        getline(cin, filter);
       
        cout << "Операция: ";
        getline(cin, filter);
        num = atoi(filter.c_str());

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
            if(!isDateCorrect(filter)){
                cerr << "\nERROR: введена некорректная дата\n";
                system("pause");
                exit(1);   
            }
            newEvent->date = dateToInt(filter);
            newEvent->dateStr = filter;

            cout << "Дата конца: "; 
            getline(cin, filter);
            newEvent->dateEnd = dateToInt(filter);
            if(!isDateCorrect(filter) || newEvent->date > newEvent->dateEnd){
                cerr << "\nERROR: введена некорректная дата\n";
                system("pause");
                exit(1);   
            }
            newEvent->dateStrEnd = filter;
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

//сравнение шаблона события с событием(для фильтрации)
bool eventList::satisfyPatternEvent(event* pattern, event* event){
    //если поле шаблона установлено в дефолт->игнорируем, иначе сравниваем с событием
    if(pattern->id != -1 && pattern->id != event->id) return false;
    if(pattern->type != -1 && pattern->type != event->type) return false;
    if(pattern->airportId != -1 && pattern->airportId != event->airportId) return false;
    if(pattern->flightNumber.compare("-") && pattern->flightNumber.compare(event->flightNumber)) return false;
    if(pattern->planeBrandId != -1 && pattern->planeBrandId != event->planeBrandId) return false;
    if(pattern->distance.compare("-") && pattern->distance.compare(event->distance)) return false;
    if(pattern->onBoard != -1 && pattern->onBoard != event->onBoard) return false;
    if(pattern->sold != -1 && pattern->sold != event->sold) return false;
    if(pattern->ticketPrice != -1 && pattern->ticketPrice != event->ticketPrice) return false;
    if(pattern->date != -1 && pattern->dateEnd != -1 && (pattern->date > event->date || pattern->dateEnd < event->date)) return false;
    return true;
}

//поиск по списку элементов, соответствующих шаблону
void eventList::select(event* Event){
    if(empty()){
        cout << "Список пуст\n";
        return;
    }

    int equalsCount = 0;
    event* curr = head;
    cout << "Найденные события:\n";
    
    bool tableHead = false;     //для печати шапки таблицы
    do{
        if(satisfyPatternEvent(Event, curr)) {
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

//удаление элементов, соответствующих шаблону
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

//перемещение элемента вперед по списку
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

//перемещение элемента назад по списку
void eventList::moveBack(int eventId, int steps){
    steps = (count - steps - 1)%count;
    if(steps < 0) steps += count;
    moveForward(eventId, steps);
}

//печать головы таблицы
void eventList::printTableHead(){
    int spaces[] = {2, 4, 17, 10, 12, 15, 8, 7, 4, 11};
    
    cout << "+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }

    cout << "\n|id|type|     airport     |   date   |flightNumber|  planeBrand   |distance|onBoard|sold|ticketPrice|\n+";
    
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}

//печать события в строке таблицы
void eventList::printEventRow(event* curr){
    int spaces[] = {2, 4, 17, 10, 12, 15, 8, 7, 4, 11};
    string etype;
    plane *plane = planes.getPlane(curr->planeBrandId);
    if(curr->type){
        etype = "Dep";
    } else etype = "Arr";
    
    cout << "|" << setw(2) << curr->id << "|" << setw(4) << etype << "|" << setw(17) << airports.getData(curr->airportId) << "|";
    cout << setw(10) << curr->dateStr << "|" << setw(12) << curr->flightNumber << "|" << setw(15) << plane->name << "|" << setw(8) << curr->distance;
    cout << "|" << setw(7) << curr->onBoard << "|" << setw(4) << curr->sold << "|" << setw(11) << curr->ticketPrice << "|\n+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}

//дата string(дд.мм.гг)->int        
int eventList::dateToInt(string date){
    int days = (date[0] - 48) * 10 + date[1] - 48;
    int months = (date[3] - 48) * 10 + date[4] - 48;
    int years = (date[6] - 48) * 10 + date[7] - 48;
    return days + 30*months + 365*years;
}

//проверка введенной даты на корректность (дд.мм.гг)
bool eventList::isDateCorrect(string date){
    if(date.length() != 8 || date[2] != 46 || date[5] != 46)  return false; //46 - код "."
    if(date[0] < 48 || date[0] > 57)  return false;     //является ли символ цифрой
    if(date[1] < 48 || date[1] > 57)  return false;     //48 - код "0", 57 - код "9"
    if(date[3] < 48 || date[3] > 57)  return false;
    if(date[4] < 48 || date[4] > 57)  return false;
    if(date[6] < 48 || date[6] > 57)  return false;
    if(date[7] < 48 || date[7] > 57)  return false;

    int days = (date[0] - 48) * 10 + date[1] - 48;
    int months = (date[3] - 48) * 10 + date[4] - 48;
    int years = (date[6] - 48) * 10 + date[7] - 48;

    if(days < 1 || days > 31 || months < 1 || months > 12 || years < 0) return false;   //корректность дня, месяца и года

    return true;
}

