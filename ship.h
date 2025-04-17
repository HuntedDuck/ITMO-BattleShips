#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <cstring>
#include <fstream>
#include <random>
#include <iostream>
#ifndef UNTITLED9_SHIP_H
#define UNTITLED9_SHIP_H


class ship{
private:
    uint8_t size;
    char direction;
    char* health;
    uint64_t x;
    uint64_t y;
public:
    ship(uint8_t size, char direction, uint64_t x, uint64_t y): size(size), direction(direction), x(x), y(y){
        health = new char[size];
        for(int i=0; i<size; i++){health[i] = '1';}
    };

    ship(): size(1), direction('h'), x(0), y(0){
        health = new char[size];
        for(int i=0; i<size; i++){health[i] = '1';}
    };

    bool is_dead(){
        for(int i=0; i<size; i++){
            if(health[i]=='1'){
                return false;
            }
        }
        return true;
    }

    bool shoted(uint64_t x_cordinate, uint64_t y_cordinate){
        if(x>x_cordinate || y>y_cordinate){
            return false;
        }
        uint64_t part_of_the_ship;
        switch(direction){
            case 'h':
                part_of_the_ship = x_cordinate-x;
                break;
            case 'v':
                part_of_the_ship = y_cordinate - y;
                break;
        }
        if(part_of_the_ship+1>size){
            return false;
        }
        health[part_of_the_ship] = '0';
        return true;
    }

    uint8_t get_size(){
        return size;
    }

    void set_size(uint8_t new_size){
        size = new_size;
        delete[] health;
        health = new char[size];
        for(int i=0; i<size; i++){
            health[i] = '1';
        }
    }

    uint64_t get_x(){
        return x;
    }

    void set_x(uint64_t new_x){
        x = new_x;
    }

    uint64_t get_y(){
        return y;
    }

    void set_y(uint64_t new_y){
        y = new_y;
    }

    char get_direction(){
        return direction;
    }

    void set_direction(char new_direction){
        direction = new_direction;
    }

    void print_health(){
        for(int i=0; i<size; i++){
            std::cout<<health[i];
        }
        std::cout<<std::endl;
    }
};


#endif //UNTITLED9_SHIP_H
