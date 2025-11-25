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
    white,
    RESET
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


            } else if (str[i + 1] == '#' && str[i+2] == 'R') {
                result +="\e[31m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'G') {
                result +="\e[32m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'Y') {
                result +="\e[33m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'B') {
                result +="\e[34m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'C') {
                result +="\e[36m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;

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
    case colours::RESET:    output += " \033[0m"; break;
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
    case colours::RESET:    output += " \033[0m"; break;

default:
    break;
}



return output;
}

// void col(bool reset = true){
//     cout <<"\033[0m";
// }

//CHANGE DEFUALT SPEED TO 0.035
void typeWrite(std::string textTitle, colours forcolour = colours::Default, double delay = 0.01)
{
    std::string text = output(textTitle,forcolour);
    /*      when using ansi escape codes the characters are placed at the front of the string
    therefore what ouputs as "text" is actually "\033[39mtext".
    theses characters are processed in the for loop which causes a noticable delay before
    the first character is actually outputted.
    output() will always lead a string with "\033[39m\033[49m" or the corresponding colour val
    this is 16 characters long, so if the first two characters are '\0' then assume the whole
    code is being used and immediatley start printing from the 17th [index 16] character*/

    //if string is a title from text.txt

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
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     //UNCOMMENT TO DISCARD INPUT WHEN TYPING
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






void entityInteraction(){
    typeWrite("You notice a \033[33mglint\033[36m out of the corner of your eye\n", colours::cyan); timeDelay(.5);      // 33: yellow fg, 36: cyan fg
    // std::cout <<col(colours::blue)<< " do you want to \033[41;39mINSPECT\033[49;9m [yes]\033[0m/no]\n"<<col();

    std::cout << col(colours::cyan)<< " do you want to INSPECT "<< col(colours::RESET) <<"yes]/no]\n\n" << col();
    std::cout << "Enter command:\033[31m YES \033[?25l"<<col(colours::black)<<std::endl;                            // ?25l: hides cursor
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    typeWrite("towards_note", colours::red);
    system("cls");

    outputface();

    for(int i = 1; i<= 4; i++){
        // std::cout << "\n\n";
        std::string interactText = "first_interaction["+std::to_string(i)+"]";
        typeWrite(interactText, colours::red);
    }
    std::cout << std::endl;
    std::cout << col(colours::cyan)<<"Do you choose to accept?"<<col(colours::RESET)<<" [yes/no]\n";

    std::cout << "Enter command:" <<col(colours::cyan) << "\033[7mREMEMBER WHAT YOU WERE TOLD \033[?25l"<<col(colours::RESET);                            // ?25l: hides cursor
    timeDelay(1);
//          \r goes back to start of line wihtout new line so text gets replaces            hence the large blank space ↓ to overwrite 
    std::string entAnswer ="";
    while(lowercase(entAnswer) !="no"){
        std::cout << "\rEnter command:" <<col(colours::cyan) << "yes/"<<col(colours::cyan,colours::white)<<"NO\033[0m                             "<<std::endl;
        std::getline(std::cin,entAnswer);

        if(lowercase(entAnswer) != "no"){
            std::cout<<col(colours::cyan, colours::white) << "ge";
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    system("cls");
}

