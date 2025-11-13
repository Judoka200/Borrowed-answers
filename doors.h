#include <iostream>
#include <string>
#include <vector>



struct door{
    int fromX;
    int fromY;
    int toX;
    int toY;
    bool locked;
    std::string key;
};


door tutorialDoor = {0,1,1,1,true,""};

door exitDoor = {2,1,3,1 ,true,"Rusty key"};
door hallwayDoor = {1,1,2,1,true,""};


bool checkLocked(  int fromX, int fromY, int toX, int toY){
    if(exitDoor.fromX == fromX && exitDoor.fromY == fromY && exitDoor.toX == toX && exitDoor.toY == toY){
        return exitDoor.locked;
    }

    if(hallwayDoor.fromX == fromX && hallwayDoor.fromY == fromY && hallwayDoor.toX == toX && hallwayDoor.toY == toY){
        return hallwayDoor.locked;
    }
   
    if(tutorialDoor.fromX == fromX && tutorialDoor.fromY == fromY && tutorialDoor.toX == toX && tutorialDoor.toY == toY){
        return tutorialDoor.locked;
    }
    return false;    

}

void unlockDoor(door& gate, std::string key = ""){
    if(key == gate.key){
        gate.locked = false;
    }
}


