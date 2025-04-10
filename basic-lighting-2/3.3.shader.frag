#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; // can store as individual uniforms but struct is more organized

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; // different intensities of light source for each of the three components since they require diff values (Dark, Light)
uniform Light light;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float timeValue; // time value for fun
uniform Material material; // material struct

void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // diffusion 
    vec3 unitNormalVector = normalize(Normal);
    vec3 unitlightDirectonVector = normalize(light.position-FragPos); // Light-Point -> we get a vector AB
    float diff = max(dot(unitNormalVector, unitlightDirectonVector), 0.0); // if negative we take 0, if angle > 90 we get neg (put component 0 then cause no light comes)
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    float specularStrength = 0.65;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-unitlightDirectonVector, unitNormalVector); // reflect light vector
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //shinyness factor
    vec3 specular = light.specular * (spec * material.specular); 

    // result
    vec3 result = ambient+diffuse+specular;
    FragColor = vec4(result, 1.0); 
}
