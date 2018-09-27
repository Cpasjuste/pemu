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
COMPAT_VARYING     float _frame_rotation;
COMPAT_VARYING     vec2 _pixel_no;
COMPAT_VARYING     vec2 _c11;
COMPAT_VARYING     vec4 _c12_22;
COMPAT_VARYING     vec4 _c21_02;
COMPAT_VARYING     vec4 _c20_01;
COMPAT_VARYING     vec4 _c00_10;
struct tex_coord {
    vec4 _c00_10;
    vec4 _c20_01;
    vec4 _c21_02;
    vec4 _c12_22;
    vec2 _c11;
    vec2 _pixel_no;
};
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
vec4 _oPosition1;
input_dummy _IN1;
vec4 _r0017;
vec4 VertexCoord;
vec4 COLOR;
COMPAT_VARYING vec4 COL0;
vec4 TexCoord;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
COMPAT_VARYING vec4 TEX5;
COMPAT_VARYING vec4 TEX6;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    VertexCoord = gl_Vertex;
	TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	COLOR = gl_Color;

    vec4 _oColor;
    tex_coord _coords;
    float _dx;
    float _dy;
    vec2 _TMP6;
    vec2 _TMP7;
    vec2 _TMP8;
    vec2 _TMP9;
    vec2 _TMP10;
    vec2 _TMP11;
    vec2 _TMP12;
    vec2 _TMP13;
    _r0017 = VertexCoord.x*MVPMatrix[0];
    _r0017 = _r0017 + VertexCoord.y*MVPMatrix[1];
    _r0017 = _r0017 + VertexCoord.z*MVPMatrix[2];
    _r0017 = _r0017 + VertexCoord.w*MVPMatrix[3];
    _oPosition1 = _r0017;
    _oColor = COLOR;
    _dx = 1.00000000E+00/TextureSize.x;
    _dy = 1.00000000E+00/TextureSize.y;
    _TMP12 = TexCoord.xy + vec2(-_dx, -_dy);
    _TMP13 = TexCoord.xy + vec2(0.00000000E+00, -_dy);
    _coords._c00_10 = vec4(_TMP12.x, _TMP12.y, _TMP13.x, _TMP13.y);
    _TMP10 = TexCoord.xy + vec2(_dx, -_dy);
    _TMP11 = TexCoord.xy + vec2(-_dx, 0.00000000E+00);
    _coords._c20_01 = vec4(_TMP10.x, _TMP10.y, _TMP11.x, _TMP11.y);
    _TMP8 = TexCoord.xy + vec2(_dx, 0.00000000E+00);
    _TMP9 = TexCoord.xy + vec2(-_dx, _dy);
    _coords._c21_02 = vec4(_TMP8.x, _TMP8.y, _TMP9.x, _TMP9.y);
    _TMP6 = TexCoord.xy + vec2(0.00000000E+00, _dy);
    _TMP7 = TexCoord.xy + vec2(_dx, _dy);
    _coords._c12_22 = vec4(_TMP6.x, _TMP6.y, _TMP7.x, _TMP7.y);
    _coords._c11 = TexCoord.xy;
    _coords._pixel_no = TexCoord.xy*TextureSize;
    gl_Position = _r0017;
    COL0 = COLOR;
    TEX1 = _coords._c00_10;
    TEX2 = _coords._c20_01;
    TEX3 = _coords._c21_02;
    TEX4 = _coords._c12_22;
    TEX5.xy = TexCoord.xy;
    TEX6.xy = _coords._pixel_no;
}
