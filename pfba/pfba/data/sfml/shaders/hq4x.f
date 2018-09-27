#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 FragColor;
#else
#define COMPAT_VARYING varying
#define FragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
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
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
};

vec4 _ret_0;
float _TMP43;
float _TMP41;
float _TMP42;
float _TMP38;
float _TMP37;
float _TMP40;
vec3 _TMP36;
vec3 _TMP35;
vec3 _TMP39;
float _TMP34;
float _TMP33;
vec3 _TMP32;
vec3 _TMP31;
float _TMP30;
float _TMP29;
vec3 _TMP28;
vec3 _TMP27;
float _TMP26;
float _TMP25;
vec3 _TMP24;
vec3 _TMP23;
float _TMP22;
float _TMP21;
vec3 _TMP20;
float _TMP19;
float _TMP18;
vec3 _TMP17;
vec3 _TMP16;
vec3 _TMP15;
vec3 _TMP14;
vec3 _TMP13;
vec4 _TMP12;
vec4 _TMP11;
vec4 _TMP10;
vec4 _TMP9;
vec4 _TMP8;
vec4 _TMP7;
vec4 _TMP6;
vec4 _TMP5;
vec4 _TMP4;
vec4 _TMP3;
vec4 _TMP2;
vec4 _TMP1;
vec4 _TMP0;

uniform sampler2D Texture;

vec2 _c0054;
vec2 _c0056;
vec2 _c0058;
vec2 _c0060;
vec2 _c0062;
vec2 _c0064;
vec2 _c0066;
vec2 _c0068;

vec2 _c0070;

vec2 _c0072;

vec2 _c0074;

vec2 _c0076;

vec2 _c0078;

vec3 _a0080;

vec3 _a0084;

vec3 _a0088;

vec3 _a0092;

vec3 _a0096;

vec3 _a0104;

vec3 _a0112;

vec3 _a0114;

vec3 _a0116;

vec3 _a0118;

vec3 _a0120;

vec3 _a0122;

vec3 _a0124;

vec3 _a0126;

vec3 _a0128;

vec3 _a0130;

vec3 _a0132;

vec3 _a0134;

vec3 _a0136;

vec3 _a0138;

vec3 _a0140;

vec3 _a0142;

float _TMP143;

float _x0144;

float _TMP149;

float _x0150;

float _TMP155;

float _x0156;

float _TMP161;

float _x0162;

COMPAT_VARYING vec4 TEX0;

COMPAT_VARYING vec4 TEX1;

COMPAT_VARYING vec4 TEX2;

COMPAT_VARYING vec4 TEX3;

COMPAT_VARYING vec4 TEX4;

COMPAT_VARYING vec4 TEX5;

COMPAT_VARYING vec4 TEX6;



 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()

