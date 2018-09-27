
#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     vec4 _t41;
COMPAT_VARYING     vec4 _t31;
COMPAT_VARYING     vec4 _t21;
COMPAT_VARYING     vec4 _t11;
COMPAT_VARYING     vec2 _CT;
COMPAT_VARYING     vec4 _position2;
COMPAT_VARYING     vec4 _color2;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord2;
COMPAT_VARYING     vec4 _color1;
COMPAT_VARYING     vec4 _position1;
COMPAT_VARYING     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct VERTEX_OUTPUT {
    vec4 _position1;
    vec4 _color1;
    vec2 _texCoord2;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
};
struct output_dummy {
    vec4 _color2;
};
struct VERTEX_INPUT {
    vec4 _position2;
    vec2 _CT;
    vec4 _t11;
    vec4 _t21;
    vec4 _t31;
    vec4 _t41;
};
VERTEX_OUTPUT _ret_0;
float _TMP2;
float _TMP1;
input_dummy _IN1;
vec4 _r0009;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    VERTEX_OUTPUT _OUT;
    vec2 _ps;
    float _dx;
    float _dy;
    vec2 _texCoord;

	//gl_Position = MVPMatrix * gl_Vertex;
	//gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	//gl_FrontColor = gl_Color;
/*
    _r0009 = VertexCoord.x*MVPMatrix[0];
    _r0009 = _r0009 + VertexCoord.y*MVPMatrix[1];
    _r0009 = _r0009 + VertexCoord.z*MVPMatrix[2];
    _r0009 = _r0009 + VertexCoord.w*MVPMatrix[3];
*/
	_r0009 = MVPMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;

    if (TextureSize.x == 0.00000000E+00) { 
        _TMP1 = TextureSize.x;
    } else {
        _TMP1 = TextureSize.x;
    } 
    if (TextureSize.y == 0.00000000E+00) { 
        _TMP2 = TextureSize.y;
    } else {
        _TMP2 = TextureSize.y;
    } 
    _ps = vec2(1.00000000E+00/_TMP1, 1.00000000E+00/_TMP2);
    _dx = _ps.x*5.00000000E-01;
    _dy = _ps.y*5.00000000E-01;
    _texCoord = gl_TexCoord[0].xy + vec2( 1.00000001E-07, 1.00000001E-07);
    _OUT._t1.xy = _texCoord + vec2(-_dx, 0.00000000E+00);
    _OUT._t2.xy = _texCoord + vec2(_dx, 0.00000000E+00);
    _OUT._t3.xy = _texCoord + vec2(0.00000000E+00, -_dy);
    _OUT._t4.xy = _texCoord + vec2(0.00000000E+00, _dy);
    _OUT._t1.zw = _texCoord + vec2(-_dx, -_dy);
    _OUT._t2.zw = _texCoord + vec2(-_dx, _dy);
    _OUT._t3.zw = _texCoord + vec2(_dx, -_dy);
    _OUT._t4.zw = _texCoord + vec2(_dx, _dy);
    _ret_0._position1 = _r0009;
    _ret_0._color1 = COLOR;
    _ret_0._texCoord2 = _texCoord;
    _ret_0._t1 = _OUT._t1;
    _ret_0._t2 = _OUT._t2;
    _ret_0._t3 = _OUT._t3;
    _ret_0._t4 = _OUT._t4;
    gl_Position = _r0009;
    COL0 = COLOR;
    TEX0.xy = _texCoord;
    TEX1 = _OUT._t1;
    TEX2 = _OUT._t2;
    TEX3 = _OUT._t3;
    TEX4 = _OUT._t4;
    return;
    COL0 = _ret_0._color1;
    TEX0.xy = _ret_0._texCoord2;
    TEX1 = _ret_0._t1;
    TEX2 = _ret_0._t2;
    TEX3 = _ret_0._t3;
    TEX4 = _ret_0._t4;
} 
