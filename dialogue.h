#include <string>
#include <fstream>  // for file interactions
#include <limits>   // used for max()
#include "vars.h"
#define storyFile "text.txt"

#ifndef VARS_H

 #define normaltext
// #define quicktext

#ifdef normaltext
#define descDelay 0.0025   
#define typeW_delay 0.015  
#define txtDelay 0.45       
#endif

#ifdef quicktext
#define descDelay 0.00025    // default 0.0025
#define typeW_delay 0.0005  // 0.015
#define txtDelay 0.15       // 0.065
#endif

#endif

enum colours{
    Default,    black,    red,    green,    yellow,
    blue,    magenta,    cyan,    white,    RESET
};


std::string col(colours forColour  , colours backColour );          // func definition


std::string processEscapes(const std::string& str) {
    /* processes escape codes from raw text taken from file
       file reading only gives character by character output
       so will read '\' then 'n' instead of '\n' as one */
    std::string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            // Check what comes after the backslash
            if (str[i + 1] == 'n') {                                // NEWLINE
                result += '\n';  // Convert \n to newline
                i++;  // Skip the 'n'
            } else if (str[i + 1] == 't') {                         // TAB
                result += '\t';  // Convert \t to tab
                i++;  // Skip the 't'
            } else if (str[i + 1] == '\\') {                        // '\'
                result += '\\';  // Convert \\ to single backslash
                i++;  // Skip the second backslash


            } else if (str[i + 1] == '#' && str[i+2] == 'R') {      // single word RED
                result +="\e[31m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'r') {      // RED continues
                result +="\e[31m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'G') {      // single word GREEN
                result +="\e[32m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'Y') {      // single word YELLOW
                result +="\e[33m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'B') {      // single word BLUE
                result +="\e[34m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'C') {      // single word CYAN
                result +="\e[36m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'M') {      // single word MAGENTA
                result +="\e[35m";      i+=3;
                while(i < str.length() && str[i] != ' '){
                    result += str[i];       i++;}       result += "\e[0m";
                i--;
            } else if (str[i + 1] == '#' && str[i+2] == 'O') {      // single word BLACK
                result +="\e[30m";      i+=3;
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

/* --------------------GETTING TEXT FROM .TXT FILE-------------------- */

// must be used with std::cout 
std::string output(const std::string textTitle,colours forColour  = colours::Default, colours backColour = colours::Default, bool preserveWhitespace = false){
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
        // looks fo the delimiter 
        size_t delimPosition = line.find('|');

        // if delmiter found then execute, but if not found goes to next line
        if (delimPosition != std::string::npos)
        {
            // extract text before the delimiter
            std::string title = line.substr(0, delimPosition);

            title.erase(0, title.find_first_not_of(" \t")); // remove leading whitespace
            title.erase(title.find_last_not_of(" \t") + 1 /*, to end of line by defualt*/); // remove trailing whitespace

            if (title == textTitle){
                // extract the text after the delimiter
                std::string content = line.substr(delimPosition + 1);
                if(!preserveWhitespace){
                    content.erase(0, content.find_first_not_of(" \t"));
                    content.erase(content.find_last_not_of(" \t") + 1);
                }
                // Closes the textFile and returns the message


/* --------------------COLOUR FOR TEXT-------------------- */
                if (forColour != colours::Default || backColour != colours::Default){
                    std::string output="";
                    output += col(forColour,backColour);
                    output += content;
                    output += "\033[0m";

                    output = processEscapes(output);
                    textFile.close();
                    return output;
                } else {
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

/**- col( ) sets FG and BG colour to Default
 * 
 * - col( colours::RESET ) resets all formatting
 */
std::string col(colours forColour  = colours::Default, colours backColour = colours::Default ){
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

void typeWrite(std::string textTitle, colours forcolour = colours::Default, double delay = typeW_delay)
{
    std::string text = output(textTitle,forcolour);
    /*      when using ansi escape codes the characters are placed at the front of the string
    therefore what ouputs as "text" is actually "\033[39mtext".
    theses characters are processed in the for loop which causes a noticable delay before
    the first character is actually outputted.
    output() will always lead a string with "\033[39m\033[49m" or the corresponding colour val
    this is 16 characters long, so if the first two characters are '\0' then assume the whole
    code is being used and immediatley start printing from the 17th [index 16] character*/

    // if string is a title from text.txt

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
                        timeDelay(txtDelay);
                        i++;\
                    continue;}}
                std::cout << text[i];
                timeDelay(delay);
            }
        }
        // if string is just text to be outputted
        else {
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

/* -----------------------DISPLAY GRAPHICS----------------------- */
void showEntity(std::string faceType = "Default"){
    if(faceType == "Default"){
        for (int lineNum = 1; lineNum<17;lineNum++){
        std::string face = "face["+std::to_string(lineNum) +"]";
            std::string line = output(face, colours::red, colours::Default,true);
            std::cout << line << std::endl;
            }
        }
    if(faceType =="angry"){
        for (int lineNum = 1; lineNum<14; lineNum++){
            std::string face = "face_angry["+std::to_string(lineNum) +"]";
            std::string line = output(face, colours::red, colours::Default,true);
            std::cout << line << std::endl;
            }

    }
}
void showWinScreens(int ending){
    if(ending == 1){ //good ending
        for (int lineNum = 1; lineNum<31; lineNum++){
        std::string win = "win_screen_good["+std::to_string(lineNum) +"]";
            std::string line = output(win);
            std::cout << line << std::endl;
        }
    }
    if(ending == 2){ //bad ending
        for (int lineNum = 1; lineNum<31; lineNum++){
        std::string win = "win_screen_bad["+std::to_string(lineNum) +"]";
            std::string line = output(win);
            std::cout << line << std::endl;
        }
    }
}
void showKey(){
    for (int lineNum = 1; lineNum <= 9; lineNum++){
        std::string key = "key["+std::to_string(lineNum) +"]";
        std::string line = output(key, colours::yellow, colours::Default,true);
        std::cout << line << std::endl;
    }
} 




void entityInteraction(){
    /* covers the first interaction with the 'entity' in the cells room*/

    typeWrite("You notice a \033[33mglint\033[36m out of the corner of your eye\n", colours::cyan); timeDelay(.5);      // 33: yellow fg, 36: cyan fg
    //  std::cout <<col(colours::blue)<< " do you want to \033[41;39mINSPECT\033[49;9m [yes]\033[0m/no]\n"<<col();

    std::cout << col(colours::cyan)<< " do you want to INSPECT "<< col() <<"[yes/no]\n\n" << col();
    std::cout << "Enter command:\033[31m"<<col(colours::black);                            
    typeWrite("YES", colours::red, 0.1);
    std::cout <<"\033[?25l" << col(colours::black);                                             // ?25l: hides cursor
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    typeWrite("towards_note", colours::red);
    clearScreen();

    showEntity();

    for(int lineNum = 1; lineNum<= 5; lineNum++){
        //loops through the first_interaction lines
        std::string interactText = "first_interaction["+std::to_string(lineNum)+"]";
        std::cout << "\e[3m";                                                                   // 3: italics
        typeWrite(interactText, colours::red);
    }
    std::cout << std::endl;
    std::cout << col(colours::cyan)<<"Do you choose to accept?"<<col(colours::RESET)<< std::endl;

    std::cout << "Enter command:" <<col(colours::cyan) << "[yes/no]"<<col(colours::RESET);                            // ?25l: hides cursor
    std::cout << "\rEnter command:" <<col(colours::cyan) << "\033[7mREMEMBER WHAT YOU WERE TOLD \033[?25l"<<col(colours::RESET);    // \r
    timeDelay(2);
    std::string entAnswer ="";

    while(lowercase(entAnswer) !="no"){
        //          \r goes back to start of line wihtout new line so text gets replaced                                   hence the large blank space ↓ to overwrite 
        std::cout << "\rEnter choice:" <<col(colours::black,colours::red) << "yes\033[36;49m/"<<col(colours::cyan,colours::white)<<"NO\033[0m                             ";
        
        if(lowercase(entAnswer) != "no" || lowercase(entAnswer) != "yes"){
            std::cout<<col(colours::green) << "\033[?25h" << "\n-----> "<<col();
            std::getline(std::cin,entAnswer);
            if(lowercase(entAnswer) == "no" || lowercase(entAnswer) == "yes"){
                break;
            }
            std::cout << "\033[1A\033[2K\033[1A";  // 1A: move cursor up 1 line, 2k:delete whole line
        }
    }
    
    if (lowercase(entAnswer) == "yes"){
        isGood = false;
        clearScreen();

        showEntity();
        std::cout << "\033[3m";
        typeWrite("first_interaction_bad");
        

    } else if(lowercase(entAnswer) == "no"){
        isGood = true;
        clearScreen();

        showEntity("angry");
        std::cout << "\033[3m";
        typeWrite("first_interaction_good");
        std::cout<< std::endl << output("view_invis_good",colours::cyan,colours::white);

    } else {
        std::cout << "you shouldnt see this";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    clearScreen();
}

void sentryInteraction(){
    std::string sentryAns = "";
    if(!talkedSentry){
        typeWrite("sentry_greeting");
            talkedSentry = true;
    }
    /*  
    bad reputation (accepted entity) -----> easy question
    good reputation (denied entity)  -----> hard question     (requires finding book)
    */
    switch (isGood){
        case true:
            typeWrite("Your Question is as follows:\n\n",colours::magenta);
            typeWrite("sentry_hard_question");
        break;

        case false:
            typeWrite("Your Question is as follows:\n\n",colours::magenta);
            std::cout<< "\033[4m"<<output("sentry_easy_warning", colours::Default, colours::magenta);
            typeWrite("sentry_easy_question");
            timeDelay(0.5);
            std::cout << col(colours::red) << "\n\n...you hear a whisper in your mind...\n\033[3m" << col(colours::red);
            std::cout << "\"My strength is limited here so i can only help you so much: \n " <<
                         "your answer can be found within the " << col(colours::black, colours::red) << "\"Hallway\"\n";
            std::cout << col(colours::red) << "\"Now remember your promise to me...\"\n" << col(colours::RESET);
            timeDelay(1.0);
        break;  
    }

    typeWrite("What is your answer: ", colours::green);
    std::cin >> sentryAns;
    // std::cin.ignore(); //this one clears the '\n' thats left over from std::cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if(isGood && lowercase(sentryAns) == sentryHardAns){   // hard answer [serpent]
        
        for(int lineNum = 1; lineNum <= 4; lineNum++){
            std::string line = "sentry_hard_correct[" +std::to_string(lineNum)+ "]";
            typeWrite(line, colours::magenta,0.025);
            std::cout << std::endl;            
        }
        showKey();
        
        
        GAME_LOOP_WON = true;
        
    } else if(!isGood && lowercase(sentryAns) == sentryEasyAns){  // easy answer [raven]
        
        
        for(int lineNum = 0; lineNum <= 4; lineNum++){
            std::string line = "sentry_easy_correct[" +std::to_string(lineNum)+ "]";
            typeWrite(line, colours::magenta,0.025);
            std::cout << std::endl;
            timeDelay(0.5);
        }
        showKey();


        GAME_LOOP_WON = true;

    } else if(lowercase(sentryAns) == "back" || lowercase(sentryAns) == "cancel"){
        guessesRemaining +=4;           //removes guessesRemaing from the possible range of the switch case statement as no guess made

    } else {        
        std::cout <<col(colours::magenta); 
                                /* ---------------------------------- Incorrect guess---------------------------------- */
        switch (guessesRemaining){
            case 3:
            std::cout << "unfortunatley not, and seeing as i am in no place to be infinitley benevolent,\n this is the first of three chances i give to you.\n";
            std::cout << col(colours::Default,colours::magenta) <<"Talk to me again when you know your next answer\n"<< col();
            guessesRemaining -= 1;
            break;
            case 2:
            std::cout << "Wrong again. Your chances dwindle, this is your second strike,\n One opportunity remains.\nSome things can be revealed by Higlighting\n";        
            std::cout << col(colours::Default,colours::magenta) <<"Talk to me again when you know your next answer\n"<< col();
            guessesRemaining -= 1;
                break;
            case 1:
            std::cout << "thrice you have failed, your chances are spent\n";
            
            std::cout<< "\033[38;5;245mPress Enter to continue...";                   // grey colour from table 
            
            GAME_LOOP_END = true; 
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    guessesRemaining -= 4;              // Reverts guessesRemaing back to its value
}



