#ifdef GL_ES
precision lowp float;
#define MED mediump
#else
#define MED
#endif

varying vec4 v_fragmentColor;

varying MED vec2 v_texCoords0;
varying MED vec2 v_texCoords1;
varying MED vec2 v_texCoords2;
varying MED vec2 v_texCoords3;
varying MED vec2 v_texCoords4;

uniform float u_isDiffuse;

const float center = 0.2270270270;
const float close  = 0.3162162162;
const float far    = 0.0702702703;
void main()
{
    vec4 textureColor = far * v_fragmentColor * texture2D(CC_Texture0, v_texCoords0);
    textureColor +=  close * v_fragmentColor *  texture2D(CC_Texture0, v_texCoords1);
    textureColor +=  center * v_fragmentColor *  texture2D(CC_Texture0, v_texCoords2);
    textureColor +=  close * v_fragmentColor *  texture2D(CC_Texture0, v_texCoords3);
    textureColor +=  far * v_fragmentColor * texture2D(CC_Texture0, v_texCoords4);
    if(u_isDiffuse > 0.5){
        gl_FragColor.rgb = textureColor.rgb;
        //Don't know what to feed in alpha so 1.0
        gl_FragColor.a = 1.0;
    }else{
        gl_FragColor = textureColor;
    }

}