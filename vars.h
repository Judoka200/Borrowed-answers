// # https://www.reddit.com/r/cpp_questions/comments/p6zlqn/why_cant_i_define_global_variables_in_h_files_if/


#ifndef VARS_H
#define VARS_H

#include <string>

//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  from main.cpp  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma region      constants and setup values
const int mapWidth = 4;  // X
const int mapHeight = 3;  // Y
int stepsRemaining = 10;
int playerX = 0, playerY = 1; //    player starting position
int nextX = -1, nextY = -1;
#pragma endregion


/*    ============================TUTORIAL================================== */
bool tutorialComplete = false;
bool tutorialViewedInv = false;
bool tutorialTakenMatch = false;
bool tutorialUsedMatch = false;
bool tutorialObserved = false;
//  ====================================================================== 
bool canViewInvisible = false;
bool showInventory = false;

bool isGood ;


// increasing:          X: →               Y: ↓    
std::string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL", "Cells", "WALL", "WALL"},                    // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room", "Hallway", "Sentry", "Exit"},        // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL", "Tome_hall_Entrance", "Tome_hall", "WALL"}    // [2][0] | [2][1] | [2][2] | [2][3]
};

bool visible[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};




//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  items.h  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 bool usedMatch = false;











//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  dialouge.h  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 #define normaltext
// #define quicktext

#ifdef normaltext
#define descDelay 0.0025   
#define typeW_delay 0.015  
#define txtDelay 0.45       
#endif

#ifdef quicktext
#define descDelay 0.00025    //default 0.0025
#define typeW_delay 0.0005  // 0.015
#define txtDelay 0.15       // 0.065
#endif


#endif