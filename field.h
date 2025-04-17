#include "ship.h"
#ifndef UNTITLED9_FIELD_H
#define UNTITLED9_FIELD_H

class field{
private:
    uint64_t width;
    uint64_t height;
    std::vector<ship> ships;
    std::mt19937 rand_gen;

public:
    field(uint64_t width, uint64_t height): width(width), height(height){};

    field():width(0), height(0){};

    ship get_ship(uint64_t index){
        return ships[index];
    }

    void remove_ship(int index_of_ship_to_remove){
        ships.erase(ships.begin() + index_of_ship_to_remove);
    }

    bool check_gameover(){
        return ships.size()==0;
    }

    uint64_t get_ship_count(uint8_t size) {
        uint64_t count = 0;
        for (auto& s : ships) {
            if (s.get_size() == size) {
                ++count;
            }
        }
        return count;
    }

    bool add_ship(ship& new_ship) {
        if(can_place_ship_here(new_ship.get_x(), new_ship.get_y(), new_ship.get_direction(), new_ship.get_size())){
            ships.push_back(new_ship);
            return true;
        }
        return false;
    }

    int is_ship_there(uint64_t x_cordinate, uint64_t y_cordinate) {
        int index=0;
        for (auto& s : ships) {
            uint64_t ship_start_x = s.get_x();
            uint64_t ship_start_y = s.get_y();
            uint64_t ship_end_x = (s.get_direction() == 'h') ? ship_start_x + s.get_size() - 1 : ship_start_x;
            uint64_t ship_end_y = (s.get_direction() == 'v') ? ship_start_y + s.get_size() - 1 : ship_start_y;

            if (x_cordinate >= ship_start_x && x_cordinate <= ship_end_x &&
                y_cordinate >= ship_start_y && y_cordinate <= ship_end_y) {
                return index;
            }
            index++;
        }
        return -1;
    }

    bool can_place_ship_here(uint64_t x_cordinate, uint64_t y_cordinate, char direction, uint8_t size) {
        if (size == 0) return false;

        if (direction == 'v') {
            if (x_cordinate >= width || y_cordinate + size - 1 >= height) {
                return false;
            }
        } else if (direction == 'h') {
            if (x_cordinate + size - 1 >= width || y_cordinate >= height) {
                return false;
            }
        } else {
            return false;
        }

        for (auto& s : ships) {
            uint64_t ship_start_x = s.get_x();
            uint64_t ship_start_y = s.get_y();
            uint64_t ship_end_x = (s.get_direction() == 'h') ? std::min(ship_start_x + s.get_size() - 1, width - 1) : ship_start_x;
            uint64_t ship_end_y = (s.get_direction() == 'v') ? std::min(ship_start_y + s.get_size() - 1, height - 1) : ship_start_y;

            uint64_t check_start_x = (ship_start_x > 0) ? ship_start_x - 1 : 0;
            uint64_t check_end_x = std::min(ship_end_x + 1, width - 1);
            uint64_t check_start_y = (ship_start_y > 0) ? ship_start_y - 1 : 0;
            uint64_t check_end_y = std::min(ship_end_y + 1, height - 1);

            for (uint64_t i = 0; i < size; ++i) {
                uint64_t new_ship_x = x_cordinate + (direction == 'h' ? i : 0);
                uint64_t new_ship_y = y_cordinate + (direction == 'v' ? i : 0);

                uint64_t check_start_x = (new_ship_x > 0) ? new_ship_x - 1 : 0;
                uint64_t check_end_x = std::min(new_ship_x + 1, width - 1);
                uint64_t check_start_y = (new_ship_y > 0) ? new_ship_y - 1 : 0;
                uint64_t check_end_y = std::min(new_ship_y + 1, height - 1);

                if (check_end_x >= ship_start_x && check_start_x <= ship_end_x &&
                    check_end_y >= ship_start_y && check_start_y <= ship_end_y) {
                    return false;
                }
            }
        }
        return true;
    }

