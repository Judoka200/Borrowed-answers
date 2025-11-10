#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#include <windows.h> //for sleep() function, used to for delay between text outputs
#endif

#ifdef linux
#include <unistd.h> //for usleep() function, used to for delay between text outputs
#endif

template <int X, int Y>
bool entered(int *array[Y][X]){


}

template <int ROWS, int COLS>
void outputARY(int *array[ROWS][COLS])
{
     for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            std::cout << std::setw(3) << array[row][col] << " ";
        }
        std::cout << std::endl;
    }

    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            std::cout << std::setw(3) <<"["<<row<<"," <<col<<"]";
        }
        std::cout << std::endl;
    }
}

/*
    fill array with increasing values from 0 
*/
template <int ROWS, int COLS>
std::string fillARY(std::string array[ROWS][COLS],int reset = 0)
//if reset is set to anything other than 0, Set all values in array to 0 
//reset = 0 therfore argument is optional and will not reset the function by default 
{
    //fill with 0 
    if(reset){
        for(int row = 0; row < ROWS; row++){
            for(int col = 0; col < COLS; col++){
                array[row][col] = "WALL";
            }
        }
    }else{
    
    int value = 0; //or start with one for indexing starting at
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            array[row][col] = static_cast<int>(value);
            value++;
            }
        }
    }
}



void timeDelay(double duration_inSeconds){
/* 
    Windows and Linux/Unix have differing ways of a sleep function
*/

    #ifdef _WIN32
        Sleep(duration_inSeconds*1000); //Sleep() takes an intager in milli-seconds so we muultiply by 1,000
    #endif

    #ifdef linux
        usleep(duration_inSeconds*1000000); //usleep() takes an intager in micro-seconds so we muultiply by 1,000,000
    #endif
}

void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear";)
    #endif
}
