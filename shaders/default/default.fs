#version 410 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform int screenWidth;
uniform int screenHeight;
uniform int mode;

void main()
{ 


    float dX = 1.0 / screenWidth;
    float dY = 1.0 / screenHeight;

    vec2 offsets[9] = vec2[](
        vec2(-dX,  dY), // top-left
        vec2( 0.0f,    dY), // top-center
        vec2( dX,  dY), // top-right
        vec2(-dX,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( dX,  0.0f),   // center-right
        vec2(-dX, -dY), // bottom-left
        vec2( 0.0f,   -dY), // bottom-center
        vec2( dX, -dY)  // bottom-right    
    );

    float kernel_H[9] = float[](
        -1, -2, -1,
        0,  0, 0,
        1, 2, 1
    );


    float kernel_V[9] = float[](
        1, 0, -1,
        2, 0, -2,
        1, 0, -1
    );

    vec4 H = texture(screenTexture, TexCoord.st + offsets[0]) * kernel_H[0] +
             texture(screenTexture, TexCoord.st + offsets[1]) * kernel_H[1] +
             texture(screenTexture, TexCoord.st + offsets[2]) * kernel_H[2] +
             texture(screenTexture, TexCoord.st + offsets[3]) * kernel_H[3] +
             texture(screenTexture, TexCoord.st + offsets[4]) * kernel_H[4] +
             texture(screenTexture, TexCoord.st + offsets[5]) * kernel_H[5] +
             texture(screenTexture, TexCoord.st + offsets[6]) * kernel_H[6] +
             texture(screenTexture, TexCoord.st + offsets[7]) * kernel_H[7] +
             texture(screenTexture, TexCoord.st + offsets[8]) * kernel_H[8];

    vec4 V = texture(screenTexture, TexCoord.st + offsets[0]) * kernel_V[0] +
            texture(screenTexture, TexCoord.st + offsets[1]) * kernel_V[1] +
            texture(screenTexture, TexCoord.st + offsets[2]) * kernel_V[2] +
            texture(screenTexture, TexCoord.st + offsets[3]) * kernel_V[3] +
            texture(screenTexture, TexCoord.st + offsets[4]) * kernel_V[4] +
            texture(screenTexture, TexCoord.st + offsets[5]) * kernel_V[5] +
            texture(screenTexture, TexCoord.st + offsets[6]) * kernel_V[6] +
            texture(screenTexture, TexCoord.st + offsets[7]) * kernel_V[7] +
            texture(screenTexture, TexCoord.st + offsets[8]) * kernel_V[8];
    
    vec4 B;
    if(mode == 0){
        B = sqrt(H*H+V*V);
    }else{
        B = texture(screenTexture, TexCoord);
    }
    FragColor = B;
}