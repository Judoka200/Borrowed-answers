#ifndef VARS_H   
/*
    if VARS_H is undefined, define it, setting all the variables.
    will not define the variables if it has already been defined from whithn another file
    preventing redeclaration errors
*/
#define VARS_H

#include <string>  //just for the dungeon layout array

// #define dev

//#######################################################################################################################################################################################################

//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ main.cpp  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool GAME_LOOP_WON = false;
bool GAME_LOOP_END = false;


const int mapWidth = 4;                                 // X
const int mapHeight = 3;                                // Y
//access the layout by dungeonlayout[ Y POS][X POS]
// increasing:          X: →               Y: ↓    
std::string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL", "Cells", "WALL", "WALL"},                      // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room", "Hallway", "Sentry", "Exit"},         // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL", "Tome_hall_Entrance", "Tome_hall", "WALL"}     // [2][0] | [2][1] | [2][2] | [2][3]
};

//                  constants and setup values
int playerX = 0, playerY = 1;                           // player starting position
int nextX = -1, nextY = -1;
                  

int guessesRemaining = 3;

/*    ============================TUTORIAL================================== */
bool tutorialComplete = false;
bool tutorialViewedInv = false;
bool tutorialTakenMatch = false;
bool tutorialUsedMatch = false;
bool tutorialObserved = false;
//  ====================================================================== 

bool canViewInvisible = false;
bool showInventory = false;

bool talkedToSentry = false;
bool seenSentry = false;
bool refusedEntity = true ;

bool visible[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};


std::string sentryHardAns = "serpent";                  // if denied entity | hard 
std::string sentryEasyAns = "raven";                    // if accepted entity | easy 



//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  items.h  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 bool usedMatch = false;
//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  dialouge.h  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef dev
#define textNorm
#endif

#ifdef dev
#define textFast
#endif


#ifdef textNorm
#define descDelay 0.0025   
#define typeW_delay 0.015  
#define txtDelay 0.45       
#endif

#ifdef textFast
#define descDelay 0.00025    //default 0.0025
#define typeW_delay 0.0005  // 0.015
#define txtDelay 0.15       // 0.065
#endif

//###################################################################################################################################################################################################
#endif