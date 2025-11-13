#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>  // used for tolower() 

#ifdef _WIN32
#include <windows.h> //for sleep() function, used to for delay between text outputs
#endif

#ifdef linux
#include <unistd.h> //for usleep() function, used to for delay between text outputs
#endif

// returns true/false if a valid direction input
bool moveDirection(std::string str){
    if (str == "w"||str == "W"||str == "up"||str == "Up"
      ||str == "a"||str == "A"||str == "left"||str == "Left"
      ||str == "s"||str == "S"||str == "down"||str == "Down"
      ||str == "d"||str == "D"||str == "right"||str == "Right")
      return true;  
      else return false;
    }




/** reference and return versions
 * adding a bool value to the function gives reference which will change the actual string
 * no bool will make a copy
*/
void lowerCase(std::string &str, bool){
for(int i=0; i <str.length();i++)
    {str[i]= tolower(str[i]);}
}

/** reference and return versions
 * adding a bool value to the function gives reference which will change the actual string
 * no bool will make a copy
*/
std::string lowerCase(std::string str){
    for(int i=0; i <str.length();i++)
    {str[i]= tolower(str[i]);}
    // std::cout<< "\n str is:" << str << std::endl<<std::endl;             DEBUG
    return str;
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
