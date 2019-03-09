#include <windows.h>
#include <Python.h> // make sure this file can be find
#include <regex>
#include <string>
#include <iostream>
#include <map>
#include <exception>

// map from color words to windows api color code
static const std::map<std::string, int> color_dict = {
 {"black", 0},  {"blue",    1},  {"green",   2},  {"aqua",   3},
 {"red",   4},  {"purple",  5},  {"yellow",  6},  {"white",  7},
 {"gray",  8},  {"lblue",   9},  {"lgreen",  10}, {"laqua",  11},
 {"lred",  12}, {"lpurple", 13}, {"lyellow", 14}, {"bwhite", 15}
};

static const char* _color_regex_str = R"((Black|Blue|Green|Aqua|Red|Purple|Yellow|White|Gray|LBlue|LGreen|LAqua|LRed|LPurple|LYellow|BWhite)[,;:.?!<>|\\/@#$%\^\&\*-\+= \t]*(Black|Blue|Green|Aqua|Red|Purple|Yellow|White|Gray|LBlue|LGreen|LAqua|LRed|LPurple|LYellow|BWhite)*)";
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

static PyObject * Py_setcolor(PyObject *self, PyObject *args){
    int color_code;
    if(!(PyArg_ParseTuple(args, "i", &color_code)))
    {
        return nullptr;
    }
    return (PyObject*)Py_BuildValue("i", setcolor(color_code));
}

// emmm..., I can't understand this change of char*, however, it is true
static PyObject * Py_set_color_with_words(PyObject *self, PyObject *args){
    char *color;
    if(!(PyArg_ParseTuple(args, "s", &color)))  // remember the `&`
    {
        return nullptr;
    }
    return (PyObject*)Py_BuildValue("i", set_color_with_words(color));
}

static PyObject * Py_get_current_color(PyObject *self, PyObject *args){
    return (PyObject*) Py_BuildValue("i", get_current_color());
}

static PyMethodDef jshl_Py_methods[] = {
    {"setcolor", Py_setcolor, METH_VARARGS},
    {"set_color_with_words", Py_set_color_with_words, METH_VARARGS},
    {"get_current_color", Py_get_current_color, METH_VARARGS},
    {nullptr, nullptr},
};

static struct PyModuleDef color_module =
{
    PyModuleDef_HEAD_INIT,
    "_color",    // name of module
    "",         // module documentation, may be NULL
    -1,         // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    jshl_Py_methods
};

PyMODINIT_FUNC PyInit__color(void){
    return PyModule_Create(&color_module);
}