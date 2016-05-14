#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 AmbientColor;

void main()
{
    vec4 textureColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.rgb = (textureColor.rgb + AmbientColor.rgb);
    gl_FragColor.a = 1.0;
}

