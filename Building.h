

#include <iostream>
#include "lab_m1/Tema2/Tema2.h"





class Building
{ 
    public: 
    glm::mat4 position_matrix = glm::mat4(1);
    float size;
    
    
    

    Building(glm::mat4 InitMatrix, float init_size){
        position_matrix = InitMatrix;
        size = init_size;

        position_matrix = glm::translate(position_matrix, glm::vec3(0,size/2,0));
        position_matrix = glm::scale(position_matrix, glm::vec3(size/3, size, size/2));
    }


};