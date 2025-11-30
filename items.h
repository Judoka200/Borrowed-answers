#include <string>
#include <vector>
#include "doors.h"
#include "vars.h"


#ifndef VARS_H
 bool usedMatch = false;

#endif


struct item{
    std::string itemTitle;
    std::string itemDesc;
    bool canPickup;
    bool visible;
    colours colour = colours::Default;
    colours backgroundColour = colours::Default;

};


//creates a vector of each room which holds a vector of items 
std::vector<item> roomItems[mapHeight][mapWidth];
std::vector<item> inventory;

#pragma region 
//   item           title           desc                                              pickable     visible    FG col            BG col
item campfire =   {"campfire", " campfireDesc"                                          ,false,       true};
item testItem =   {"test item", "desc"                                                  ,true,        true,  colours::magenta};
item key =        {"Rusty key", "rusty key desc"                                        ,true,        true,  colours::yellow};
item dagger =     {"dagger", "   likely once a gleaming blade, now a historical relic"  ,true,        true,  colours::red};
item book =       {"book", "     old Book desc"                                         ,true,        false, colours::black, colours::black};
item match =      {"match", "    a simple match to light a fire"                        ,true,        true,  colours::yellow};
item entityNote = {"note",       output("Entity_warning")                               ,false,       true,  colours::cyan};

#pragma endregion


void generateItems()
{   //roomItems[Y][X]
    roomItems[1][0].push_back(match);
    roomItems[1][0].push_back(campfire);
    #ifdef dev
    roomItems[1][0].push_back(testItem);
    #endif

    roomItems[0][1].push_back(key);

    roomItems[2][1].push_back(entityNote);
    roomItems[2][1].push_back(dagger);

    roomItems[2][2].push_back(book);
}

bool pickupItem(std::string itemName,int pX,int pY) {
    
    // std::cout << "Looking for: '" << itemName << "'" << std::endl;                     DEBUG
    // std::cout << "Items in room: " << roomItems[pY][pX].size() << std::endl;           DEBUG
    // std::cout <<"curernt room"<<pX<<" "<<pY<<std::endl;                                DEBUG

    /*
        https://www.w3schools.com/cpp/cpp_iterators.asp 
    */
    for (auto it = roomItems[pY][pX].begin(); it != roomItems[pY][pX].end(); ++it) {
                //std::cout << "Checking: '" << it->itemTitle << "' canPickup=" << it->canPickup << std::endl;
        if (lowercase(it->itemTitle) == lowercase(itemName) && it->canPickup) { //  -> is used to get value of struct 'feature' becuase auto i is a pointer to roomItems[pY][pX] 
                //add item vec to inventory 
            inventory.push_back(*it);    //  *i derefrences i, refrencing back to roomItems[pY][pX]?
                //remove item from room
                std::cout << "Picked up the: " << col(it->colour,it->backgroundColour)<< it->itemTitle << col() << std::endl;
                roomItems[pY][pX].erase(it);  

            return true;
        }else if (lowercase(it->itemTitle) == lowercase(itemName) && (it->canPickup == false))
        {
                std::cout << "That item can't be picked up." << std::endl;
                return false;  
        }
        
    }
    std::cout << "Item doesn't exist." << std::endl;
    return false;
}

bool hasItem(std::string item){
    for(auto& i : inventory){
        if(i.itemTitle == lowercase(item)){
            return true;
        }
    } 
    return false;
}

void listItems(bool viewInvisible = false){
std::cout << "\033[38;5;203m" <<"\t----------------Items in room----------------\n" <<col() ;
 for (const auto& item : roomItems[playerY][playerX]) {
    if(item.visible && !viewInvisible){
            if (item.itemTitle =="campfire" && usedMatch){
//                                              5: blinking
                std::cout << "  - " << item.itemTitle << ":\033[5m " // -->
                  <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col(colours::RESET) << std::endl;
            }else{
                std::cout << "  - " << item.itemTitle << ": " <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<< std::endl;

            }
        }else{
            // std::cout << "  - " << item.itemTitle << ": " << item.itemDesc << std::endl;

        }
        if(viewInvisible){
            if (item.itemTitle =="campfire" && usedMatch){
//                                              5: blinking
                std::cout << "  - " << item.itemTitle << ":\033[5m " // -->
                 << col(item.colour,item.backgroundColour)<< item.itemDesc << col(colours::RESET) << std::endl;
            }else{
                std::cout << "  - " << item.itemTitle << ": " <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<< std::endl;

            }

        }
    }
std::cout << "\033[38;5;203m" <<"\t---------------------------------------------\n" <<col() ;

}

//returns true/false if item is/isn't in room[Y][X]
bool roomHasItem(std::string itemName, int X, int Y){
    for(auto it : roomItems[Y][X]){
        if(lowercase(itemName) == it.itemTitle){
            #ifdef dev  
            std::cout << "room has " << itemName<< std::endl; 
            #endif            
            return true;
        }
    }
    #ifdef dev  
    std::cout << "room doesnt have "<< itemName <<std::endl;
    #endif
    return false;
}



// -------INV------
//     item: desc
// ~--------------
void viewInventory() {
    std::cout << "\033[38;5;202m" << "------------------INVENTORY------------------\n" << col();
    if (inventory.empty()) {
        
        std::cout << "\t\t  Empty" << std::endl;
    } else {

        for (const auto& item : inventory) {
            std::cout << "  - " <<col(item.colour,item.backgroundColour)<< item.itemTitle << ": " << item.itemDesc << std::endl;
            std::cout << col();
        }    
    }
    std::cout << "\033[38;5;202m" <<"---------------------------------------------\n" <<col() ;
}

// pX & pY used to check current room for campfire 
void viewBook(){
    std::string f;
    std::string line;
    bool lit = roomHasItem("campfire",playerX,playerY);
    for(int i = 1; i <= 20; i++){
        if(lit){
            f = "book_shown[" + std::to_string(i) + "]";   
            line = output(f, book.colour, book.backgroundColour, true);
            std::cout << line << std::endl;
        }
    }
    if(!lit){
        std::cout << output("book_dark");
    }
}
 
bool useItem(std::string itemName) {
    // Check if player has the item in inventory
    bool hasItem = false;
    for(auto& i : inventory) {
        if(lowercase(i.itemTitle) == lowercase(itemName)) {
            hasItem = true;
            break;
        }
    }
    if(!hasItem) {
        std::cout << "You don't have that item in your inventory." << std::endl;
        return false;
    }
    
    // -------------MATCH------------ //
    if(lowercase(itemName) == "match" && playerX == 0 && playerY == 1) {
       for(auto& item : roomItems[playerY][playerX]) {
            if(lowercase(item.itemTitle) == "campfire"&& usedMatch == false) {
                usedMatch = true;
                item.backgroundColour = colours::red;
                item.colour = colours::yellow;
                std::cout << "You light the campfire with the match. The room fills with warmth and light." << std::endl;
                return true;
            }
        }
    }       


    if (lowercase(itemName) == "rusty key" && playerX == 1 && playerY == 1){
        unlockDoor(hallwayDoor,"");
        #ifdef dev
        std::cout<<"trying to unlock hallway door\n";
        #endif
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            auto a= *it;             
            inventory.erase(it);
            break;
        }
        return true;
    }  
    // -------------BOOK------------ //
    if(lowercase(itemName) == "book") {
        viewBook();
        return true;
    }
    
    
    // Item exists but can't be used here/now
    std::cout << "You can't use the " << itemName << " right now." << std::endl;
    return false;


}


