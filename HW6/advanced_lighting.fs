#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light{ //added
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ObjColor;

uniform Light light; //added

uniform bool LKey; //added for spotlight

void main()
{           

    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = 0.1 *  color;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    const float specularStrength = 0.5;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32); //32

    vec3 specular = specularStrength * lightColor * spec; 


    //toggle
    if(LKey){ //spotlight(angular attenuation)
        float theta=dot(lightDir,normalize(-light.direction));
        float epsilon=(light.cutOff-light.outerCutOff);
        float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);
        diffuse*=intensity;
        specular*=intensity;
    }

    //radial attenuation(added)
    float dist=length(lightPos-fs_in.FragPos); //FragPos -> fs_in.FragPos
    float attenuation=1.0/(light.constant + light.linear*dist+light.quadratic*(dist*dist));

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    //

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}