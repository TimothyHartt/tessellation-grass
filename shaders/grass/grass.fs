#version 410 core

out vec4 FragColor;

in GE_OUT {
	vec3 TanFragPos;
	vec3 FragColor;
    vec3 TanLightPos;
    vec3 TanViewPos;
} ge_in; 

void main()
{
    //With no normal map to reference the tangent normal will be in the positive z direction   
    vec3 normal = -vec3(0.0, 0.0, 1.0);
    vec3 color = ge_in.FragColor;


    vec3 lightDir = normalize(ge_in.TanLightPos - ge_in.TanFragPos);
    vec3 viewDir = normalize(ge_in.TanViewPos - ge_in.TanFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    //Ambient
    vec3 ambient = 0.6 * color;

    //Determine which side of the surface the light is on, and which side we're viewing.
    float side = dot(viewDir, normal);

    if (side < 0.0)
        normal = -normal;
     
    //Diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color * 0.3;

    //Specular
    float specularStrength = 0.1;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4.0);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);


    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}

