#include "utils/glm_utils.h"
#include "components/simple_scene.h"

namespace enemiesTanks
{
    class Enemies
    {
        public:
        struct TankInfo
        {
            float x, y, z;  // Tank position
            int health;      // Tank health (you can customize this based on your requirements)
        };

        Enemies() {};

        std::tuple<float, float, float> GenerateRandomPosition(float minX, float maxX, float minZ, float maxZ)
        {
            float randomX = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maxX - minX));
            float randomZ = minZ + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maxZ - minZ));
            return std::make_tuple(randomX, 0, randomZ);
        }

         std::vector<std::tuple<float, float, float>> generateEnemies(int size){
            std::vector<std::tuple<float, float, float>> result;
            for (auto i = 0; i < size; i++) {
                result.push_back(GenerateRandomPosition(-50, 50, -50, 50));
            }
            return result;            
        }
    };
}
