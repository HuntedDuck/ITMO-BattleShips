
#include "field.h"
#ifndef UNTITLED9_GAME_H
#define UNTITLED9_GAME_H


class game{
private:
    enum game_type{master, slave} type;
    enum game_state{not_started, started, in_process, paused, finished} state;
    enum strategies{ordered, custom} strategy = custom;
    bool if_one_device = true;
    uint64_t last_shot_x_cordinate = 0;
    uint64_t last_shot_y_cordinate = 0;
    std::vector<std::pair<uint64_t, uint64_t> > shot_coordinates;
    std::vector<int> shot_results;
    std::mt19937_64 rand_gen;


public:
    field my_field;
    field enemy_field;

    game(): state(not_started){
        std::random_device rd;
        rand_gen = std::mt19937_64(rd());
    };

    void set_type(enum game_type given_type){
        type = given_type;
    }

    void set_strategy(enum strategies given_strategy){
        strategy = given_strategy;
    }

    void change_amount_of_device(bool input){
        if_one_device = input;
    }

    void send_command(std::string command){
        std::cout<<command<<std::endl;
    }

    std::vector<std::string> split_string(const std::string& str) {
        std::istringstream iss(str);
        std::vector<std::string> result;
        std::string token;

        while (iss >> token) {
            result.push_back(token);
        }

        return result;
    }

    void start(){
        if(state!=not_started){
            return;
        }
        state=started;

        if(type==slave){
            send_command("set height 10");
            my_field.set_height(10);
            enemy_field.set_height(10);
            send_command("set width 10");
            my_field.set_width(10);
            enemy_field.set_width(10);
            send_command("set count 4 1");
            send_command("set count 3 2");
            send_command("set count 2 3");
            send_command("set count 1 4");
            my_field.set_count(4, 1, 10, 10);
            enemy_field.set_count(4, 1, 10, 10);
            my_field.set_count(3, 2, 10, 10);
            enemy_field.set_count(3, 2, 10, 10);
            my_field.set_count(2, 3, 10, 10);
            enemy_field.set_count(2, 3, 10, 10);
            my_field.set_count(1, 4, 10, 10);
            enemy_field.set_count(1, 4, 10, 10);
            enemy_turn();
        }
        else{
            my_turn();
        }
    }