{



    vec3 _c;

    vec3 _i1;

    vec3 _i2;

    vec3 _i3;

    vec3 _i4;

    vec3 _o1;

    vec3 _o3;

    vec3 _o2;

    vec3 _o4;

    vec3 _s1;

    vec3 _s2;

    vec3 _s3;

    vec3 _s4;

    float _ko1;

    float _ko2;

    float _ko3;

    float _ko4;

    float _k1;

    float _k2;

    float _w1;

    float _w2;

    float _w3;

    float _w4;

    vec3 _TMP52;



    _c0054 = vec2(float(TEX0.x), float(TEX0.y));

    _TMP0 = COMPAT_TEXTURE(Texture, _c0054);

    _c = vec3(float(_TMP0.x), float(_TMP0.y), float(_TMP0.z));

    _c0056 = vec2(float(TEX1.x), float(TEX1.y));

    _TMP1 = COMPAT_TEXTURE(Texture, _c0056);

    _i1 = vec3(float(_TMP1.x), float(_TMP1.y), float(_TMP1.z));

    _c0058 = vec2(float(TEX2.x), float(TEX2.y));

    _TMP2 = COMPAT_TEXTURE(Texture, _c0058);

    _i2 = vec3(float(_TMP2.x), float(_TMP2.y), float(_TMP2.z));

    _c0060 = vec2(float(TEX3.x), float(TEX3.y));

    _TMP3 = COMPAT_TEXTURE(Texture, _c0060);

    _i3 = vec3(float(_TMP3.x), float(_TMP3.y), float(_TMP3.z));

    _c0062 = vec2(float(TEX4.x), float(TEX4.y));

    _TMP4 = COMPAT_TEXTURE(Texture, _c0062);

    _i4 = vec3(float(_TMP4.x), float(_TMP4.y), float(_TMP4.z));

    _c0064 = vec2(float(TEX5.x), float(TEX5.y));

    _TMP5 = COMPAT_TEXTURE(Texture, _c0064);

    _o1 = vec3(float(_TMP5.x), float(_TMP5.y), float(_TMP5.z));

    _c0066 = vec2(float(TEX6.x), float(TEX6.y));

    _TMP6 = COMPAT_TEXTURE(Texture, _c0066);

    _o3 = vec3(float(_TMP6.x), float(_TMP6.y), float(_TMP6.z));

    _c0068 = vec2(float(TEX5.z), float(TEX5.w));

    _TMP7 = COMPAT_TEXTURE(Texture, _c0068);

    _o2 = vec3(float(_TMP7.x), float(_TMP7.y), float(_TMP7.z));

    _c0070 = vec2(float(TEX6.z), float(TEX6.w));

    _TMP8 = COMPAT_TEXTURE(Texture, _c0070);

    _o4 = vec3(float(_TMP8.x), float(_TMP8.y), float(_TMP8.z));

    _c0072 = vec2(float(TEX1.z), float(TEX1.w));

    _TMP9 = COMPAT_TEXTURE(Texture, _c0072);

    _s1 = vec3(float(_TMP9.x), float(_TMP9.y), float(_TMP9.z));

    _c0074 = vec2(float(TEX2.z), float(TEX2.w));

    _TMP10 = COMPAT_TEXTURE(Texture, _c0074);

    _s2 = vec3(float(_TMP10.x), float(_TMP10.y), float(_TMP10.z));

    _c0076 = vec2(float(TEX3.z), float(TEX3.w));

    _TMP11 = COMPAT_TEXTURE(Texture, _c0076);

    _s3 = vec3(float(_TMP11.x), float(_TMP11.y), float(_TMP11.z));

    _c0078 = vec2(float(TEX4.z), float(TEX4.w));

    _TMP12 = COMPAT_TEXTURE(Texture, _c0078);

    _s4 = vec3(float(_TMP12.x), float(_TMP12.y), float(_TMP12.z));

    _a0080 = _o1 - _c;

    _TMP39 = abs(vec3(float(_a0080.x), float(_a0080.y), float(_a0080.z)));

    _TMP13 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP13.x), float(_TMP13.y), float(_TMP13.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _ko1 = float(_TMP40);

    _a0084 = _o2 - _c;

    _TMP39 = abs(vec3(float(_a0084.x), float(_a0084.y), float(_a0084.z)));

    _TMP14 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP14.x), float(_TMP14.y), float(_TMP14.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _ko2 = float(_TMP40);

    _a0088 = _o3 - _c;

    _TMP39 = abs(vec3(float(_a0088.x), float(_a0088.y), float(_a0088.z)));

    _TMP15 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP15.x), float(_TMP15.y), float(_TMP15.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _ko3 = float(_TMP40);

    _a0092 = _o4 - _c;

    _TMP39 = abs(vec3(float(_a0092.x), float(_a0092.y), float(_a0092.z)));

    _TMP16 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP16.x), float(_TMP16.y), float(_TMP16.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _ko4 = float(_TMP40);

    _a0096 = _i1 - _i3;

    _TMP39 = abs(vec3(float(_a0096.x), float(_a0096.y), float(_a0096.z)));

    _TMP17 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP17.x), float(_TMP17.y), float(_TMP17.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP18 = float(_TMP40);

    _TMP41 = max(float(_ko1), float(_ko3));

    _TMP19 = float(_TMP41);

    _TMP42 = min(float(_TMP18), float(_TMP19));

    _k1 = float(_TMP42);

    _a0104 = _i2 - _i4;

    _TMP39 = abs(vec3(float(_a0104.x), float(_a0104.y), float(_a0104.z)));

    _TMP20 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _TMP40 = dot(vec3(float(_TMP20.x), float(_TMP20.y), float(_TMP20.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP21 = float(_TMP40);

    _TMP41 = max(float(_ko2), float(_ko4));

    _TMP22 = float(_TMP41);

    _TMP42 = min(float(_TMP21), float(_TMP22));

    _k2 = float(_TMP42);

    _w1 = _k2;

    if (_ko3 < _ko1) { 
        _w1 = _k2*(_ko3/_ko1);

    } 
    _w2 = _k1;

    if (_ko4 < _ko2) { 
        _w2 = _k1*(_ko4/_ko2);

    } 
    _w3 = _k2;

    if (_ko1 < _ko3) { 
        _w3 = _k2*(_ko1/_ko3);

    } 
    _w4 = _k1;

    if (_ko2 < _ko4) { 
        _w4 = _k1*(_ko2/_ko4);

    } 
    _c = (_w1*_o1 + _w2*_o2 + _w3*_o3 + _w4*_o4 + 1.00040436E-003*_c)/(_w1 + _w2 + _w3 + _w4 + 1.00040436E-003);

    _a0112 = _i1 - _c;

    _TMP39 = abs(vec3(float(_a0112.x), float(_a0112.y), float(_a0112.z)));

    _TMP23 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0114 = _i3 - _c;

    _TMP39 = abs(vec3(float(_a0114.x), float(_a0114.y), float(_a0114.z)));

    _TMP24 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0116 = _TMP23 + _TMP24;

    _TMP40 = dot(vec3(float(_a0116.x), float(_a0116.y), float(_a0116.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP25 = float(_TMP40);

    _a0118 = _i1 + _i3;

    _TMP40 = dot(vec3(float(_a0118.x), float(_a0118.y), float(_a0118.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP26 = float(_TMP40);

    _w1 = (-1.09960938E+000*_TMP25)/(1.25000000E-001*_TMP26 + 3.30078125E-001);

    _a0120 = _i2 - _c;

    _TMP39 = abs(vec3(float(_a0120.x), float(_a0120.y), float(_a0120.z)));

    _TMP27 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0122 = _i4 - _c;

    _TMP39 = abs(vec3(float(_a0122.x), float(_a0122.y), float(_a0122.z)));

    _TMP28 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0124 = _TMP27 + _TMP28;

    _TMP40 = dot(vec3(float(_a0124.x), float(_a0124.y), float(_a0124.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP29 = float(_TMP40);

    _a0126 = _i2 + _i4;

    _TMP40 = dot(vec3(float(_a0126.x), float(_a0126.y), float(_a0126.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP30 = float(_TMP40);

    _w2 = (-1.09960938E+000*_TMP29)/(1.25000000E-001*_TMP30 + 3.30078125E-001);

    _a0128 = _s1 - _c;

    _TMP39 = abs(vec3(float(_a0128.x), float(_a0128.y), float(_a0128.z)));

    _TMP31 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0130 = _s3 - _c;

    _TMP39 = abs(vec3(float(_a0130.x), float(_a0130.y), float(_a0130.z)));

    _TMP32 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0132 = _TMP31 + _TMP32;

    _TMP40 = dot(vec3(float(_a0132.x), float(_a0132.y), float(_a0132.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP33 = float(_TMP40);

    _a0134 = _s1 + _s3;

    _TMP40 = dot(vec3(float(_a0134.x), float(_a0134.y), float(_a0134.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP34 = float(_TMP40);

    _w3 = (-1.09960938E+000*_TMP33)/(1.25000000E-001*_TMP34 + 3.30078125E-001);

    _a0136 = _s2 - _c;

    _TMP39 = abs(vec3(float(_a0136.x), float(_a0136.y), float(_a0136.z)));

    _TMP35 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0138 = _s4 - _c;

    _TMP39 = abs(vec3(float(_a0138.x), float(_a0138.y), float(_a0138.z)));

    _TMP36 = vec3(float(_TMP39.x), float(_TMP39.y), float(_TMP39.z));

    _a0140 = _TMP35 + _TMP36;

    _TMP40 = dot(vec3(float(_a0140.x), float(_a0140.y), float(_a0140.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP37 = float(_TMP40);

    _a0142 = _s2 + _s4;

    _TMP40 = dot(vec3(float(_a0142.x), float(_a0142.y), float(_a0142.z)), vec3( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _TMP38 = float(_TMP40);

    _w4 = (-1.09960938E+000*_TMP37)/(1.25000000E-001*_TMP38 + 3.30078125E-001);

    _x0144 = _w1 + 1.00000000E+000;

    _TMP42 = min(7.50000000E-001, float(_x0144));

    _TMP43 = float(_TMP42);

    _TMP41 = max(2.99987793E-002, float(_TMP43));

    _TMP143 = float(_TMP41);

    _x0150 = _w2 + 1.00000000E+000;

    _TMP42 = min(7.50000000E-001, float(_x0150));

    _TMP43 = float(_TMP42);

    _TMP41 = max(2.99987793E-002, float(_TMP43));

    _TMP149 = float(_TMP41);

    _x0156 = _w3 + 1.00000000E+000;

    _TMP42 = min(7.50000000E-001, float(_x0156));

    _TMP43 = float(_TMP42);

    _TMP41 = max(2.99987793E-002, float(_TMP43));

    _TMP155 = float(_TMP41);

    _x0162 = _w4 + 1.00000000E+000;

    _TMP42 = min(7.50000000E-001, float(_x0162));

    _TMP43 = float(_TMP42);

    _TMP41 = max(2.99987793E-002, float(_TMP43));

    _TMP161 = float(_TMP41);

    _TMP52 = (_TMP143*(_i1 + _i3) + _TMP149*(_i2 + _i4) + _TMP155*(_s1 + _s3) + _TMP161*(_s2 + _s4) + _c)/(2.00000000E+000*(_TMP143 + _TMP149 + _TMP155 + _TMP161) + 1.00000000E+000);

    _ret_0 = vec4(_TMP52.x, _TMP52.y, _TMP52.z, 1.00000000E+000);

    FragColor = vec4(float(_ret_0.x), float(_ret_0.y), float(_ret_0.z), float(_ret_0.w));

    return;

}
