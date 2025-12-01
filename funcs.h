#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>  // used for tolower() 
#include "vars.h"
#include <chrono>
#ifdef _WIN32
#include <windows.h> //for sleep() function, used to for delay between text outputs
#endif

#ifdef linux
#include <unistd.h> //for usleep() function, used to for delay between text outputs
#endif

//declarations for functions that use them
bool currentRoom(std::string roomName);
std::string lowercase(std::string str);

std::string format_duration( std::chrono::seconds s ) {
    using namespace std::chrono;
    auto secs = duration_cast<seconds>(s);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);
    auto hour = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hour);

    std::stringstream ss;
    if(hour.count()){
    ss << hour.count() << " Hours : " << mins.count() << " Minutes : " << secs.count() << " Seconds : ";
    } else {
    ss << mins.count() << " Minutes : " << secs.count() << " Seconds : ";
    }
    return ss.str();
}


// returns true/false if a valid direction input
bool moveDirection(std::string str){
    if (str == "w"||str == "W"||str == "up"||str == "Up"
      ||str == "a"||str == "A"||str == "left"||str == "Left"
      ||str == "s"||str == "S"||str == "down"||str == "Down"
      ||str == "d"||str == "D"||str == "right"||str == "Right")
      return true;  
      else return false;
    }

void showCommands(){
    std::cout << "\033[32m" << "\n========== AVAILABLE COMMANDS ==========" << "\033[0m" << std::endl;
    std::cout << "  quit/exit            - Exit the game" << std::endl;
    std::cout << "  observe/look/o       - Examine your surroundings" << std::endl;
    std::cout << "  inventory/inv/i      - View your items" << std::endl;
    std::cout << "  pickup/take <item>   - Pick up an item" << std::endl;
    std::cout << "  w/a/s/d              - Move (or up/down/left/right)" << std::endl;
    std::cout << "  help/h               - Show this help message" << std::endl;
if(seenSentry){
    if(currentRoom("Sentry")){  std::cout <<"\033[94;107m"; }else{  std::cout <<"\033[2m";    }   // dims the text if not in Sentry room
    std::cout << "  TALK                 - Talk to the sentry guard" << "\033[0m" <<std::endl;
}
#ifdef dev
    std::cout << "\033[32m" << "\n==================DEV===================" << "\033[0m" << std::endl;
    std::cout << "  unlock               - Unlock all doors" << std::endl;
    std::cout << "  map                  - View the dungeon map" << std::endl;

#endif
    std::cout << "\033[32m" << "========================================\n" << "\033[0m" << std::endl;
   
}


/** reference and return versions
 * adding a bool value to the function gives reference which will change the actual string
 * no bool will make a copy
*/
void lowercase(std::string &str, bool){
for(int i=0; i <str.length();i++)
    {str[i]= tolower(str[i]);}
}

/** reference and return versions
 * adding a bool value to the function gives reference which will change the actual string
 * no bool will make a copy
*/
std::string lowercase(std::string str){
    for(int i=0; i <str.length();i++)
    {str[i]= tolower(str[i]);}
    return str;
}

bool currentRoom(std::string roomName){
    if (lowercase(roomName) == lowercase(dungeonlayout[playerY][playerX])){
        return true;
    }
return false;
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
        system("clear");
    #endif
}

