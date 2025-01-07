#include <stdio.h>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    const char *DATA_LOCATION = "../data";

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
    }

    return 0;
}