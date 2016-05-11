attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix*a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}