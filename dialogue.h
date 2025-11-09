#include <string>
#include <fstream>
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

std::string display(const std::string &textTitle,colours forColour  = colours::Default, colours backColour = colours::Default)
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
        size_t delimPos = line.find('|');

        // if delmiter found then execute, but if not found, skip the code, close the file, and return end message
        if (delimPos != std::string::npos)
        {
            // extract the text before the delimiter
            std::string key = line.substr(0, delimPos);

            key.erase(0, key.find_first_not_of(" \t")); // remove leading whitespace
            key.erase(key.find_last_not_of(" \t") + 1); // remove trailing whitespace

            // check if the section key is the 'wanted' one inputted as an argument
            if (key == textTitle)
            {
                // extract the text after the delimiter
                std::string content = line.substr(delimPos + 1);
                content.erase(0, content.find_first_not_of(" \t"));
                content.erase(content.find_last_not_of(" \t") + 1);

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

void typeWrite(std::string textTitle, colours forcolour = colours::Default, double delay = 0.035)
{
    std::string text = display(textTitle,forcolour);
    if (text[0] == '\\' && text[1]=='0')
    { // becuase \ is an escape code, two are needed to check for a single "\"
        for (int i = 16; i <= text.length(); i++)
        {
            std::cout << text[i];
            timeDelay(delay);
        }
    }
    else{
        for (int i = 0; i <= text.length(); i++)
        {
            std::cout << text[i];
            timeDelay(delay);
        }
    }
}