    bool try_to_place_ship_here(uint8_t size, uint64_t width, uint64_t height) {
        if (size == 0 || width == 0 || height == 0) return false;

        std::uniform_int_distribution<uint64_t> dist_x(0, width - 1);
        std::uniform_int_distribution<uint64_t> dist_y(0, height - 1);
        std::uniform_int_distribution<int> dist_dir(0, 1);

        uint64_t max_attempts = std::min(100ull, width * height);

        for (uint64_t attempts = 0; attempts < max_attempts; ++attempts) {
            uint64_t x = dist_x(rand_gen);
            uint64_t y = dist_y(rand_gen);
            char direction = (dist_dir(rand_gen) == 0) ? 'v' : 'h';

            if (direction == 'h' && x + size - 1 >= width) {
                direction = 'v';
            } else if (direction == 'v' && y + size - 1 >= height) {
                direction = 'h';
            }

            if (can_place_ship_here(x, y, direction, size)) {
                ships.emplace_back(size, direction, x, y);
                return true;
            }
        }
        return false;
    }

    bool set_count(uint8_t size, uint64_t quantity, uint64_t width, uint64_t height) {
        if (size == 0 || width == 0 || height == 0) return false;  // Проверка на некорректные данные
        if (quantity >= ((height / 2) * (width / 2))) return false; // Ограничение на максимальное количество кораблей

        uint64_t temp_width = width;
        uint64_t temp_height = height;

        while (quantity != 0 && (temp_height >= size || temp_width >= size)) {
            if (try_to_place_ship_here(size, temp_width, temp_height)) {
                quantity--;
            } else {
                std::uniform_int_distribution<int> dist_side(0, 2);
                uint8_t side = dist_side(rand_gen);

                // Разделение поля на разные части в зависимости от side
                if (side == 0) {
                    // Разделение на три равные части по горизонтали
                    try_to_place_ship_here(size, temp_width / 3, temp_height);
                    try_to_place_ship_here(size, 2 * (temp_width / 3), temp_height);
                } else if (side == 1) {
                    // Разделение на три равные части по вертикали
                    try_to_place_ship_here(size, temp_width, temp_height / 3);
                    try_to_place_ship_here(size, temp_width, 2 * (temp_height / 3));
                } else {
                    // Разделение на три квадранта
                    try_to_place_ship_here(size, temp_width / 2, temp_height / 2);
                    try_to_place_ship_here(size, temp_width / 2, temp_height);
                    try_to_place_ship_here(size, temp_width, temp_height / 2);
                }

                // Уменьшаем размеры случайным образом
                if (temp_width > 1) temp_width = fmaxl(temp_width / 2, size);
                if (temp_height > 1) temp_height = fmaxl(temp_height / 2, size);
            }
        }

        return (quantity == 0);
    }

    void dump_field(std::string file){
        if(file.empty()){
            std::cout<<"Error, you need to give output file to upload the data"<<std::endl;
            std::cout<<"Usage: dump PATH"<<std::endl;
            return;
        }

        std::ofstream res_file(file);

        if (res_file.is_open()==false){
            std::cout<<"Cant open the output file: "<< file;
            return;
        }

        res_file<<width<<" "<<height<<std::endl;

        for(auto& s : ships){
            res_file<<s.get_size()<<" "<<s.get_direction()<<" "<<s.get_x()<<" "<<s.get_y()<<std::endl;
        }

        res_file.close();
    }

    void load_field(std::string file){
        if(file.empty()){
            std::cout<<"Error you need to give input file to load the data"<<std::endl;
            std::cout<<"Usage: Load PATH"<<std::endl;
            return;
        }

        std::ifstream input_file(file);

        if(input_file.is_open()==false){
            std::cout<<"Cant open the input file"<<std::endl;
            return;
        }

        uint64_t width;
        uint64_t height;
        input_file>>width>>height;

        set_width(width);
        set_height(height);

        uint8_t size;
        char direction;
        uint64_t x_cordinate;
        uint64_t y_cordinate;

        while(input_file>>size>>direction>>x_cordinate>>y_cordinate){
            ship new_ship(size, direction, x_cordinate, y_cordinate);
            ships.push_back(new_ship);
        }

        input_file.close();
    }

    void set_height(uint64_t given_height){
        height=given_height;
    }

    void set_width(uint64_t given_width){
        width=given_width;
    }

    uint64_t get_height(){
        return height;
    }

    uint64_t get_width(){
        return width;
    }

    void print_field(){
        std::cout<<" ";
        for(int i=0; i<width; i++){
            std::cout<<i;

        }
        std::cout<<std::endl;

        uint64_t counter = 0;
        for(int j=0; j<height; j++){
            std::cout<<counter;
            counter++;
            for(int i=0; i<width; i++){
                if(is_ship_there(i, j)>=0){
                    std::cout<<"*";
                }
                else{
                    std::cout<<"#";
                }
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
};

#endif
