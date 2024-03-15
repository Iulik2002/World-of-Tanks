#include "Building.h"
#include <iostream>



class City
{ 
    public: 
    std::vector<Building *> city;    
    
    void add_building(Building* building){
        city.push_back(building);
    }

    void remove_building(Building* building){
        for (auto j = city.begin(); j < city.end(); j++){
            if (*j == building){
                city.erase(j);
            }
       }
    }
};