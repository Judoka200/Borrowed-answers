#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>




template <int ROWS, int COLS>
void outputARY(int array[ROWS][COLS])
{
     for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            std::cout << std::setw(3) << array[row][col] << " ";
        }
        std::cout << std::endl;
    }


}
/* ^
template<int ROWS, int COLS>
void outputARY(int array[ROWS][COLS])
{
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            std::cout << std::setw(3) << array[row][col] << " ";
        }
        std::cout << std::endl;
    }
}*/


template <int ROWS, int COLS>
void fillARY(int array[ROWS][COLS],int reset = 0)
//if reset is set to anything other than 0, Set all values in array to 0 
//reset = 0 therfore argument is optional and will not reset the function by default 
{
    if(reset){
        for(int row = 0; row < ROWS; row++){
            for(int col = 0; col < COLS; col++){
                array[row][col] = 0;
            }
        }
    }else{
     int value = 0;  // or start with 1 if you prefer
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            array[row][col] = value;
            value++;
          }
        }
    }
}



