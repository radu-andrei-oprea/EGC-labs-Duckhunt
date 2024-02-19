#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/t2_camera.h"


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

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::T2_Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float left, right, bottom, top;
        float zNear, zFar;
        float fov, aspect;

        // Body Part Coordinates
        float headMoveX, headMoveZ, originalHX, originalHZ;
        float torsoMoveX, torsoMoveZ, originalX, originalZ;
        float leftAMoveX, leftAMoveZ, originalLAX, originalLAZ;
        float rightAMoveX, rightAMoveZ, originalRAX, originalRAZ;
        float leftLMoveX, leftLMoveZ, originalLLX, originalLLZ;
        float rightLMoveX, rightLMoveZ, originalRLX, originalRLZ;

        // Distance between an arm/leg and the center of the body
        float legDist;
        float armDist;
        
        // Rotation angles and limits
        float partRotation, animationRotation;
        int angleLimit;

        // Direction indicator
        int direction;

        // angles
        float turnLangle, turnRangle, turnBackNegAngle, turnBackPosAngle, camerAngle;

        //distante
        float C2PDistance;

    };
}   // namespace m1
