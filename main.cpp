#include "funcs.h"
#include "dialogue.h"
#include "items.h"
#include "vars.h"

#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
#include <chrono>


using enum colours; //used for col() to colour text | prevents having to use colours:: every time in main.cpp 
using namespace std;

#ifndef VARS_H
 

const int mapWidth = 4;  // X
const int mapHeight = 3;  // Y
int stepsRemaining = 10;
int playerX = 0, playerY = 1; //    player starting position
int nextX = -1, nextY = -1;
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


#endif


enum commandType {
    QUIT,    OBSERVE,    _MAP_,    INVENTORY,    PICKUP,
    _UNLOCK_,    MOVE,    USE,     HELP,    INVALID, TALK,
    _DEV_
};


#pragma region functions 
bool isBlocked(int fromX,int fromY, int ToX, int ToY){
    //checks for doors and virtual 'Walls between rooms'
    string fromRoom = dungeonlayout[fromY][fromX];
    string toRoom = dungeonlayout[ToY][ToX];

    if(checkLocked(fromX, fromY, ToX, ToY)){    //returns true if path is locked 
        return true;
    }
    // add virtual wall between sentry room and Tome_hall
    if((fromRoom =="Sentry" && toRoom =="Tome_hall")||(fromRoom =="Tome_hall" && toRoom =="Sentry") )
    {return true;}

    #ifndef dev
    if(toRoom == "Cells" && !hasItem("dagger")){
        return true;
    }
    #endif

    else {return false;}
}
bool isValid(int X, int Y)
{
    if (X < 0 || X >= mapWidth || Y < 0 || Y >= mapHeight) //checks if current position is within a 1-room margin (if there's a room in that)
    {
        return false;
    }
    else if(isBlocked(playerX,playerY,X,Y))
    {return false;}
    
    return dungeonlayout[Y][X] != "WALL";   //returns true if the room isnt a wall
    


}
void revealAdjcent(){   //sets all adjacent rooms to visible using bool array
    if (playerY > 0) visible[playerY - 1][playerX] = true;
    if (playerY < mapHeight ) visible[playerY + 1][playerX] = true;
    if (playerX > 0) visible[playerY][playerX - 1] = true;
    if (playerX < mapWidth ) visible[playerY][playerX + 1] = true;

}
vector<string> getMoves() //using vector becuase size of list will change depending on postition 

