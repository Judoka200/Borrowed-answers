#include <string>
#include <vector>

const int mapwidth = 4; //X
const int mapheight =3;  //Y

struct item{
    std::string itemTitle;
    std::string itemDesc;
    bool canPickup;
    bool visible;

};

//creates a vector of each room which holds a vector of items 
std::vector<item> roomItems[mapheight][mapwidth];
std::vector<item> inventory;

item campfire = {"Campfire", "campfireDesc", false,true};
item testItem = {"test item","desc", true, true};
item key = {"Rusty Key", "rusty key desc", true, true};
item cleaver = {"cleaver", "cleaverdesc", true, true};
item book = {"Old book", "old Book desc", true, false};
void generateItems()
{
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
    
    std::cout << "Looking for: '" << itemName << "'" << std::endl;
    std::cout << "Items in room: " << roomItems[pY][pX].size() << std::endl;
    std::cout <<"curernt room"<<pX<<" "<<pY<<std::endl;

    /*
        https://www.w3schools.com/cpp/cpp_iterators.asp 
    */
    for (auto it = roomItems[pY][pX].begin(); it != roomItems[pY][pX].end(); ++it) {
                std::cout << "Checking: '" << it->itemTitle << "' canPickup=" << it->canPickup << std::endl;
        if (it->itemTitle == itemName && it->canPickup) { //  -> is used to get value of struct 'feature' becuase auto i is a pointer to roomItems[pY][pX] 
                //add item vec to inventory 
            inventory.push_back(*it);    //  *i derefrences i, refrencing back to roomItems[pY][pX]?
                //remove item from room
            roomItems[pY][pX].erase(it);  
            std::cout << "Picked up: " << itemName << std::endl;
            return true;
        }
    }
    std::cout << "Can't pick up that item." << std::endl;
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

void viewInventory()
{
    // for(auto i = inventory.begin(); i != inventory.end(); i++)
    for(auto i : inventory)
        {
            std::string name = i.itemTitle;
            
        }

}

// Display inventory
void displayInventory() {
    if (inventory.empty()) {
        std::cout << "Empty" << std::endl;
    } else {
        for (const auto& item : inventory) {
            std::cout << "  - " << item.itemTitle << ": " << item.itemDesc << std::endl;
        }
    }
}