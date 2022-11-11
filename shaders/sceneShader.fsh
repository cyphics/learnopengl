#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 ViewDir);

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
    PointLight point;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
};
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos);


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3       viewPos;
uniform Material   material;
uniform DirLight   dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
//        vec3 lighting = vec3(0.0);
    vec3 lighting = CalculateDirectionalLight(dirLight, norm, viewDir);

    // phase 2: Point lights
//    for (int i=0; i < NR_POINT_LIGHTS; i++)
//        lighting += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
//
//    // phase 3: Spot light
//    lighting += CalculateSpotLight(spotLight, norm, FragPos);
//    FragColor = vec4(lighting, 1.0);
    vec4 texColor = texture(material.diffuse, TexCoords);
    FragColor = texColor;
//        FragColor = vec4(1.0);
};

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient          * texture(material.diffuse,  TexCoords).rgb;
    vec3 diffuse  = light.diffuse   * diff * texture(material.diffuse,  TexCoords).rgb;
    vec3 specular = light.specular  * spec * texture(material.specular, TexCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine values
    vec3 ambient  = light.ambient         * texture(material.diffuse,  TexCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,  TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return clamp((ambient + diffuse + specular) * attenuation, 0.0, 1.0);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir   = normalize(light.point.position - fragPos);
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.point.ambient * texture(material.diffuse, TexCoords).rgb;
    if (theta > light.outerCutOff) {
        float distance = length(light.point.position - FragPos);
        float attenuation = 1.0 / (light.point.constant + light.point.linear * distance + light.point.quadratic * distance * distance);
        // ambient

        // diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.point.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(lightDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.point.specular;


        return ambient + (diffuse + specular) * attenuation * intensity;
    } else {
        return ambient;
    }
}