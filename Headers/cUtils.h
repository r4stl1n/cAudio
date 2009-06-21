#ifndef CUTILS_H_INCLUDED
#define CUTILS_H_INCLUDED
#include <string>

//!Grabs the current extention of a given string.
std::string getExt(const std::string& filename){
    if(filename.find_last_of(".") == std::string::npos) return filename;
    return filename.substr(filename.find_last_of(".") + 1, filename.length()-filename.find_last_of(".")-1);
}

#endif //! CUTILS_H_INCLUDED
