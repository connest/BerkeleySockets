#pragma once

#include <string>
#include <vector>
#include <map>
#include <getopt.h>
class ArgsParser
{
    std::vector<option> opts;
    std::string help_text;
    std::map<char, std::string> opt_values;
    std::string optString{"?"};
public:

    /**
     * @brief add console option
     * @param name optin name like '--help'
     * @param letter pseudonym  like '-h'
     * @param arg_option required_argument or no_argument
     * @return this object
     */
    ArgsParser& addOption(const std::string& name,
                   const char letter,
                   int arg_option
                   );

    /**
     * @brief set help text
     * @param text text of help
     * @return this object
     */
    ArgsParser& setHelpText(std::string&& text);

    /**
     * @brief parse console arguments
     * @param argc arguments count
     * @param argv arguments list
     * @return 1 if show help or 0
     */
    int parse(int argc, char *argv[]);

    /**
     * @brief Is option exists
     * @param letter pseudonym  like '-h'
     * @return true/false
     */
    bool hasOption(char letter);

    /**
     * @brief get option value
     * @param letter pseudonym  like '-h'
     * @return
     * string value if exists and has value
     * true if exists without value
     * empty string if not exists
     */
    std::string getOption(char letter);

    /**
     * @brief show help message
     */
    void showHelp();
};

