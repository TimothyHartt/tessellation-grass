#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float dtime;
uniform float time;


//We will transform these to tangent space
uniform vec3 lightPos;
uniform vec3 viewPos;


out GE_OUT {
	vec3 TanFragPos;
	vec3 FragColor;
    vec3 TanLightPos;
    vec3 TanViewPos;
} ge_out; 


/**********
A random value genorator taken from booksofshaders.com

https://thebookofshaders.com/10/

We will use this function to randomly rotate the grass around the y axis.
*/

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}



/**********
A rotation function made by Niel Mendoza

http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
*/

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 color1 = vec3(0.0f, 0.4f, 0.0f);
vec3 color2 = vec3(0.5f, 1.0f, 0.0f);

vec3 colors[3] = vec3[3](
    color1,
    color1,
    color2 * .75f
); 


vec3 points[3] = vec3[3](
    vec3(-0.05, 0, 0.0),
    vec3(0.05, 0, 0.0),
    vec3(0.0, 0.5, 0.0)
); 



void GenGrass(int i){
    //Generate grass
    vec3 pos = gl_in[i].gl_Position.xyz;
    gl_Position =  vec4(pos + points[0], 1.0);
    EmitVertex();

    pos = gl_in[i].gl_Position.xyz;
    gl_Position =  vec4(pos + points[1], 1.0);
    EmitVertex();

    pos = gl_in[i].gl_Position.xyz;
    gl_Position =  vec4(pos + points[2], 1.0);
    EmitVertex();

    EndPrimitive();

}


void main() {

    

    //Temp Wind Dir
    vec3 windDir = vec3(1.0, 0.0, 0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);


    //Get the Offset in clip space
    mat4 offset = mat4(1.0);
    offset[3] = model * gl_in[0].gl_Position;

    //Generate a Random number based off of our pos in world space
    float rand = random(offset[3].zx);
    float height = .3 + rand * .7;
    points[2].y *= height;


    mat4 offset2 = mat4(1.0);


    offset2[0][0] = model[0][0] * .15;
    offset2[1][1] = model[1][1] * .5;


    rand = random(offset[3].xz);
    mat4 rotation_x = rotationMatrix(vec3(1.0, 0.0, 0.0), radians(rand * 30));
    mat4 rotation_y = rotationMatrix(vec3(0.0, 1.0, 0.0), radians(rand * 180.0 - 90));

    
    //The axis of rotation of wind
    vec3 wind_rot = cross(up, windDir);


    //Wind transformation. Taller grass will be rotated more and have an offset of height
    float amplitude = 10;
    float frequency = 5;
    mat4 wind = rotationMatrix(wind_rot, radians(height * amplitude * sin(frequency * dtime + time + height)));


    //Model matrix to transform the grass into its generated world space.
    mat4 grass_model = offset * wind * rotation_y * rotation_x * offset2;

    //By default the generated triangle has a normal pointing at the positive z axis.
    vec3 normal = vec3(0.0, 0.0, 1.0);

    //Get the normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(grass_model)));
    normal = normalize(normalMatrix * normal);

    //Create the TBN matrix, where bitangent is Up.
    vec3 tangent = -normalize(cross(up, normal));

    tangent = normalize(normalMatrix * tangent);
    up = normalize(normalMatrix * up);

    mat3 TBN = transpose(mat3(tangent, up, normal));


    for( int i = 0; i < 3; i++){

        vec4 pos = vec4(points[i], 1.0);
        pos = grass_model * pos;

        ge_out.TanFragPos = TBN * pos.xyz;
        ge_out.TanLightPos = TBN * lightPos;
        ge_out.TanViewPos = TBN * viewPos;
        ge_out.FragColor = colors[i];

        gl_Position = projection *  view  * pos;
        EmitVertex();
    }


    EndPrimitive();
}
