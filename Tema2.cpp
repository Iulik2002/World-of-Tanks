#include "Tema2.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new myCamera::Camera();
    camera->Set(glm::vec3(0, 2.5, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Shader *shader = new Shader("Texture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Default.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* tank = new Mesh("tank");
        tank->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "blender"), "tank.obj");
        meshes[tank->GetMeshID()] = tank;
   
        Mesh* tureta = new Mesh("tureta");
        tureta->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "blender"), "tureta.obj");
        meshes[tureta->GetMeshID()] = tureta;

        Mesh* bullet = new Mesh("bullet");
        bullet->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "blender"), "bullet.obj");
        meshes[bullet->GetMeshID()] = bullet;

        Mesh* grass_plane = new Mesh("grass_plane");
        grass_plane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "blender"), "grass_plane.obj");
        meshes[grass_plane->GetMeshID()] = grass_plane;
    }

    {
        Mesh* house = new Mesh("house");
        house->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[house->GetMeshID()] = house;
   
    }

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    prevX = translateX;
    prevZ = translateY;

    bullet_time_start = 0;

    angularStepOY_tureta =0;
    camera->RotateThirdPerson_OY(1.57);

    enemies = new enemiesTanks::Enemies();
    enemies_vector = enemies->generateEnemies(5);

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    for (int i=0; i <5; i ++){
        int x = rand()%45 - 22;
        int y = rand()%45 - 22;

        glm::mat4 position = glm::mat4(1);
        position[3][0] = x;
        position[3][2] = y;


        whorehouses.add_building(new Building(position, 5));
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.706, 0.922, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

}

void Tema2::RenderScene() {

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(translateX, 0, translateZ);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    RenderMesh(meshes["tank"], shaders["Texture"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    modelMatrix *= transform3D::Translate(translateX, 0, translateZ);
    modelMatrix *= transform3D::RotateOY(angularStepOY_tureta + angularStepOY);
    RenderMesh(meshes["tureta"], shaders["Texture"], modelMatrix);
    for (int i = 0; i < enemies_vector.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
        modelMatrix *= transform3D::Translate(std::get<0>(enemies_vector[i]), std::get<1>(enemies_vector[i]), std::get<2>(enemies_vector[i]));
        RenderMesh(meshes["tank"], shaders["Texture"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
        modelMatrix *= transform3D::Translate(std::get<0>(enemies_vector[i]), std::get<1>(enemies_vector[i]), std::get<2>(enemies_vector[i]));
        RenderMesh(meshes["tureta"], shaders["Texture"], modelMatrix);
    }
    



    for (const auto& bullet : bullets) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
        modelMatrix *= transform3D::Translate(bullet.x, bullet.y, bullet.z);
        RenderMesh(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);
    RenderMesh(meshes["grass_plane"], shaders["Texture"], modelMatrix);

    for (auto i = whorehouses.city.begin(); i < whorehouses.city.end(); i++){
        RenderMesh(meshes["house"], shaders["VertexColor"], (*i)->position_matrix);
    }

}

void Tema2::Update(float deltaTimeSeconds)
{
    RenderScene();

    bullet_time_start += deltaTimeSeconds;
    auto it = bullets.begin();
    while (it != bullets.end()) {
        bool hitEnemy = false;
        // Update bullet position based on its initial orientation
        it->x += glm::sin(it->initialOrientation - 1.57) * deltaTimeSeconds * 28;
        it->z += glm::cos(it->initialOrientation - 1.57) * deltaTimeSeconds * 28;
        for (auto i = enemies_vector.begin(); i < enemies_vector.end(); i++) {
            if ((it->x >= std::get<0>(*i) - 3 && it->x <= std::get<0>(*i) + 3) && 
                (it->z >= std::get<2>(*i) - 3 && it->z <= std::get<2>(*i) + 3)) {
                enemies_vector.erase(i);
                hitEnemy = true;
            }
        }
        
        // Check if the bullet has exceeded its lifetime (4 seconds)
        if (bullet_time_start - it->createTime > 2 || hitEnemy) {
            it = bullets.erase(it);  // Remove expired bullet
        } else {
            it++;
        }
    }
    
    // Store the previous position of the tank for future use
    prevX = translateX;
    prevZ = translateZ;

    if (enemies_vector.size() == 0) {
        window->Close();
        cout << "=====================================================\n" << 
        "==================YOU WIN THE GAME===================\n" << 
        "================YOU KILLED 5 ENEMIES=================\n";
    }
    
}

void Tema2::FrameEnd()
{
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.5f;
    if (window->KeyHold(GLFW_KEY_A))
    {
        camera->RotateThirdPerson_OY(0.00695);
        angularStepOY += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D))
    {
        camera->RotateThirdPerson_OY(-0.00695);
        angularStepOY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_W))
    {
        bool canMoveForward = true;
        for (const auto& enemy : enemies_vector) {
                float distance = glm::distance(glm::vec3(translateX, 0, translateZ), glm::vec3(std::get<0>(enemy), 0, std::get<2>(enemy)));
                if (distance < 5) {
                    canMoveForward = false;
                    break;
                }
            }
            if (canMoveForward) {
                camera->TranslateForward(deltaTime * cameraSpeed);
                translateX += glm::sin(angularStepOY - 1.57) * deltaTime * 5;
                translateZ += glm::cos(angularStepOY - 1.57) * deltaTime * 5;
            }
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        camera->TranslateForward(-deltaTime * cameraSpeed);
        translateX -= glm::sin(angularStepOY-1.57) * deltaTime * 5;
        translateZ -= glm::cos(angularStepOY-1.57) * deltaTime * 5;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float sensitivity = 0.005f;
    angularStepOY_tureta -= sensitivity * static_cast<float>(deltaX);

    angularStepOY_tureta = glm::clamp(angularStepOY_tureta, -glm::pi<float>(), glm::pi<float>());
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - bullet_time_start;
        Bullet newBullet;
        newBullet.x = translateX;
        newBullet.y = 0;
        newBullet.z = translateZ;
        newBullet.createTime = currentTime;
        newBullet.initialOrientation = angularStepOY_tureta + angularStepOY;  // Store the initial orientation
        newBullet.trajectory.push_back(glm::vec3(newBullet.x, newBullet.y, newBullet.z));
        bullets.push_back(newBullet);

        bullet_time_start = currentTime;  // Update the time for the next bullet
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
