#include "funcs.h"

//initialise size of dungeon (set the width and height)
const int dungeonRowSize = 5;
const int dungeonColSize = 10;
  int dungeonlayout[dungeonRowSize][dungeonColSize];

using namespace std;

void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize]);

int main(){
    int stepsRemaining = -1;  // setup  

dungeonSetup(dungeonlayout);
outputARY<dungeonRowSize, dungeonColSize> (dungeonlayout);}


void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize])
{
    int value = 0;  // or start with 1 if you prefer
    for(int row = 0; row < dungeonRowSize; row++){
        for(int col = 0; col < dungeonColSize; col++){
            dungeonlayout[row][col] = value;
            value++;
        }
    }
}



