#include <string>
#include <fstream>

#define storyFile "text.txt"

void outputDialogue()
{
    std::ifstream text_file;
    text_file.open(storyFile);
    if(text_file.is_open()){

    std::string desc;
    std::getline(text_file,desc); 
    std::cout<<desc;
    
    }else{
        std::cout<<"ERROR OPENING "<< storyFile<<std::endl;
    }


}




std::string output(const std::string& textTitle) {
    // 1. Opens the file using ifstream
    std::ifstream file("text.txt");
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        return "";  // Return empty string on error
    }
    
    std::string line;
    // 2. Reads the file line by line with getline()
    while (std::getline(file, line)) {
        // 3. For each line, finds the | delimiter using find()
        size_t delimPos = line.find('|');
        // std::cout<<std::endl<<delimPos<<std::endl;

        if (delimPos != std::string::npos) {
            // 4. Extracts the text before | (the key) and trims whitespace
            std::string key = line.substr(0, delimPos);
            
            // Remove leading/trailing whitespace from key
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            
            // 5. Compares the key with textTitle
            if (key == textTitle) {
                // 6. If matched, extracts and returns the text after |
                std::string content = line.substr(delimPos + 1);
                content.erase(0, content.find_first_not_of(" \t"));
                content.erase(content.find_last_not_of(" \t") + 1);
                
                // 7. Closes the file when done
                file.close();
                return content;  // Return the content instead of printing
            }
        }
    }
    
    // 7. Closes the file when done
    file.close();
    return "";  // Return empty string if not found
}





enum class colours{
    Default,
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white  
};



std::string col(colours forColour  = colours::Default, colours backColour = colours::Default )
{
/*
    to get integer value use  static_cast<int>(colours:'col')
*/
std::string output ="";

switch (forColour){

    case colours::Default:  output += "\033[39m"; break;
    case colours::black:    output += "\033[30m"; break;
    case colours::red:      output += "\033[31m"; break;
    case colours::green:    output += "\033[32m"; break;
    case colours::yellow:   output += "\033[33m"; break;
    case colours::blue:     output += "\033[34m"; break;
    case colours::magenta:  output += "\033[35m"; break;
    case colours::cyan:     output += "\033[36m"; break;
    case colours::white:    output += "\033[37m"; break;
default:
    break;
}

switch (backColour){
    case colours::Default:  output += "\033[49m"; break;
    case colours::black:    output += "\033[40m"; break;
    case colours::red:      output += "\033[41m"; break;
    case colours::green:    output += "\033[42m"; break;
    case colours::yellow:   output += "\033[43m"; break;
    case colours::blue:     output += "\033[44m"; break;
    case colours::magenta:  output += "\033[45m"; break;
    case colours::cyan:     output += "\033[46m"; break;
    case colours::white:    output += "\033[47m"; break;

default:
    break;
}



return output;
}   

