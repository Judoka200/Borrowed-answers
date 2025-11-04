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


