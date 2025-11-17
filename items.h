#include <string>
#include <vector>

const int mapwidth = 4; //X
const int mapheight =3;  //Y

bool usedMatch = false;
struct item{
    std::string itemTitle;
    std::string itemDesc;
    bool canPickup;
    bool visible;
    colours colour;
    colours backgroundColour;

};

//creates a vector of each room which holds a vector of items 
std::vector<item> roomItems[mapheight][mapwidth];
std::vector<item> inventory;

#pragma region 
item campfire = {"Campfire", "campfireDesc", false, true};
item testItem = {"test item", "desc", true, true, colours::magenta};
item key =      {"Rusty Key", "rusty key desc", true, true, colours::yellow};
item cleaver =  {"cleaver", "cleaverdesc", true, true, colours::red};
item book =     {"book", "old Book desc", true, false, colours::black, colours::white};
item match =    {"match", "a simple match to light a fire", true,true, colours::yellow};
#pragma endregion


void generateItems()
{   //roomItems[Y][X]
    roomItems[1][0].push_back(match);

        //campfire in Campfire_room
    roomItems[1][0].push_back(campfire);
        //key in Dumping_Grounds
    roomItems[1][0].push_back(testItem);

    roomItems[0][1].push_back(key);
        //
    roomItems[2][1].push_back(cleaver);
        //book in Library 
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
            roomItems[pY][pX].erase(it);  
            std::cout << "Picked up the: " << col(it->colour,it->backgroundColour)<< it->itemTitle << col() << std::endl;

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

void listItems(int pX, int pY,bool viewInvisible = false){
std::cout << "\033[38;5;203m" <<"--------Items in room--------\n" <<col() ;
 for (const auto& item : roomItems[pY][pX]) {
    if(item.visible && !viewInvisible){
            if (item.itemTitle =="Campfire" && usedMatch){
                std::cout << "    - " << item.itemTitle << ":\033[5m " // -->
                  <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<<"\033[0m" << std::endl;
                std::cout<<"blinking\n";
            }else{
                std::cout << "  - " << item.itemTitle << ": " <<col(item.colour,item.backgroundColour)<< item.itemDesc <<col()<< std::endl;

            }
        }else{
            // std::cout << "  - " << item.itemTitle << ": " << item.itemDesc << std::endl;

        }
    }
}


// Display inventory
void viewInventory() {
    if (inventory.empty()) {
        std::cout << "Empty" << std::endl;
    } else {
        for (const auto& item : inventory) {
            std::cout << "  - " <<col(item.colour,item.backgroundColour)<< item.itemTitle << ": " << item.itemDesc << std::endl;
            std::cout << col();
        }
    }
}

void viewBook(){
// std::cout <<
}
/*
bool lightCampfire(){

    for (auto i : inventory){
        if (i.itemTitle == "match"){
        campfire.backgroundColour = colours::red;
       }
    }

    return true ;
}
*/
 bool useItem(std::string itemName, bool& effect, int pX, int pY) {
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
    if(lowercase(itemName) == "match" && pX == 0 && pY == 1) {
       for(auto& item : roomItems[pY][pX]) {
            if(lowercase(item.itemTitle) == "campfire"&& usedMatch == false) {
                usedMatch = true;
                item.backgroundColour = colours::red;
                item.colour = colours::yellow;
                effect = true;
                std::cout << "You light the campfire with the match. The room fills with warmth and light." << std::endl;
                return true;

                
            }
    }
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