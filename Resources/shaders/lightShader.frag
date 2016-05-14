#ifdef GL_ES
precision lowp float;
#endif

uniform float u_isGamma;

varying vec4 v_fragmentColor;

void main(){
    if(u_isGamma > 0.5){
        gl_FragColor = sqrt(v_fragmentColor);
    }else{
        gl_FragColor = v_fragmentColor;
    }
}