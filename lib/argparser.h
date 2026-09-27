#pragma once

namespace nargparse {

    const char kNargsOneOrMore = '1';
    const char kNargsZeroOrMore = '0';
    const char kNargsRequired = '3';
    const char kNargsOptional = '4';
    const char kNargsUsed = '5';

    struct Flag {
        const char* short_form = nullptr;
        const char* full_form = nullptr;
        bool* to_change = nullptr;
        const char* name = nullptr;
        bool by_default;   
        const char* description = nullptr;
    };

    struct Argument {
        int values_count = 0;
        int len_val_arr = 10;
        char type = 'n';
        char req_type = kNargsRequired;
        const char* name = nullptr;
        const char* short_form = nullptr;
        const char* full_form = nullptr;
        const char* description = nullptr;
        int* all_val_int = nullptr;    
        float* all_val_float = nullptr;
        char** all_val_char = nullptr;
        void* to_change = nullptr;
        bool (*check_func_int)(const int&) = nullptr;
        bool (*check_func_char)(const char* const&) = nullptr;
        bool (*check_func_float)(const float&) = nullptr;
    };

    struct ArgumentParser {
        const char* name;
        int max_arg_len = 64;
        int len_flag_arr = 5;
        int len_arg_arr = 5;
        int flags_count = 0;
        int args_count = 0;
        bool help_enable = false;

        Flag* parsing_flags = nullptr;
        Argument* parsing_args = nullptr;
    };

    void AddFlag(ArgumentParser& parser, const char* short_form, const char* full_form, bool* to_change, const char* name, bool by_default = false, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, int* to_change, const char* name, char args_type = kNargsRequired, bool (*check_func)(const int&) = nullptr, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, float* to_change, const char* name, char args_type = kNargsRequired, bool (*check_func)(const float&) = nullptr, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, char (*to_change)[], const char* name, char args_type = kNargsRequired, bool (*check_func)(const char* const&) = nullptr, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, int* to_change, const char* name, char args_type = kNargsRequired, bool (*check_func)(const int&) = nullptr, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, float* to_change, const char* name, char args_type = kNargsRequired, bool (*check_func)(const float&) = nullptr, const char* description = nullptr);
    void AddArgument(ArgumentParser& parser, const char* short_form, const char* full_form, char (*to_change)[], const char* name, char args_type = kNargsRequired, bool (*check_func)(const char* const&) = nullptr, const char* description = nullptr);
    bool Parse(ArgumentParser& parser,  int args_count, const char* argv[]);
    bool Parse(ArgumentParser& parser,  int args_count, char* argv[]);
    int GetRepeatedCount(ArgumentParser& parser, const char* name);
    bool GetRepeated(ArgumentParser& parser, const char* name, int index, int* to_val);
    bool GetRepeated(ArgumentParser& parser, const char* name, int index, float* to_val);
    bool GetRepeated(ArgumentParser& parser, const char* name, int index, const char** to_val);
    ArgumentParser CreateParser(const char* name, int max_arg_len = 100);
    void FreeParser(ArgumentParser& parser);
    void AddHelp(ArgumentParser& parser);
    bool PrintHelp(ArgumentParser& parser);

};