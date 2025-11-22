            #pragma region SETUP&MORE
#include "funcs.h"
#include "dialogue.h"
#include "items.h"
#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
using enum colours; //used for col() to colour text | prevents having to use colours:: every time in main.cpp 
using namespace std;
// #define dev true             //FOR DEBUG/DEV UNCOMMENT
#pragma region      constants and setup values
const int mapWidth = 4;  // X
const int mapHeight =3;  // Y
int stepsRemaining = 10;
int playerX = 0, playerY = 1; //    player starting position
int nextX = -1, nextY = -1;
#pragma endregion

// increasing:          X: →               Y: ↓    
string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL", "Cells", "WALL", "WALL"},                    // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room", "Hallway", "Sentry", "Exit"},        // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL", "Tome_hall_Entrance", "Tome_hall", "WALL"}    // [2][0] | [2][1] | [2][2] | [2][3]
};
enum commandType {
    QUIT,    OBSERVE,    MAP,    INVENTORY,    PICKUP,
    UNLOCK,    MOVE,    USE,     HELP,    INVALID 
};

//initialise size of dungeon (set the width and height)
//access the layout by dungeonlayout[ Y POS][X POS]
bool visible[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};

#pragma region BOOLS
/*    ============================TUTORIAL================================== */
   bool tutorialComplete = false;
   bool tutorialViewedInv = false;
   bool tutorialTakenMatch = false;
   bool tutorialUsedMatch = false;
   bool tutorialObserved = false;
   //  ====================================================================== 
   bool canViewInvisible = false;
   bool showInventory = false;
#pragma endregion
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

    if(toRoom == "Cells" && !hasItem("dagger")){
        return true;
    }

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
void revealAdjcent(){   //sets all adjacent rooms to visible 
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
            cout << "theres no pathway in that direction";      
        }else if(checkLocked(playerX,playerY,newX,newY)){//invlaid becuase locked path
            cout<<"This pathway is blocked by a \033[43mlocked\033[0m gate\n";       //43: yellow background

        }else if(dungeonlayout[newY][newX] == "Cells" && !hasItem("dagger")){
            cout << col(red) << "when taking a step towards the doorway, you feel an overwhelming amouunt of dread\n its better to not go unarmed into this place"; 
            
        }else
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
    cout << col(blue) << "+========================================================+\n" << col();
    cout << "ROOM name: " << dungeonlayout[playerY][playerX] << endl;
    cout << "MAP Position X: " << playerX << "\tY: " << playerY;
    cout << "\t| Steps Remaining: "<< stepsRemaining;
    cout << col(blue) << "\n+========================================================+\n" << col();
}
void displayMap()
{
    cout << col(cyan) << "+========================== MAP ==========================+\n" << col();
    #ifndef dev
            for(int Y = 0; Y < mapHeight; Y++){
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
                            if(dungeonlayout[Y][X].length() >4)
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
                    if(X==0){cout<<"\t\t"<<setw(4);}
                    if(X == playerX && Y == playerY){
                        cout<< col(yellow,cyan)<<"[YOU] "<<col();
                    }else if(dungeonlayout[Y][X] == "WALL"){
                        cout << col(red,cyan)<<"[###] " <<col();
                    }else{
                        // cout <<col(red,cyan) <<"{" << X << "," << Y << "} "<<col();
                        string roomName;
                        if(dungeonlayout[Y][X].length() >3)
                        { roomName = dungeonlayout[Y][X].substr(0,3);}
                        cout <<col(red,cyan) <<"{" << roomName << "} "<<col();
                    }
            }
            cout<<endl;
        }
    #endif
    cout << col(cyan) << "\n+========================================================+\n" << col();
}
#pragma endregion
#pragma endregion