    int my_shoot(uint64_t x_cordinate, uint64_t y_cordinate){
        uint64_t result = enemy_field.is_ship_there(x_cordinate, y_cordinate);
        if(result==-1){
            return -1;
        }
        else{
            ship shot_ship = enemy_field.get_ship(result);
            if(shot_ship.shoted(x_cordinate, y_cordinate)){
                if(shot_ship.is_dead()){
                    enemy_field.remove_ship(result);
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                return -1;
            }
        }
    }

    bool win(){
        return enemy_field.check_gameover();
    }

    bool lose(){
        return my_field.check_gameover();
    }

    bool finish(){
        return state==finished;
    }

    int8_t set_result(std::string result_of_shot) {
        while (true) {
            if (result_of_shot == "miss") {
                return -1;
            }
            else if (result_of_shot == "hit") {
                return 0;
            }
            else if (result_of_shot == "kill") {
                return 1;
            }
            else {
                std::cout << "Wrong input" << std::endl;
                std::cout << "Usage: set result [miss, hit, kill]" << std::endl;
                std::string new_result;
                std::cin >> new_result;
                result_of_shot = new_result;
            }
        }
    }

    bool is_number(const std::string& input) {
        if (input.empty()) return false;

        size_t start_idx = 0;

        if (input[0] == '-') {
            if (input.size() == 1) return false;
            start_idx = 1;
        }

        bool dot_found = false;

        for (size_t i = start_idx; i < input.size(); ++i) {
            if (input[i] == '.') {
                if (dot_found) return false;
                dot_found = true;
            } else if (!std::isdigit(input[i])) {
                return false;
            }
        }
        return true;
    }


    void handle_commands(const std::string& cmd) {
        std::vector<std::string> args = split_string(cmd);

        if (args[0] == "exit") {
            std::cout << "ok" << std::endl;
            exit(0);
        } else if (args[0] == "create") {
            if (args[1] == "master") {
                set_type(master);
                std::cout << "ok" << std::endl;
            } else if (args[1] == "slave") {
                set_type(slave);
                std::cout << "ok" << std::endl;
            }
        } else if (args[0] == "start") {
            start();
            std::cout << "ok" << std::endl;
        } else if (args[0] == "stop") {
            if (state == started || state == in_process) {
                state = paused;
                std::cout << "ok" << std::endl;
            }
        } else if (args[0] == "set" && args[1] == "width") {
            uint64_t new_width = std::stoull(args[2]);
            if (new_width > 0) {
                my_field.set_width(new_width);
                enemy_field.set_width(new_width);
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (args[0] == "get" && args[1] == "width") {
            std::cout << my_field.get_width() << std::endl;
        } else if (args[0] == "set" && args[1] == "height") {
            uint64_t new_height = std::stoull(args[2]);
            if (new_height > 0) {
                my_field.set_height(new_height);
                enemy_field.set_height(new_height);
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (args[0] == "get" && args[1] == "height") {
            std::cout << my_field.get_height() << std::endl;
        } else if (args[0] == "set" && args[1] == "count") {
            uint8_t ship_size = std::stoi(args[2]);
            uint64_t count = std::stoull(args[3]);
            if (ship_size >= 1 && ship_size <= 4 && count > 0) {
                if (my_field.set_count(ship_size, count, my_field.get_width(), my_field.get_height()) && enemy_field.set_count(ship_size, count, enemy_field.get_width(), enemy_field.get_height())) {
                    std::cout << "ok" << std::endl;
                } else {
                    std::cout << "failed" << std::endl;
                }
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (args[0] == "get" && args[1] == "count") {
            uint8_t ship_size = std::stoi(args[2]);
            if (ship_size >= 1 && ship_size <= 4) {
                std::cout << my_field.get_ship_count(ship_size) << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        }else if(args[0]=="set" && args[1]=="result"){
            set_result(args[2]);
            std::cout<<"ok"<<std::endl;
        }else if (args[0] == "set" && args[1] == "strategy") {
            if (args[2] == "ordered" || args[2] == "custom") {
                if(args[2] == "ordered"){
                    set_strategy(ordered);
                }
                else{
                    set_strategy(custom);
                }
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (args[0] == "shot") {
            uint64_t x = std::stoull(args[1]);
            uint64_t y = std::stoull(args[2]);
            uint8_t result = enemy_shoot(x, y);
            if(result==1){
                std::cout<<"kill"<<std::endl;
            }
            else if(result==0){
                std::cout<<"hit"<<std::endl;
            }
            else{
                std::cout<<"miss"<<std::endl;
                enemy_turn();
            }
        } else if (args[0] == "finished"){
            bool result = finish();
            if(result){
                std::cout<<"yes"<<std::endl;;
            }
            else{
                std::cout<<"no"<<std::endl;
            }
        } else if (args[0] == "win"){
            bool result = win();
            if(result){
                std::cout<<"yes"<<std::endl;
            }
            else{
                std::cout<<"no"<<std::endl;
            }
        } else if (args[0] == "lose"){
            bool result = lose();
            if(result){
                std::cout<<"yes"<<std::endl;
            }
            else{
                std::cout<<"no"<<std::endl;
            }
        } else if (args[0] == "dump"){
            my_field.dump_field(args[1]);
        } else if (args[0] == "load"){
            my_field.load_field(args[1]);
        }
        else{
            std::cout << "unknown command" << std::endl;
        }
    }


    void my_turn() {
        uint64_t x, y;
        while (true) {
            std::cout << "shot ";
            std::cin >> x >> y;
            int result = my_shoot(x, y);
            if (result == 0) {
                std::cout << "hit" << std::endl;
            } else if (result == 1) {
                std::cout << "kill" << std::endl;
            } else {
                std::cout << "miss" << std::endl;
                enemy_turn();
                return;
            }
        }
    }


    int8_t enemy_shoot(uint64_t x_cordinate, uint64_t y_cordinate){
        std::cout<<"shot "<<x_cordinate<<" "<<y_cordinate<<std::endl;
        std::cout<<std::endl;
        if(if_one_device){
            my_field.print_field();
            std::cout<<std::endl;
        }
        std::string result_of_shot;
        std::cin>>result_of_shot;
        int8_t result = set_result(result_of_shot);
        return result;
    }

    void enemy_turn(){
        if(strategy==custom){
            custom_strategy(shot_coordinates, shot_results);
        }
        else{
            ordered_strategy(last_shot_x_cordinate, last_shot_y_cordinate);
        }
    }


    void ordered_strategy(uint64_t& last_shot_x_cordinate, uint64_t& last_shot_y_cordinate) {
        if (last_shot_y_cordinate >= my_field.get_height()) {
            return;
        }
        uint8_t result = enemy_shoot(last_shot_x_cordinate, last_shot_y_cordinate);

        last_shot_x_cordinate++;
        if (last_shot_x_cordinate >= my_field.get_width()) {
            last_shot_x_cordinate = 0;
            last_shot_y_cordinate++;
        }

        if (result == 0 || result == 1) {
            ordered_strategy(last_shot_x_cordinate, last_shot_y_cordinate);
        }
        else{
            my_turn();
        }
    }

    int find_in_map(const std::vector<std::pair<uint64_t, uint64_t> >& shot_coordinates,
                        const std::vector<int>& shot_results, const std::pair<uint64_t, uint64_t>& target) {
        for (size_t i = 0; i < shot_coordinates.size(); ++i) {
            if (shot_coordinates[i] == target) {
                return shot_results[i];
            }
        }
        return -1;
    }

    void mark_zone(std::vector<std::pair<uint64_t, uint64_t> >& shot_coordinates,
                   std::vector<int>& shot_results, const std::pair<uint64_t, uint64_t>& place_of_death_ship) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int64_t new_x = static_cast<int64_t>(place_of_death_ship.first) + i;
                int64_t new_y = static_cast<int64_t>(place_of_death_ship.second) + j;

                if (new_x >= 0 && new_x < static_cast<int64_t>(my_field.get_width()) &&
                    new_y >= 0 && new_y < static_cast<int64_t>(my_field.get_height())) {

                    std::pair<uint64_t, uint64_t> place_to_mark;

                    place_to_mark.first = static_cast<uint64_t>(new_x);
                    place_to_mark.second = static_cast<uint64_t>(new_y);

                    if (find_in_map(shot_coordinates, shot_results, place_to_mark) == -1) {
                        shot_coordinates.push_back(place_to_mark);
                        shot_results.push_back(0);
                    }
                }
            }
        }
    }


    void custom_strategy(std::vector<std::pair<uint64_t, uint64_t> >& shot_coordinates,
                         std::vector<int>& shot_results) {
        int count = 0;
        for (auto it = shot_coordinates.rbegin(); it != shot_coordinates.rend() && count < 4; ++it, ++count) {
            const auto& coordinates = *it;
            int value = shot_results[shot_coordinates.size() - 1 - count]; // Индекс для соответствующего результата

            if (value == 1) {
                uint64_t x_cordinate_of_last_shot = coordinates.first;
                uint64_t y_cordinate_of_last_shot = coordinates.second;

                std::pair<uint64_t, uint64_t> try_shot;

                std::vector<std::pair<int, int> > directions;
                std::pair<int, int> colebration_pair;
                colebration_pair = std::make_pair(-1, 0);
                directions.push_back(colebration_pair);
                colebration_pair = std::make_pair(1, 0);
                directions.push_back(colebration_pair);
                colebration_pair = std::make_pair(0, -1);
                directions.push_back(colebration_pair);
                colebration_pair = std::make_pair(0, 1);
                directions.push_back(colebration_pair);

                for (const auto& dir : directions) {
                    int64_t new_x = static_cast<int64_t>(x_cordinate_of_last_shot) + dir.first;
                    int64_t new_y = static_cast<int64_t>(y_cordinate_of_last_shot) + dir.second;

                    if (new_x >= 0 && new_x < static_cast<int64_t>(my_field.get_width()) &&
                        new_y >= 0 && new_y < static_cast<int64_t>(my_field.get_height())) {
                        std::pair<uint64_t, uint64_t> try_shot;
                        try_shot.first = static_cast<uint64_t>(new_x);
                        try_shot.second = static_cast<uint64_t>(new_y);


                        if (find_in_map(shot_coordinates, shot_results, try_shot) == -1) {
                            int8_t result = enemy_shoot(try_shot.first, try_shot.second);

                            if (result == 1) {
                                shot_coordinates.push_back(try_shot);
                                shot_results.push_back(0);
                                mark_zone(shot_coordinates, shot_results, try_shot);
                                custom_strategy(shot_coordinates, shot_results);
                            } else if (result == 0) {
                                shot_coordinates.push_back(try_shot);
                                shot_results.push_back(1);
                            } else {
                                shot_coordinates.push_back(try_shot);
                                shot_results.push_back(0); // Неизвестно
                                return;
                            }
                        }
                    }
                }
            }
        }

        std::uniform_int_distribution<uint64_t> dist_x(0, my_field.get_width() - 1);
        std::uniform_int_distribution<uint64_t> dist_y(0, my_field.get_height() - 1);
        uint64_t x_cordinate = dist_x(rand_gen);
        uint64_t y_cordinate = dist_y(rand_gen);
        std::pair<uint64_t, uint64_t> try_shot;
        try_shot.first = x_cordinate;
        try_shot.second = y_cordinate;

        if (find_in_map(shot_coordinates, shot_results, try_shot) == -1) {
            int8_t result = enemy_shoot(try_shot.first, try_shot.second);

            if (result == 1) {
                shot_coordinates.push_back(try_shot);
                shot_results.push_back(0);
                mark_zone(shot_coordinates, shot_results, try_shot);
            } else if (result == 0) {
                shot_coordinates.push_back(try_shot);
                shot_results.push_back(1);
                custom_strategy(shot_coordinates, shot_results);
            } else {
                my_turn();
            }
        }
    }


};

#endif
