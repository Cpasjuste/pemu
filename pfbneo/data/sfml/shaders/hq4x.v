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

COMPAT_VARYING     vec4 _t6;
COMPAT_VARYING     vec4 _t5;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord;
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

struct out_vertex {
    vec4 _position1;
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
};

out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0027;
vec4 VertexCoord;
vec4 TexCoord;

COMPAT_VARYING vec4 TEX0;
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

    float _x;
    float _y;
    vec2 _dg1;
    vec2 _dg2;
    vec2 _sd1;
    vec2 _sd2;
    vec2 _ddx;
    vec2 _ddy;
    vec2 _TMP11;
    vec2 _TMP12;
    vec2 _TMP13;
    vec2 _TMP14;
    vec2 _TMP15;
    vec2 _TMP16;
    vec2 _TMP17;
    vec2 _TMP18;
    vec2 _TMP19;
    vec2 _TMP20;
    vec2 _TMP21;
    vec2 _TMP22;
    out_vertex _TMP23;

    _x = 5.00000000E-001*(1.00000000E+000/TextureSize.x);
    _y = 5.00000000E-001*(1.00000000E+000/TextureSize.y);
    _dg1 = vec2(_x, _y);
    _dg2 = vec2(-_x, _y);
    _sd1 = _dg1*5.00000000E-001;
    _sd2 = _dg2*5.00000000E-001;
    _ddx = vec2(_x, 0.00000000E+000);
    _ddy = vec2(0.00000000E+000, _y);
    _r0027 = VertexCoord.x*MVPMatrix[0];
    _r0027 = _r0027 + VertexCoord.y*MVPMatrix[1];
    _r0027 = _r0027 + VertexCoord.z*MVPMatrix[2];
    _r0027 = _r0027 + VertexCoord.w*MVPMatrix[3];
    _TMP21 = TexCoord.xy - _sd1;
    _TMP22 = TexCoord.xy - _ddy;
    _TMP23._t1 = vec4(_TMP21.x, _TMP21.y, _TMP22.x, _TMP22.y);
    _TMP19 = TexCoord.xy - _sd2;
    _TMP20 = TexCoord.xy + _ddx;
    _TMP23._t2 = vec4(_TMP19.x, _TMP19.y, _TMP20.x, _TMP20.y);
    _TMP17 = TexCoord.xy + _sd1;
    _TMP18 = TexCoord.xy + _ddy;
    _TMP23._t3 = vec4(_TMP17.x, _TMP17.y, _TMP18.x, _TMP18.y);
    _TMP15 = TexCoord.xy + _sd2;
    _TMP16 = TexCoord.xy - _ddx;
    _TMP23._t4 = vec4(_TMP15.x, _TMP15.y, _TMP16.x, _TMP16.y);
    _TMP13 = TexCoord.xy - _dg1;
    _TMP14 = TexCoord.xy - _dg2;
    _TMP23._t5 = vec4(_TMP13.x, _TMP13.y, _TMP14.x, _TMP14.y);
    _TMP11 = TexCoord.xy + _dg1;
    _TMP12 = TexCoord.xy + _dg2;
    _TMP23._t6 = vec4(_TMP11.x, _TMP11.y, _TMP12.x, _TMP12.y);
    _ret_0._position1 = _r0027;
    _ret_0._texCoord = TexCoord.xy;
    _ret_0._t1 = _TMP23._t1;
    _ret_0._t2 = _TMP23._t2;
    _ret_0._t3 = _TMP23._t3;
    _ret_0._t4 = _TMP23._t4;
    _ret_0._t5 = _TMP23._t5;
    _ret_0._t6 = _TMP23._t6;
    gl_Position = vec4(float(_r0027.x), float(_r0027.y), float(_r0027.z), float(_r0027.w));

    TEX0.xy = TexCoord.xy;
    TEX1 = _TMP23._t1;
    TEX2 = _TMP23._t2;
    TEX3 = _TMP23._t3;
    TEX4 = _TMP23._t4;
    TEX5 = _TMP23._t5;
    TEX6 = _TMP23._t6;

    return;

}
