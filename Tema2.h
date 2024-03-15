#pragma once

#include "components/simple_scene.h"
#include "Camera.h"
#include "Enemies.h"
#include "City.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene();
     protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;

        myCamera::Camera *camera;
        glm::mat4 projectionMatrix;

        struct Bullet {
            float x, y, z;  // position
            float createTime;  // time of creation
            float initialOrientation;
            float lifetime;  // lifetime of the bullet in seconds
            std::vector<glm::vec3> trajectory;  // stores trajectory points
        };

        float amgle_bullet_OY;
        float bulletInitialOrientation;
        bool shoot;
        std::vector<Bullet> bullets;
        float bullet_time_start;

        float angularStepOY_tureta;
        enemiesTanks::Enemies * enemies;
        std::vector<std::tuple<float, float, float>> enemies_vector;

        float prevX;
        float prevZ;
        
        City whorehouses;
    };
}   // namespace m1

