#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
    vec4 diffuse = texture(texture_diffuse1, TexCoords);
    vec4 specular = texture(texture_specular1, TexCoords);
    FragColor = vec4(diffuse + specular);
//    FragColor = vec4(0.0f);
}