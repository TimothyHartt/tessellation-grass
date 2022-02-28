#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GL/gl.h>
#include <stack>
#include "windowhandle.hpp"
#include "shaderhandler.hpp"
#include "matrixhandler.hpp"
#include "texturehandler.hpp"
#include "apphandler.hpp"
#include "object.hpp"



int main(int argc, char** argv){


   AppHandler::getInstance();

    auto a = glGetString(GL_VERSION);

    std::cout << a << std::endl;
    
    int max_patch_vertices = 0;
    glGetIntegerv (GL_MAX_TESS_GEN_LEVEL, &max_patch_vertices);
    printf ("Max supported patch vertices %i\n", max_patch_vertices);
    
    Uint32 framestart;
    int frameTime;
    const int FPS = 60;
    const int delay = 1000 / FPS;


    for(;;){

        framestart = WindowHandler::getInstance()->GetTicks();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        AppHandler::getInstance()->Update();

        frameTime = WindowHandler::getInstance()->GetTicks() - framestart;

        if(frameTime < delay){
            SDL_Delay(delay - frameTime);
        }
 
    }
    
    WindowHandler::getInstance()->DestroyWindow();


    return 0;
}