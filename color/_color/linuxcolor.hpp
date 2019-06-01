#pragma once
#ifndef JSHL_LINUXCOLOR_HPP
#define JSHL_LINUXCOLOR_HPP

#include <regex>
#include <string>
#include <iostream>
#include <map>
#include <exception>

// map from color words to ANSI color code
static const std::map<std::string, int> color_dict = {
 {"black",  30},
 {"red",    31},
 {"green",  32},
 {"yellow", 33},
 {"blue",   34},
 {"purple", 35}, {"megenta", 35},
 {"aqua",   36}, {"cyan",    36},
 {"white",  37},
 {"gray",   90}, {"bblack",  90},
 {"lred",   91}, {"bred",    91},
 {"lgreen", 92}, {"bgreen",  92},
 {"lyellow",93}, {"byellow", 93},
 {"lblue",  94}, {"bblue",   94},
 {"lpurple",95}, {"bmegenta",95},
 {"laqua",  96}, {"bcyan",   96},
 {"bwhite", 97}
};

static const char* _color_name_regex = R"((b?black|[lb]?blue|[lb]?green|l?aqua|b?cyan|[lb]?red|l?purple|b?megenta|[lb]?yellow|b?white|gray)[;:,\. \t]*(b?black|[lb]?blue|[lb]?green|l?aqua|b?cyan|[lb]?red|l?purple|b?megenta|[lb]?yellow|b?white|gray)?)";

static const char* _color_code_regex = R"(((3|4|9|10)[0-7])[;:,\. \t]((3|4|9|10)[0-7])?)";

static const char* _color_rgb_regex = R"((\d{1,3})[;:,\. \t](\d{1,3})[;:,\. \t](\d{1,3}))";

static const std::regex name_regex(_color_name_regex, std::regex::icase);
static const std::regex code_regex(_color_code_regex, std::regex::icase);
static const std::regex  rgb_regex(_color_rgb_regex, std::regex::icase);

static std::string _current_color = "\033[0m";

const char* get_current_color(){
    return _current_color.c_str();
}

// set color with code
// color code is defined as background-color-code * 1000 + foreground-color-code
int setcolor(int color_code){
    std::string color="\033[";
    if(color_code < 1000)
    {
        color += std::to_string(color_code) + ";40m";
    }
    else
    {
        color = color + std::to_string(color_code % 1000) + ";" + std::to_string(color_code / 1000) + "m";
    }
    _current_color = color;
    std::cout << color << std::flush;
    return 1;
}

std::string lower(std::string s){
    const char step = 'a' - 'A';
    for(auto &c : s) if(c >= 'A' && c <= 'Z') c += step;
    return s;
}

int str_to_code(const std::string &str){
    if(str.empty()) return -1;
    std::smatch sm;
    std::regex_search(str, sm, name_regex);
    if(!sm.empty())
    {
        const std::string color_one = lower(sm.str(1));
        const std::string color_two = lower(sm.str(2));
        if(color_two == "") return color_dict.at(color_one);
        else return (color_dict.at(color_one) + 10) * 1000 + color_dict.at(color_two);
    }
    return -1;
}

int set_color_with_words(const std::string &color){
    if(color[0] == '\033')
    {
        std::cout << color << std::flush;
        _current_color = color;
        return 1;
    }
    std::smatch sm;
    std::regex_search(color, sm, code_regex);
    if(!sm.empty())
    {
        std::string ansi_code = "\033[";
        ansi_code += sm.str(1);
        if(sm.str(3) != "") ansi_code += ";" + sm.str(3) + "m";
        else ansi_code += ";40m";
        _current_color = ansi_code;
        std::cout << ansi_code << std::flush;
        return 1;
    }
    else
    {
        std::regex_search(color, sm, rgb_regex);
        if(!sm.empty())
        {
            std::string rgb_color = "\033[38;2;";
            rgb_color += sm.str(1) + ";" + sm.str(2) + ";" + sm.str(3) + "m";
            _current_color = rgb_color;
            std::cout << rgb_color << std::flush;
            return 1;
        }
        else
        {
            int color_code = str_to_code(color);
            if(color_code == -1) return 0;
            return setcolor(color_code);
        }
    }
}

#endif // JSHL_LINUXCOLOR_HPP