attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
#endif

uniform mat4 Proj;

void main(){
    gl_Position = Proj * a_position;
    v_fragmentColor =  a_color;
    v_texCoord = a_texCoord;
}