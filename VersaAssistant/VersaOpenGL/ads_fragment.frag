#version 330 core

// Light information
uniform vec4 lightPosition;
uniform vec3 lightIntensity;

// Material information
uniform vec3 Ka;            // mAmbient
uniform vec3 Kd;            // mDiffuse
uniform vec3 Ks;            // mSpecular
uniform float shininess;    // mShinines

in vec3 normal;
in vec3 position;

layout (location = 0) out vec4 fragColor;

vec3 adsModel(const in vec3 norm)
{
    // 计算光线方向
    vec3 s = normalize( lightPosition.xyz - position);

    // 计算从片段到眼睛位置的向量
    vec3 v = normalize( -position.xyz );

    // 使用法线反射光线
    vec3 r = reflect( -s, norm);

    // 计算扩散贡献
    vec3 diffuseIntensity = vec3( max( dot( s, norm), 0.0) );

    // 计算镜面反射贡献
    vec3 specularIntensity = vec3(0.0);
    if( dot( s, norm) > 0.0 )
    {
        specularIntensity = vec3( pow( max( dot(r, v), 0.0), shininess));
    }

    // 计算最终颜色
    return lightIntensity * (Ka + Kd * diffuseIntensity + Ks * specularIntensity);
}

void main()
{
    fragColor = vec4(adsModel(normalize(normal)), 1.0);
}
