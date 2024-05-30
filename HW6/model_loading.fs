#version 330 core
out vec4 FragColor;

struct Light{ //added

    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 ObjColor;
uniform bool hasTextures;

uniform Light light; //added

uniform bool LKey; //added for spotlight

void main()
{    
    vec3 fColor = hasTextures ? texture(texture_diffuse1, TexCoords).rgb : ObjColor;

    // ambient
    const float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    const float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32); //32
    vec3 specular = specularStrength * spec * lightColor;  


    //toggle
    if(LKey){ //for spotlight(angular attenuation)
        float theta=dot(lightDir,normalize(-light.direction));
        float epsilon=(light.cutOff-light.outerCutOff);
        float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);
        diffuse*=intensity;
        specular*=intensity;
    }

    //radial attenuation(added)
    float dist=length(lightPos-FragPos);
    float attenuation=1.0/(light.constant + light.linear*dist+light.quadratic*(dist*dist));

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    //


        
    vec3 result = (ambient + diffuse + specular) * fColor;
    FragColor = vec4(result, 1.0);
}