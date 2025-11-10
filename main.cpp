#include "funcs.h"
#include "dialogue.h"
#include "items.h"
#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;


//initialise size of dungeon (set the width and height)
//access the layout by dungeonlayout[ Y POS][X POS]
const int mapWidth = 4; //X
const int mapHeight =3;  //Y
string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL","Dumping_Grounds","WALL","WALL"},           // [0][0] | [0][1] | [0][2] | [0][3]
{"Campfire_Room","Hallway","Sentry","Exit"},        // [1][0] | [1][1] | [1][2] | [1][3]
{"WALL","Library_Entrance","Library","WALL"}        // [2][0] | [2][1] | [2][2] | [2][3]
};

bool visited[mapHeight][mapWidth] ={false};
bool observed[mapHeight][mapWidth] ={false};

int playerX = 0;
int playerY = 1;

bool isBlocked(int fromX,int fromY, int ToX, int ToY){
    string fromRoom = dungeonlayout[fromY][fromX];
    string toRoom = dungeonlayout[ToY][ToX];

    if(fromRoom =="Sentry" && toRoom =="Library")
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

//using vector becuase size of list will change depending on postition 
vector<string> getMoves()
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

bool move(string direction)
{
    int newX = playerX;
    int newY = playerY;

    if (direction =="w" || direction =="W" || direction =="up" || direction =="Up")
        {newY -= 1;}
    if (direction =="a" || direction =="A" || direction =="left" || direction =="Left")
        {newX -= 1;}
    if (direction =="s" || direction =="S" || direction =="down" || direction =="Down")
        {newY += 1;}
    if (direction =="d" || direction =="D" || direction =="right" || direction =="Right")
        {newX += 1;}

    if(!isValid(newX,newY))
    {
        if(newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)
        {
            cout << "Error";
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

void GAME_LOOP()
{
    string command;
    visited[playerY][playerX] = true;
    revealAdjcent();
    

    // typeWrite("Campfire_room_opening",green);
    typeWrite("WHY???",red);
    timeDelay(1.5);
    
    while(true)
    {  
        displayPlrPos();
        
        if(!observed[playerY][playerX]) //if room hasnt been enterd, provide the room's description 
        {
            cout<< "You observe the room your in\n";
            typeWrite(dungeonlayout[playerY][playerX]+"_desc");
            observed[playerY][playerX] = true;
            cout<<endl;
        }
        vector<string> moves = getMoves();
        for (int i =0; i <moves.size(); i++){
            cout<<moves[i];
            if (i < moves.size() - 1) cout << ", "; // seperates each move by , unless there are no more moves
        }
        cout<<endl;
        displayMap();
/*
---------------------------GET COMMAND--------------------------
*/
        cout << "Enter command: ";
        getline(cin,command);

        if (command =="quit"){
            cout << "bye";
            break;
        
        }
        else if((command =="observe"|| command =="o"))
            {
             typeWrite(dungeonlayout[playerY][playerX]+"_desc");
                cout<<endl;
            cout<< "Press Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        else if(command == "map"){
            displayMap();
            cout<< "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        // else if(){}
        // else if(){}
        // else if(){}
        // else if(){}


        else if(moveDirection(command)){
                if(move(command)){
                cout<< "you moved:" << command<<endl;
                }
                cout<<"\n press enter to continure ...";   
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }else {
            cout << "Invalid command! Use W/A/S/D, 'map', or 'quit'" << endl;
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        clearScreen();
    }
}



int main(){

    generateItems();
    clearScreen()  ;
    cout<<col(); //reset all colour formatting
    int stepsRemaining = 10;  // setup ]
    GAME_LOOP();
    
}


