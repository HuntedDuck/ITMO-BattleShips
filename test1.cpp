#include "game.h"

int main() {
    game bsGame;

    bsGame.handle_commands("create master");
    bsGame.handle_commands("set height 10");
    bsGame.handle_commands("set width 10");
    bsGame.handle_commands("set count 4 2");
    bsGame.handle_commands("set count 1 4");

    bsGame.handle_commands("dump /Users/dmitrijgaev/CLionProjects/battle_ships/test1.txt");

    bsGame.handle_commands("start");
    bsGame.handle_commands("4 5");
    //bsGame.handle_commands("kill");

    // Создаем поток, эмулирующий ввод пользователя
    std::istringstream simulatedInput("kill");
    std::cin.rdbuf(simulatedInput.rdbuf()); // Перенаправляем std::cin на simulatedInput

    std::string command;
    std::cin >> command; // Теперь std::cin получает "kill"
    bsGame.handle_commands(command);

    bsGame.handle_commands("4 5");
    return 0;
}