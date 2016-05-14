attribute vec4 a_position;
attribute vec4 a_color;
attribute float a_fraction;

uniform mat4 Proj;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main(){
    gl_Position = Proj * a_position;
    v_fragmentColor = a_fraction * a_color;
}