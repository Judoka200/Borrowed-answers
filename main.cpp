#include "funcs.h"
#include "dialogue.h"
#include "items.h"
// #include "doors.h"
#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;
// #define dev true             //FOR DEBUG/DEV UNCOMMENT
const int mapWidth = 4;  // X
const int mapHeight =3;  // Y

// increasing:          X: →               Y: ↓    

string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL","Prison","WALL","WALL"},                    // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room","Hallway","Sentry","Exit"},        // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL","Library_Entrance","Library","WALL"}        // [2][0] | [2][1] | [2][2] | [2][3]
};

enum commandType {
    QUIT,    OBSERVE,    MAP,    INVENTORY,    PICKUP,    UNLOCK,    MOVE,    USE,     HELP,    INVALID
};

//initialise size of dungeon (set the width and height)
//access the layout by dungeonlayout[ Y POS][X POS]
bool visible[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};

int playerX = 0;
int playerY = 1;
/*
    ============================TUTORIAL================================== 
*/
#pragma region 
   bool tutorialComplete = false;
   bool tutorialViewedInv = false;
   bool tutorialTakenMatch = false;
   bool tutorialUsedMatch = false;
   bool tutorialObserved = false;
#pragma endregion
//  ====================================================================== 



bool isBlocked(int fromX,int fromY, int ToX, int ToY){
    string fromRoom = dungeonlayout[fromY][fromX];
    string toRoom = dungeonlayout[ToY][ToX];

    if(checkLocked(fromX, fromY, ToX, ToY)){    //returns true if path is locked 
        return true;
    }

    // remove room transition from Sentry room to Library 
    if((fromRoom =="Sentry" && toRoom =="Library")||(fromRoom =="Library" && toRoom =="Sentry") )
    {return true;}

    else {return false;}
}

