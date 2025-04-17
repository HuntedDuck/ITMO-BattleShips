#include "game.h"



int main() {
    game warship;

    std::cout<<"Are you gonna play on one device?"<<std::endl;
    std::string answer;
    std::getline(std::cin, answer);
    answer == "yes" ? warship.change_amount_of_device(1) : warship.change_amount_of_device(0);

    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "ok" << std::endl;
            break;
        }

        warship.handle_commands(input);
    }

    return 0;
    return 0;
}