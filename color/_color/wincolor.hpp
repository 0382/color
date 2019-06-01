#pragma once
#ifndef JSHL_WINCOLOR_HPP
#define JSHL_WINCOLOR_HPP
#include <windows.h>
#include <regex>
#include <string>
#include <iostream>
#include <map>
#include <exception>

// map from color words to windows api color code
static const std::map<std::string, int> color_dict = {
 {"black",  0},
 {"blue",   1},
 {"green",  2}, 
 {"aqua",   3},  {"cyan",    3},
 {"red",    4},
 {"purple", 5},  {"megenta", 5},
 {"yellow", 6},
 {"white",  7},
 {"gray",   8},  {"bblack",  8},
 {"lblue",  9},  {"bblue",   9},
 {"lgreen", 10}, {"bgreen",  10},
 {"laqua",  11}, {"bcyan",   11},
 {"lred",   12}, {"bred",    12},
 {"lpurple",13}, {"bmegenta",13},
 {"lyellow",14}, {"byellow", 14},
 {"bwhite", 15}
};

//static const char* _color_regex_str = R"((Black|Blue|Green|Aqua|Red|Purple|Yellow|White|Gray|LBlue|LGreen|LAqua|LRed|LPurple|LYellow|BWhite)[,;:.?!<>|\\/@#$%\^\&\*-\+= \t]*(Black|Blue|Green|Aqua|Red|Purple|Yellow|White|Gray|LBlue|LGreen|LAqua|LRed|LPurple|LYellow|BWhite)*)";
static const char* _color_regex_str = R"((b?black|[lb]?blue|[lb]?green|l?aqua|b?cyan|[lb]?red|l?purple|b?megenta|[lb]?yellow|b?white|gray)[;:,\. \t]*(b?black|[lb]?blue|[lb]?green|l?aqua|b?cyan|[lb]?red|l?purple|b?megenta|[lb]?yellow|b?white|gray)?)";
static const std::regex color_regex(_color_regex_str, std::regex::icase);

// set color with windows api
int setcolor(int color_code){
    return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_code);
}

// get current console color setting
int get_current_color(){
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO current_screen_buffer_info;
    GetConsoleScreenBufferInfo(hd, &current_screen_buffer_info);
    return current_screen_buffer_info.wAttributes;
}

std::string lower(std::string s){
    const char step = 'a' - 'A';
    for(auto &c : s) if(c >= 'A' && c <= 'Z') c += step;
    return s;
}

// change string to windows api color code
// return -1 indicates wrong string, 
// in this condition, won't set color
int str_to_code(const std::string &str){
    if(str.empty()) return -1;
    if(str.size()<=2)
    {
        try{
            return std::stoi(str, nullptr, 16);
        } catch (std::exception error) {
            return -1;
        }
    }
    std::smatch sm;
    std::regex_search(str, sm, color_regex);
    if(sm.empty()) return -1;
    else
    {
        const std::string color_one = lower(sm.str(1));
        const std::string color_two = lower(sm.str(2));
        if(color_two == "") return color_dict.at(color_one);
        else return color_dict.at(color_one) << 4 | color_dict.at(color_two);
    }
}

int set_color_with_words(const std::string &color){
    int color_code = str_to_code(color);
    if(color_code == -1) return 0;
    setcolor(color_code);
    return 1;
}

#endif // JSHL_WINCOLOR_HPP