bool isValid(int X, int Y)
{
    if (X < 0 || X >= mapWidth || Y < 0 || Y >= mapHeight) //checks if current position is within a 1-room margin (if there's a room in that)
    {
        return false;
    }
    if(isBlocked(playerX,playerY,X,Y))
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
{
    int newX = playerX;
    int newY = playerY;
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

    if(!isValid(newX,newY))     //if the direction isnt valid 
    {
        if(newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)
        {
            cout << "theres no pathway in that direction";      //invalid becuase no room
        }else if(checkLocked(playerX,playerY,newX,newY)){
        cout<<"This pathway is blocked by a \033[43mlocked\033[0m gate\n";      //invlaid becuase locked path


        }else{
            cout<<"theres no way to go this direction \n";  //any exceptions e.g. Blocked path between rooms
            }
        return false;   //move was unsuccesful
    }
//  set the current player position to the next position 
    playerX = newX;     
    playerY = newY;
    visible[playerY][playerX] = true; //current room to visible 
    revealAdjcent();        
    return true;        //move was successful
}
void displayPlrPos()
{
    cout << col(blue) << "\n+========================================================+\n" << col();
    cout << "ROOM name: " << dungeonlayout[playerY][playerX] << endl;
    cout << "MAP Position X: " << playerX << "\tY: " << playerY ;
    cout << col(blue) << "\n+========================================================+\n" << col();
}
void displayMap()
{
    cout << col(cyan) << "\n+========================== MAP ==========================+\n" << col();
    #ifndef dev
            for(int Y = 0; Y < mapHeight; Y++){
                for(int X = 0; X < mapWidth; X++){
                    if(X == playerX && Y == playerY){
                            cout<< col(yellow)<<"[YOU] "<<col();
                        }else if(!visible[Y][X]){
                            cout << "[~~~] ";
                        }else if(dungeonlayout[Y][X] == "WALL"){
                            // cout << col(red)<<"[###] " <<col();
                            cout << col(red)<<"[###] " <<col();
                        }else{
                        cout <<col(red) <<"(" << X << "," << Y << ") "<<col();
                        }
            }
            cout<<endl;
        }
    #endif
    
    #ifdef dev
            for(int Y = 0; Y < mapHeight; Y++){
                for(int X = 0; X < mapWidth; X++){
                    if(X == playerX && Y == playerY){
                        cout<< col(yellow,cyan)<<"[YOU] "<<col();
                    }else if(dungeonlayout[Y][X] == "WALL"){
                        cout << col(red,cyan)<<"[###] " <<col();
                    }else{
                        cout <<col(red,cyan) <<"{" << X << "," << Y << "} "<<col();
                    }
            }
            cout<<endl;
        }
    #endif
    cout << col(cyan) << "\n+========================================================+\n" << col();
}


/*
    --------------------------------COMMAND--------------------------------
*/
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
        cout<<"pot args:" << argumentsOut<<endl;
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
    cout << col(black,cyan)<<"argumetns are: "<<arguments<<endl;
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
            viewInventory();
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
            if(move(arguments)){cout<< "you moved:"<<arguments<<endl;}
            else{cout<<"something went wrong and you didnt go anywhere\n";}
            break;
            
        case commandType::INVALID:
            cout << "Invalid command!" << endl;
            break;
        case commandType::USE:
            if(!arguments.empty()) {
                cout << col()<<"DEBUG:argumetns are: "<<arguments<<endl;
                lowercase(arguments,true);
                cout << col()<<"DEBUG:lower argumetns are: "<<arguments<<endl;
                
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
    }
        cout<< "\033[38;5;245mPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GAME_LOOP()
{
    string command;
    visible[playerY][playerX] = true;
    revealAdjcent();
    

    // typeWrite("Campfire_room_opening",green);
    typeWrite("WHY???",red);
    timeDelay(0.5);
    
    while(true)
    {  
/*
    --------------------------------DISPLAY UI--------------------------------        
*/
        displayPlrPos();
        displayMap();
        listItems(playerX,playerY);
        
/*
    --------------------------------ROOM DESCRIPTION--------------------------------
    ----------------------------------upon entering--------------------------------- 
*/        
        if(!observed[playerY][playerX]) //if room hasnt been enterd, provide the room's description 
        {
            cout<< "You observe the room your in\n";
                if(dungeonlayout[playerY][playerX] == "Prison") {
                // cursedNote();
            } else {
                typeWrite(dungeonlayout[playerY][playerX] + "_desc");
            }
            observed[playerY][playerX] = true;
            cout<<endl;
        }
/*
    --------------------------------DIPLAY MOVES--------------------------------        
*/
        vector<string> moves = getMoves();
        for (int i = 0; i < moves.size(); i++){
            cout << moves[i];
            if (i < moves.size() - 1)
                cout << ", "; // seperates each move by , unless there are no more moves
        }
        cout << endl;
            /*
            ---------------------------GET COMMAND--------------------------
            */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
            
        clearScreen();               
        }
}

void TUTORIAL_LOOP(){
    string command;
    
    
    while(!tutorialComplete)
    {  
/*
    --------------------------------DISPLAY UI--------------------------------        
*/  
        showCommands();

        cout<<col(cyan)<< "++++++++++++++++TUTORIAL COMPLETION++++++++++++++++ \n"<<col();
        cout<< ((tutorialObserved)?    "\033[2;9m - OBSERVE the room your in with:       \033[0m[#]\n":" - \033[4mOBSERVE\033[0m the room your in with:       [ ]\n") ;
        cout << ((tutorialTakenMatch)? "\033[2;9m - PICKUP and Item within the room with:\033[0m[#]\n":" - \033[4mPICKUP\033[0m and Item within the room with:[ ]\n");
        cout << ((tutorialViewedInv)?  "\033[2;9m - VIEW your inventory to see the item: \033[0m[#]\n":" - \033[4mVIEW\033[0m your inventory to see the item: [ ]\n");
        cout << ((tutorialUsedMatch)?  "\033[2;9m - USE the item:                        \033[0m[#]\n":" - \033[4mUSE\033[0m the item:                        [ ]\n") ;
        cout<< col(cyan)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n" <<col();

        cout<<endl;
        listItems(playerX,playerY);

        
        cout << endl;
            /*
            ---------------------------GET COMMAND--------------------------
            */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
            
        
        
        if (tutorialObserved&&tutorialTakenMatch&&tutorialUsedMatch&&tutorialViewedInv)
            {tutorialComplete = true;
            unlockDoor(tutorialDoor);
            }
        clearScreen();               
        }
    
    
    }



int main(){
    tutorialComplete = true;
    // typeWrite("notice",green);
    // typeWrite("warnign",red);
    generateItems();
    clearScreen()  ;
    cout<<col(); //reset all colour formatting
    int stepsRemaining = 10;  // setup ]

    if (!tutorialComplete){
    TUTORIAL_LOOP();
    }

    
    
    GAME_LOOP();
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}