/*    --------------------------------COMMAND--------------------------------    */
commandType processCommand(const string input,string& argumentsOut) {  //return the commandtype of the command 
    
    #ifdef dev
        cout<<col(black,cyan)<< "DEV: input is:"<<input<<col()<<endl;
    #endif
    
    int spacePosition = input.find(' ');   //find the pos of the space which should seperate the command and args
    string cmd;
    if (spacePosition != string::npos){
        cmd = input.substr(0,spacePosition);  // gives cmd the command half
    }else {
        cmd = input;}
    if (spacePosition != string::npos){
        argumentsOut = input.substr(spacePosition + 1);
        // cout<<"pot args:" << argumentsOut<<endl;
    } else{
        argumentsOut = "";}
   
    if(tutorialComplete){        
        if(cmd == "quit" || cmd == "exit") return {commandType::QUIT};
        if(cmd == "observe" || cmd == "look") return {commandType::OBSERVE};
        if(cmd == "map") return {commandType::MAP};
        if(cmd == "inventory" || cmd == "inv") return {commandType::INVENTORY};
        if(cmd == "pickup" || cmd == "take") return {commandType::PICKUP};
        if(cmd == "help") return {commandType::HELP};
        if(cmd == "use") {
            // argumentsOut = input;
            return{commandType::USE};}
        if(cmd == "unlock") {
            // argumentsOut = input;
            return {commandType::UNLOCK};};
        if(moveDirection(input)) {
            argumentsOut = input;
            return {commandType::MOVE};}
    } 
    
    else{
        if(cmd == "quit" || cmd == "exit") {
            return {commandType::QUIT};};
        if(cmd == "inventory" || cmd == "inv") {
            tutorialViewedInv = true;
            return {commandType::INVENTORY};};
        if((cmd == "pickup" || cmd == "take" ) && lowercase(argumentsOut) =="match") {           
            tutorialTakenMatch = true;
            return {commandType::PICKUP};}
        if(cmd == "observe" || cmd == "look"){
            tutorialObserved = true;
            return {commandType::OBSERVE};}
        if(cmd == "use" && lowercase(argumentsOut) == "match") {
            return{commandType::USE};}
    }

    return {commandType::INVALID};

}
void executeCommand(commandType type,string arguments) {
    #ifdef dev
    cout << col(black,cyan)<<"argumetns are: "<<arguments<<col()<<endl;
    #endif
    switch(type) {
        case commandType::QUIT:
            break;
            
        case commandType::OBSERVE:
            typeWrite(dungeonlayout[playerY][playerX] + "_desc");
            break;
            
        case commandType::MAP:
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
            
        case commandType::UNLOCK:
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
                if(lowercase(arguments) == "match" && hasItem("match"))
                {tutorialUsedMatch = true;}
                
                if(useItem(arguments,  playerX, playerY)) {
                    cout << "You used the " << arguments << endl;
                }
            } else {
                cout << "Use what? Specify an item name." << endl;
            }
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
    
/*
    typeWrite("Campfire_room_opening",green);
    typeWrite("#YOU NEED TO GET OUT#",red);
    timeDelay(2.0);
*/
    system("cls");
   #pragma region           LOOP
    while(true)
    {  
#pragma region          /*    -----------------------------------DISPLAY UI---------------------------------   */
        displayPlrPos();
        displayMap();
                if(!showInventory){
                listItems(playerX,playerY,canViewInvisible);
                }else{viewInventory();}
                showInventory = false;
        cout<<"canViewInvis: "<<canViewInvisible;
        cout<<endl;
#pragma endregion 
        
#pragma region          /*    --------------------------------ROOM DESCRIPTION------------------------------   */
        if(!observed[playerY][playerX]) //if room hasnt been enterd, provide the room's description 
        {
            cout<< col(green)<<"You observe the room your in: \n"<< col();     //1: bold , 22:  revert underline 
                if(dungeonlayout[playerY][playerX] == "Cells") {
                entityInteraction();
                canViewInvisible = true;
            } else {
                typeWrite(dungeonlayout[playerY][playerX] + "_desc");
            }
            observed[playerY][playerX] = true;
            cout<<endl;
        }
        
#pragma endregion 

#pragma region          /*    ----------------------------------DIPLAY MOVES--------------------------------   */
        cout<<col(green)<<"possible moves: "<<col();
        vector<string> moves = getMoves();
        for (int i = 0; i < moves.size(); i++){
            cout << moves[i];
            if (i < moves.size() - 1)
                cout << ", "; // seperates each move by , unless there are no more moves
        }
        cout << endl;
#pragma endregion

                        /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
            
        clearScreen();               
        }
#pragma endregion 
}

void TUTORIAL_LOOP(){
    string command;
    
    
    while(!tutorialComplete)
    {  
                        /*    -----------------------------------DISPLAY UI---------------------------------    */  
        showCommands();
#pragma region          /*    -------------------------------TUTORIAL COMPLETION----------------------------    */
        cout<<col(cyan)<< "++++++++++++++++TUTORIAL COMPLETION++++++++++++++++ \n"<<col();
//                                                      2: dimmed, 4: underline, 9: strikethrough
        cout<< ((tutorialObserved)?    "\033[2;9m - OBSERVE the room your in with:       \033[0m[#]\n":" - \033[4mOBSERVE\033[0m the room your in with:       [ ]\n") ;    
        cout << ((tutorialTakenMatch)? "\033[2;9m - PICKUP and Item within the room with:\033[0m[#]\n":" - \033[4mPICKUP\033[0m and Item within the room with:[ ]\n");
        cout << ((tutorialViewedInv)?  "\033[2;9m - VIEW your inventory to see the item: \033[0m[#]\n":" - \033[4mVIEW\033[0m your inventory to see the item: [ ]\n");
        cout << ((tutorialUsedMatch)?  "\033[2;9m - USE the item:                        \033[0m[#]\n":" - \033[4mUSE\033[0m the item:                        [ ]\n") ;
        cout<< col(cyan)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n" <<col();

        cout<<endl;
        #pragma endregion
        
        listItems(playerX,playerY);
        cout << endl;
//                      /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
        
         if (tutorialObserved&&tutorialTakenMatch&&tutorialUsedMatch&&tutorialViewedInv){
             system("cls");
             cout<< col(black,yellow)<<"\n\n\nYou passed the Tutorial\nnow to face reality"<<col();
             timeDelay(1.5);
             tutorialComplete = true;
            }       
        clearScreen();               
    }    
}


int main(){
    tutorialComplete = true;
    /*
    typeWrite("notice",green);  typeWrite("warning",red);
    cout<< "\n\033[38;5;245mPress Enter to continue...";             // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    */
   generateItems();
   clearScreen()  ;
    cout<<col(); //reset all colour formatting
    int stepsRemaining = 10;  // setup 

    if (!tutorialComplete) {TUTORIAL_LOOP();}
    unlockDoor(tutorialDoor);  //not placed in TUTORIAL_LOOP, so will be unlocked even if TUT skipped

    system("cls");

    
    GAME_LOOP();


    cout<< "\033[38;5;245mPress Enter to continue...";                   // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}



