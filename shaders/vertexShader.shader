#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float currentTime;

void main() {
    mat3 m;
    float pi = 3.14159265;
    float time_to_angle = currentTime;
    while (time_to_angle > 10) {
        time_to_angle -= 10;
    }
    float angle = time_to_angle / 10 * (2 * pi);
    m[0] = vec3(cos(angle), -sin(angle), 0.0f);
    m[1] = vec3(sin(angle), cos(angle), 0.0f);
    m[2] = vec3(0.0f, 0.0f, 1.0f);

    gl_Position = vec4(aPos * m, 1.0);
//    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
};
