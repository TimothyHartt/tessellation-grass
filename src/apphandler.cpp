#include "apphandler.hpp"
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <functional>

#include "windowhandle.hpp"
#include "shaderhandler.hpp"
#include "matrixhandler.hpp"
#include "texturehandler.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


using namespace std;

#define degreesToRadians(x) x*(3.141592f/180.0f)
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))
unsigned int textTest;

void calcTangent(vertex& v0, vertex& v1, vertex& v2)
{
    glm::vec3 edge1 = v1.position - v0.position;
    glm::vec3 edge2 = v2.position - v0.position;


    glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
    glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;
    glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;

    //Cross product between tangent and normal

    v0.bitan = glm::normalize(bitangent);
    v1.bitan = glm::normalize(bitangent);
    v2.bitan = glm::normalize(bitangent);

    v0.tangent = glm::normalize(tangent);
    v1.tangent = glm::normalize(tangent);
    v2.tangent = glm::normalize(tangent);

    return;
}

//Draw With Displacement Map
void drawPlain(Object * obj){

    MatStack::getInstance()->Push();
    MatStack::getInstance()->Translate(obj->getPosition());
    //No Rotation
    MatStack::getInstance()->Scale(obj->getScale());

    obj->bindBuffers();

    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, -1.0f);

    //Generate Vertices
    vertex point1 = { glm::vec3(0.0f,  0.0f, 0.0f), bitangent,normal, normal,  glm::vec2(0.0f, 0.0f) };
    vertex point2 = { glm::vec3( 0.0f,   0.0f, -0.5f), bitangent, normal, normal,  glm::vec2(0.0f, 1.0f) };
    vertex point3 = { glm::vec3(0.5f, 0.0f,  -0.5f), bitangent, normal,normal, glm::vec2(1.0f, 1.0f) };
    vertex point4 = { glm::vec3(0.5f,  0.0f, 0.0f), bitangent,normal, normal, glm::vec2(1.0f, 0.0f) };
    
    vector <vertex> vertices;
    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point3);
    vertices.push_back(point4);

    int f = 0;

    for(int i = 0; i < vertices.size() - 2; i++){
        calcTangent(vertices[i + f], vertices[i+1], vertices[i + 2 - f]);
        f = 2 - f;
    }



    //Bind Buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obj->disMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obj->normalMap);

    obj->applyShader();

    obj->shaderProgram->setFloat("dtime", WindowHandler::getInstance()->GetDeltaTime());
    obj->shaderProgram->setFloat("time", WindowHandler::getInstance()->GetTicks()/500.0f);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glDrawArrays(GL_PATCHES, 0, 4);



    MatStack::getInstance()->Pop();
    return;
}

//For Drawing Normal Maps
void drawPlain2(Object * obj){

    MatStack::getInstance()->Push();
    MatStack::getInstance()->Translate(obj->getPosition());
    //MatStack::getInstance()->Rotate(degreesToRadians(obj->degrees),obj->getRotation());
    MatStack::getInstance()->Scale(obj->getScale());

    obj->bindBuffers();


    //A default normal
    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, -1.0f);



    //The Normal attribute is used to calculate the tangent space matrix
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

    //Generate Vertices
    vertex point1 = { glm::vec3(0.0f, 0.0f, 0.0f), bitangent, normal,normal, glm::vec2(0.0f, 0.0f) };
    vertex point2 = { glm::vec3(0.0f, 0.0f, -0.5f), bitangent,normal ,normal, glm::vec2(0.0f, 1.0f) };
    vertex point3 = { glm::vec3(0.5f, 0.0f, 0.0f), bitangent, normal,normal, glm::vec2(1.0f, 0.0f) };
    vertex point4 = { glm::vec3(0.5f, 0.0f, -0.5f), bitangent, normal,normal, glm::vec2(1.0f, 1.0f) };

    vector <vertex> vertices;
    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point3);
    vertices.push_back(point4);

    int f = 0;

    for(int i = 0; i < vertices.size() - 2; i++){
        calcTangent(vertices[i + f], vertices[i+1], vertices[i + 2 - f]);
        f = 2 - f;
    }


    //Bind Buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textTest);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obj->disMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obj->normalMap);


    //Use its Shader
    obj->shaderProgram->use();
    obj->shaderProgram->setInt("texture1", 0);
    obj->shaderProgram->setInt("dispMap", 1);
    obj->shaderProgram->setInt("normalMap", 2);

        //Apply Camera matrix
    obj->shaderProgram->setMat4("model", MatStack::getInstance()->getTop());
    obj->shaderProgram->setMat4("view", WindowHandler::getInstance()->camera->getViewMatrix());
    obj->shaderProgram->setMat4("projection", WindowHandler::getInstance()->camera->getProjectionMatrix());

    //Set Tesslation Level
    obj->shaderProgram->setInt("innerTess", WindowHandler::getInstance()->tessInner);
    obj->shaderProgram->setInt("outerTess", WindowHandler::getInstance()->tessOuter);
    
    //Set Lighting Variables
    obj->shaderProgram->setVec3("lightPos", AppHandler::getInstance()->light->getPosition());
    obj->shaderProgram->setVec3("viewPos", WindowHandler::getInstance()->camera->m_position);

    obj->shaderProgram->setFloat("dtime", WindowHandler::getInstance()->GetDeltaTime());
    obj->shaderProgram->setFloat("time", WindowHandler::getInstance()->GetTicks()/500.0f);



    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    MatStack::getInstance()->Pop();

    return;
}

