#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // diffusion 
    vec3 unitNormalVector = normalize(Normal);
    vec3 unitlightDirectonVector = normalize(lightPos-FragPos); // Light-Point -> we get a vector AB
    float diff = max(dot(unitNormalVector, unitlightDirectonVector), 0.0); // if negative we take 0, if angle > 90 we get neg (put component 0 then cause no light comes)
    vec3 diffuse = diff * lightColor;
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-unitlightDirectonVector, unitNormalVector); // reflect light vector
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = (ambient+diffuse+specular) * objectColor;
    FragColor = vec4(result, 1.0); 
}
