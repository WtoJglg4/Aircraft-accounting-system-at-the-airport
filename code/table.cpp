
#include <iostream>
using namespace std;

void printTableHead(){
   int spaces[] = {2, 4, 7, 4, 12, 10, 8, 7, 4, 11};
    
    cout << "+";
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
    
    cout << "\n|id|type|airport|time|flightNumber|planeBrand|distance|onBoard|sold|ticketPrice|\n+";
    
    for(int i = 0; i < size(spaces); i++){
        for(int j = 0; j < spaces[i]; j++) cout << "-";
        cout << "+";
    }
}

int main(){
    
    printTableHead(); 


}