{
    vector<string> moves; //just a tempory vector to store the possible moves in
    
    //if valid room to 'direction' of player then add the corresponding string to the vector
    if (playerY > 0 && isValid(playerX, playerY - 1))   
    { moves.push_back("Up (w)"); }

    if (playerX > 0 && isValid(playerX - 1, playerY))   
    { moves.push_back("Left (a)"); }
    
    if (playerY < mapHeight - 1 && isValid(playerX, playerY + 1))   
    { moves.push_back("Down (s)"); }
    
    if (playerX < mapWidth - 1 && isValid(playerX + 1, playerY))    
    { moves.push_back("Right (d)"); }
    
    return moves;   //returns the possible moves which is a vector of strings 
}
bool move(string& direction)
{/*    reference to string 'direction' used to change the input to the direction when printed   */
    int newX = playerX, newY = playerY;
    

/*
                   increasing:         X: →               Y: ↓    
*/
    if (direction =="w" || direction =="W" || direction =="up" || direction =="Up")
        {newY -= 1;
        direction = "up";}
    if (direction =="a" || direction =="A" || direction =="left" || direction =="Left")
        {newX -= 1;
        direction = "left";}
    if (direction =="s" || direction =="S" || direction =="down" || direction =="Down")
        {newY += 1;
        direction = "down";}
    if (direction =="d" || direction =="D" || direction =="right" || direction =="Right")
        {newX += 1;
        direction = "right";}

    if(!isValid(newX,newY))     //if the direction to next room is INVALID
    {
        if(newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)    //invalid becuase no room
        {
            cout << "theres no pathway in that direction\n";      
        }else if(checkLocked(playerX,playerY,newX,newY)){//invlaid becuase locked path
            cout<<"This pathway is blocked by a \033[43mlocked\033[0m gate\n";       //43: yellow background
        }
        #ifndef dev
        else if(dungeonlayout[newY][newX] == "Cells" && !hasItem("dagger")){
            cout << col(red) << "when taking a step towards the doorway, you feel an overwhelming amount of dread\n its better to not go unarmed into this place\n"; 
            
        }
         #endif
        else
        {
            cout<<"theres no way to go this direction \n";  //any exceptions e.g. Blocked path between rooms
            }
        return false;   //move was unsuccesful
    }
//  set the current player position to the next position 
    playerX = newX;     
    playerY = newY;
    nextX = newX;
    nextY = newY;
    visible[playerY][playerX] = true; //current room to visible 
    revealAdjcent();        
    return true;        //move was successful
}
void displayPlrPos()
{
    cout << "\e[38;5;27m" << "+========================== ROOM =========================+\n" << col();
    cout << "ROOM name: " << dungeonlayout[playerY][playerX] << endl;
    cout << "_MAP_ Position X: " << playerX << "\tY: " << playerY;
    cout << "\t| Steps Remaining: "<< stepsRemaining;
    cout << "\e[38;5;27m" << "\n+=========================================================+\n" << col();
}
void displayMap()
{//         a lavender colour
    cout << "\e[38;5;62m" << "+========================== _MAP_ ==========================+\n" << col();
    #ifndef dev
            for(int Y = 0; Y < mapHeight; Y++){
                cout<<"\t \t";
                for(int X = 0; X < mapWidth; X++){
                    if(X == playerX && Y == playerY){
                            cout<< col(yellow)<<"[PLYR] "<<col();
                        }else if(!visible[Y][X]){
                            cout << "[~~~~] ";
                        }else if(dungeonlayout[Y][X] == "WALL"){
                            // cout << col(red)<<"[###] " <<col();
                            cout << col(red)<<"[####] " <<col();
                        }else{
                            string roomName;
                            if(dungeonlayout[Y][X].length() >=4)
                            { roomName = dungeonlayout[Y][X].substr(0,4);}
                        // cout <<col(red) <<"(" << X << "," << Y << ") "<<col();
                        cout <<col(red) <<"(" << roomName<< ") "<<col();
                        }
            }
            cout<<endl;
        }
    #endif
    
    #ifdef dev
            for(int Y = 0; Y < mapHeight; Y++){
                for(int X = 0; X < mapWidth; X++){
                    if(X==0){cout<<"\t\t";}
                    if(X == playerX && Y == playerY){
                        cout<< col(yellow)<<"[PLYR] "<<col();
                    }else if(dungeonlayout[Y][X] == "WALL"){
                        cout << col(cyan)<<"(####) " <<col();
                    }else{
                        // cout <<col(red,cyan) <<"{" << X << "," << Y << "} "<<col();
                        string roomName;
                        if(dungeonlayout[Y][X].length() >=4)
                        { roomName = dungeonlayout[Y][X].substr(0,4);}
                        cout <<col(cyan) <<"(" << roomName << ") "<<col();
                    }
            }
            cout<<endl;
        }
    #endif
    cout << "\e[38;5;62m" << "\n+=========================================================+\n" << col();
}
#pragma endregion

