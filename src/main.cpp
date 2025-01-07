#include <stdio.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char *argv[])
{
    const char *DATA_LOCATION = "data/db.txt";

    while (true)
    {
        printf("%s\n", "Select which action you would like to choose: ");
        printf("%s\n%s\n", "1. Read passwords", "2. Save password");

        std::string input;

        getline(std::cin, input);

        if (input != "1" && input != "2")
        {
            printf("%s\n", "Your input is wrong.");
            exit(1);
        }

        // Save a password to the database
        if (input == "2")
        {
            printf("%s", "TEDSADSA");
            std::string pass;

            printf("%s\n", "Enter the password you want to save.");
            getline(std::cin, pass);

            std::string user;

            printf("%s\n", "Enter the username for which you want to store the password.");
            getline(std::cin, user);

            if (std::filesystem::exists(DATA_LOCATION))
            {
                std::ofstream FILE(DATA_LOCATION, std::ios_base::app);

                FILE << user << " " << pass << "\n";
                FILE.close();
            }
            else
            {
                std::ofstream FILE(DATA_LOCATION);

                FILE << user << " " << pass << "\n";
                FILE.close();
            }
        }
    }

    return 0;
}