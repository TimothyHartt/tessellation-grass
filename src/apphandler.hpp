#pragma once 
#include "shaderhandler.hpp"
#include "camerahandler.hpp"
#include "object.hpp"
#include <vector>


class Object;

class AppHandler {
private:


    /*Framebuffer variables
    We will only have 1 framebuffer so we can simply have 1 texture and 1 renderbuffer
    */
    unsigned int fbo;
    unsigned int framebufferTexture;
    //Write only buffers, we will use this for the depth and stencil buffers
    unsigned int rbo;

    //Post process shader
    Shader * pps;

    //Quad drawn to screen for post processing
    Object * quad;

    static AppHandler * instance;

    unsigned int dis1;
    unsigned int dis2;

    //Object vector
    std::vector<Object*> objects;
    std::vector<Shader*> shaders;



public:

    Object * light;


    AppHandler();
    ~AppHandler();

    static AppHandler * getInstance();

    //Testing function for drawing a simple triangle
    AppHandler * drawTriangle();

    AppHandler * DrawBuffers(GLenum mode, int count, GLenum type = GL_UNSIGNED_INT);

    AppHandler * applyShader( Shader * shader );

    AppHandler * Update();

    AppHandler * addObject(Object * object);
    AppHandler * addShader(Shader * shader);

    AppHandler * loadScene();

    //Load specific scene 
    AppHandler *  drawGrass();
    AppHandler *  drawTile();

    //Not used, but the intention is to have a function that swap the shaders for a group of objects.
    AppHandler * updateShaders(int i);

    AppHandler * genBuffers();
    AppHandler * bindFrameBuffer();
    AppHandler * postProcess();
    AppHandler * unbindFrameBuffer();

};