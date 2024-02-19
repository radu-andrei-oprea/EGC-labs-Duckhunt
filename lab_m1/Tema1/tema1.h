#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix, headMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        //hitbox variable
        float side;

        // duck variables
        float directionX, directionY;  // direction of duck
        float speed;             // speed of duck
        float a, newA;              // for duck rotation
        float flap, flap_move;   // for wings movement

        float duckX, duckY;      // original position
        float limX, limY;        // limits of the game


        // crosshair variables
        float crossX, crossY;
        
        
        // real-time coordinates
        float bodyX, bodyY;      // body/hitbox coords/origin
        float headX, headY;      // head coords


        double mouseX, mouseY;
    

        // gameplay elements
        int duckStatus;                     // 0 - flying; 1 - hit; 2 - escaped;
        int inputStatus;                    // 1 - open; 0 - closed
        double now, after10;                // timer - now and after 10 seconds
        int hearts, bullets, maxBullets;    // counters for lifes, bullets and max capacity
        int score, level;                   // counters for score and levels
        short runningGame;                  // game status
    };
}   // namespace m1
