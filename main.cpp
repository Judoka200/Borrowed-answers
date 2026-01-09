#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max() to remove whole input buffer
#include <chrono> // used for time taken (start - end times)
#include "funcs.h"
#include "dialogue.h"
#include "items.h"
#include "vars.h"

// dev is defined in vars.h 

using namespace std;


enum commandType {
    QUIT,    OBSERVE,    _MAP_,    INVENTORY,    PICKUP,
    _UNLOCK_,    MOVE,    USE,     HELP,    INVALID, TALK
};

#pragma region functions 
bool isBlocked(int fromX,int fromY, int ToX, int ToY){
    // checks for doors and prevents specific room transitions
    string fromRoom = dungeonlayout[fromY][fromX];
    string toRoom = dungeonlayout[ToY][ToX];

    if(checkLocked(fromX, fromY, ToX, ToY)){                     // returns true if path is locked 
        return true;
    }
    // block room between sentry room and Tome_hall
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
    if (X < 0 || X >= mapWidth || Y < 0 || Y >= mapHeight){ // checks if player has a room between them and the border (wont go outside the map)
        return false;
    }
    else if(isBlocked(playerX,playerY,X,Y)){
        return false;
    } else if(dungeonlayout[Y][X] == "WALL"){
        return false;
    }
    return true;
}
void revealAdjcent(){   // sets all adjacent rooms to visible using bool array
/* the if statemetns prevent trying to write true to a place outside of the array */
    if (playerY > 0) visible[playerY - 1][playerX] = true;          // up

    if (playerY < mapHeight ) visible[playerY + 1][playerX] = true; //  down

    if (playerX > 0) visible[playerY][playerX - 1] = true;          // left

    if (playerX < mapWidth ) visible[playerY][playerX + 1] = true;  // right
}
vector<string> getMoves(){ // using vector becuase size of list will change depending on the room 

    vector<string> moves; // just a tempory vector to store the possible moves in
    // if valid room to 'direction' of player then add the corresponding string to the vector
    if (playerY > 0 && isValid(playerX, playerY - 1))   
    { moves.push_back("Up (w)"); }

    if (playerX > 0 && isValid(playerX - 1, playerY))   
    { moves.push_back("Left (a)"); }
    
    if (playerY < mapHeight - 1 && isValid(playerX, playerY + 1))   
    { moves.push_back("Down (s)"); }
    
    if (playerX < mapWidth - 1 && isValid(playerX + 1, playerY))    
    { moves.push_back("Right (d)"); }
    return moves;   // returns the possible moves as a vector of strings 
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

    if(!isValid(newX,newY))     // if the direction to next room is INVALID
    {
        if(newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)    // invalid becuase no room
        {
            cout << "theres no pathway in that direction\n";      
        } else if(checkLocked(playerX,playerY,newX,newY)){                    // invlaid becuase locked path
            cout<<"This pathway is blocked by a \033[43mlocked\033[0m gate\n";       // 43: yellow background
        }
        #ifndef dev
        else if(dungeonlayout[newY][newX] == "Cells" && !hasItem("dagger")){ //  invalid becuase unarmmed
            cout << col(red) << "when taking a step towards the doorway, you feel an overwhelming amount of dread\n its better to not go unarmed into this place\n"; 
        }
         #endif
        else {
            cout<<"theres no way to go this direction \n";  // any exceptions e.g. Blocked path between rooms    
        }
        return false;   // move was unsuccesful for some reason
    }
//  set the current player position to the next position 
    playerX = newX;     
    playerY = newY;
    nextX = newX;       
    nextY = newY;
    visible[playerY][playerX] = true; // current room to visible 
    revealAdjcent();        
    return true;        // move was successful
}
void displayPlrPos()
{
    cout << "\e[38;5;27m" << "+========================== ROOM =========================+\n" << col();
    cout << "ROOM name: " << dungeonlayout[playerY][playerX] << endl;
    cout << "_MAP_ Position X: " << playerX << "\tY: " << playerY;
    // cout << "\t| Steps Remaining: "<< stepsRemaining;
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
                        } else if(!visible[Y][X]){
                            cout << "[~~~~] ";
                        } else if(dungeonlayout[Y][X] == "WALL"){
                            // cout << col(red)<<"[###] " <<col();
                            cout << col(red)<<"[####] " <<col();
                        } else {
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
                        cout<< col(colours::yellow)<<"[PLYR] "<<col();
                    } else if(dungeonlayout[Y][X] == "WALL"){
                        cout << col(colours::cyan)<<"(####) " <<col();
                    } else {
                        // cout <<col(red,cyan) <<"{" << X << "," << Y << "} "<<col();
                        string roomName;
                        if(dungeonlayout[Y][X].length() >=4)
                        { roomName = dungeonlayout[Y][X].substr(0,4);}
                        cout <<col(colours::cyan) <<"(" << roomName << ") "<<col();
                    }
            }
            cout<<endl;
        }
    #endif
    cout << "\e[38;5;62m" << "\n+=========================================================+\n" << col();
}
#pragma endregion

