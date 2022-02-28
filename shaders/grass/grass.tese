#version 410 core

layout(quads, equal_spacing, cw, point_mode) in;


vec3 linearInterpolate3D(vec3 a, vec3 b, vec3 c, vec3 d)
{

    vec3 p0 = mix(a,d, gl_TessCoord.x);
    vec3 p1 = mix(b,c, gl_TessCoord.x);
    vec3 r = mix(p0, p1, gl_TessCoord.y);

    return r;
}

vec2 linearInterpolate2D(vec2 a, vec2 b, vec2 c, vec2 d)
{

    vec2 p0 = mix(a,d, gl_TessCoord.x);
    vec2 p1 = mix(b,c, gl_TessCoord.x);
    vec2 r = mix(p0, p1, gl_TessCoord.y);

    return r;
}

void main()
{  
    //Interpolate the vertex positions for new pos
    vec3 newPos = linearInterpolate3D(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz);

    gl_Position = vec4(newPos, 1.0f);

}
