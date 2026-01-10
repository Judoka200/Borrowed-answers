#include <string>
#include <vector>
#include "doors.h"
#include "vars.h"

struct item{
    std::string itemTitle;
    std::string itemDesc;
    bool canPickup;
    bool visible;
    colours colour = colours::Default;
    colours backgroundColour = colours::Default;

};


//creates a array the layout which holds a vector of items for each room 
std::vector<item> roomItems[mapHeight][mapWidth];
std::vector<item> inventory;

//   item           title           desc                                                takable     visible    FG col            BG col
item campfire =   {"campfire", " campfireDesc"                                          ,false,       true};
item testItem =   {"test item", "desc"                                                  ,true,        true,  colours::magenta};
item key =        {"rusty key", "rusty key desc"                                        ,true,        true,  colours::yellow};
item dagger =     {"dagger", "   likely once a gleaming blade, now a historical relic"  ,true,        true,  colours::red};
item book =       {"book", "     old Book desc"                                         ,true,        false, colours::black, colours::black};
item match =      {"match", "    a simple match to light a fire"                        ,true,        true,  colours::yellow};
item entityNote = {"note",       output("Entity_warning")                               ,false,       true,  colours::cyan};


void generateItems(){    // places items in the layout using the roomItems[ ][ ] array  
 // roomItems[Y][X]

        // Campfire_Room
    roomItems[1][0].push_back(match);
    roomItems[1][0].push_back(campfire);

        // Cells
    roomItems[0][1].push_back(key);

        // Tome_hall_Entrance
    roomItems[2][1].push_back(entityNote);
    roomItems[2][1].push_back(dagger);

        // Tome_hall
    roomItems[2][2].push_back(book);
}

bool pickupItem(std::string itemName,int pX,int pY) {
#ifdef dev
    std::cout << col(colours::black, colours::cyan);
    std::cout << "Looking for: '" << itemName << "'" << std::endl;
    std::cout << "Items in room: " << roomItems[pY][pX].size() << std::endl;
    std::cout <<"current room X:" << pX << " Y:" << pY << std::endl << col();
#endif
    /*
        https://www.w3schools.com/cpp/cpp_iterators.asp 
    */
    for (auto item = roomItems[pY][pX].begin(); item != roomItems[pY][pX].end(); ++item) {
#ifdef dev         
        std::cout << col(colours::black, colours::cyan) << "Checking: '" << item->itemTitle << "' canPickup=" << item->canPickup << std::endl << col();
#endif
        if (lowercase(item->itemTitle) == lowercase(itemName) && item->canPickup) {                         //   '->' is used to get value of struct becuase auto 'item' is a pointer to roomItems[pY][pX] 
                // add item to inventory vector
            inventory.push_back(*item);                                                                   //   *i derefrences i, refrencing back to roomItems[pY][pX]?
            std::cout << "Picked up the: " << col(item->colour, item->backgroundColour)<< item->itemTitle << col() << std::endl;
            roomItems[pY][pX].erase(item);                                                                // remove item from room
            return true;

        } else if (lowercase(item->itemTitle) == lowercase(itemName) && (item->canPickup == false)){
            std::cout << "That item can't be picked up." << std::endl;
            return false;  
        }
        
    }
    std::cout << "Item doesn't exist." << std::endl;
    return false;
}

bool hasItem(std::string item){   
    for(auto i : inventory){
        if(i.itemTitle == lowercase(item)){
            return true;
        }
    } 
    return false;
}

