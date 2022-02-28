#version 410 core

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{


    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);

    //Ambient
    vec3 ambient = 0.1 * color;

    //Diffuse
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 result = (ambient + diffuse);

    FragColor = vec4(result, 1.0f);
}

