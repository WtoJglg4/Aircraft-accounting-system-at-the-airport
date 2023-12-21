/********************************************************************
*                КАФЕДРА № 304 2 КУРС  1 СЕМ                        *
*-------------------------------------------------------------------*
* File Name     : eventList.cpp                                     *
* Language      : C++                                               *
* Programmer(s) : Глазов В.В.                                       *
* Modifyed By   :                                                   *
* Created       : 20/11/23                                          *
* Last Revision : 18/12/23                                          *
* Comment(s)    : Курсовая работа                                   *
*********************************************************************/
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
        cerr << "\nError: введена некорректная дата\n";
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
        Error* err = printEventRow(curr);    //печать элемента в строке таблицы
        if(err){
            cout << err->message;
            return;
        }
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
        // getline(cin, filter);
       
        while(true){
            cout << "Операция: ";
            getline(cin, filter);
            if(filter.length() == 1 && filter[0] > 47 && filter[0] < 58 || filter.length() == 2 && filter[0] == 49 && filter[1] == 48){
                break;
            }
            cerr << "Некорректный ввод. Введите заново.\n";
        }
        
        num = atoi(filter.c_str());

        switch (num)
        {
        case 0:         
            while(true){
                cout << "ID: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cout << "Некорректный ввод. Введите заново.\n";
            }            
            newEvent->id = atoi(filter.c_str());
            break;
        case 1: 
            while(true){
                cout << "Type: ";
                getline(cin, filter);
                if(!filter.compare("Dep")){
                    newEvent->type = 1;
                    break;
                } else if(!filter.compare("Arr")){
                    newEvent->type = 0;
                    break;
                }
                cout << "Некорректный ввод. Введите заново.\n";
            }
            break;
        case 2:
            airports.print();
            while(true){
                cout << "Airport ID: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cerr << "Некорректный ввод. Введите заново.\n";
            }   
            newEvent->airportId = atoi(filter.c_str());
            break;
        case 3:
            while(true){
                cout << "Дата начала: ";
                getline(cin, filter);
                if(!isDateCorrect(filter)){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;
                }
                newEvent->date = dateToInt(filter);
                newEvent->dateStr = filter;

                cout << "Дата конца: "; 
                getline(cin, filter);
                newEvent->dateEnd = dateToInt(filter);
                if(!isDateCorrect(filter) || newEvent->date > newEvent->dateEnd){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;   
                } else{
                    newEvent->dateStrEnd = filter;
                    break;
                }
            }
            break;
        case 4:
            cout << "Бортовой номер: ";
            newEvent->flightNumber = filter;
            break;
        case 5:
            planes.print();
            while(true){
                cout << "ID марки самолета: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cerr << "Некорректный ввод. Введите заново.\n";
            } 
            newEvent->planeBrandId = atoi(filter.c_str());
            break;
        case 6:
            cout << "Дистанция: ";
            getline(cin, filter);
            newEvent->distance = filter;
            break;
        case 7:
            while(true){
                cout << "Пассажиры: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cerr << "Некорректный ввод. Введите заново.\n";
            }
            newEvent->onBoard = atoi(filter.c_str());
            break;
        case 8:
            while(true){
                cout << "Проданных билетов: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cerr << "Некорректный ввод. Введите заново.\n";
            }
            newEvent->sold = atoi(filter.c_str());
            break;
        case 9:
            while(true){
                cout << "Цена билета: ";
                getline(cin, filter);
                if(isDigitCorrect(filter)) break;
                cerr << "Некорректный ввод. Введите заново.\n";
            }
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
            Error* err = printEventRow(curr);    //печать элемента в строке таблицы
            if(err){
                cout << err->message;
                return;
            }
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
            
            Error* err = printEventRow(curr);    //печать элемента в строке таблицы
            if(err){
                cout << err->message;
                return;
            }
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
Error* eventList::printEventRow(event* curr){
    int spaces[] = {2, 4, 17, 10, 12, 15, 8, 7, 4, 11};
    string etype = "-";
    Error* err = new Error;

    if(curr->type == 1){
        etype = "Dep";
    } else if (curr->type == 0) etype = "Arr";
    
    string airportName = "-";
    if(curr->airportId > -1){   
        airport* airport = airports.getAirport(curr->airportId);
        if(!airport){
            err->code = 1;
            err->message = "ошибка печати события\n";
            return err;
        }
        airportName = airport->name;
    }

    string planeName = "-";
    if(curr->planeBrandId > -1){
        plane *plane = planes.getPlane(curr->planeBrandId);
        if(!plane){
            err->code = 1;
            err->message = "ошибка печати события\n";
            return err;
        }
        planeName = plane->name;
    }
    
    
    cout << "|" << setw(2) << curr->id << "|" << setw(4) << etype << "|" << setw(17) << airportName << "|";
    cout << setw(10) << curr->dateStr << "|" << setw(12) << curr->flightNumber << "|" << setw(15) << planeName << "|" << setw(8) << curr->distance;
    cout << "|" << setw(7) << curr->onBoard << "|" << setw(4) << curr->sold << "|" << setw(11) << curr->ticketPrice << "|\n+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;

    return nullptr;
}

//дата string(дд.мм.гг)->int        
int eventList::dateToInt(string date){
    int days = (date[0] - 48) * 10 + date[1] - 48;
    int months = (date[3] - 48) * 10 + date[4] - 48;
    int years = (date[6] - 48) * 10 + date[7] - 48;
    return days + 30*months + 365*years;
}

bool eventList::isDigitCorrect(string idStr){
    for(int i = 0; i < idStr.length(); i++){
        if(idStr[i] < 48 || idStr[i] > 57)  return false;
    }
    return true;
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

void eventList::queries(){  
    event* defaultEvent = new event;
    defaultEvent->id = -1;
    defaultEvent->type = -1;
    defaultEvent->airportId = -1;
    defaultEvent->date = -1;
    defaultEvent->dateEnd = -1;
    defaultEvent->flightNumber = "-";
    defaultEvent->planeBrandId = -1;
    defaultEvent->distance = "-";
    defaultEvent->onBoard = -1;
    defaultEvent->sold = -1;
    defaultEvent->ticketPrice = -1;

    //5 запросов
    cout << "\n1. Прилеты/вылеты из/в конкретного аэропорта\n";
    cout << "2. Прилеты/вылеты из/в конкретного аэропорта в заданное время\n";
    cout << "3. Прилеты/вылеты самолетов заданной марки в заданное время\n";
    cout << "4. Прилеты в конкретный аэропорт в заданное время\n";
    cout << "5. Вылеты из конкретного аэропорта в заданное время\n";
    cout << "6. Прилеты/вылеты заданной дальности\n";
    string command, filter;
    while(true){
        cout << "Операция: ";
        getline(cin, command);
        if(command.length() == 1 && command[0] > 47 && command[0] < 58){
            break;
        }
        cerr << "Некорректный ввод. Введите заново.\n";
    }
    int com = atoi(command.c_str()), id;

    switch(com){
        case 1:
            airports.print();
            while(true){
                cout << "Airport ID: ";
                getline(cin, filter);

                if(!isDigitCorrect(filter)){
                    cerr << "Некорректный ввод. Введите заново.\n";
                    continue;
                } else if(atoi(filter.c_str()) < 0 || atoi(filter.c_str()) >= airports.length()){
                    cerr << "Такого ID не существует. Введите заново.\n";
                    continue;
                }
                break;
            }   
            defaultEvent->airportId = atoi(filter.c_str());

            cout << "\nРЕЗУЛЬТАТ: Прилеты/вылеты из/в "<< airports.getAirport(defaultEvent->airportId)->name << ":\n";
            break;
        case 2:
            airports.print();
            while(true){
                cout << "Airport ID: ";
                getline(cin, filter);

                if(!isDigitCorrect(filter)){
                    cerr << "Некорректный ввод. Введите заново.\n";
                    continue;
                } else if(atoi(filter.c_str()) < 0 || atoi(filter.c_str()) >= airports.length()){
                    cerr << "Такого ID не существует. Введите заново.\n";
                    continue;
                }
                break;
            }   
            defaultEvent->airportId = atoi(filter.c_str());

            while(true){
                cout << "Дата начала: ";
                getline(cin, filter);
                if(!isDateCorrect(filter)){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;
                }
                defaultEvent->date = dateToInt(filter);
                defaultEvent->dateStr = filter;

                cout << "Дата конца: "; 
                getline(cin, filter);
                defaultEvent->dateEnd = dateToInt(filter);
                if(!isDateCorrect(filter) || defaultEvent->date > defaultEvent->dateEnd){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;   
                } else{
                    defaultEvent->dateStrEnd = filter;
                    break;
                }
            }

            cout << "\nРЕЗУЛЬТАТ: Прилеты/вылеты из/в " << airports.getAirport(defaultEvent->airportId)->name;
            cout << " в промежутке с " << defaultEvent->dateStr << " по " << defaultEvent->dateStrEnd << ":\n";
            break;
        case 3:
            planes.print();
            while(true){
               cout << "ID марки самолета: ";
                getline(cin, filter);

                if(!isDigitCorrect(filter)){
                    cerr << "Некорректный ввод. Введите заново.\n";
                    continue;
                } else if(atoi(filter.c_str()) < 0 || atoi(filter.c_str()) >= planes.length()){
                    cerr << "Такого ID не существует. Введите заново.\n";
                    continue;
                }
                break;
            }   
            defaultEvent->planeBrandId = atoi(filter.c_str());

            while(true){
                cout << "Дата начала: ";
                getline(cin, filter);
                if(!isDateCorrect(filter)){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;
                }
                defaultEvent->date = dateToInt(filter);
                defaultEvent->dateStr = filter;

                cout << "Дата конца: "; 
                getline(cin, filter);
                defaultEvent->dateEnd = dateToInt(filter);
                if(!isDateCorrect(filter) || defaultEvent->date > defaultEvent->dateEnd){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;   
                } else{
                    defaultEvent->dateStrEnd = filter;
                    break;
                }
            }
            
            cout << "\nРЕЗУЛЬТАТ: Прилеты/вылеты " << planes.getPlane(defaultEvent->planeBrandId)->name;
            cout << " в промежутке с " << defaultEvent->dateStr << " по " << defaultEvent->dateStrEnd << ":\n";
            break;   
        case 4:
            defaultEvent->type = 0;

            airports.print();
            while(true){
                cout << "Airport ID: ";
                getline(cin, filter);

                if(!isDigitCorrect(filter)){
                    cerr << "Некорректный ввод. Введите заново.\n";
                    continue;
                } else if(atoi(filter.c_str()) < 0 || atoi(filter.c_str()) >= airports.length()){
                    cerr << "Такого ID не существует. Введите заново.\n";
                    continue;
                }
                break;
            }   
            defaultEvent->airportId = atoi(filter.c_str());

            while(true){
                cout << "Дата начала: ";
                getline(cin, filter);
                if(!isDateCorrect(filter)){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;
                }
                defaultEvent->date = dateToInt(filter);
                defaultEvent->dateStr = filter;

                cout << "Дата конца: "; 
                getline(cin, filter);
                defaultEvent->dateEnd = dateToInt(filter);
                if(!isDateCorrect(filter) || defaultEvent->date > defaultEvent->dateEnd){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;   
                } else{
                    defaultEvent->dateStrEnd = filter;
                    break;
                }
            }
            
            cout << "\nРЕЗУЛЬТАТ: Прилеты в " << airports.getAirport(defaultEvent->airportId)->name;
            cout << " в промежутке с " << defaultEvent->dateStr << " по " << defaultEvent->dateStrEnd << ":\n";
            break;
        case 5:
            defaultEvent->type = 1;

            airports.print();
            while(true){
                cout << "Airport ID: ";
                getline(cin, filter);

                if(!isDigitCorrect(filter)){
                    cerr << "Некорректный ввод. Введите заново.\n";
                    continue;
                } else if(atoi(filter.c_str()) < 0 || atoi(filter.c_str()) >= airports.length()){
                    cerr << "Такого ID не существует. Введите заново.\n";
                    continue;
                }
                break;
            }   
            defaultEvent->airportId = atoi(filter.c_str());

            while(true){
                cout << "Дата начала: ";
                getline(cin, filter);
                if(!isDateCorrect(filter)){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;
                }
                defaultEvent->date = dateToInt(filter);
                defaultEvent->dateStr = filter;

                cout << "Дата конца: "; 
                getline(cin, filter);
                defaultEvent->dateEnd = dateToInt(filter);
                if(!isDateCorrect(filter) || defaultEvent->date > defaultEvent->dateEnd){
                    cerr << "\nError: введена некорректная дата\n";
                    continue;   
                } else{
                    defaultEvent->dateStrEnd = filter;
                    break;
                }
            }

            cout << "\nРЕЗУЛЬТАТ: Вылеты из " << airports.getAirport(defaultEvent->airportId)->name;
            cout << " в промежутке с " << defaultEvent->dateStr << " по " << defaultEvent->dateStrEnd << ":\n";
            break;
        case 6:
            cout << "Дистанция: ";
            getline(cin, filter);
            defaultEvent->distance = filter;
            
            cout << "\nРЕЗУЛЬТАТ: Прилеты/вылеты дальности " << defaultEvent->distance << ":\n";
            break;
        default:
            break;
    }

    select(defaultEvent);
}

