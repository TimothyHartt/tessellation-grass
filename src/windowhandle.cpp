#include "windowhandle.hpp"
#include "matrixhandler.hpp"
#include "apphandler.hpp"
#include "texturehandler.hpp"
#include <stdio.h>
#include <iostream>
#include "glm/glm.hpp"

WindowHandler * WindowHandler::instance = nullptr;

WindowHandler * WindowHandler::getInstance()
{
    if (instance == nullptr)
    {
        instance = new WindowHandler();
    }
    return instance;
}

WindowHandler::WindowHandler()
{
    mode = 0;
    mouseMode = 0;
    renderMode = 0;
}

WindowHandler* WindowHandler::GenWindow(const char* title, int width, int height, int x, int y) {

    //Init SDL, check for failure
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr 
        << "SDL2 every subsystem couldn't be initialized." 
        << SDL_GetError()    
        << std::endl;
        exit(0);
    }
     

    window = SDL_CreateWindow(title, x, y, width, height,  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    glContext = SDL_GL_CreateContext(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        DestroyWindow();
        exit(0);
    }

    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    
    camera = new CameraHandler();
    tessInner = tessOuter = 16;
    glGetIntegerv (GL_MAX_TESS_GEN_LEVEL, &maxTess);
    return this;
}

WindowHandler* WindowHandler::Resize(int width, int height ){
    int x = width;
    int y = height;
    if (width == -1 || height == -1) {
        SDL_GetWindowSize(window, &x, &y);
    }
    camera->m_aspectRatio = (float)x / (float)y;
    glViewport(0, 0, x, y);

    return this;
}

unsigned int WindowHandler::GetTicks() {
    return SDL_GetTicks();
}

WindowHandler* WindowHandler::EventLoop() {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        int x,y;
                        SDL_GetWindowSize(window, &x, &y);
                        Resize(x,y);
                        break;
                }
                break;

            case SDL_KEYDOWN:
            {
                auto key = event.key.keysym.sym;
                KeyEvent(key);
                break;
            }

            case SDL_MOUSEMOTION:
            {
                int x = event.motion.xrel;
                int y = event.motion.yrel;

                camera->ProcessMouseMovement(x, y, deltaTime, true);

                break;
            }
            case SDL_QUIT:
                WindowHandler::getInstance()->DestroyWindow();
                    delete WindowHandler::getInstance();
                    delete MatStack::getInstance();
                    delete TextureHandler::getInstance();
                    delete AppHandler::getInstance();
                exit(0);
                break;

        }
    }
    return this;
}

WindowHandler * WindowHandler::KeyEvent(SDL_Keycode key) {
    switch (key)
    {
    case SDLK_ESCAPE:
        WindowHandler::getInstance()->DestroyWindow();
                exit(0);
        break;
    case SDLK_w:
        camera->processKeyboard(FORWARD, deltaTime);
        break;
     case SDLK_s:
        camera->processKeyboard(BACKWARD, deltaTime);
        break;
     case SDLK_a:
        camera->processKeyboard(LEFT, deltaTime);
        break;
     case SDLK_d:
        camera->processKeyboard(RIGHT, deltaTime);
        break;
    case SDLK_SPACE:
        camera->processKeyboard(UP, deltaTime);
        break;
    case SDLK_LCTRL:
        camera->processKeyboard(DOWN, deltaTime);
        break;
    case SDLK_q:
        if(mouseMode == 0)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
        mouseMode = 1 - mouseMode;
        break;
    case SDLK_1:
        mode = 1 - mode;
        break;
    case SDLK_2:
        camera->m_mode = 1 - camera->m_mode;
        break;

    default:

        break;
    }

    return this;
}

WindowHandler * WindowHandler::Update() {
    deltaTime = (float)(WindowHandler::getInstance()->GetTicks() - lastFrame) / 1000.0f;
    lastFrame = WindowHandler::getInstance()->GetTicks();

    EventLoop();

    return this;
}

WindowHandler* WindowHandler::Swap() {
    SDL_GL_SwapWindow(window);
    return this;
}

WindowHandler * WindowHandler::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return this;
}

void WindowHandler::DestroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

float WindowHandler::GetDeltaTime(){
    return deltaTime;
}


int WindowHandler::windowWidth(){
    //Get window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return w;
}

int WindowHandler::windowHeight(){
    //Get window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return h;
}