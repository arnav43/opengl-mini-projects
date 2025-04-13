#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; // can store as individual uniforms but struct is more organized

struct Light {
    vec3 position; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular; // different intensities of light source for each of the three components since they require diff values (Dark, Light)
     
    float constant;
    float linear;
    float quadratic;

}; 
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float timeValue; // time value for fun
uniform Material material; // material struct

void main()
{

    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffusion 
    vec3 unitNormalVector = normalize(Normal);
    vec3 unitlightDirectonVector = normalize(light.position-FragPos); // Light-Point -> we get a vector AB
    float diff = max(dot(unitNormalVector, unitlightDirectonVector), 0.0); // if negative we take 0, if angle > 90 we get neg (put component 0 then cause no light comes)
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-unitlightDirectonVector, unitNormalVector); // reflect light vector
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //shinyness factor
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  // basically if black, we dont make shiny, else we do, through multiplication
    // we can control shininess through the shininess factor in the material struct

    // attenuation for point light
    float distance = length(light.position-FragPos); // distance from light source to fragment
    float attenuation = 1.0/(light.constant+(light.linear*distance)+(light.quadratic*(distance*distance)));

    ambient = ambient * attenuation;
    diffuse = diffuse * attenuation;
    specular = specular * attenuation;


    // result
    vec3 result = ambient+diffuse+specular;
    FragColor = vec4(result, 1.0); 
}
