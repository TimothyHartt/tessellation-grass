#pragma once
#include <SDL.h>
#include "camerahandler.hpp"


/*
This .hpp and .cpp contains reused code.
*/

class WindowHandler
{
    private:
        static WindowHandler* instance;
        float deltaTime;
        float lastFrame;


    public:
    unsigned int tessInner, tessOuter;
    unsigned int tessInnerGrass, tessOuterGrass;
    int maxTess;
    unsigned int mode;
    unsigned int mouseMode;
    unsigned int renderMode;

    SDL_Window* window;
    SDL_GLContext glContext;

    SDL_Renderer *renderer;
    CameraHandler * camera;

    WindowHandler();
    static WindowHandler* getInstance();

    WindowHandler* GenWindow(const char* title, int width, int height, int x, int y);
    void DestroyWindow();

    WindowHandler * Clear();
    WindowHandler * Swap();

    int windowWidth();
    int windowHeight();

    unsigned int GetTicks();
    float GetDeltaTime();

    WindowHandler* Resize(int width = -1, int height = -1);
    WindowHandler* Project();
    WindowHandler* EventLoop();
    WindowHandler* KeyEvent( SDL_Keycode key);
    

    WindowHandler * Update();
    
};