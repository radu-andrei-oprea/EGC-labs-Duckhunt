#include "lab_m1/Tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

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
    renderCameraTarget = false;

    camera = new implemented::T2_Camera();
    camera->Set(glm::vec3(0, 2.5, 0.3), glm::vec3(0, 2.5, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters

    fov = 60;
    aspect = window->props.aspectRatio;
    zNear = 0.01f;
    zFar = 200.0f;
    
    projectionMatrix = glm::perspective(RADIANS(fov), aspect, zNear, zFar);
    
    left = -5;
    right = 20;
    top = 20;
    bottom = -5;


    headMoveX = 0; headMoveZ = 0;
    torsoMoveX = 0; torsoMoveZ = 0;
    leftAMoveX = 0; leftAMoveZ = 0;
    rightAMoveX = 0; rightAMoveZ = 0;
    leftLMoveX = 0; leftLMoveZ = 0;
    rightLMoveX = 0; rightLMoveZ = 0;

    originalX = 0; originalZ = -2;
    originalLAX = -0.5; originalLAZ = -2;
    originalRAX = 0.5; originalRAZ = -2;
    originalLLX = -0.2; originalLLZ = -2;
    originalRLX = 0.2; originalRLZ = -2;
    partRotation = 0; animationRotation = 0;

    legDist = 0; armDist = 0.5;
    //turn = 0;

    turnLangle = M_PI / 2;
    turnRangle = -M_PI / 2;

    turnBackNegAngle = -M_PI;
    turnBackPosAngle = M_PI;

    camerAngle = 0;


    direction = 0;
    

    C2PDistance = camera->GetTargetPosition().x - originalX;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    {
        //glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        //modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

       // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        // glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        //modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        //RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        //glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        //RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.
    {
        // glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 1, -5));
       // modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
        //modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 1, 1));

       // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        //glm::mat4 modelMatrix = glm::mat4(1);
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 1, -5));
        //modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(1, 0, 0));
       // modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 3, 1));

       // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }


    // Torso
    {   
        glm::mat4 torso = glm::mat4(1);
        torso = glm::translate(torso, glm::vec3(0, 1.5, -2));
        torso = glm::translate(torso, glm::vec3(torsoMoveX, 0, torsoMoveZ));
        torso = glm::rotate(torso, partRotation, glm::vec3(0, 1, 0));
        torso = glm::scale(torso, glm::vec3(0.8, 1.1, 0.6));
        RenderMesh(meshes["box"], shaders["VertexNormal"], torso);
    }
    // Left leg
    {   
        glm::mat4 leftLeg = glm::mat4(1);
        leftLeg = glm::translate(leftLeg, glm::vec3(-0.2, 0.5, -2));
        leftLeg = glm::translate(leftLeg, glm::vec3(leftLMoveX, 0, leftLMoveZ)); 

        {
            leftLeg = glm::translate(leftLeg, glm::vec3(originalX - originalLLX, 0, originalZ - originalLLZ));
            leftLeg = glm::rotate(leftLeg, partRotation, glm::vec3(0, 1, 0));

            if (direction == 0) {
                leftLeg = glm::rotate(leftLeg, animationRotation, glm::vec3(1, 0, 0));
            }
            else {
                leftLeg = glm::rotate(leftLeg, animationRotation, glm::vec3(0, 0, 1));
            }
                
            leftLeg = glm::translate(leftLeg, glm::vec3(-originalX + originalLLX, 0,-originalZ + originalLLZ));
        }

        leftLeg = glm::scale(leftLeg, glm::vec3(0.3, 1, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], leftLeg);
    }
    // Right leg
    {   
        glm::mat4 rightLeg = glm::mat4(1);
        rightLeg = glm::translate(rightLeg, glm::vec3(0.2, 0.5, -2));
        rightLeg = glm::translate(rightLeg, glm::vec3(rightLMoveX, 0, rightLMoveZ));
        {
            rightLeg = glm::translate(rightLeg, glm::vec3(originalX - originalRLX, 0, originalZ - originalRLZ));
            rightLeg = glm::rotate(rightLeg, partRotation, glm::vec3(0, 1, 0));
            
            if (direction == 0) {
                rightLeg = glm::rotate(rightLeg, -animationRotation, glm::vec3(1, 0, 0));
            }
            else {
                rightLeg = glm::rotate(rightLeg, -animationRotation, glm::vec3(0, 0, 1));
            }         
            
            rightLeg = glm::translate(rightLeg, glm::vec3(-originalX + originalRLX, 0, -originalZ + originalRLZ));
        }
        rightLeg = glm::scale(rightLeg, glm::vec3(0.3, 1, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], rightLeg);
    }
    // Left arm
    {   
        glm::mat4 leftArm = glm::mat4(1);
        leftArm = glm::translate(leftArm, glm::vec3(-0.5, 1.5, -2));
        leftArm = glm::translate(leftArm, glm::vec3(leftAMoveX, 0, rightAMoveZ));
        {   
            leftArm = glm::translate(leftArm, glm::vec3(originalX - originalLAX, 0, originalZ - originalLAZ));
            leftArm = glm::rotate(leftArm, partRotation, glm::vec3(0, 1, 0));

            if (direction == 0) {
                leftArm = glm::rotate(leftArm, -animationRotation, glm::vec3(1, 0, 0));
            }
            else {
                leftArm = glm::rotate(leftArm, -animationRotation, glm::vec3(0, 0, 1));
            }

            leftArm = glm::translate(leftArm, glm::vec3(-originalX + originalLAX, 0, -originalZ + originalLAZ));
        }
        leftArm = glm::scale(leftArm, glm::vec3(0.3, 1, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], leftArm);
    }
    // Right arm
    {   
        glm::mat4 rightArm = glm::mat4(1);
        rightArm = glm::translate(rightArm, glm::vec3(0.5, 1.5, -2));
        rightArm = glm::translate(rightArm, glm::vec3(rightAMoveX, 0, rightAMoveZ));
        {
            rightArm = glm::translate(rightArm, glm::vec3(originalX - originalRAX, 0, originalZ - originalRAZ));
            rightArm = glm::rotate(rightArm, partRotation, glm::vec3(0, 1, 0));

            if (direction == 0) {
                rightArm = glm::rotate(rightArm, animationRotation, glm::vec3(1, 0, 0));
            }
            else {
                rightArm = glm::rotate(rightArm, animationRotation, glm::vec3(0, 0, 1));
            }

            rightArm = glm::translate(rightArm, glm::vec3(-originalX + originalRAX, 0, -originalZ + originalRAZ));
        }

        rightArm = glm::scale(rightArm, glm::vec3(0.3, 1, 0.3));
        RenderMesh(meshes["box"], shaders["VertexNormal"], rightArm);
    }
    // Head
    {   
        glm::mat4 head = glm::mat4(1);
        head = glm::translate(head, glm::vec3(0, 2.3, -2));
        head = glm::translate(head, glm::vec3(headMoveX, 0, headMoveZ));
        head = glm::rotate(head, partRotation, glm::vec3(0, 1, 0));
        head = glm::scale(head, glm::vec3(0.5, 0.5, 0.5));
        RenderMesh(meshes["box"], shaders["VertexNormal"], head);
    }

    /*{
        glm::mat4 floor = glm::mat4(1);
        floor = glm::translate(floor, glm::vec3(0, 0, 0));
        floor = glm::scale(floor, glm::vec3(50, 0, 50));
        RenderMesh(meshes["box"], shaders["VertexNormal"], floor);
    }*/



    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }

    cout << camerAngle <<endl;
    //cout << camera->GetTargetPosition() << " " << originalX << " " << originalZ << endl;
    //cout << camera->GetTargetPosition() << endl;
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
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
    float moveSpeed = 2.0f;
    float cameraSpeed = 2.0f;


    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {

            direction = 0;

            headMoveZ -= deltaTime * moveSpeed;
            torsoMoveZ -= deltaTime * moveSpeed;
            leftAMoveZ -= deltaTime * moveSpeed;
            rightAMoveZ -= deltaTime * moveSpeed;
            leftLMoveZ -= deltaTime * moveSpeed;
            rightLMoveZ -= deltaTime * moveSpeed;

            originalZ -= deltaTime * moveSpeed;
            originalLLZ -= deltaTime * moveSpeed;
            originalRLZ -= deltaTime * moveSpeed;
            originalLAZ -= deltaTime * moveSpeed;
            originalRAZ -= deltaTime * moveSpeed;


            if (partRotation > 0) {
                partRotation -= deltaTime * moveSpeed * 5;
                if (partRotation <= 0) {
                    partRotation = 0;
                }
            }
            else if (partRotation < 0) {
                partRotation += deltaTime * moveSpeed * 5;
                if (partRotation >= 0) {
                    partRotation = 0;
                }
            }

            if (angleLimit == 0) {
                animationRotation += deltaTime * moveSpeed;
                if (animationRotation >= M_PI / 16) {
                    angleLimit = 1;
                }
            }
            else {
                animationRotation -= deltaTime * moveSpeed;
                if (animationRotation <= -M_PI / 16) {
                    angleLimit = 0;
                }
            }


            if (camerAngle > 0) {
                camera->RotateFirstPerson_OY(-camerAngle);
                camerAngle += -camerAngle;
            }
            else if (camerAngle < 0) {
                camera->RotateFirstPerson_OY(-camerAngle);
                camerAngle += -camerAngle;
            }

            if (camera->GetTargetPosition().x > originalX) {
                camera->TranslateRight(-deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().x < originalX)
                    camera->TranslateRight(originalX - camera->GetTargetPosition().x);
                else camera->TranslateRight(-camera->GetTargetPosition().x + originalX);
            }
            else if (camera->GetTargetPosition().x < originalX){
                camera->TranslateRight(deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().x > originalX)
                    camera->TranslateRight(-camera->GetTargetPosition().x + originalX);
                else camera->TranslateRight(originalX - camera->GetTargetPosition().x);
            }

            if (camera->GetTargetPosition().z - originalZ > C2PDistance) {
                camera->MoveForward(deltaTime * cameraSpeed);
            }
           
        }

        if (window->KeyHold(GLFW_KEY_S)) {

            direction = 0;

            headMoveZ += deltaTime * moveSpeed;
            torsoMoveZ += deltaTime * moveSpeed;
            leftAMoveZ += deltaTime * moveSpeed;
            rightAMoveZ += deltaTime * moveSpeed;
            leftLMoveZ += deltaTime * moveSpeed;
            rightLMoveZ += deltaTime * moveSpeed;

            originalZ += deltaTime * moveSpeed;
            originalLLZ += deltaTime * moveSpeed;
            originalRLZ += deltaTime * moveSpeed;
            originalLAZ += deltaTime * moveSpeed;
            originalRAZ += deltaTime * moveSpeed;


            if (partRotation >= 0) {
                partRotation += deltaTime * moveSpeed * 5;
                if (partRotation >= turnBackPosAngle) {
                    partRotation = turnBackPosAngle;
                }
            }

            else if (partRotation < 0) {
                partRotation -= deltaTime * moveSpeed * 5;
                if (partRotation <= turnBackNegAngle) {
                    partRotation = turnBackNegAngle;
                }
            }

            if (angleLimit == 0) {
                animationRotation += deltaTime * moveSpeed;
                if (animationRotation >= M_PI / 16) {
                    angleLimit = 1;
                }
            }
            else {
                animationRotation -= deltaTime * moveSpeed;
                if (animationRotation <= -M_PI / 16) {
                    angleLimit = 0;
                }
            }
            
            if (camerAngle < M_PI && camerAngle != 0) {
                camera->RotateFirstPerson_OY(M_PI - camerAngle);
                camerAngle += M_PI - camerAngle;
            }
            else if (camerAngle > -M_PI && camerAngle != 0) {
                camera->RotateFirstPerson_OY(-M_PI +camerAngle);
                camerAngle += -M_PI + camerAngle;
            }
            

            if (camera->GetTargetPosition().x > originalX) {
                camera->TranslateRight(-deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().x < originalX)
                    camera->TranslateRight(originalX - camera->GetTargetPosition().x);
                else camera->TranslateRight(-camera->GetTargetPosition().x + originalX);
            }
            else if (camera->GetTargetPosition().x < originalX) {
                camera->TranslateRight(deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().x > originalX)
                    camera->TranslateRight(-camera->GetTargetPosition().x + originalX);
                else camera->TranslateRight(originalX - camera->GetTargetPosition().x);
            }

            if (camera->GetTargetPosition().z - originalZ > C2PDistance) {
                camera->MoveForward(deltaTime * cameraSpeed);
            }
            
            
        }

        if (window->KeyHold(GLFW_KEY_A)) {

            direction = 0;

            headMoveX -= deltaTime * moveSpeed;
            torsoMoveX -= deltaTime * moveSpeed;
            leftAMoveX -= deltaTime * moveSpeed;
            rightAMoveX -= deltaTime * moveSpeed;
            leftLMoveX -= deltaTime * moveSpeed;
            rightLMoveX -= deltaTime * moveSpeed;

            originalX -= deltaTime * moveSpeed;
            originalLLX -= deltaTime * moveSpeed;
            originalRLX -= deltaTime * moveSpeed;
            originalLAX -= deltaTime * moveSpeed;
            originalRAX -= deltaTime * moveSpeed;

            if (partRotation <= turnBackNegAngle) {
                partRotation = turnBackPosAngle;
            }

            if (partRotation < turnLangle) {
                partRotation += deltaTime * moveSpeed * 5;
                if (partRotation >= turnLangle) {
                    partRotation = turnLangle;
                }
            }
            else if (partRotation > turnLangle) {
                partRotation -= deltaTime * moveSpeed * 5;
                if (partRotation <= turnLangle) {
                    partRotation = turnLangle;
                }
            }

            // Arms and Legs moving
            if (angleLimit == 0) {
                animationRotation += deltaTime * moveSpeed;
                if (animationRotation >= M_PI / 16) {
                    angleLimit = 1;
                }
            }
            else {
                animationRotation -= deltaTime * moveSpeed;
                if (animationRotation <= -M_PI / 16) {
                    angleLimit = 0;
                }
            }


            if (camerAngle < M_PI_2) {
                camera->RotateFirstPerson_OY(M_PI_2 - camerAngle);
                camerAngle += M_PI_2 - camerAngle;
            }
            else if (camerAngle > M_PI_2){
                camera->RotateFirstPerson_OY(M_PI_2 - camerAngle);
                camerAngle += camerAngle - M_PI_2;
            }
            

            if (camera->GetTargetPosition().z > originalZ) {
                camera->TranslateRight(deltaTime* cameraSpeed);
                if (camera->GetTargetPosition().z < originalZ)
                    camera->TranslateRight(originalZ - camera->GetTargetPosition().z);
                else camera->TranslateRight(-originalZ + camera->GetTargetPosition().z);
            }
            else if (camera->GetTargetPosition().z < originalZ) {
                camera->TranslateRight(-deltaTime* cameraSpeed);
                if (camera->GetTargetPosition().z > originalZ)
                    camera->TranslateRight(-originalZ + camera->GetTargetPosition().z);
                else camera->TranslateRight(originalZ - camera->GetTargetPosition().z);
            }

            if (camera->GetTargetPosition().x - originalX > C2PDistance) {
                camera->MoveForward(deltaTime* cameraSpeed);    
            }

            
        }

        if (window->KeyHold(GLFW_KEY_D)) {

            direction = 0;

            headMoveX += deltaTime * moveSpeed;
            torsoMoveX += deltaTime * moveSpeed;
            leftAMoveX += deltaTime * moveSpeed;
            rightAMoveX += deltaTime * moveSpeed;
            leftLMoveX += deltaTime * moveSpeed;
            rightLMoveX += deltaTime * moveSpeed;

            originalX += deltaTime * moveSpeed;
            originalLLX += deltaTime * moveSpeed;
            originalRLX += deltaTime * moveSpeed;
            originalLAX += deltaTime * moveSpeed;
            originalRAX += deltaTime * moveSpeed;

            if (partRotation >= turnBackPosAngle) {
                partRotation = turnBackNegAngle;
            }


            if (partRotation > turnRangle) {
                partRotation -= deltaTime * moveSpeed * 5;

                if (partRotation <= turnRangle) {
                    partRotation = turnRangle;
                }
            }
            else if (partRotation < turnRangle) {
                partRotation += deltaTime * moveSpeed * 5;

                if (partRotation >= turnRangle) {
                    partRotation = turnRangle;
                }
            }

            if (angleLimit == 0) {
                animationRotation += deltaTime * moveSpeed;
                if (animationRotation >= M_PI / 16) {
                    angleLimit = 1;
                }
            }
            else {
                animationRotation -= deltaTime * moveSpeed;
                if (animationRotation <= -M_PI / 16) {
                    angleLimit = 0;
                }
            }

            if (camerAngle > -M_PI_2) {
                camera->RotateFirstPerson_OY(-M_PI_2 - camerAngle);
                camerAngle += -M_PI_2 - camerAngle;
            }
            else if (camerAngle < -M_PI_2) {
                camera->RotateFirstPerson_OY( -M_PI_2 - camerAngle);
                camerAngle += -M_PI_2 - camerAngle;
            }

            // camera turn
            if (camera->GetTargetPosition().z > originalZ) {
                camera->TranslateRight(-deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().z < originalZ)
                    camera->TranslateRight(-originalZ + camera->GetTargetPosition().z);
                else camera->TranslateRight(originalZ - camera->GetTargetPosition().z);
            }
            else if (camera->GetTargetPosition().z < originalZ) {
                camera->TranslateRight(-deltaTime * cameraSpeed);
                if (camera->GetTargetPosition().z > originalZ)
                    camera->TranslateRight(-originalZ + camera->GetTargetPosition().z);
                else camera->TranslateRight(originalZ - camera->GetTargetPosition().z);
            }
            
            if (originalX - camera->GetTargetPosition().x > C2PDistance) {
                camera->MoveForward(deltaTime * cameraSpeed);
            }
            
        }
    }

    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;   

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }

    }

    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_L)) {
            fov += deltaTime * cameraSpeed * 10;
            projectionMatrix = glm::perspective(RADIANS(fov), aspect, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_K)) {
            fov -= deltaTime * cameraSpeed * 10;
            projectionMatrix = glm::perspective(RADIANS(fov), aspect, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_J)) {

            left += deltaTime * cameraSpeed * 10;
            right += deltaTime * cameraSpeed * 10;
            top += deltaTime * cameraSpeed * 10;
            bottom += deltaTime * cameraSpeed * 10;

            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_H)) {
        
            left -= deltaTime * cameraSpeed * 10;
            right -= deltaTime * cameraSpeed * 10;
            top -= deltaTime * cameraSpeed * 10;
            bottom -= deltaTime * cameraSpeed * 10;

            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }
    }
    
    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.


}


void Tema2::OnKeyPress(int key, int mods)
{   
    /*partRotation = 0;
    if (key == GLFW_KEY_A)
    {
        partRotation += M_PI;
    }

    if (key == GLFW_KEY_D)
    {
        partRotation -= M_PI;
    }*/
    if (key == GLFW_KEY_Z)
    {
        camera->RotateFirstPerson_OY(M_PI / 2);
    }
    


    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections

    if (key == GLFW_KEY_O)
    {
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    if (key == GLFW_KEY_P)
    {
        projectionMatrix = glm::perspective(RADIANS(fov), aspect , zNear, zFar);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
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
