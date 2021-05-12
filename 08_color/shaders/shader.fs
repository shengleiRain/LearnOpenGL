#version 330 core

struct Material {
    sampler2D diffuse; //漫反射光照下，物体的颜色, sampler2D是不透明类型，只能通过uniform来定义它， 无法将它实例化
    sampler2D specular; //镜面光照对物体的颜色影响
    sampler2D emission;
    float shininess; //影响镜面高光的散射/半径
};

struct DirLight { //定向光
    vec3 direction; //光的方向
    vec3 ambient; //环境光照强度
    vec3 diffuse; //漫反射强度
    vec3 specular; //镜面反射强度
};

struct PointLight {//点光源
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; //衰减
    float linear;
    float quadratic;
}

struct SpotLight {//聚光灯
    vec3 direction;
    vec3 position;

    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; //衰减
    float linear;
    float quadratic;
}

struct 

out vec4 fragColor;
//灯光的颜色
uniform vec3 lightColor;
//观察者的位置
uniform vec3 viewPos;

//物体材质
uniform Material material;
uniform Light light;
uniform vec3 objectColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords; //纹理坐标


void main()
{
    //ambient
    vec3 ambient=light.ambient * lightColor * texture(material.diffuse, texCoords).rgb;
    
    //diffuse
    vec3 norm=normalize(normal);
    vec3 lightDir=normalize(-light.direction);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse * lightColor * diff * texture(material.diffuse, texCoords).rgb;

    //specular 镜面光照，依赖于观察方向
    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(dot(viewDir, reflectDir), material.shininess);
    vec3 specular = light.specular * lightColor * spec * texture(material.specular, texCoords).rgb;

    //emission
    vec3 emission = texture(material.emission, texCoords).rgb;
    
    vec3 result = ambient + diffuse + specular + emission;
    fragColor=vec4(result,1.0);

}
