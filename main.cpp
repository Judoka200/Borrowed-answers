#include "funcs.h"

//initialise size of dungeon
const int dungeonRowSize = 5;
const int dungeonColSize = 5;
  int dungeonlayout[dungeonRowSize][dungeonColSize];

using namespace std;

void dungeonSetup(int dungeonlayout[dungeonRowSize][dungeonColSize]);
// void outputARY(int array[dungeonRowSize][dungeonColSize]);

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


// void outputARY(int array[dungeonRowSize][dungeonColSize])
// {
//      for(int row = 0; row < dungeonRowSize; row++){
//         for(int col = 0; col < dungeonColSize; col++){
//             cout << setw(3) << dungeonlayout[row][col] << " ";
//         }
//         cout << endl;
//     }


// }

