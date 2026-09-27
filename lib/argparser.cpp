#include "argparser.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace nargparse{
    int CompareArgs(const char* a, const char* b){
        if (a == nullptr || b == nullptr){return -1;}
        while (*a != '\0' && *a == *b) {
            if (*a != *b){
                return -1;
            }
            a++;
            b++;
        }
        if (*a == *b){return 0;}
        return -1; 
    }

    char IdentityType(const char* a){
        const char* str = a;
        bool f = true;
        bool i = true;
        int point_count = 0;
        while (*str != '\0'){
            if ((*str < '0' or *str > '9') && *str != '-' && *str != '.'){
                f = false;
                i = false;
                return 'c';
            } 
            if (*str == '.'){
                i = false;
                point_count++;
            }
            str++;
        }
        if (i) {
            return 'i';
        }
        if (f && point_count == 1) { 
            return 'f';
        }
        return 'c';
    }

    void AddToArrayDinamically(char** &array_to, char* item, int& item_count, int& arr_len){
        if (item_count < arr_len) {
            array_to[item_count] = new char[128];
            std::strcpy(array_to[item_count], item);
            ++item_count;
        }
        else{
            char** to_delete = array_to;
            array_to = new char*[arr_len * 2];
            for (int i = 0; i < arr_len; i++) {
                array_to[i] = to_delete[i];
            }
            arr_len *= 2;
            delete[] to_delete;
            array_to[item_count] = item;
            ++item_count;
        }
    }

    void AddToArrayDinamically(float* &array_to, float item, int& item_count, int& arr_len){
        if (item_count < arr_len) {
            array_to[item_count] = item;
            ++item_count;
        }
        else{
            float* to_delete = array_to;
            array_to = new float[arr_len * 2];
            for (int i = 0; i < arr_len; i++) {
                array_to[i] = to_delete[i];
            }
            arr_len *= 2;
            delete[] to_delete;
            array_to[item_count] = item;
            ++item_count;
        }
    }

    void AddToArrayDinamically(int* &array_to, int item, int& item_count, int& arr_len){
        if (item_count < arr_len) {
            array_to[item_count] = item;
            ++item_count;
        }
        else{
            int* to_delete = array_to;
            array_to = new int[arr_len * 2];
            for (int i = 0; i < arr_len; i++) {
                array_to[i] = to_delete[i];
            }
            arr_len *= 2;
            delete[] to_delete;
            array_to[item_count] = item;
            ++item_count;
        }
    }

    void IncreaseArray(Argument* &array_to, int& arr_len){
        Argument* to_delete = array_to;
        array_to = new Argument[arr_len * 2];
        for (int i = 0; i < arr_len; i++) {
            array_to[i] = to_delete[i];
        }
        arr_len *= 2;
        delete[] to_delete;
    }

    void IncreaseArray(Flag* &array_to, int& arr_len){
        Flag* to_delete = array_to;
        array_to = new Flag[arr_len * 2];
        for (int i = 0; i < arr_len; i++) {
            array_to[i] = to_delete[i];
        }
        arr_len *= 2;
        delete[] to_delete;

    }

    void AddFlag(ArgumentParser& parser, const char* short_form, const char* full_form, bool* to_change, const char* name, bool by_default, const char* description){
        if (parser.flags_count >= parser.len_flag_arr){
            IncreaseArray(parser.parsing_flags, parser.len_flag_arr);
        }
        parser.parsing_flags[parser.flags_count] = {
            short_form,
            full_form,
            to_change,
            name, 
            by_default,
            description
        };
        *to_change = by_default;
        parser.flags_count++;
    }

    void AddArgument(ArgumentParser& parser, int* to_change, const char* name, char args_type, bool (*check_func)(const int&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_int = new int[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'i';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].check_func_int = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_char;
        delete[] parser.parsing_args[parser.args_count].all_val_float;
        parser.parsing_args[parser.args_count].all_val_char = nullptr;
        parser.parsing_args[parser.args_count].all_val_float = nullptr;
        parser.args_count++;

    }

    void AddArgument(ArgumentParser& parser, float* to_change, const char* name, char args_type , bool (*check_func)(const float&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_float = new float[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'f';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].check_func_float = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_char;
        delete[] parser.parsing_args[parser.args_count].all_val_int;
        parser.parsing_args[parser.args_count].all_val_char = nullptr;
        parser.parsing_args[parser.args_count].all_val_int = nullptr;
        parser.args_count++;
    }

    void AddArgument(ArgumentParser& parser, char (*to_change)[], const char* name, char args_type, bool (*check_func)(const char* const&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_char = new char*[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'c';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].check_func_char = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_int;
        delete[] parser.parsing_args[parser.args_count].all_val_float;
        parser.parsing_args[parser.args_count].all_val_int = nullptr;
        parser.parsing_args[parser.args_count].all_val_float = nullptr;
        parser.args_count++;
    }

    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, int* to_change, const char* name, char args_type , bool (*check_func)(const int&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_int = new int[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'i';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].short_form = short_form;
        parser.parsing_args[parser.args_count].full_form = full_form;
        parser.parsing_args[parser.args_count].check_func_int = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_char;
        delete[] parser.parsing_args[parser.args_count].all_val_float;
        parser.parsing_args[parser.args_count].all_val_char = nullptr;
        parser.parsing_args[parser.args_count].all_val_float = nullptr;
        parser.args_count++;
    }

    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, float* to_change, const char* name, char args_type , bool (*check_func)(const float&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_float = new float[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'f';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].short_form = short_form;
        parser.parsing_args[parser.args_count].full_form = full_form;
        parser.parsing_args[parser.args_count].check_func_float = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_char;
        delete[] parser.parsing_args[parser.args_count].all_val_int;
        parser.parsing_args[parser.args_count].all_val_char = nullptr;
        parser.parsing_args[parser.args_count].all_val_int = nullptr;
        parser.args_count++;
    }

    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, char (*to_change)[], const char* name, char args_type, bool (*check_func)(const char* const&), const char* description){
        if (parser.args_count >= parser.len_arg_arr){
            IncreaseArray(parser.parsing_args, parser.len_arg_arr);
            for (int i = parser.args_count; i < parser.len_arg_arr; i++){
                parser.parsing_args[i].all_val_char = new char*[parser.parsing_args[i].len_val_arr];
            }
        }
        parser.parsing_args[parser.args_count].description = description;
        parser.parsing_args[parser.args_count].name = name;
        parser.parsing_args[parser.args_count].type = 'c';
        parser.parsing_args[parser.args_count].to_change = to_change;
        parser.parsing_args[parser.args_count].req_type = args_type;
        parser.parsing_args[parser.args_count].short_form = short_form;
        parser.parsing_args[parser.args_count].full_form = full_form;
        parser.parsing_args[parser.args_count].check_func_char = check_func;
        delete[] parser.parsing_args[parser.args_count].all_val_int;
        delete[] parser.parsing_args[parser.args_count].all_val_float;
        parser.parsing_args[parser.args_count].all_val_int = nullptr;
        parser.parsing_args[parser.args_count].all_val_float = nullptr;
        parser.args_count++;
    }

    bool GetArgVal(Argument& argument, char* arg) {
        if (argument.type != IdentityType(arg)){
            return false;
        }
        if (argument.all_val_char != nullptr){
            if (argument.check_func_char){
                switch (argument.check_func_char(arg)){
                case true:
                    break;
                case false:
                    return false;
                }}
            switch (argument.req_type)
            {
            case kNargsRequired:
                std::strcpy((char*)argument.to_change, arg); 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOptional:
                std::strcpy((char*)argument.to_change, arg); 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOneOrMore:
                if (argument.values_count == 0){
                    std::strcpy((char*)argument.to_change, arg); 
                }
                AddToArrayDinamically(argument.all_val_char, arg, argument.values_count, argument.len_val_arr);
                break;
            case kNargsZeroOrMore:
                if (argument.values_count == 0){
                    std::strcpy((char*)argument.to_change, arg); 
                }
                AddToArrayDinamically(argument.all_val_char, arg, argument.values_count, argument.len_val_arr);
                break;
            }
        }
        if (argument.all_val_int != nullptr){
            int value = atoi(arg);
            if (argument.check_func_int){
                switch (argument.check_func_int(value)){
                case true:
                    break;
                case false:
                    return false;
                }}
            switch (argument.req_type)
            {
            case kNargsRequired:
                *(int*)argument.to_change = value; 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOptional:
                *(int*)argument.to_change = value; 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOneOrMore:
                if (argument.values_count == 0){
                    *(int*)argument.to_change = value; 
                }
                AddToArrayDinamically(argument.all_val_int, value, argument.values_count, argument.len_val_arr);
                break;
            case kNargsZeroOrMore:
                if (argument.values_count == 0){
                    *(int*)argument.to_change = value; 
                }
                AddToArrayDinamically(argument.all_val_int, value, argument.values_count, argument.len_val_arr);
                break;
            }
        }
        if (argument.all_val_float != nullptr){
            float value = atof(arg);
            if (argument.check_func_float){
                switch (argument.check_func_float(value)){
                case true:
                    break;
                case false:
                    return false;
                }}

            switch (argument.req_type)
            {
            case kNargsRequired:
                *(float*)argument.to_change = value; 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOptional:
                *(float*)argument.to_change = value; 
                argument.req_type = kNargsUsed;
                break;
            case kNargsOneOrMore:
                if (argument.values_count == 0){
                    *(float*)argument.to_change = value; 
                }
                AddToArrayDinamically(argument.all_val_float, value, argument.values_count, argument.len_val_arr);
                break;
            case kNargsZeroOrMore:
                if (argument.values_count == 0){
                    *(float*)argument.to_change = value; 
                }
                AddToArrayDinamically(argument.all_val_float, value, argument.values_count, argument.len_val_arr);
                break;
            }
        }
        return true;
    }

    bool Parse(ArgumentParser& parser,  int args_count, const char* argv[]){
        for (int i = 1; i < args_count; i++){

            if (std::strlen(argv[i]) >= parser.max_arg_len) {return false;}
            char* arg_copy = new char[parser.max_arg_len];
            std::strcpy(arg_copy, argv[i]);
            bool arg_is_processed = false;

            for (int j = 0; j < parser.flags_count; j++){
                if (CompareArgs(arg_copy, parser.parsing_flags[j].short_form) == 0 || 
                    CompareArgs(arg_copy, parser.parsing_flags[j].full_form) == 0){
                        *parser.parsing_flags[j].to_change = true;
                        arg_is_processed = true;
                        break;
                    }
            }
            if (arg_is_processed) {delete[] arg_copy; continue;}

            bool eq_sign = false;
            char* arg = arg_copy;
            char* val = nullptr;

            for (int k = 0;;k++) {
                    if (arg[k] == '='){
                        arg[k] = '\0';
                        val = new char[parser.max_arg_len];
                        val = std::strcpy(val, arg + k + 1);
                        break;
                    }
                    else if (arg[k] == '\0'){
                        break;
                    }
            }
            if (val != nullptr) {
                eq_sign = true;
            }
            else if (i+1 < args_count) {
                if (std::strlen(argv[i+1]) >= parser.max_arg_len) {delete[] arg_copy; return false;}
                val = new char[parser.max_arg_len];
                std::strcpy(val, argv[i+1]);
            }
            for (int j = 0; j < parser.args_count; j++){
                if (CompareArgs(arg, parser.parsing_args[j].short_form) == 0 ||
                    CompareArgs(arg, parser.parsing_args[j].full_form) == 0){
                        if (parser.parsing_args[j].req_type == kNargsUsed) {delete[] arg_copy; delete[] val; return false;}
                        if (val == nullptr) {delete[] arg_copy; return false;}
                        if (!GetArgVal(parser.parsing_args[j], val)) {delete[] arg_copy; delete[] val; return false;}
                        if (!eq_sign){i++;}
                        arg_is_processed = true;
                        break;
                    }
                }
            if (arg_is_processed) {delete[] arg_copy; delete[] val; continue;}
            for (int j = 0; j < parser.args_count; j++){
                if (parser.parsing_args[j].short_form ==
                    parser.parsing_args[j].full_form &&
                    parser.parsing_args[j].req_type != kNargsUsed){
                    if (!GetArgVal(parser.parsing_args[j], arg_copy)) {delete[] arg_copy; delete[] val; return false;}
                    arg_is_processed = true;
                    break;
                    }
                }   
            if (arg_is_processed){delete[] arg_copy; delete[] val; continue;}       
            delete[] arg_copy; 
            delete[] val;
        }
        for (int i = 0; i < parser.args_count; i++){
            if (parser.parsing_args[i].req_type == kNargsRequired) {return false;}
            else if (parser.parsing_args[i].req_type == kNargsOneOrMore && parser.parsing_args[i].values_count == 0) {return false;}
        }
        return true;
    }

    int GetRepeatedCount(ArgumentParser& parser, const char* name){
        for (int j = 0; j < parser.args_count; j++){
            if (CompareArgs(parser.parsing_args[j].name, name) == 0){
                return parser.parsing_args[j].values_count;
            }
        }
        return 0;
    }

    bool GetRepeated(ArgumentParser& parser, const char* name, int index, int* to_val){

        for (int j = 0; j < parser.args_count; j++){
            if (CompareArgs(parser.parsing_args[j].name, name) == 0 && index < parser.parsing_args[j].values_count){
                *to_val = parser.parsing_args[j].all_val_int[index];
                return true;
            }
        }
        return false;
    }

    bool GetRepeated(ArgumentParser& parser, const char* name, int index, float* to_val)
    {
        for (int j = 0; j < parser.args_count; j++){
            if (CompareArgs(parser.parsing_args[j].name, name) == 0 && index < parser.parsing_args[j].values_count){
                *to_val = parser.parsing_args[j].all_val_float[index];
                return true;
            }
        }
        return false;
    }

    bool GetRepeated(ArgumentParser& parser, const char* name, int index, const char** to_val){
        for (int j = 0; j < parser.args_count; j++){
            if (CompareArgs(parser.parsing_args[j].name, name) == 0 && index < parser.parsing_args[j].values_count){
                *to_val = parser.parsing_args[j].all_val_char[index];
                return true;
            }
        }
        return false;
    }

    void FreeParser(ArgumentParser& parser){
        delete[] parser.parsing_flags;
        for (int i = 0; i < parser.len_arg_arr; i++) {
            delete[] parser.parsing_args[i].all_val_int;
            delete[] parser.parsing_args[i].all_val_float;
            if (parser.parsing_args[i].type == 'c'){
                for (int j = 0; j < parser.parsing_args[i].values_count; j++){
                    delete[] parser.parsing_args[i].all_val_char[j];
                }
            }
            delete[] parser.parsing_args[i].all_val_char;
        }
        delete[] parser.parsing_args;
    }

    void AddHelp(ArgumentParser& parser){
        parser.help_enable = true;
    }   

    bool PrintHelp(ArgumentParser& parser){
        if (!parser.help_enable) {return false;}
        for (int i = 0; i < parser.flags_count; i++){
            std::cout << ((parser.parsing_flags[i].name != nullptr) ? parser.parsing_flags[i].name: " ") << ": ";
            std::cout << ((parser.parsing_flags[i].short_form != nullptr) ? parser.parsing_flags[i].short_form: "") << " ";
            std::cout << ((parser.parsing_flags[i].full_form != nullptr) ? parser.parsing_flags[i].full_form: "") << " / ";  
            std::cout << ((parser.parsing_flags[i].description != nullptr) ? parser.parsing_flags[i].description: "") << " " << std::endl;
        }
        for (int i = 0; i < parser.args_count; i++){
            std::cout << ((parser.parsing_args[i].name != nullptr) ? parser.parsing_args[i].name: "") << ": ";
            std::cout << ((parser.parsing_args[i].short_form != nullptr) ? parser.parsing_args[i].short_form: "") << " ";
            std::cout << ((parser.parsing_args[i].full_form != nullptr) ? parser.parsing_args[i].full_form: "") << " / ";
            std::cout << ((parser.parsing_args[i].description != nullptr) ? parser.parsing_args[i].description: "") << " " << std::endl;
        }
        return true;
    }

    ArgumentParser CreateParser(const char* name, int max_arg_len){
        ArgumentParser parser;
        parser.name = name;
        parser.max_arg_len = max_arg_len;
        parser.parsing_args = new Argument[parser.len_arg_arr];
        parser.parsing_flags = new Flag[parser.len_flag_arr];
        for(int i = 0; i < parser.len_arg_arr; i++){
            parser.parsing_args[i].all_val_int = new int[parser.parsing_args[i].len_val_arr];
            parser.parsing_args[i].all_val_float = new float[parser.parsing_args[i].len_val_arr];
            parser.parsing_args[i].all_val_char = new char*[parser.parsing_args[i].len_val_arr];
        }
        return parser;
    }
};