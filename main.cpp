#include "funcs.h"
#include "dialogue.h"





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

outputDialogue();
}


void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize])
{
    fillARY<dungeonRowSize,dungeonColSize>(dungeonlayout);
   
}



