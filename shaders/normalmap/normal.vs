#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aBitangent;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;


void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(model)));

    vec3 normal = normalize(normalMatrix * aNormal);
    vec3 bitangent = normalize(normalMatrix * aBitangent);
    vec3 tangent = normalize(normalMatrix * aTangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    TBN = transpose(TBN);


    //Matrix from tangent space to model space
    vs_out.TBN = mat3(tangent, bitangent, normal);

    vs_out.FragPos = (model * vec4(aPos, 1.0)).xyz;
    vs_out.TexCoord = aTexCoord;
    

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}