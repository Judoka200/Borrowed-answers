#include "funcs.h"
#include "dialogue.h"

using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;

//initialise size of dungeon (set the width and height)
const int dungeonRowSize =6;
const int dungeonColSize = 4;
int dungeonlayout[dungeonRowSize][dungeonColSize]; //initialise the array for the board
/*
[0][0] | [0][1] | [0][2]| [0][3]                   | [0][1] |        |       
-------|--------|-------|-------            -------|--------|--------|-------
[1][0] | [1][1] | [1][2]| [1][3]            [1][0] | [1][1] | [1][2] | [1][2] 
-------|--------|-------|-------            -------|--------|--------|-------
[2][0] | [2][1] | [2][2]| [2][3]                   | [2][1] |        |       
-------|--------|-------|-------            -------|--------|--------|-------
[3][0] | [3][1] | [3][2]| [3][3]                   | [3][1] | [3][2] | [3][2]
-------|--------|-------|-------            -------|--------|--------|-------
[4][0] | [4][1] | [4][2]| [4][3]                   |        | [4][2] | [4][2]
-------|--------|-------|-------            -------|--------|--------|-------
[4][0] | [4][1] | [4][2]| [4][3]                   |        |        |       

*/


//Function Calls
void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize]);

int main(){


cout<<col(); //reset all colour formatting
    int stepsRemaining = 10;  // setup 



// dungeonSetup(dungeonlayout);
// outputARY<dungeonRowSize, dungeonColSize> (dungeonlayout);
// auto currentpos = dungeonlayout[1][0];

typeWrite("intro[1]");
cout<<"\033[3 q";
typeWrite("intro[1]",red,0.25);

}





void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize])
{
    fillARY<dungeonRowSize,dungeonColSize>(dungeonlayout,true);
   
}



