#include <string>
#include <vector>

const int mapwidth = 4; //X
const int mapheight =3;  //Y

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

item campfire = {"Campfire", "campfireDesc", false, true, colours::red};
item testItem = {"test item", "desc", true, true, colours::blue};
item key =      {"Rusty Key", "rusty key desc", true, true, colours::yellow};
item cleaver =  {"cleaver", "cleaverdesc", true, true, colours::red};
item book =     {"book", "old Book desc", true, false, colours::black, colours::white};

void generateItems()
{   //roomItems[Y][X]
    
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
        if (lowerCase(it->itemTitle) == lowerCase(itemName) && it->canPickup) { //  -> is used to get value of struct 'feature' becuase auto i is a pointer to roomItems[pY][pX] 
                //add item vec to inventory 
            inventory.push_back(*it);    //  *i derefrences i, refrencing back to roomItems[pY][pX]?
                //remove item from room
            roomItems[pY][pX].erase(it);  
            std::cout << "Picked up the: " << col(it->colour,it->backgroundColour)<< it->itemTitle << col() << std::endl;

            return true;
        }else if (lowerCase(it->itemTitle) == lowerCase(itemName) && (it->canPickup == false))
        {
                std::cout << "That item can't be picked up." << std::endl;
                return false;  
        }
        
    }
    std::cout << "Item doesn't exist." << std::endl;
    return false;
}

void listItems(int pX, int pY,bool viewInvisible = false){

 for (const auto& item : roomItems[pY][pX]) {
    if(item.visible && !viewInvisible){
            std::cout << "  - " << item.itemTitle << ": " << item.itemDesc << std::endl;
        }else{
            std::cout << "  - " << item.itemTitle << ": " << item.itemDesc << std::endl;

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


