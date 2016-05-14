attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
#define MED mediump
varying lowp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#define MED
#endif

varying MED vec2 v_texCoords0;
varying MED vec2 v_texCoords1;
varying MED vec2 v_texCoords2;
varying MED vec2 v_texCoords3;
varying MED vec2 v_texCoords4;

uniform mat4 Proj;
uniform vec2 u_blurAxis;
uniform vec2 u_winsize;

void main()
{
    vec2 further = vec2(3.2307692308 / u_winsize.x, 3.2307692308 / u_winsize.y );
    vec2 closer = vec2(1.3846153846 / u_winsize.x, 1.3846153846 / u_winsize.y );
    vec2 f = further * u_blurAxis;
    vec2 c = closer * u_blurAxis;
    v_texCoords0 = a_texCoord - f;
    v_texCoords1 = a_texCoord - c;
    v_texCoords2 = a_texCoord;
    v_texCoords3 = a_texCoord + c;
    v_texCoords4 = a_texCoord + f;
    gl_Position = Proj * a_position;
    v_fragmentColor = a_color;
}