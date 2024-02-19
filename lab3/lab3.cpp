#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;
    


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
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

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    float cx, cy;
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    translateLimit = 1;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    scaleLimit = 1;

    // Initialize angularStep   
    angularStep = 0;


    //// Bonus values
    //translateCar = 0;
    //scaleCarBody = 3;
    //wheelRotation = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    /*Mesh* masina = object2D::CreateSquare("masina", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(masina);

    Mesh* roata = object2D::CreateSquare("roata", corner, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(roata);*/

    //bonus
    bounce_up = 0;
    advance = 0;
    rotate = 0;
    acc = 0;
    gravity = 0;

    Mesh* bouncy = object2D::CreateSquare("bouncy", corner, squareSide, glm::vec3(0.8, 0.5, 0.5), true);
    AddMeshToList(bouncy);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 250);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!
    
    // TRANSLATIE STANGA - DREAPTA
    {
        if (translateX > 90) {
            translateLimit = -1;
        }
        else if (translateX < -90) {
            translateLimit = 1;
        }
        
        translateX += translateLimit * 100 * deltaTimeSeconds;
        
        
        modelMatrix *= transform2D::Translate(translateX, translateY);
    }

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 250);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    {   // ROTATIE
        angularStep -= deltaTimeSeconds * 5.f;
        
        modelMatrix *= transform2D::Translate(50.f, 50.f);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Translate(-50.f, -50.f);
    }

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 250);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    {
        if (scaleX > 2.f || scaleY > 2.f) {
            scaleLimit = -1;
        }
        else if (scaleX < -2.f || scaleY < -2.f) {
            scaleLimit = 1;
        }

        scaleX += scaleLimit * deltaTimeSeconds;
        scaleY += scaleLimit * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(50.f, 50.f);
        modelMatrix *= transform2D::Scale(scaleX, scaleY);
        modelMatrix *= transform2D::Translate(-50.f, -50.f);

    }

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    // testing rotation

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(800, 250);

    modelMatrix *= transform2D::Translate(-250, 0);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(250, 0);
    
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


    /* BONUS: Alungiti un patrat, care va fi corpul masinii. Celelalte 2 patrate vor fi rotile masinii.
    Masina va merge intr-o directie (deci toate patratele vor merge in directia aceea), iar
    rotile masinii se vor invarti. */
    {
    //    // Car body
    //    modelMatrix = glm::mat3(1);
    //    modelMatrix *= transform2D::Translate(250, 550);

    //    modelMatrix *= transform2D::Translate(50.f, 50.f);
    //    modelMatrix *= transform2D::Scale(scaleCarBody, 1);
    //    modelMatrix *= transform2D::Translate(-50.f, -50.f);

    //    translateCar += 50 * deltaTimeSeconds;
    //    modelMatrix *= transform2D::Translate(translateCar, 0);

    //    RenderMesh2D(meshes["masina"], shaders["VertexColor"], modelMatrix);

    //    // Car roof
    //    modelMatrix = glm::mat3(1);
    //    modelMatrix *= transform2D::Translate(300, 600);

    //    modelMatrix *= transform2D::Translate(scaleCarBody * translateCar, 0);

    //    RenderMesh2D(meshes["masina"], shaders["VertexColor"], modelMatrix);
    //    
    //    // Wheel 1
    //    modelMatrix = glm::mat3(1);
    //    modelMatrix *= transform2D::Translate(170, 470);

    //    wheelRotation -= deltaTimeSeconds * 5.f;

    //    modelMatrix *= transform2D::Translate(scaleCarBody * translateCar, 0);

    //    modelMatrix *= transform2D::Translate(50.f, 50.f);
    //    modelMatrix *= transform2D::Rotate(wheelRotation);
    //    modelMatrix *= transform2D::Translate(-50.f, -50.f);

    //    RenderMesh2D(meshes["roata"], shaders["VertexColor"], modelMatrix);

    //    // Wheel 2
    //    modelMatrix = glm::mat3(1);
    //    modelMatrix *= transform2D::Translate(330, 470);

    //    modelMatrix *= transform2D::Translate(scaleCarBody * translateCar, 0);

    //    modelMatrix *= transform2D::Translate(50.f, 50.f);
    //    modelMatrix *= transform2D::Rotate(wheelRotation);
    //    modelMatrix *= transform2D::Translate(-50.f, -50.f);

    //    RenderMesh2D(meshes["roata"], shaders["VertexColor"], modelMatrix);

    //    if (translateCar > 380) {    // Exit stage right, Enter stage left
    //        translateCar = -150;
    //    }

    }

    /*BONUS: patrat saltaret. Sare, se roteste o data si aterizeaza, se translateaza in dreapta*/
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(250, 450);

        if (bounce_up > 90) {
            gravity = -1;
        }
        else if (bounce_up <= 0) {
            gravity = 1;
        }

        advance += 100 * deltaTimeSeconds;

        bounce_up += gravity * 150 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(advance, 0);

        modelMatrix *= transform2D::Translate(0, bounce_up);

        acc -= deltaTimeSeconds * 2.f;

        modelMatrix *= transform2D::Translate(50.f, 50.f);
        modelMatrix *= transform2D::Rotate(acc);
        modelMatrix *= transform2D::Translate(-50.f, -50.f);

        
        RenderMesh2D(meshes["bouncy"], shaders["VertexColor"], modelMatrix);
    }
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
