#version 410 core

layout (vertices = 4) out;

uniform int innerTess = 2;
uniform int outerTess = 2;


void main()
{
    //Set Output Variables
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID == 0){
      gl_TessLevelInner[0] = innerTess;
      gl_TessLevelInner[1] = innerTess;

      gl_TessLevelOuter[0] = outerTess;
      gl_TessLevelOuter[1] = outerTess;
      gl_TessLevelOuter[2] = outerTess;
      gl_TessLevelOuter[3] = outerTess;

    }
}