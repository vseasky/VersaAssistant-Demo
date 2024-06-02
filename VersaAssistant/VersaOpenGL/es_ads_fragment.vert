#version 120

attribute highp vec3 vertexPosition;
attribute highp vec3 vertexNormal;

uniform highp mat4 qt_ModelViewProjectionMatrix;
varying highp vec4 qt_TexCoord0;

uniform highp mat4 MV;
uniform highp mat3 N;
uniform highp mat4 MVP;

varying highp vec3 normal;
varying highp vec3 position;

void main(void)
{
    // Transform to eye coordinates
    // 转换为眼坐标
    normal = normalize( N * vertexNormal );
    position = vec3( MV * highp vec4( vertexPosition, 1.0 ) );
    gl_Position = MVP * highp vec4( vertexPosition, 1.0 );
}
