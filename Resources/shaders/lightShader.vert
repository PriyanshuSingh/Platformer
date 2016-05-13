attribute vec4 a_position;
attribute vec4 a_color;
attribute mat4 u_projTrans;
attribute float s;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main(){
    gl_Position = u_projTrans * a_position;
    v_fragmentColor = s * a_color;
}