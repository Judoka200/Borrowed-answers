#include "funcs.h"
#include "dialogue.h"
#include "items.h"
#include "doors.h"
#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;

const int mapWidth = 4; //X
const int mapHeight =3;  //Y
string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL","Prison","WALL","WALL"},                    // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room","Hallway","Sentry","Exit"},        // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL","Library_Entrance","Library","WALL"}        // [2][0] | [2][1] | [2][2] | [2][3]
};

enum commandType {
    QUIT,    OBSERVE,    MAP,    INVENTORY,    PICKUP,    UNLOCK,    MOVE,    USE,    INVALID
};

//initialise size of dungeon (set the width and height)
//access the layout by dungeonlayout[ Y POS][X POS]
bool visited[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};

int playerX = 0;
int playerY = 1;
/*
    ============================TUTORIAL================================== 
*/
bool tutorialComplete = false;
bool tutorialViewedInv = false;
bool tutorialTakenMatch = false;
bool tutorialUsedMatch = false;
bool tutorialObserved = false;




bool isBlocked(int fromX,int fromY, int ToX, int ToY){
    string fromRoom = dungeonlayout[fromY][fromX];
    string toRoom = dungeonlayout[ToY][ToX];

    if(checkLocked(fromX, fromY, ToX, ToY)){
        return true ;
    }

    if((fromRoom =="Sentry" && toRoom =="Library")||(fromRoom =="Library" && toRoom =="Sentry") )
    {return true;}

    else {return false;}
}

bool isValid(int X, int Y)
{
    if (X < 0 || X >= mapWidth || Y < 0 || Y >= mapHeight)
    {
        return false;
    }
    if(isBlocked(playerX,playerY,X,Y))
    {return false;}
    return dungeonlayout[Y][X] != "WALL";


}
void revealAdjcent(){
      if (playerY > 0) visited[playerY - 1][playerX] = true;
    if (playerY < mapHeight ) visited[playerY + 1][playerX] = true;
    if (playerX > 0) visited[playerY][playerX - 1] = true;
    if (playerX < mapWidth ) visited[playerY][playerX + 1] = true;

}
vector<string> getMoves() //using vector becuase size of list will change depending on postition 

{
    vector<string> moves;
    if (playerY > 0 && isValid(playerX, playerY - 1))
    { moves.push_back("Up (w)"); }
    
    if (playerY < mapHeight - 1 && isValid(playerX, playerY + 1))
    { moves.push_back("Down (s)"); }
    
    if (playerX > 0 && isValid(playerX - 1, playerY))
    { moves.push_back("Left (a)"); }
    
    if (playerX < mapWidth - 1 && isValid(playerX + 1, playerY))
    { moves.push_back("Right (d)"); }
    
    return moves;
}
bool move(string& direction)
{
    int newX = playerX;
    int newY = playerY;

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

    if(!isValid(newX,newY))
    {
        if(newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)
        {
            cout << "Error";
        }else if(checkLocked(playerX,playerY,newX,newY)){
        cout<<"This pathway is blocked by a \033[43mlocked\033[0m gate\n";


        }else{
            cout<<"theres no way to go this direction \n";
            }
        return false;
    }

    playerX = newX;
    playerY = newY;
    visited[playerY][playerX] = true;
    revealAdjcent();
    return true;
}
void displayPlrPos()
{
    cout << col(blue) << "\n+========================================================+\n" << col();
    cout << "ROOM name: " << dungeonlayout[playerY][playerX] << endl;
    cout << "MAP Position X: " << playerX << "\tY: " << playerY ;
    cout << col(blue) << "\n+========================================================+\n" << col();
}
void displayMap(bool dev = false)
{
    cout << col(cyan) << "\n+========================== MAP ===========================+\n" << col();
        if(!dev){
            for(int Y = 0; Y < mapHeight; Y++){
                for(int X = 0; X < mapWidth; X++){
                    if(X == playerX && Y == playerY){
                            cout<< col(yellow)<<"[YOU] "<<col();
                        }else if(!visited[Y][X]){
                            cout << "[~~~] ";
                        }else if(dungeonlayout[Y][X] == "WALL"){
                            cout << col(red)<<"[###] " <<col();
                        }else{
                        cout <<col(red) <<"(" << X << "," << Y << ") "<<col();
                        }
            }
            cout<<endl;
        }
    }
        if(dev){
            for(int Y = 0; Y < mapHeight; Y++){
                for(int X = 0; X < mapWidth; X++){
                    if(X == playerX && Y == playerY){
                        cout<< col(yellow)<<"[YOU] "<<col();
                    }else if(dungeonlayout[Y][X] == "WALL"){
                        cout << col(red)<<"[###] "<<col();
                    }else{
                        cout <<col(red) <<"(" << X << "," << Y << ") "<<col();
                    }
            }
            cout<<endl;
        }
    }
    cout << col(cyan) << "\n+========================================================+\n" << col();
}



/*
    --------------------------------COMMAND--------------------------------
*/
commandType processCommand(const string input,string& argumentsOut) {  //return the commandtype of the command 
    int spacePosition = input.find(' ');
    string cmd;
    if (spacePosition != string::npos){
        cmd = input.substr(0,spacePosition);
    }else {
        cmd = input;}
    if (spacePosition != string::npos){
        argumentsOut = input.substr(spacePosition + 1);
    } else{
        argumentsOut = "";}
   
    if(tutorialComplete){        
        if(cmd == "quit" || cmd == "exit") return {commandType::QUIT};
        if(cmd == "observe" || cmd == "look") return {commandType::OBSERVE};
        if(cmd == "map") return {commandType::MAP};
        if(cmd == "inventory" || cmd == "inv") return {commandType::INVENTORY};
        if(cmd == "pickup" || cmd == "take") return {commandType::PICKUP};
        if(cmd == "use") {
            return{commandType::USE};}
        if(cmd == "unlock") {
            argumentsOut = input;
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
        if((cmd == "pickup" || cmd == "take" ) && argumentsOut =="match") {           
            return {commandType::PICKUP};}
        if(cmd == "observe" || cmd == "look"){
            tutorialObserved = true;
            return {commandType::OBSERVE};}
        if(cmd == "use") {
            return{commandType::USE};}
    }
        return {commandType::INVALID};
}
void executeCommand(commandType type,string arguments) {
    // cout<<"args:"<<arguments<<endl<<endl;

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
            unlockDoor(exitDoor,"Rusty key");
            unlockDoor(tutorialDoor,"");
            break;
            
        case commandType::MOVE:
            if(move(arguments)){cout<< "you moved:"<<arguments<<endl;}
            break;
            
        case commandType::INVALID:
            cout << "Invalid command!" << endl;
            break;
        case commandType::USE:
            if(!arguments.empty()) {
                bool effect = false;
                if(useItem(arguments, effect, playerX, playerY)) {
                    cout << "You used the " << arguments << endl;
                    if(effect) {
                        cout << "Something changed..." << endl;}
                }
            } else {
                cout << "Use what? Specify an item name." << endl;
            }
            break;
    }
        cout<< "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GAME_LOOP()
{
    string command;
    visited[playerY][playerX] = true;
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
                cursedNote();
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
        lowerCase(command);

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
        displayPlrPos();
        displayMap();
        listItems(playerX,playerY);


        cout << endl;
            /*
            ---------------------------GET COMMAND--------------------------
            */
        cout << "Enter command: ";
        getline(cin, command);
        lowerCase(command);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
            
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


