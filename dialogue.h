#include <string>
#include <fstream>

#define storyFile "text.txt"

void outputDialogue()
{
    std::ifstream text_file;
    text_file.open(storyFile);
    if(text_file.is_open()){

    std::string desc;
    std::getline(text_file,desc,'|'); 
    std::cout<<desc;
    
    }else{
        std::cout<<"ERROR OPENING "<< storyFile<<std::endl;
    }



}


