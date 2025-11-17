#include <string>
#include <fstream>
// #include <cstdlib>
#include <limits>
#define storyFile "text.txt"

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


std::string col(colours forColour  , colours backColour );

/*
   ---------- getting  \n \t and \ for escape codes instead of being interpreted as individual chars----------
*/
std::string processEscapes(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            // Check what comes after the backslash
            if (str[i + 1] == 'n') {
                result += '\n';  // Convert \n to newline
                i++;  // Skip the 'n'
            } else if (str[i + 1] == 't') {
                result += '\t';  // Convert \t to tab
                i++;  // Skip the 't'
            } else if (str[i + 1] == '\\') {
                result += '\\';  // Convert \\ to single backslash
                i++;  // Skip the second backslash

            } else {
                result += str[i];  // Keep the backslash if not recognized
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

/*
    --------------------GETTING TEXT FROM .TXT FILE--------------------
*/

std::string output(const std::string &textTitle,colours forColour  = colours::Default, colours backColour = colours::Default, bool preserveWhitespace = false)
/*
    returns string instead of directly outputting 
    allows use of ANSI escape codes 
*/
{   
    
    // open the text file
    std::ifstream textFile("text.txt");

    if (!textFile.is_open())
    {
        std::cout << "Error: Could not open textFile" << std::endl;
        return "There was a problem retrieving this text";
    }

    std::string line;
    while (std::getline(textFile, line))
    {
        // checks if the line has the delimiter
        size_t delimPosition = line.find('|');

        // if delmiter found then execute, but if not found, skip the code, close the file, and return end message
        if (delimPosition != std::string::npos)
        {
            // extract the text before the delimiter
            std::string key = line.substr(0, delimPosition);

            key.erase(0, key.find_first_not_of(" \t")); // remove leading whitespace
            key.erase(key.find_last_not_of(" \t") + 1); // remove trailing whitespace

            // check if the section key is the 'wanted' one inputted as an argument
            if (key == textTitle)
            {
                // extract the text after the delimiter
                std::string content = line.substr(delimPosition + 1);
                if(!preserveWhitespace){
                    content.erase(0, content.find_first_not_of(" \t"));
                    content.erase(content.find_last_not_of(" \t") + 1);
                }
                // Closes the textFile and returns the message
                

/*
 --------------------COLOUR FOR TEXT--------------------
*/  
                if (static_cast<int> (forColour) || static_cast<int>(backColour)){

                    
                    std::string output="";
                    output += col(forColour,backColour);
                    output += content;
                    output += "\033[0m";

                    output = processEscapes(output);
                    textFile.close();
                    return output;
            
                }else{
                    content = processEscapes(content);

                    textFile.close();
                    return content;
                }
          
        }
    }

   
    
}
textFile.close();
return "";
}

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



//CHANGE DEFUALT SPEED TO 0.035
void typeWrite(std::string textTitle, colours forcolour = colours::Default, double delay = 0.01)
{
    //if string is a title from text.txt
    std::string text = output(textTitle,forcolour);
        /*      when using ansi escape codes the characters are placed at the front of the string
            therefore what ouputs as "text" is actually "\033[39mtext".
             theses characters are processed in the for loop which causes a noticable delay before 
            the first character is actually outputted. 
             output() will always lead a string with "\033[39m\033[49m" or the corresponding colour val
            this is 16 characters long, so if the first two characters are '\0' then assume the whole
            code is being used and immediatley start printing from the 17th [index 16] character*/

      if (text[0] == '\\' && text[1]=='0')
        { // becuase \ is an escape code, two are needed to check for a single "\"
            for (int i = 16; i < text.length(); i++)
            {
                if(text[i] == '\\' && i+1 < text.length() && text[i+1] == 'd'){
                 
                        timeDelay(0.65);
                        i++;
                    continue;}
                std::cout << text[i];
                timeDelay(delay);
                    }
            }
        
        else if (!text.empty()){
            for (int i = 0; i < text.length(); i++)
            {
                if(text[i] == '\\'){
                    if (text[i+1] == 'd' && i+1 < text.length()){
                        timeDelay(0.65);
                        i++;
                    continue;}}
                std::cout << text[i];
                timeDelay(delay);
                
            }
        }
        // if string is just text to be outputted
        else{
            text =textTitle;
            std::cout<<col(forcolour);
            for (int i = 0; i < text.length(); i++){
                std::cout<<text[i];
                timeDelay(delay);
            }
            std::cout<<col();
        }
}

void outputface(std::string faceType = "Default"){
    if(faceType == "Default"){    
        for (int i = 1; i<17;i++){
        std::string f = "face["+std::to_string(i) +"]";    
            std::string line = output(f, colours::red, colours::Default,true);  
            std::cout << line << std::endl;      
            }
        }
    if(faceType =="angry"){
        for (int i = 1; i<14; i++){
            std::string f = "face_angry["+std::to_string(i) +"]";    
            std::string line = output(f, colours::red, colours::Default,true);  
            std::cout << line << std::endl;      
            }

    }    
}

void cursedNote(){
    typeWrite("You notice something out the corner of your eye\n", colours::cyan);
    std::cout <<col(colours::blue)<< " do you want to \033[41;39mINSPECT\033[49;9m [yes]\033[0m/no]\n"<<col();
    std::string inspectNote;
    // while(true){
    int round = 1;
    getline(std::cin, inspectNote);
    lowercase(inspectNote, true);
    if (inspectNote == "yes"){
        typeWrite("note_observation1",colours::cyan);
        round = 2;
    }
    else{
        clearScreen();
        typeWrite("\033[9;31mWRONG\033[0m");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        outputface();
        timeDelay(0.15);
        clearScreen();
        typeWrite("note_observation2");
        typeWrite("Do you want to look? [yes/no] ");
        getline(std::cin, inspectNote);
        lowercase(inspectNote,true);
        round = 2;
        if(inspectNote =="yes"){
            typeWrite("note_content");
        }else{
                switch (round){
                case 1: 
                    typeWrite("\033[9;31mWRONG\033[0m");
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    outputface();
                    timeDelay(0.15);
                    clearScreen();
                    // typeWrite("note_content");
                    break;
                case 2:
                    typeWrite("\033[9;31mWRONG AGAIN");
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    outputface("angry");
                    timeDelay(0.45);
                    clearScreen();
                    typeWrite("etched into the wall by a sharp object are a set of broken, jagged lines\n they are barley readable yet you manage to make out some parts:\n\n");

                    typeWrite("note_content",colours::Default,0.07);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
            }
    }
    // typeWrite("note_content",colours::Default,0.07);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

