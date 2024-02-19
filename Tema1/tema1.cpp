#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include <random>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;


    float cx, cy;
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    directionX = 1; directionY = 1;


    // Initialize angularStep   
    srand((time(NULL)));    
    a = 1 + rand() % 11;

    angularStep = a * M_PI / 12;
    newA = angularStep;

    side = 120;

    // spawn positions
    duckX = resolution.x / 2;
    duckY = 37.5;

    // original speed
    speed = 250;

    // gameplay setup
    after10 = now + 10;
    duckStatus = 0;       
    inputStatus = 1;

    maxBullets = 3;
    hearts = 3; bullets = maxBullets; score = 0; level = 1;
    runningGame = 1;


    // bullet (body and tip)
    Mesh* tip = object2D::CreateCircle("tip", corner, 15, glm::vec3(1, 1, 0));
    AddMeshToList(tip);
    Mesh* bullet = object2D::MakeSquare("bullet", corner, 30, glm::vec3(0.65f, 0.4f, 0.3f), true);
    AddMeshToList(bullet);


    // hearts
    Mesh* heartC = object2D::CreateCircle("heartC", corner, 15, glm::vec3(1, 0.5f, 0.5f));
    AddMeshToList(heartC);
    Mesh* heartT = object2D::CreateTriangle("heartT", corner, 30, glm::vec3(1, 0.5f, 0.5f));
    AddMeshToList(heartT);


    // score 
    Mesh* wireframe = object2D::MakeSquare("wireframe", corner, 30, glm::vec3(0.1f, 0.1f, 0.1f), false);
    AddMeshToList(wireframe);
    Mesh* point = object2D::MakeSquare("point", corner, 30, glm::vec3(0.1f, 0.1f, 1), true);
    AddMeshToList(point);

    // land
    Mesh * land = object2D::MakeSquare("land", corner, resolution.y / 3, glm::vec3(0.3f, 0.8f, 0.3f), true);
    AddMeshToList(land);


    // hitbox
    Mesh* square = object2D::MakeSquare("square", corner, side, glm::vec3(1, 0, 0), false);
    AddMeshToList(square);


    // body
    Mesh* triangle = object2D::CreateTriangle("triangle", corner, 150, glm::vec3(0.45f, 0.31f, 0.2f));
    AddMeshToList(triangle);
    // body : +1UP
    Mesh* triangle_heal = object2D::CreateTriangle("triangle_heal", corner, 150, glm::vec3(0.9f, 0.9f, 0.9f));
    AddMeshToList(triangle_heal);
    // body : +1 bullet
    Mesh* triangle_ammo = object2D::CreateTriangle("triangle_ammo", corner, 150, glm::vec3(0.65f, 0.4f, 0.3f));
    AddMeshToList(triangle_ammo);
    // body : damage 1 heart
    Mesh* triangle_bad = object2D::CreateTriangle("triangle_bad", corner, 150, glm::vec3(0, 0.4f, 0.3f));
    AddMeshToList(triangle_bad);

    // head
    Mesh* circle = object2D::CreateCircle("circle", corner, 25, glm::vec3(0, 1, 0));
    AddMeshToList(circle);
    
    // wings
    Mesh* wing = object2D::CreateTriangle("wing", corner, 125, glm::vec3(0.45f, 0.31f, 0.2f));
    AddMeshToList(wing);

    // text square
    Mesh* line = object2D::MakeSquare("line", corner, 10, glm::vec3(0, 0, 0), true);
    AddMeshToList(line);

    // crosshair square
    Mesh* cross = object2D::MakeSquare("cross", corner, 10, glm::vec3(0.8, 0.5, 0.5), true);
    AddMeshToList(cross);

    limX = resolution.x;
    limY = resolution.y;

    // origin of body, hair and wings
    bodyX = duckX; bodyY = duckY;
    headX = duckX + 75; headY = duckY;
    // rotation of wings
    flap = 0; flap_move = -1;


    // crosshair position
    crossX = limX / 2;
    crossY = limY / 2;
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5, 0.8f, 0.9f, 1); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}


