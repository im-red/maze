#version 330 core

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    vec3 result = (ambientStrength + diff * diffuseStrength + spec * specularStrength) * lightColor * vec3(texture(ourTexture, TexCoord));
    color = vec4(result, 1.0f);
}
