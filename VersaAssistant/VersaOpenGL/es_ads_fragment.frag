#version 120

// Light information
// 光信息
uniform highp vec4 lightPosition;
uniform highp vec3 lightIntensity;

// Material information
// 材料信息
uniform lowp vec3 Ka;
uniform lowp vec3 Kd;
uniform lowp vec3 Ks;
uniform lowp float shininess;

varying highp vec3 normal;
varying highp vec3 position;

highp vec3 adsModel(vec3 norm)
{
    // Calculate light direction
    // 计算光线方向
    highp vec3 s = normalize( lightPosition.xyz - position);
    // Calculate the vector from the fragment to eye position
    // 计算从片段到眼睛位置的向量
    highp vec3 v = normalize( -position.xyz );
    // Reflect the light using the normal
    // 使用法线反射光线
    highp vec3 r = reflect( -s, norm);
    // Calculate the diffuse contribution
    // 计算扩散贡献
    highp vec3 diffuseIntensity = vec3( max( dot( s, norm), 0.0) );
    // Calculate specular contribution
    // 计算镜面反射贡献
    highp vec3 specularIntensity = vec3(0.0);
    if( dot( s, norm) > 0.0 )
    {
        specularIntensity = highp vec3( pow( max( dot(r, v), 0.0), shininess));
    }
    // Calculate final color
    // 计算最终颜色
    return lightIntensity * (Ka + Kd * diffuseIntensity +  Ks * specularIntensity);
}

void main(void)
{
    gl_FragColor = highp vec4(adsModel(normalize(normal)), 1.0);
}
