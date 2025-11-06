#include "funcs.h"
#include "dialogue.h"

using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;

//initialise size of dungeon (set the width and height)
const int dungeonRowSize =5;
const int dungeonColSize = 3;
int dungeonlayout[dungeonRowSize][dungeonColSize]; //initialise the array for the board


//Function Calls
void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize]);

int main(){

    int stepsRemaining = -1;  // setup 

//dungeonSetup(dungeonlayout);
//outputARY<dungeonRowSize, dungeonColSize> (dungeonlayout);


typeWrite("introduction");
cout << "\033[3 q";
cin.ignore(1);
typeWrite("intro");
cin.ignore(1);

typeWrite("introduction",red,0.1);
cout << "\033[3 q";
cin.ignore(1);
typeWrite("intro",green);
cin.ignore(1);




}




void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize])
{
    fillARY<dungeonRowSize,dungeonColSize>(dungeonlayout);
   
}



