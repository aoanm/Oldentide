// Filename:    utils.cpp
// Author:      Joseph DeVictoria
// Date:        Mar_10_2016
// Purpose:     Miscellaneous functions.

#include "Utils.h"
#include <algorithm>
#include <regex>
#include <iostream>
#include <iterator>
#include <string.h>

#define MIN_ACCOUNT_NAME_LENGTH 3
#define MIN_ACCOUNT_NAME_LENGTH_STRING "3"
#define MAX_ACCOUNT_NAME_LENGTH 30
#define MAX_ACCOUNT_NAME_LENGTH_STRING "30"
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 30

namespace utils{

    // Simple function that separates a string (s) into tokens based on a delimiter (delim).
    std::vector<std::string> Tokenfy(std::string s, char delim) {
        size_t start=0;
        size_t end=s.find_first_of(delim);
        std::vector<std::string> tokens;
        while (end <= std::string::npos) {
	        tokens.emplace_back(s.substr(start, end-start));
	        if (end == std::string::npos)
	    	    break;
    	    start=end+1;
    	    end = s.find_first_of(delim, start);
        }
        return tokens;
    }

    // CLIENT.
    bool CheckPasswordLength(char *password) {
        // Check to make sure password is reasonable
        if (strlen(password) > MAX_PASSWORD_LENGTH) {
            printf("Password is too large!\n");
            return 0;
        }
        else if (strlen(password) < MIN_PASSWORD_LENGTH) {
            printf("Password is too small!\n" );
            return 0;
        }
        else {
            // Password is of a good length
            return 1;
        }
    }

    // CLIENT/SERVER.
    bool CheckAccountNameLength(char *account_name) {
        // Check to make sure account_name is reasonable
        if (strlen(account_name) > MAX_ACCOUNT_NAME_LENGTH) {
            printf("Account name is too large!\n");
            return 0;
        }
        else if (strlen(account_name) < MIN_ACCOUNT_NAME_LENGTH) {
            printf("Account name is too small!\n" );
            return 0;
        }
        else {
            // Account name is of good length
            return 1;
        }
    }
    
    // CLIENT/SERVER. Checks to make sure the passed string looks like a valid account name
    // Checks the length of the string, and then makes sure it is alphanumeric and underscore only.
    bool SanitizeAccountName(char *account_name) {
        if (!utils::CheckAccountNameLength(account_name)) {
            return false;
        }
        if (!utils::SanitizeAlphanumeric(account_name)) {
            printf("Invalid account name! Account name must be only contain characters a-z, A-Z, 0-9, _\n");
            return false;
        }
        else {
            return true;
        }
    }

    // CLIENT/SERVER. Checks to makesure the passed string is only alphanumeric characters and underscore.   
    // Use regex to check that the account name is only alpha-numeric
    // Regex: \^\w{3,30}$\
    // Tested regex at regex101.com using javascript (ECMAScript) flavor
    // NOTE: Account names are stored in a case-insensitive way in the db
    // NOTE: In C, adjacent string literals are concatenated (MACRO
    // TODO: Wow. Adding regex stuff adds a full three seconds to the compile time... What's the deal?
    bool SanitizeAlphanumeric(char *string) {
        std::regex check_alpha_regex("^\\w{" MIN_ACCOUNT_NAME_LENGTH_STRING "," MAX_ACCOUNT_NAME_LENGTH_STRING "}$");
        if (!regex_match(string, check_alpha_regex)) {
            return false;
        }
        else {
            return true;
        }
    }

    // CLIENT/SERVER. Checks to make sure the passed string is only hex - a-f, A-F, 0-9.
    // Returns 1 if it's hex, 0 if not.
    bool SanitizeHexString(char *string) {
        // Check to see if the passed string is at least one character (+) of only hex characters 
        std::regex check_hex_regex("^[a-fA-F0-9]+$");
        if (!regex_match(string, check_hex_regex)) {
            return false;
        }
        else {
            return true;
        }
    }
}