void Tema1::Update(float deltaTimeSeconds)
{
    if ((duckStatus == 2 && a != 4) || (duckStatus == 1 && a == 4)) {
        glClearColor(0.9f, 0.6f, 0.6f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else {
        glClearColor(0.5, 0.8f, 0.9f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // crosshair
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(crossX, crossY);
        modelMatrix *= transform2D::Scale(3, 1);
        RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(crossX, crossY);
        modelMatrix *= transform2D::Scale(1, 3);
        RenderMesh2D(meshes["cross"], shaders["VertexColor"], modelMatrix);
    }


    // land (for backround)
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(limX, 1);

    RenderMesh2D(meshes["land"], shaders["VertexColor"], modelMatrix);


    // game over
    if (runningGame == 0) {
        return;
    }


    // bullet body and tip
    for (int i = 0; i < bullets * 45; i += 45)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + i, 650);
        modelMatrix *= transform2D::Scale(1, 3);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + i, 695);
        RenderMesh2D(meshes["tip"], shaders["VertexColor"], modelMatrix);
    }

    // hearts
    for (int i = 0; i < hearts * 45; i += 45) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(965 + i, 650);
        modelMatrix *= transform2D::Scale(3, 1);
        modelMatrix *= transform2D::Rotate(3 * M_PI / 2);

        RenderMesh2D(meshes["heartT"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(954 + i, 669);
        modelMatrix *= transform2D::Scale(0.75, 0.75);
        RenderMesh2D(meshes["heartC"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(976 + i, 669);
        modelMatrix *= transform2D::Scale(0.75, 0.75);
        RenderMesh2D(meshes["heartC"], shaders["VertexColor"], modelMatrix);
    }

    // mini-boss at round 10
    if (score == 9) {
        speed += level;
    }
    // increase speed at each level
    else if (score == 10) {
        score = 0;
        speed -= level;
        speed += level / 3;
        level++;
    }


    // score
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1100, 600);
        modelMatrix *= transform2D::Scale(10, 1);

        RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < score * 30; i += 30) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(965 + i, 600);
            RenderMesh2D(meshes["point"], shaders["VertexColor"], modelMatrix);
        }
    }

    // lvl (dont know how to add text)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(900, 555);
        modelMatrix *= transform2D::Scale(0.5, 3.5);
        RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(915, 550);

        modelMatrix *= transform2D::Rotate(M_PI / 8);
        modelMatrix *= transform2D::Scale(0.5, 3);
        RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(925, 550);

        modelMatrix *= transform2D::Rotate(7 * M_PI / 8);
        modelMatrix *= transform2D::Scale(0.5, 3);
        RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(940, 555);
        modelMatrix *= transform2D::Scale(0.5, 3.5);
        RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < level * 45; i += 45) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(965 + i, 550);
            RenderMesh2D(meshes["tip"], shaders["VertexColor"], modelMatrix);
        }
    }

    if (duckStatus == 0) {

        // set timer, after 10 seconds you lose a life
        now = glfwGetTime();
        if ((now > 10 && now >= after10) || bullets == 0)
        {
            if (a != 4)
                hearts--;
            duckStatus = 2;
        }

        // collisions and rotations
        if (translateX > limX / 2) {
            translateX = limX / 2;
            directionX = -(directionX);

            if (newA > 0 && newA < M_PI / 2)
                newA = M_PI - newA;
            else if (newA > 3 * M_PI / 2 && newA < 2 * M_PI)
                newA = 3 * M_PI - newA;
        }
        else if (translateX < -(limX / 2)) {
            translateX = -(limX / 2);
            directionX = -(directionX);

            if (newA > M_PI / 2 && newA < M_PI)
                newA = M_PI - newA;
            else if (newA > M_PI && newA < 3 * M_PI / 2)
                newA = 3 * M_PI - newA;
        }

        if (translateY > limY - 50) {
            translateY = limY - 50;
            directionY = -1;

            newA = 2 * M_PI - newA;
        }
        else if (translateY < 0) {
            translateY = 0;
            directionY = 1;

            newA = 2 * M_PI - newA;
        }
    }

    // duck was hit
    else if (duckStatus == 1) {

        inputStatus = 0;
        directionY = 1;

        // descend duck
        angularStep = 3 * M_PI / 2;
        newA = 3 * M_PI / 2;

        // reset game
        if (translateY < -20) {
            translateX = 0;
            translateY = 0;


            directionX = 1; directionY = 1;

            bodyX = duckX; bodyY = duckY;
            headX = duckX + 75; headY = duckY;

            if (a == 7)
                hearts++;
            else if (a == 5)
                maxBullets += 1;
            else if (a == 4)
                hearts--;

            a = 1 + rand() % 11;
            angularStep = a * M_PI / 12;
            newA = angularStep;

            duckStatus = 0;
            after10 = now + 10;
            inputStatus = 1;
            bullets = maxBullets;

            if (hearts == 0) {
                duckStatus = 2;
                runningGame = 0;
            }
        }
    }

    // duck escaped
    else if (duckStatus == 2) {

        inputStatus = 0;
        directionY = 1;

        // ascend duck
        angularStep = M_PI / 2;
        newA = M_PI / 2;

        // reset game
        if (translateY > limY + 20) {
            translateX = 0;
            translateY = 0;


            directionX = 1; directionY = 1;

            bodyX = duckX; bodyY = duckY;
            headX = duckX + 75; headY = duckY;

            a = 1 + rand() % 11;
            angularStep = a * M_PI / 12;
            newA = angularStep;

            duckStatus = 0;
            after10 = now + 10;
            inputStatus = 1;
            bullets = maxBullets;

            if (hearts == 0) {
                duckStatus = 2;
                runningGame = 0;
            }
        }
    }




    // hitbox
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(duckX, duckY);

    translateX += directionX * speed * deltaTimeSeconds * cos(angularStep);
    translateY += directionY * speed * deltaTimeSeconds * sin(angularStep);

    bodyX += translateX; bodyY += translateY;
    headX += translateX; headY += translateY;

    modelMatrix *= transform2D::Translate(translateX, translateY);

    // Only shown if needed in the evaluation
    //RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);


    // duck body
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(duckX, duckY);
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Rotate(newA);
    }


    // head
    {
        headMatrix = glm::mat3(1);

        headMatrix *= transform2D::Translate(duckX + 75, duckY);
        headMatrix *= transform2D::Translate(translateX, translateY);

        headMatrix *= transform2D::Translate(bodyX - headX, bodyY - headY);
        headMatrix *= transform2D::Rotate(newA);
        headMatrix *= transform2D::Translate(-bodyX + headX, -bodyY + headY);
    }

    // render head
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], headMatrix);

    // render body
    {
        if (a == 7)
            RenderMesh2D(meshes["triangle_heal"], shaders["VertexColor"], modelMatrix);
        else if (a == 5)
            RenderMesh2D(meshes["triangle_ammo"], shaders["VertexColor"], modelMatrix);
        else if (a == 4)
            RenderMesh2D(meshes["triangle_bad"], shaders["VertexColor"], modelMatrix);
        else
            RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
    }

    // wings movement
    {
        if (flap > M_PI / 8) {
            flap = M_PI / 8;
            flap_move = -flap_move;
        }
        else if (flap < -M_PI / 8) {
            flap = -M_PI / 8;
            flap_move = -flap_move;
        }

        flap += flap_move * deltaTimeSeconds * 2;
    }

    // wing1
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(duckX, duckY);
        modelMatrix *= transform2D::Translate(translateX, translateY);

        modelMatrix *= transform2D::Rotate(newA);
        modelMatrix *= transform2D::Rotate(-M_PI / 2);
        modelMatrix *= transform2D::Rotate(flap);

        RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
    }

    // wing2
    {
        modelMatrix = glm::mat3(1);

        modelMatrix *= transform2D::Translate(duckX, duckY);
        modelMatrix *= transform2D::Translate(translateX, translateY);

        modelMatrix *= transform2D::Rotate(newA);
        modelMatrix *= transform2D::Rotate(M_PI / 2);
        modelMatrix *= transform2D::Rotate(-flap);

        RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
    }
    
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Reset game
    // RESET EVERYTHING
    if (key == GLFW_KEY_R) {
        translateX = 0;
        translateY = 0;

        directionX = 1; directionY = 1;


        // Initialize angularStep   
        srand((time(NULL)));
        a = 1 + rand() % 11;

        angularStep = a * M_PI / 12;
        newA = angularStep;


        speed = 250;

        after10 = now + 10;
        duckStatus = 0;
        inputStatus = 1;

        maxBullets = 3;
        hearts = 3; bullets = maxBullets; score = 0; level = 1;
        runningGame = 1;

        bodyX = duckX; bodyY = duckY;
        headX = duckX + 75; headY = duckY;

        flap = 0; flap_move = -1;

        crossX = limX / 2;
        crossY = limY / 2;
    }
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Track the crosshair
    float cursorX = mouseX;
    float cursorY = limY - mouseY;

    crossX = cursorX;
    crossY = cursorY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // coordinates for target
    float cursorX = mouseX;
    float cursorY = limY - mouseY;

    float targetX = translateX + duckX;
    float targetY = translateY + duckY;


    if (inputStatus == 1) {
        bullets--;
        if ((cursorX > (targetX - (side / 2)) && cursorX < (targetX + (side / 2))) &&
            (cursorY > (targetY - (side / 2)) && cursorY < (targetY + (side / 2)))) {
            duckStatus = 1;
            score++;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