/*    --------------------------------COMMAND--------------------------------    */
commandType processCommand(const string input,string& argumentsOut) {  //return the commandype of the command 
    
    #ifdef dev
        cout<<col(black,cyan)<< "DEV: input is:"<<input<<col()<<endl;
    #endif
    
    int spacePosition = input.find(' ');   //find the pos of the space which should seperate the command and args
    string cmd;
    //get the command keyword half
    if (spacePosition != string::npos){
        cmd = input.substr(0,spacePosition);  // gives cmd the command half
    } else {
        cmd = input;}
        //get the argument half
    if (spacePosition != string::npos){
        argumentsOut = input.substr(spacePosition + 1);
    } else {
        argumentsOut = "";
    }
   
    if(tutorialComplete){        
        if(cmd == "quit" || cmd == "exit"){
            return {commandType::QUIT};
    }
        if(cmd == "observe" || cmd == "look" || cmd == "o" ){
            return {commandType::OBSERVE};
        }
        if(cmd == "inventory" || cmd == "inv" || cmd == "i"){
            return {commandType::INVENTORY};
        }
        if(cmd == "pickup" || cmd == "take"){
            return {commandType::PICKUP};
        }
        if(cmd == "help" || cmd == "h") {
            return {commandType::HELP};
        }
        if(cmd == "use") {
            return{commandType::USE};
        }
        if(cmd == "talk" && currentRoom("Sentry")){
            return{commandType::TALK};
        }
        if(moveDirection(input)){
            argumentsOut = input;
            return {commandType::MOVE};
        }
#ifdef dev
            if(cmd == "unlock") return {commandType::_UNLOCK_};                    
            if(cmd == "map") return {commandType::_MAP_};
#endif
    } 
    
    else{
        if(cmd == "quit" || cmd == "exit") {
            return {commandType::QUIT};
        }
        if(cmd == "inventory" || cmd == "inv" || cmd == "i") {
            tutorialViewedInv = true;
            return {commandType::INVENTORY};
        }
        if(cmd == "pickup" || cmd == "take" ) {      
            if( lowercase(argumentsOut) =="match"){     
                tutorialTakenMatch = true;}
            return {commandType::PICKUP};
        }
        if(cmd == "observe" || cmd == "look" || cmd == "o"){
            tutorialObserved = true;
            return {commandType::OBSERVE};
        }
        if(cmd == "use" && lowercase(argumentsOut) == "match") {
            return{commandType::USE};
        }
#ifdef dev
            if(cmd == "unlock") return {commandType::_UNLOCK_};                    
            if(cmd == "map") return {commandType::_MAP_};
            if(moveDirection(input)){
                argumentsOut = input;
                return {commandType::MOVE};
            }
#endif
    }

    return {commandType::INVALID};

}
void executeCommand(commandType type,string arguments) {
    #ifdef dev
    cout << col(black,cyan)<<"argumetns are: "<<arguments<<col()<<endl;
    #endif
    lowercase(arguments,true);
    switch(type) {
        case commandType::QUIT:
            break;

        case commandType::OBSERVE:
            typeWrite(dungeonlayout[playerY][playerX] + "_desc",colours::Default,descDelay);
            break;
            
        case commandType::_MAP_:      // dev mode only
            displayMap();
            break;
            
        case commandType::INVENTORY:
            // viewInventory();
            showInventory = true;
            break;
            
        case commandType::PICKUP:
            if(!arguments.empty()) {
           
                pickupItem(arguments, playerX, playerY);
            }
            break;
        case commandType::_UNLOCK_:    // dev mode only
            unlockDoor(hallwayDoor,"");
            unlockDoor(exitDoor,"");
            unlockDoor(tutorialDoor,"");
            break;
          
        case commandType::MOVE:
            if(move(arguments)){cout<< "you moved: "<<arguments<<endl;}
            else if(checkLocked(playerX,playerY,nextX,nextY)){cout<<"something went wrong and you didnt go anywhere\n";}
            break;
            
            case commandType::USE:
            if(!arguments.empty()) {                
                if(arguments == "match" && hasItem("match"))  // tutorial check
                    {tutorialUsedMatch = true;}
                
                if(useItem(arguments)) {
                    if(arguments== "book"){
                        cout << "You read the " << arguments << endl;
                    }else{
                    cout << "You used the " << arguments << endl;
                    }
                }
            } else {
                cout << "Use what? Specify an item name." << endl;
            }
            break;


            case commandType::TALK:
            clearScreen();
                cout<<col(magenta)<<"You talk to the guard"<<endl;
                sentryInteraction();
                

            break;
            
            case commandType::HELP:
            showCommands();
            break;

            case commandType::INVALID:
                cout << "Invalid command!" << endl;
                break;
        }
        
        cout<< "\033[38;5;245mPress Enter to continue..."; // grey colour from table 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GAME_LOOP()
{
    string command;
    visible[playerY][playerX] = true;
    revealAdjcent();
    
    typeWrite("Campfire_room_opening",magenta,descDelay);
    /*
    typeWrite("#YOU NEED TO GET OUT#",red);
    timeDelay(2.0);
    */

   while(!GAME_LOOP_END)
   {  
       clearScreen();
       #pragma region          col(item.colour,item.backgroundColour) /*    -----------------------------------DISPLAY UI---------------------------------   */

       displayPlrPos();
       displayMap();

       // toggles the item list for the inventory list when viewing inventory
                if(!showInventory){                 
                    listItems(canViewInvisible);
                }else{viewInventory();}
                showInventory = false;

        cout<<endl;
#pragma endregion 
        
#pragma region                  /*    -----------------------------------ROOM CHECKS------------------------------   */
        if(!observed[playerY][playerX]) //if room hasnt been enterd, provide the room's description 
        {
            cout<< col(green)<<"You observe the room your in: \n"<< col();     //1: bold , 22:  revert underline 
                if(currentRoom("Cells")) {
                    entityInteraction();
                    canViewInvisible = true;
             
            #ifndef dev
                }else if (currentRoom("Sentry")){
                // gain TALK command
                seenSentry = true;
                typeWrite("You face a towering, stone, cloaked stautewith a raven on its shoulder\n when looking at its face, all you see is a flash of white\n ", Default, 0.01);
                cout << col(blue, white) << "And feel the ability to commune with what towers in front of you\n" << col();
                showCommands();
            #endif
            } else {
                typeWrite(dungeonlayout[playerY][playerX] + "_desc",Default, descDelay);
            }
            observed[playerY][playerX] = true;  // sets the current room to have been observed on entry
            cout<<endl;
        }
        
#pragma endregion 

#pragma region                  /*    ----------------------------------DIPLAY MOVES--------------------------------   */
        cout<<col(green)<<"possible moves: "<<col();
        vector<string> moves = getMoves();      //gets a vector of possible moves 
        for (int i = 0; i < moves.size(); i++){
            cout << moves[i];
            if (i < moves.size() - 1)
                cout << ", "; // seperates each direction by ', ' unless there are no more moves
        }
        cout << endl;
#pragma endregion

#pragma region                  /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command, true);

        string argument;  // temp variable that will store the argument, using pointer in processCommand()
        commandType type = processCommand(command, argument);
        cout << endl;
        executeCommand(type,argument);
            
        clearScreen();               
#pragma endregion
        }
}

void TUTORIAL_LOOP(){
    string command;
    
    
    while(!tutorialComplete)
    {  
                        /*    -----------------------------------DISPLAY UI---------------------------------    */  
        showCommands();
#pragma region          /*    -------------------------------TUTORIAL COMPLETION----------------------------    */
        cout<<col(cyan)<< "++++++++++++++++TUTORIAL COMPLETION++++++++++++++++ \n"<<col();
//                                                      2: dimmed, 4: underline, 9: strikethrough, 0: RESET
        cout<< ((tutorialObserved)?    "\033[2;9m - OBSERVE the room your in with:          \033[0m[#]\n":" - \033[4mOBSERVE\033[0m the room your in with:          [ ]\n") ;    
        cout << ((tutorialTakenMatch)? "\033[2;9m - PICKUP and Item within the room with:   \033[0m[#]\n":" - \033[4mPICKUP\033[0m and Item within the room with:   [ ]\n");
        cout << ((tutorialViewedInv)?  "\033[2;9m - VIEW at your inventory to see the item: \033[0m[#]\n":" - \033[4mVIEW\033[0m at your inventory to see the item: [ ]\n");
        cout << ((tutorialUsedMatch)?  "\033[2;9m - USE the item:                           \033[0m[#]\n":" - \033[4mUSE\033[0m the item:                           [ ]\n") ;
        cout<< col(cyan)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n" <<col();

        cout<<endl;
        #pragma endregion
        
        if(!showInventory){
            listItems();
        } else {
            viewInventory();
        }
        showInventory = false;
        cout << endl;
//                      /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command,true);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
        
         if (tutorialObserved&&tutorialTakenMatch&&tutorialUsedMatch&&tutorialViewedInv){
             clearScreen();
             cout<< col(black,yellow)<<"\n\n\nYou passed the Tutorial\nnow to face reality\n"<<col();
             typeWrite("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ",colours::green, 0.01);
             tutorialComplete = true;
            }       
        clearScreen();               
    }    
}

int main(){
    typeWrite("notice",green);  typeWrite("warning",red);
    cout<< "\n\033[38;5;245mPress Enter to continue...";             // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    generateItems();
    clearScreen()  ;
    
    cout<<col(); //reset all colour formatting
#ifndef dev
    tutorialComplete = false;
    if (!tutorialComplete) {TUTORIAL_LOOP();}
#endif
tutorialComplete = true;
    unlockDoor(tutorialDoor);  //not placed in TUTORIAL_LOOP, so will be unlocked even if TUTORIAL skipped
    clearScreen();

    auto startTime = chrono::high_resolution_clock::now(); //stores the start time to calculate time taken 

    GAME_LOOP();
    auto stopTime = chrono::high_resolution_clock::now(); //stores the start time to calculate time taken 
    
    auto time = duration_cast<std::chrono::seconds>(stopTime - startTime);
    std::cout << "your final time was: " << format_duration(time);
    
    cout<< "\033[38;5;245mPress Enter to continue...";                   // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
}



