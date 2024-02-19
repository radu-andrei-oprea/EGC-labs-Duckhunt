#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

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

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();

    miniX = 0; miniY = 0;
    
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    isSpacePressed = 0;

    // Bonus values
    pos2x = 0.0f; pos2y = 0.5f; pos2z = -1.5f;
    tBx = 0.0f; tBy = pos2y; tBz = 0.0f; aBy = 0;
    ax = 0; ay = 0; az = 0;

    negX = 1; negZ = 1;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);

    // modif pt bonus

    //rotatia in jurul unui obiect: translatie, rotatie, translatie inapoi
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOY(aBy);
    modelMatrix *= transform3D::Translate(0.0f, -0.5f, 1.5f);

    // translatie in pozitia dorita
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);

    // rotatie in jurul centrului sau
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    
    
    

    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, 
        miniViewportArea.width, miniViewportArea.height);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // TODO(student): render the scene again, in the new viewport
    RenderScene();
    DrawCoordinateSystem();
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    
    // CUB !
    if (window -> KeyHold(GLFW_KEY_W))
    {
        translateZ -= 3 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        translateX -= 3 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        translateZ += 3 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        translateX += 3 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_R))
    {
        translateY += 3 * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_F))
    {
        translateY -= 3 * deltaTime;

    }

    // CUB 2
    if (window->KeyHold(GLFW_KEY_1))
    {
        scaleX += 3 * deltaTime;
        scaleY += 3 * deltaTime;
        scaleZ += 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2))
    {
        scaleX -= 3 * deltaTime;
        scaleY -= 3 * deltaTime;
        scaleZ -= 3 * deltaTime;
    }

    // CUB 3

    if (window->KeyHold(GLFW_KEY_3))
    {
        angularStepOX -= 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4))
    {
        angularStepOX += 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5))
    {
        angularStepOY -= 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_6))
    {
        angularStepOY += 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_7))
    {
        angularStepOZ -= 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_8))
    {
        angularStepOZ += 3 * deltaTime;
    }
    
    // BONUS

    if (window->KeyHold(GLFW_KEY_9))
    {
        aBy -= 3 * deltaTime;        
    }

    if (window->KeyHold(GLFW_KEY_0))
    {
        aBy += 3 * deltaTime;
      
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_I)
    {
        miniViewportArea.y += 15;
    }

    if (key == GLFW_KEY_J)
    {
        miniViewportArea.x -= 15;
    }

    if (key == GLFW_KEY_K)
    {
        miniViewportArea.y -= 15;
    }

    if (key == GLFW_KEY_L)
    {
        miniViewportArea.x += 15;
    }

    if (key == GLFW_KEY_U)
    {
        miniViewportArea.x += 15;
        miniViewportArea.y += 15;
        miniViewportArea.width -= 2 * 15;
        miniViewportArea.height -= 2 * 15;
    }

    if (key == GLFW_KEY_O)
    {
        miniViewportArea.x -= 15;
        miniViewportArea.y -= 15;
        miniViewportArea.width += 2 * 15;
        miniViewportArea.height += 2 * 15;
    }

}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
