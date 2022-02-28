#version 410 core

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_in;


uniform sampler2D texture1;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
    //Get Normal then transform it to world space
    vec3 normal = texture(normalMap, vs_in.TexCoord).rgb;
    vec3 color = texture(texture1, vs_in.TexCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vs_in.TBN * normal);


    vec3 lightDir = normalize(lightPos - vs_in.FragPos);
    vec3 viewDir = normalize(viewPos - vs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    //Ambient
    vec3 ambient = 0.1 * color;

    //Diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    //Specular
    float specularStrength = 0.2;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);


    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}

