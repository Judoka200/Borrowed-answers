#include "funcs.h"
#include "dialogue.h"
#include <vector>
#include <limits> // needed for numeric_limits<streamsize>::max()
using enum colours; //used for col() to colour text | prevents having to use colours:: every time 
using namespace std;

const string moveDirections[] = 
    {"w","W","up","Up",
     "a","A","left","Left",
     "s","s","down","Down",
     "d","D","right","Right"};

bool moveDirection(string str){
    if (str == "w"||str == "W"||str == "up"||str == "Up"
      ||str == "a"||str == "A"||str == "left"||str == "Left"
      ||str == "s"||str == "S"||str == "down"||str == "Down"
      ||str == "d"||str == "D"||str == "right"||str == "Right")
      return true;  
      else return false;
    }

//initialise size of dungeon (set the width and height)
//access the layout by dungeonlayout[ Y POS][X POS]
const int mapWidth = 5; //X
const int mapHeight =3;  //Y
string dungeonlayout[mapHeight][mapWidth] = {  
{"WALL","Campfire Room","WALL"},
{"Room 2","Room 3","Room 4"},
{"WALL","WALL","WALL"}
};

bool visited[mapHeight][mapWidth] ={false};

int playerX = 1;
int playerY = 1;

bool isValid(int X, int Y)
{
    if (X < 0 || X >= mapWidth || Y < 0 || Y >= mapHeight)
    {
        return false;
    }
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

    while(true)
    {
        displayPlrPos();
        vector<string> moves = getMoves();
        for (int i =0; i <moves.size(); i++){
            cout<<moves[i];
            if (i < moves.size() - 1) cout << ", "; // seperates each move by , unless there are no more moves
        }
        
        cout << "Enter command: ";
        getline(cin,command);


        if (command =="quit"){
            cout << "bye";
            break;
        
        }else if(command == "map"){
            displayMap();
            cout<< "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }else if(moveDirection(command)){
                if(move(command)){
                cout<< "you moved:" << command<<endl;
                }
                cout<<"\n press enter to continure ...";   
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cout << "Invalid command! Use W/A/S/D, 'map', or 'quit'" << endl;
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        clearScreen();
    }
}



int main(){
    cout<<col(); //reset all colour formatting
    int stepsRemaining = 10;  // setup ]
    GAME_LOOP();
    
}