//Cube to Track light Source
void drawCube( Object * obj ){

    MatStack::getInstance()->Push();
    MatStack::getInstance()->Translate(obj->getPosition());
    MatStack::getInstance()->Scale(obj->getScale());

    

    obj->bindBuffers();

        float vertices[] = {

        //Front Face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        //Back Face
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        //Left Face
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        //Right Face
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        //Bottom Face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        //Top Face
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    // Set Cube VAO, only Positional data input
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    //Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);  


    obj->shaderProgram->use();
    //Apply Camera matrix
    obj->shaderProgram->setMat4("model", MatStack::getInstance()->getTop());
    obj->shaderProgram->setMat4("view", WindowHandler::getInstance()->camera->getViewMatrix());
    obj->shaderProgram->setMat4("projection", WindowHandler::getInstance()->camera->getProjectionMatrix());

    obj->shaderProgram->setVec3("color", obj->color);
    

        //Set Lighting Variables
    obj->shaderProgram->setVec3("lightPos", AppHandler::getInstance()->light->getPosition());
    obj->shaderProgram->setVec3("viewPos", WindowHandler::getInstance()->camera->m_position);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    MatStack::getInstance()->Pop();

    return;

}


AppHandler * AppHandler::instance = NULL;

AppHandler::AppHandler() {
    //Get instance of Singltons to init classes
    MatStack::getInstance();
    TextureHandler::getInstance();

    //Gen Window
    WindowHandler::getInstance()->GenWindow("Timothy Hartt HW6", SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    genBuffers();
    loadScene();

}

AppHandler::~AppHandler(){

    for( Shader * s : shaders){
        glDeleteProgram(s->ID);
    }

    for( Object * o : objects){
        delete o;
    }

    delete WindowHandler::getInstance();
    delete MatStack::getInstance();
    delete TextureHandler::getInstance();
    delete AppHandler::getInstance();
}

AppHandler * AppHandler::getInstance() {
    if (instance == NULL) {
        instance = new AppHandler();
    }
    return instance;
}


AppHandler * AppHandler::genBuffers(){
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &framebufferTexture);
    glGenRenderbuffers(1, &rbo);

    textTest = TextureHandler::getInstance()->GenTexture("resources/stone.png");
    return this;
}


AppHandler * AppHandler::loadScene(){

    Shader * loader;

    //Basic lighting shader
    loader = new Shader("shaders/default/basicVert.vs", "shaders/default/basicFrag.fs", nullptr, nullptr, nullptr);
    addShader(loader);

        //Grass Shader
    loader = new Shader("shaders/grass/grass.vs", "shaders/grass/grass.fs", "shaders/grass/grass.tesc", "shaders/grass/grass.tese", "shaders/grass/grass.geom");
    addShader(loader);

        //Normal Map Shader
    loader = new Shader("shaders/normalmap/normal.vs", "shaders/normalmap/normal.fs", nullptr, nullptr,  nullptr);
    addShader(loader);

    pps = new Shader("shaders/default/default.vs", "shaders/default/default.fs", nullptr, nullptr, nullptr);

    glm::vec3 zero = glm::vec3(0.0f);
    glm::vec3 one = glm::vec3(1.0f);

    //Draw Grass
    drawGrass();

    //Draw Normal Map
    drawTile();


    //Floor
    Object * object1 = new Object(zero,zero,one,0, 0, shaders[0]);
    object1->color = glm::vec3(0.2f, 0.43f, 0.2f);
    object1->setPosition(glm::vec3(0, -0.01f, 0));
    object1->setScale(glm::vec3(1.0f, 0.05f, 1.0f));
    object1->setOnDraw(drawCube);

    addObject(object1);

    //Create Our Light Object
    //Its a Member variable for our class because we need to access lightpos for our shader
    light = new Object(zero,zero,one,0, 0, shaders[0]);
    light->setOnDraw(drawCube);
    light->color = glm::vec3(10.0f, 10.0f, 10.0f);
    light->setPosition(glm::vec3(0.0f, 0.8f, 0.0f));
    light->setScale(glm::vec3(.1f));
    addObject(light);

    //Quad for post processing
    quad = new Object(zero,zero,one,0, 0, pps);

    
    return this;
}

AppHandler * AppHandler::drawGrass(){

    glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    //Grass
    Object * object1 = new Object(zero, zero, scale, 0, 0, shaders[1]);
    object1->setPosition(glm::vec3(-0.25f, 0.0f, 0.25f));
    object1->setOnDraw(drawPlain);
    addObject(object1);


    return this;
}

AppHandler * AppHandler::drawTile(){
    glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 one = glm::vec3(1.75f, 1.0f, 1.75f);
    std::function<void(Object *)> onDraw;
    onDraw = drawPlain2;

    //Load Textures
    unsigned int normalMap = TextureHandler::getInstance()->GenTexture("resources/stone_normal.png");
    unsigned int texture = TextureHandler::getInstance()->GenTexture("resources/stone.png");

    //Tile
    Object * object1 = new Object(zero,zero,one,texture, 0, shaders[2]);
    object1->normalMap = normalMap;
    object1->setPosition(glm::vec3(-0.4375f, 0.025f, 0.4375f));
    object1->setOnDraw(onDraw);

    addObject(object1);

    return this;
}

AppHandler * AppHandler::addObject(Object * obj) {
    objects.push_back(obj);
    return instance;
}

AppHandler * AppHandler::addShader(Shader * shader) {
    shaders.push_back(shader);
    return instance;
}

AppHandler * AppHandler::updateShaders(int i){ 
    for (Object * obj : objects) { 
        if(obj->shaderProgram != shaders[3] && obj->shaderProgram != shaders[4]){
            obj->shaderProgram = shaders[i];
            if(i == 1){
                obj->setOnDraw(drawPlain);
                obj->disMap = dis1;
            }
            else{
                obj->setOnDraw(drawPlain2);
                obj->disMap = dis2;
            }
        }
    }
    return this;
}

AppHandler * AppHandler::Update(){

    //Update Delta Time, Pull for SDL_Event
    WindowHandler::getInstance()->Update()->Clear();

    //Light Object
    glm::vec3 pos = light->getPosition();
    pos.x =  cos(SDL_GetTicks() / 500.0f);
    pos.z =  sin(SDL_GetTicks() / 500.0f);
    light->setPosition(pos);


    //Bind our buffers, called very frame in case of resizing
    bindFrameBuffer();
    //Bind to our custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw functions
    for (int i = 0; i < objects.size(); i++) {

        objects[i]->Draw();
    }
    
    postProcess();
    //Draw to screen
    WindowHandler::getInstance()->Swap();

    return this;
}

AppHandler * AppHandler::bindFrameBuffer(){

    //Print window width and height


    //Bind Frame Buffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //Bind Texture we're 'drawing' to
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowHandler::getInstance()->windowWidth(), WindowHandler::getInstance()->windowHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    //Renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowHandler::getInstance()->windowWidth(), WindowHandler::getInstance()->windowHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //Make sure we can read from the framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    return this;
}

AppHandler * AppHandler::postProcess(){
    //For now simply draw the texture to a poly that covers the screen
    


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw the texture to the screen
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    
    quad->shaderProgram->use();
    quad->bindBuffers();
    

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    
    quad->shaderProgram->setInt("screenTexture", 0);
    quad->shaderProgram->setInt("screenWidth", WindowHandler::getInstance()->windowWidth());
    quad->shaderProgram->setInt("screenHeight", WindowHandler::getInstance()->windowHeight());
    quad->shaderProgram->setInt("mode", WindowHandler::getInstance()->mode);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glEnable(GL_DEPTH_TEST);

    //print opengl error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }

    return this;

}