/*    --------------------------------COMMAND--------------------------------    */
commandType processCommand(const string input,string& argumentsOut) {  // return the commandype of the command 
    
    #ifdef dev
        cout<<col(colours::black,colours::cyan)<< "DEV: input is:"<<input<<col()<<endl;
    #endif

    int spacePosition = input.find(' ');   // find the pos of the space which should seperate the cmd and arguments
    string cmd;
    /* get the command keyword half */
    if (spacePosition != string::npos){
        cmd = input.substr(0,spacePosition);  // store up to the space into cmd
        argumentsOut = input.substr(spacePosition + 1); // store after the space into argumetnsOut
    } else {
        cmd = input;                // if theres nothing after the command then nothing needs to be done
        argumentsOut = "";
    }

    if(tutorialComplete){                   // full command list
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
    
    else {                                   // limited command list for tutorial 
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
#ifdef dev                                   // still provides commands for dev mode
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
    cout << col(colours::black,colours::cyan)<<"argumetns are: "<<arguments<<col()<<endl;
#endif
    lowercase(arguments,true); // sets the argumetns to lowercase
    switch(type) {
        case commandType::QUIT:
            break;

        case commandType::OBSERVE:
            typeWrite(dungeonlayout[playerY][playerX] + "_desc",colours::Default,descDelay);    // double the speed for descriptions
            break;
            
        case commandType::_MAP_:      // dev mode only
            displayMap();
            break;
            
        case commandType::INVENTORY:
        /* dont call viewInventory() so that it can replace the item list section of the interface */
            showInventory = true;
            break;
            
        case commandType::PICKUP:
            if(!arguments.empty()) {
           
                pickupItem(arguments, playerX, playerY);
            } else {
                cout<<"what do you want to pickup?" << endl;
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
            if(arguments == "match" && hasItem("match"))  // tutorial use check
                {tutorialUsedMatch = true;}
            
            if(useItem(arguments)) {
                if(arguments== "book"){
                    cout << "You read the " << arguments << endl;
                } else {
                cout << "You used the " << arguments << endl;
                }
            }
        } else {
            cout << "what do you want to use?" << endl;
        }
        break;

        case commandType::TALK:
        clearScreen();
            cout<< col(yellow) <<"You talk to the guard"<< col() << endl;
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
    
    typeWrite("Campfire_room_opening",colours::magenta,descDelay);

   while(!GAME_LOOP_END)
   {  
        clearScreen();
        #pragma region          /*    -----------------------------------DISPLAY UI---------------------------------   */

        displayPlrPos();
        displayMap();

        // toggles the item list for the inventory list when inventory command is called
        if(showInventory){
            viewInventory();
        } else {
            listItems(canViewInvisible);
        }
        showInventory = false;
        cout<<endl;

#pragma endregion 
        
#pragma region                  /*    -----------------------------------ROOM CHECKS------------------------------   */
        if(!observed[playerY][playerX]) // if room hasnt been enterd, provide the room's description 
        {
            cout<< col(colours::green)<<"You observe the room your in: \n"<< col();     // 1: bold , 22:  revert underline 
            if(currentRoom("Cells")) {
                entityInteraction();
                canViewInvisible = true;
             
#ifndef dev
            } else if (currentRoom("Sentry")){         // gain TALK command
                seenSentry = true;
                typeWrite("You face a towering, stone, cloaked staute with a raven on its shoulder\n", colours::Default, 0.01);
                typeWrite(" when moving your eyes to it's cloaked gaze, all you see is a flash of white\n ", colours::Default, 0.01);
                cout << col(colours::blue, colours::white) << "You gain the ability to commune with what stands in front of you\n" << col();
                showCommands();
#endif
            } else if(currentRoom("Exit")){
                
                if(!refusedEntity){
                    for(int lineNum = 1; lineNum <=2; lineNum++){
                        std::string line = "cursed_ending[" + std::to_string(lineNum) + "]";
                        typeWrite(line, colours::yellow,0.01);
                        std::cout << std::endl;
                    }
                    cout<< "\n\033[38;5;245mPress Enter to continue...";            // grey colour from table 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');                    
                }else if(refusedEntity){
                    for(int lineNum = 1; lineNum <=2; lineNum++){
                        std::string line = "freedom_ending[" + std::to_string(lineNum) + "]";
                        typeWrite(line, colours::yellow,0.01);
                        std::cout << std::endl;
                    }
                    cout<< "\n\033[38;5;245mPress Enter to continue...";            // grey colour from table 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');                    
                
                }
                GAME_LOOP_END = true;
                break;

            } else {
                typeWrite(dungeonlayout[playerY][playerX] + "_desc",colours::Default, descDelay);
            }
            observed[playerY][playerX] = true;  // sets the current room to have been observed on entry
            cout<<endl;
        }
        
#pragma endregion 

#pragma region                  /*    ----------------------------------DIPLAY MOVES--------------------------------   */
        cout<<col(colours::green)<<"possible moves: "<<col();
        vector<string> moves = getMoves();      // gets a vector of possible moves 
        for (int i = 0; i < moves.size(); i++){
            cout << moves[i];
            if (i < moves.size() - 1){
                cout << ", ";    // seperates each direction by ', ' until no more moves
            }
        }
        cout << endl;
#pragma endregion

        /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command, true);   // sets the whole command to lowercase incase i forget to add lowercase() to if statements

        string argument;  // temp variable that will store the argument, will be edited by processCommand()
        commandType type = processCommand(command, argument);
        cout << endl;
        executeCommand(type,argument);
        
        if(GAME_LOOP_WON){unlockDoor(exitDoor);}
        
        clearScreen();

        }
}

void TUTORIAL_LOOP(){
    string command;
    
    
    while(!tutorialComplete)
    {  
        /*    -----------------------------------DISPLAY UI---------------------------------    */  
        showCommands();
        /*    -------------------------------TUTORIAL COMPLETION----------------------------    */
        cout<<col(colours::cyan)<< "++++++++++++++++TUTORIAL COMPLETION++++++++++++++++ \n"<<col();
//                                          2: dimmed, 4: underline, 9: strikethrough, 0: RESET
        cout<< ((tutorialObserved)?    "\033[2;9m - OBSERVE the room your in with:          \033[0m[#]\n":" - \033[4mOBSERVE\033[0m the room your in with:          [ ]\n") ;    
        cout << ((tutorialViewedInv)?  "\033[2;9m - VIEW at your inventory to see the item: \033[0m[#]\n":" - \033[4mVIEW\033[0m at your inventory to see the item: [ ]\n");
        cout << ((tutorialTakenMatch)? "\033[2;9m - PICKUP and Item within the room with:   \033[0m[#]\n":" - \033[4mPICKUP\033[0m and Item within the room with:   [ ]\n");
        cout << ((tutorialUsedMatch)?  "\033[2;9m - USE the item:                           \033[0m[#]\n":" - \033[4mUSE\033[0m the item:                           [ ]\n") ;
        cout<< col(colours::cyan)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++\n" <<col();

        cout<<endl;
        
        if(showInventory){
            viewInventory();
        } else {
            listItems(canViewInvisible);
        }
        showInventory = false;
        cout<<endl;
        /*    ----------------------------------GET COMMAND---------------------------------    */
        cout << "Enter command: ";
        getline(cin, command);
        lowercase(command,true);

        string argument;
        commandType type = processCommand(command, argument);
        executeCommand(type,argument);
        
         if (tutorialObserved&&tutorialTakenMatch&&tutorialUsedMatch&&tutorialViewedInv){
             clearScreen();
             cout<< col(colours::black,colours::yellow)<<"\n\n\nYou passed the Tutorial\nnow to face reality\n"<<col();
             typeWrite("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ",colours::green, 0.01);
             tutorialComplete = true;
            }       
        clearScreen();               
    }    
}

int main(){
    clearScreen();
    cout << "\e[8;32;111t"; // resizes the terminal window to the correct size [32 columns 111 rows]

    typeWrite("notice",colours::green);
    typeWrite("warning",colours::red);
    cout<< "\n\033[38;5;245mPress Enter to continue...";            // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearScreen()  ;
    generateItems();
    
    cout<<col(colours::RESET); // reset all colour formatting
#ifndef dev     // skips the tutorial if dev is defined
    tutorialComplete = false;       
    if (!tutorialComplete) {
        TUTORIAL_LOOP();
    }
#endif
    tutorialComplete = true;
    unlockDoor(tutorialDoor);  // not placed in TUTORIAL_LOOP, so will be unlocked even if TUTORIAL skipped
    clearScreen();

    auto startTime = chrono::high_resolution_clock::now(); // stores the start time to calculate time taken 

    GAME_LOOP();

    auto stopTime = chrono::high_resolution_clock::now(); // stores the start time to calculate time taken 
    auto timeTaken = duration_cast<std::chrono::seconds>(stopTime - startTime);

    clearScreen();
    if(refusedEntity){
        showWinScreens(1);
    } else {
        showWinScreens(2);
    }
    
    std::cout << "\033[38;5;15m" << format_duration(timeTaken);
    cout<< "\033[38;5;245m\nPress Enter twice to Exit";                   // grey colour from table 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
}



