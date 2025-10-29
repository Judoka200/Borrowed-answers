#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

char c = 186;
int main(){
cout << "\033[91;42;5mThis is red text \033[0m"<<c;
cout << "\033[38;5;74mherl";
cout<<"hello";
cout << "\033[2m low intensity \033[0m";
cout << "\033[1m test text\033[0m";
cin.ignore(1);
}