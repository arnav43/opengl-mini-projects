#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos; 
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // we need in world space so we multiply by model matrix

    //Normal = aNormal; -> if we do non uniform scale then normal gets messed up, we do this instead
    Normal = mat3(transpose(inverse(model))) * aNormal; // Normal Matrix * aNormal Basically 

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = projection * view * vec4(FragPos, 1.0); // same thing
}
