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

dungeonSetup(dungeonlayout);
outputARY<dungeonRowSize, dungeonColSize> (dungeonlayout);

// outputDialogue();
cout<< col(blue, blue)<<"black texrt red bakcgz"<<col()<<endl;

cout<< "\033[35;44m black texrt red bakcgz"<<col();
cout << "\033[35;44mmagenta on blue\033[0m\n";

cin.ignore(2);
}


void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize])
{
    fillARY<dungeonRowSize,dungeonColSize>(dungeonlayout);
   
}