void listItems(bool viewInvisible = false){
    std::cout << "\033[38;5;203m" <<"\t----------------Items in room----------------\n" <<col() ;
    for ( auto& item : roomItems[playerY][playerX]) {

        if(item.visible && !viewInvisible){                                                             // !viewInvisible needed so items aren't shown twice when view invisible true 
            if (item.itemTitle =="campfire" && usedMatch){                                              // lights campfire in item list (yellow, blinking text + red background )
                std::cout << "  - " << item.itemTitle << ":\033[5m " <<                                 // 5: blinking
                          col(item.colour, item.backgroundColour)<< item.itemDesc <<col(colours::RESET) << std::endl;
            } else {
                std::cout << "  - " << item.itemTitle << ": " <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<< std::endl;
            }
        }

        if(viewInvisible){
            if (item.itemTitle =="campfire" && usedMatch){
                std::cout << "  - " << item.itemTitle << ":\033[5m " <<                                  // 5: blinking
                          col(item.colour,item.backgroundColour)<< item.itemDesc << col(colours::RESET) << std::endl;
            } else {
                std::cout << "  - " << item.itemTitle << ": " <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<< std::endl;
            }
        }
    }
    std::cout << "\033[38;5;203m" <<"\t---------------------------------------------\n" <<col() ;

}

// returns   true/false if item is/isn't in room[Y][X]
bool roomHasItem(std::string itemName, int X, int Y){
    for(auto item : roomItems[Y][X]){
        if(lowercase(itemName) == item.itemTitle){     
            return true;
        }
    }
    return false;
}



// -------INV------
//     item: desc
// ~--------------
void viewInventory() {
    std::cout << "\033[38;5;202m" << "------------------INVENTORY------------------\n" << col();                 // \033[38;5;202m: orange colour;
    if (inventory.empty()) {
        std::cout << "\t\t  Empty" << std::endl;
    } else {
        for (auto item = inventory.begin(); item != inventory.end(); ++item ) {
            
//                                      sets colour for item                 itemTitle [string]         itemDesc [string]
            std::cout << "  - " << col(item->colour, item->backgroundColour) << item->itemTitle << ": " << item->itemDesc << std::endl;
            std::cout << col();                                                         // reset colour for next item
        }    
    }
    std::cout << "\033[38;5;202m" <<"---------------------------------------------\n" <<col();                   // \033[38;5;202m: orange colour
}

void viewBook(){
    std::string LineName;
    std::string line;
    bool lit = roomHasItem("campfire",playerX,playerY);             // only show the book if there is a campfire in the room [room has enough light]
    if(lit){
        for(int lineNum = 1; lineNum <= 20; lineNum++){
            LineName = "book_shown[" + std::to_string(lineNum) + "]";   
            line = output(LineName, book.colour, book.backgroundColour, true);
            std::cout << line << std::endl;
            }
        }
    if(!lit){
        std::cout << output("book_dark");                           // message saying room is too dark 
    }
}
 
bool useItem(std::string itemName) {
    if(!hasItem(itemName)) {                                                                                        // if player doesnt have item 
        std::cout << "You don't have that item in your inventory." << std::endl;
        return false;
    }
    
    // -------------MATCH------------ //
    if(lowercase(itemName) == "match" && currentRoom("Campfire_Room")) {                                            // use match in campfire room and nowhere else
       for(auto item = roomItems[playerY][playerX].begin(); item != roomItems[playerY][playerX].end(); ++item) {    // loop through roomitems and find campfire 
            if(lowercase(item->itemTitle) == "campfire"&& usedMatch == false) {
                usedMatch = true;
                item->backgroundColour = colours::red;                                                              // change campfire colours to yellow FG and red BG
                item->colour = colours::yellow;
                std::cout << "You light the campfire with the match. The room fills with warmth and light." << std::endl;
                return true;
            }
        }
    }       

    // --------------KEY------------- //
    if (lowercase(itemName) == "rusty key" && currentRoom("Hallway")){                                              // use key in hallway to open door 
        unlockDoor(hallwayDoor,"");
        return true;
    }  

    // -------------BOOK------------ //
    if(lowercase(itemName) == "book") {
        viewBook();                                                                                                  // room is checked through campfire check in viewBook()
        return true;
    }
    
    std::cout << "You can't use the " << itemName << " right now." << std::endl;                                     // has item but cant use it in current room
    return false;
}