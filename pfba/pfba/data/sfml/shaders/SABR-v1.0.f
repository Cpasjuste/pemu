#ifdef GL_ES
precision mediump float;
#endif
varying     vec4 _t1;
varying     vec2 _texCoord;
varying     float _frame_rotation;
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
};
vec4 _ret_0;
vec3 _TMP56;
vec3 _TMP57;
vec3 _TMP58;
vec3 _TMP59;
vec3 _TMP55;
vec3 _TMP51;
vec3 _TMP52;
vec3 _TMP53;
vec3 _TMP54;
vec3 _TMP50;
vec3 _TMP46;
vec3 _TMP47;
vec3 _TMP48;
vec3 _TMP49;
vec3 _TMP45;
vec3 _TMP41;
vec3 _TMP42;
vec3 _TMP43;
vec3 _TMP44;
vec3 _TMP40;
bool _TMP39;
bool _TMP38;
vec4 _TMP70;
float _TMP63;
float _TMP62;
float _TMP61;
float _TMP60;
vec4 _TMP23;
vec4 _TMP22;
vec4 _TMP21;
vec4 _TMP19;
vec4 _TMP18;
vec4 _TMP17;
vec4 _TMP16;
vec4 _TMP15;
vec4 _TMP14;
vec4 _TMP13;
vec4 _TMP12;
vec4 _TMP11;
vec4 _TMP10;
vec4 _TMP9;
vec4 _TMP8;
vec4 _TMP7;
vec4 _TMP6;
vec4 _TMP5;
vec4 _TMP3;
vec4 _TMP2;
vec4 _TMP1;
uniform sampler2D Texture;
input_dummy _IN1;
vec2 _x0075;
vec2 _c0079;
vec2 _c0081;
vec2 _c0083;
vec2 _c0087;
vec2 _c0089;
vec2 _c0091;
vec2 _c0093;
vec2 _c0095;
vec2 _c0097;
vec2 _c0099;
vec2 _c0103;
vec2 _c0105;
vec2 _c0107;
vec2 _c0109;
vec2 _c0111;
vec2 _c0113;
vec2 _c0115;
vec2 _c0119;
vec2 _c0121;
vec2 _c0123;
vec4 _x0257;
vec4 _TMP258;
vec4 _x0267;
vec4 _TMP268;
vec4 _x0277;
vec4 _TMP278;
vec4 _x0287;
vec4 _TMP288;
vec4 _TMP296;
vec4 _a0299;
vec4 _TMP300;
vec4 _a0303;
vec4 _TMP304;
vec4 _a0307;
vec4 _TMP308;
vec4 _a0311;
vec4 _TMP312;
vec4 _a0315;
vec4 _TMP318;
vec4 _a0321;
vec4 _TMP322;
vec4 _a0325;
vec4 _TMP326;
vec4 _a0329;
vec4 _TMP330;
vec4 _a0333;
vec4 _TMP334;
vec4 _a0337;
bvec4 _TMP338;
vec4 _TMP340;
vec4 _a0343;
bvec4 _TMP346;
vec4 _TMP348;
vec4 _a0351;
bvec4 _TMP354;
vec4 _TMP356;
vec4 _a0359;
bvec4 _TMP362;
vec4 _TMP364;
vec4 _a0367;
bvec4 _TMP370;
vec4 _TMP372;
vec4 _a0375;
bvec4 _TMP378;
vec4 _TMP380;
vec4 _a0383;
bvec4 _TMP386;
vec4 _TMP388;
vec4 _a0391;
bvec4 _TMP394;
vec4 _TMP396;
vec4 _a0399;
bvec4 _TMP402;
vec4 _TMP404;
vec4 _a0407;
bvec4 _TMP410;
vec4 _TMP412;
vec4 _a0415;
bvec4 _TMP418;
vec4 _TMP420;
vec4 _a0423;
vec4 _TMP426;
vec4 _a0429;
vec4 _TMP430;
vec4 _a0433;
vec4 _TMP434;
vec4 _a0437;
vec4 _TMP438;
vec4 _a0441;
varying vec4 TEX0;
varying vec4 TEX1;

uniform int FrameDirection;
uniform int FrameCount;
#ifdef GL_ES
uniform mediump vec2 OutputSize;
uniform mediump vec2 TextureSize;
uniform mediump vec2 InputSize;
#else
uniform vec2 OutputSize;
uniform vec2 TextureSize;
uniform vec2 InputSize;
#endif
void main()
{
    vec2 _fp;
    vec2 _dx;
    vec2 _dy;
    vec4 _p7;
    vec4 _p8;
    vec4 _p11;
    vec4 _p12;
    vec4 _p13;
    vec4 _p14;
    vec4 _p16;
    vec4 _p17;
    vec4 _p18;
    vec4 _p19;
    vec4 _p22;
    vec4 _p23;
    vec4 _t45;
    vec4 _t15;
    vec4 _t60;
    bvec4 _fx45;
    bvec4 _fx15;
    bvec4 _fx60;
    bvec4 _fxrn;
    vec4 _ma45;
    vec4 _ma15;
    vec4 _ma60;
    vec4 _marn;
    vec4 _e45;
    vec4 _econt;
    bvec4 _r45_1;
    bvec4 _r45_2;
    bvec4 _r45_3;
    bvec4 _r45_4_1;
    bvec4 _r45_4_2;
    bvec4 _r45_4;
    bvec4 _r45_5;
    bvec4 _r45;
    bvec4 _r15;
    bvec4 _r60;
    bvec4 _edr45;
    bvec4 _edr15;
    bvec4 _edr60;
    bvec4 _edrrn;
    bvec4 _px;
    vec3 _res;
    _x0075 = TEX0.xy*TextureSize;
    _fp = fract(_x0075);
    _dx = vec2(float(TEX1.x), float(TEX1.y));
    _dy = vec2(float(TEX1.z), float(TEX1.w));
    _c0079 = (TEX0.xy - _dx) - 2.00000000E+000*_dy;
    _TMP1 = texture2D(Texture, _c0079);
    _c0081 = TEX0.xy - 2.00000000E+000*_dy;
    _TMP2 = texture2D(Texture, _c0081);
    _c0083 = (TEX0.xy + _dx) - 2.00000000E+000*_dy;
    _TMP3 = texture2D(Texture, _c0083);
    _c0087 = (TEX0.xy - 2.00000000E+000*_dx) - _dy;
    _TMP5 = texture2D(Texture, _c0087);
    _c0089 = (TEX0.xy - _dx) - _dy;
    _TMP6 = texture2D(Texture, _c0089);
    _c0091 = TEX0.xy - _dy;
    _TMP7 = texture2D(Texture, _c0091);
    _c0093 = (TEX0.xy + _dx) - _dy;
    _TMP8 = texture2D(Texture, _c0093);
    _c0095 = (TEX0.xy + 2.00000000E+000*_dx) - _dy;
    _TMP9 = texture2D(Texture, _c0095);
    _c0097 = TEX0.xy - 2.00000000E+000*_dx;
    _TMP10 = texture2D(Texture, _c0097);
    _c0099 = TEX0.xy - _dx;
    _TMP11 = texture2D(Texture, _c0099);
    _TMP12 = texture2D(Texture, TEX0.xy);
    _c0103 = TEX0.xy + _dx;
    _TMP13 = texture2D(Texture, _c0103);
    _c0105 = TEX0.xy + 2.00000000E+000*_dx;
    _TMP14 = texture2D(Texture, _c0105);
    _c0107 = (TEX0.xy - 2.00000000E+000*_dx) + _dy;
    _TMP15 = texture2D(Texture, _c0107);
    _c0109 = (TEX0.xy - _dx) + _dy;
    _TMP16 = texture2D(Texture, _c0109);
    _c0111 = TEX0.xy + _dy;
    _TMP17 = texture2D(Texture, _c0111);
    _c0113 = TEX0.xy + _dx + _dy;
    _TMP18 = texture2D(Texture, _c0113);
    _c0115 = TEX0.xy + 2.00000000E+000*_dx + _dy;
    _TMP19 = texture2D(Texture, _c0115);
    _c0119 = (TEX0.xy - _dx) + 2.00000000E+000*_dy;
    _TMP21 = texture2D(Texture, _c0119);
    _c0121 = TEX0.xy + 2.00000000E+000*_dy;
    _TMP22 = texture2D(Texture, _c0121);
    _c0123 = TEX0.xy + _dx + 2.00000000E+000*_dy;
    _TMP23 = texture2D(Texture, _c0123);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP7.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP11.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP17.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP13.xyz);
    _p7 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP8.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP6.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP16.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP18.xyz);
    _p8 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP11.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP17.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP13.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP7.xyz);
    _p11 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP12.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP12.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP12.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP12.xyz);
    _p12 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP13.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP7.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP11.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP17.xyz);
    _p13 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP14.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP2.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP10.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP22.xyz);
    _p14 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP16.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP18.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP8.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP6.xyz);
    _p16 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP17.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP13.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP7.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP11.xyz);
    _p17 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP18.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP8.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP6.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP16.xyz);
    _p18 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP19.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP3.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP5.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP21.xyz);
    _p19 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP22.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP14.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP2.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP10.xyz);
    _p22 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _TMP60 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP23.xyz);
    _TMP61 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP9.xyz);
    _TMP62 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP1.xyz);
    _TMP63 = dot(vec3( 1.02047997E+001, 3.43296013E+001, 3.46560001E+000), _TMP15.xyz);
    _p23 = vec4(_TMP60, _TMP61, _TMP62, _TMP63);
    _t45 = vec4( 1.00000000E+000, -1.00000000E+000, -1.00000000E+000, 1.00000000E+000)*_fp.y + vec4( 1.00000000E+000, 1.00000000E+000, -1.00000000E+000, -1.00000000E+000)*_fp.x;
    _t15 = vec4( 1.00000000E+000, -1.00000000E+000, -1.00000000E+000, 1.00000000E+000)*_fp.y + vec4( 5.00000000E-001, 2.00000000E+000, -5.00000000E-001, -2.00000000E+000)*_fp.x;
    _t60 = vec4( 1.00000000E+000, -1.00000000E+000, -1.00000000E+000, 1.00000000E+000)*_fp.y + vec4( 2.00000000E+000, 5.00000000E-001, -2.00000000E+000, -5.00000000E-001)*_fp.x;
    _fx45 = bvec4(_t45.x > 1.29999995E+000, _t45.y > 3.00000012E-001, _t45.z > -6.99999988E-001, _t45.w > 3.00000012E-001);
    _fx15 = bvec4(_t15.x > 8.99999976E-001, _t15.y > 8.00000012E-001, _t15.z > -6.00000024E-001, _t15.w > -2.00000003E-001);
    _fx60 = bvec4(_t60.x > 1.79999995E+000, _t60.y > -1.00000001E-001, _t60.z > -1.20000005E+000, _t60.w > 4.00000006E-001);
    _fxrn = bvec4(_t45.x > 1.50000000E+000, _t45.y > 5.00000000E-001, _t45.z > -5.00000000E-001, _t45.w > 5.00000000E-001);
    _x0257 = (_t45 - vec4( 1.10000002E+000, 9.99999940E-002, -8.99999976E-001, 9.99999940E-002))/vec4( 7.99999952E-001, 7.99999952E-001, 7.99999952E-001, 7.99999952E-001);
    _TMP70 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _x0257);
    _TMP258 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP70);
    _ma45 = _TMP258*_TMP258*(3.00000000E+000 - 2.00000000E+000*_TMP258);
    _x0267 = (_t15 - vec4( 8.00000012E-001, 6.00000024E-001, -6.99999988E-001, -4.00000006E-001))/vec4( 4.00000036E-001, 7.99999952E-001, 3.99999976E-001, 8.00000012E-001);
    _TMP70 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _x0267);
    _TMP268 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP70);
    _ma15 = _TMP268*_TMP268*(3.00000000E+000 - 2.00000000E+000*_TMP268);
    _x0277 = (_t60 - vec4( 1.60000002E+000, -2.00000003E-001, -1.39999998E+000, 3.00000012E-001))/vec4( 8.00000072E-001, 4.00000006E-001, 7.99999952E-001, 3.99999976E-001);
    _TMP70 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _x0277);
    _TMP278 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP70);
    _ma60 = _TMP278*_TMP278*(3.00000000E+000 - 2.00000000E+000*_TMP278);
    _x0287 = (_t45 - vec4( 1.30000007E+000, 3.00000012E-001, -6.99999988E-001, 3.00000012E-001))/vec4( 7.99999833E-001, 8.00000012E-001, 8.00000012E-001, 8.00000012E-001);
    _TMP70 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _x0287);
    _TMP288 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP70);
    _marn = _TMP288*_TMP288*(3.00000000E+000 - 2.00000000E+000*_TMP288);
    _a0299 = _p12 - _p8;
    _TMP296 = abs(_a0299);
    _a0303 = _p12 - _p16;
    _TMP300 = abs(_a0303);
    _a0307 = _p18 - _p22;
    _TMP304 = abs(_a0307);
    _a0311 = _p18 - _p14;
    _TMP308 = abs(_a0311);
    _a0315 = _p17 - _p13;
    _TMP312 = abs(_a0315);
    _e45 = _TMP296 + _TMP300 + _TMP304 + _TMP308 + 4.00000000E+000*_TMP312;
    _a0321 = _p17 - _p11;
    _TMP318 = abs(_a0321);
    _a0325 = _p17 - _p23;
    _TMP322 = abs(_a0325);
    _a0329 = _p13 - _p7;
    _TMP326 = abs(_a0329);
    _a0333 = _p13 - _p19;
    _TMP330 = abs(_a0333);
    _a0337 = _p12 - _p18;
    _TMP334 = abs(_a0337);
    _econt = _TMP318 + _TMP322 + _TMP326 + _TMP330 + 4.00000000E+000*_TMP334;
    _r45_1 = bvec4(_p12.x != _p13.x && _p12.x != _p17.x, _p12.y != _p13.y && _p12.y != _p17.y, _p12.z != _p13.z && _p12.z != _p17.z, _p12.w != _p13.w && _p12.w != _p17.w);
    _a0343 = _p13 - _p7;
    _TMP340 = abs(_a0343);
    _TMP338 = bvec4(_TMP340.x < 2.00000000E+001, _TMP340.y < 2.00000000E+001, _TMP340.z < 2.00000000E+001, _TMP340.w < 2.00000000E+001);
    _a0351 = _p13 - _p8;
    _TMP348 = abs(_a0351);
    _TMP346 = bvec4(_TMP348.x < 2.00000000E+001, _TMP348.y < 2.00000000E+001, _TMP348.z < 2.00000000E+001, _TMP348.w < 2.00000000E+001);
    _r45_2 = bvec4(!_TMP338.x && !_TMP346.x, !_TMP338.y && !_TMP346.y, !_TMP338.z && !_TMP346.z, !_TMP338.w && !_TMP346.w);
    _a0359 = _p17 - _p11;
    _TMP356 = abs(_a0359);
    _TMP354 = bvec4(_TMP356.x < 2.00000000E+001, _TMP356.y < 2.00000000E+001, _TMP356.z < 2.00000000E+001, _TMP356.w < 2.00000000E+001);
    _a0367 = _p17 - _p16;
    _TMP364 = abs(_a0367);
    _TMP362 = bvec4(_TMP364.x < 2.00000000E+001, _TMP364.y < 2.00000000E+001, _TMP364.z < 2.00000000E+001, _TMP364.w < 2.00000000E+001);
    _r45_3 = bvec4(!_TMP354.x && !_TMP362.x, !_TMP354.y && !_TMP362.y, !_TMP354.z && !_TMP362.z, !_TMP354.w && !_TMP362.w);
    _a0375 = _p13 - _p14;
    _TMP372 = abs(_a0375);
    _TMP370 = bvec4(_TMP372.x < 2.00000000E+001, _TMP372.y < 2.00000000E+001, _TMP372.z < 2.00000000E+001, _TMP372.w < 2.00000000E+001);
    _a0383 = _p13 - _p19;
    _TMP380 = abs(_a0383);
    _TMP378 = bvec4(_TMP380.x < 2.00000000E+001, _TMP380.y < 2.00000000E+001, _TMP380.z < 2.00000000E+001, _TMP380.w < 2.00000000E+001);
    _r45_4_1 = bvec4(!_TMP370.x && !_TMP378.x, !_TMP370.y && !_TMP378.y, !_TMP370.z && !_TMP378.z, !_TMP370.w && !_TMP378.w);
    _a0391 = _p17 - _p22;
    _TMP388 = abs(_a0391);
    _TMP386 = bvec4(_TMP388.x < 2.00000000E+001, _TMP388.y < 2.00000000E+001, _TMP388.z < 2.00000000E+001, _TMP388.w < 2.00000000E+001);
    _a0399 = _p17 - _p23;
    _TMP396 = abs(_a0399);
    _TMP394 = bvec4(_TMP396.x < 2.00000000E+001, _TMP396.y < 2.00000000E+001, _TMP396.z < 2.00000000E+001, _TMP396.w < 2.00000000E+001);
    _r45_4_2 = bvec4(!_TMP386.x && !_TMP394.x, !_TMP386.y && !_TMP394.y, !_TMP386.z && !_TMP394.z, !_TMP386.w && !_TMP394.w);
    _a0407 = _p12 - _p18;
    _TMP404 = abs(_a0407);
    _TMP402 = bvec4(_TMP404.x < 2.00000000E+001, _TMP404.y < 2.00000000E+001, _TMP404.z < 2.00000000E+001, _TMP404.w < 2.00000000E+001);
    _r45_4 = bvec4(_TMP402.x && (_r45_4_1.x || _r45_4_2.x), _TMP402.y && (_r45_4_1.y || _r45_4_2.y), _TMP402.z && (_r45_4_1.z || _r45_4_2.z), _TMP402.w && (_r45_4_1.w || _r45_4_2.w));
    _a0415 = _p12 - _p16;
    _TMP412 = abs(_a0415);
    _TMP410 = bvec4(_TMP412.x < 2.00000000E+001, _TMP412.y < 2.00000000E+001, _TMP412.z < 2.00000000E+001, _TMP412.w < 2.00000000E+001);
    _a0423 = _p12 - _p8;
    _TMP420 = abs(_a0423);
    _TMP418 = bvec4(_TMP420.x < 2.00000000E+001, _TMP420.y < 2.00000000E+001, _TMP420.z < 2.00000000E+001, _TMP420.w < 2.00000000E+001);
    _r45_5 = bvec4(_TMP410.x || _TMP418.x, _TMP410.y || _TMP418.y, _TMP410.z || _TMP418.z, _TMP410.w || _TMP418.w);
    _r45 = bvec4(_r45_1.x && (_r45_2.x || _r45_3.x || _r45_4.x || _r45_5.x), _r45_1.y && (_r45_2.y || _r45_3.y || _r45_4.y || _r45_5.y), _r45_1.z && (_r45_2.z || _r45_3.z || _r45_4.z || _r45_5.z), _r45_1.w && (_r45_2.w || _r45_3.w || _r45_4.w || _r45_5.w));
    _a0429 = _p13 - _p16;
    _TMP426 = abs(_a0429);
    _r15 = bvec4(_p12.x != _p16.x && _p11.x != _p16.x, _p12.y != _p16.y && _p11.y != _p16.y, _p12.z != _p16.z && _p11.z != _p16.z, _p12.w != _p16.w && _p11.w != _p16.w);
    _a0433 = _p8 - _p17;
    _TMP430 = abs(_a0433);
    _r60 = bvec4(_p12.x != _p8.x && _p7.x != _p8.x, _p12.y != _p8.y && _p7.y != _p8.y, _p12.z != _p8.z && _p7.z != _p8.z, _p12.w != _p8.w && _p7.w != _p8.w);
    _edr45 = bvec4(_e45.x < _econt.x && _r45.x, _e45.y < _econt.y && _r45.y, _e45.z < _econt.z && _r45.z, _e45.w < _econt.w && _r45.w);
    _edr15 = bvec4(_edr45.x && (2.00000000E+000*_TMP426).x <= _TMP430.x && _r15.x, _edr45.y && (2.00000000E+000*_TMP426).y <= _TMP430.y && _r15.y, _edr45.z && (2.00000000E+000*_TMP426).z <= _TMP430.z && _r15.z, _edr45.w && (2.00000000E+000*_TMP426).w <= _TMP430.w && _r15.w);
    _edr60 = bvec4(_edr45.x && (2.00000000E+000*_TMP430).x <= _TMP426.x && _r60.x, _edr45.y && (2.00000000E+000*_TMP430).y <= _TMP426.y && _r60.y, _edr45.z && (2.00000000E+000*_TMP430).z <= _TMP426.z && _r60.z, _edr45.w && (2.00000000E+000*_TMP430).w <= _TMP426.w && _r60.w);
    _edrrn = bvec4(_e45.x <= _econt.x, _e45.y <= _econt.y, _e45.z <= _econt.z, _e45.w <= _econt.w);
    _a0437 = _p12 - _p13;
    _TMP434 = abs(_a0437);
    _a0441 = _p12 - _p17;
    _TMP438 = abs(_a0441);
    _px = bvec4(_TMP434.x <= _TMP438.x, _TMP434.y <= _TMP438.y, _TMP434.z <= _TMP438.z, _TMP434.w <= _TMP438.w);
    _res = _TMP12.xyz;
    _TMP38 = _edr45.x || _edr45.y || _edr45.z || _edr45.w;
    _TMP39 = _edrrn.x || _edrrn.y || _edrrn.z || _edrrn.w;
    if (_TMP38 || _TMP39) {
        if (_px.x) {
            _TMP40 = _TMP13.xyz;
        } else {
            _TMP40 = _TMP17.xyz;
        }
        if (_edr15.x && _fx15.x) {
            _TMP41 = _TMP12.xyz + _ma15.x*(_TMP40 - _TMP12.xyz);
        } else {
            if (_edr60.x && _fx60.x) {
                _TMP42 = _TMP12.xyz + _ma60.x*(_TMP40 - _TMP12.xyz);
            } else {
                if (_edr45.x && _fx45.x) {
                    _TMP43 = _TMP12.xyz + _ma45.x*(_TMP40 - _TMP12.xyz);
                } else {
                    if (_edrrn.x && _fxrn.x) {
                        _TMP44 = _TMP12.xyz + _marn.x*(_TMP40 - _TMP12.xyz);
                    } else {
                        _TMP44 = _TMP12.xyz;
                    }
                    _TMP43 = _TMP44;
                }
                _TMP42 = _TMP43;
            }
            _TMP41 = _TMP42;
        }
        if (_px.y) {
            _TMP45 = _TMP7.xyz;
        } else {
            _TMP45 = _TMP13.xyz;
        }
        if (_edr15.y && _fx15.y) {
            _TMP46 = _TMP41 + _ma15.y*(_TMP45 - _TMP41);
        } else {
            if (_edr60.y && _fx60.y) {
                _TMP47 = _TMP41 + _ma60.y*(_TMP45 - _TMP41);
            } else {
                if (_edr45.y && _fx45.y) {
                    _TMP48 = _TMP41 + _ma45.y*(_TMP45 - _TMP41);
                } else {
                    if (_edrrn.y && _fxrn.y) {
                        _TMP49 = _TMP41 + _marn.y*(_TMP45 - _TMP41);
                    } else {
                        _TMP49 = _TMP41;
                    }
                    _TMP48 = _TMP49;
                }
                _TMP47 = _TMP48;
            }
            _TMP46 = _TMP47;
        }
        if (_px.z) {
            _TMP50 = _TMP11.xyz;
        } else {
            _TMP50 = _TMP7.xyz;
        }
        if (_edr15.z && _fx15.z) {
            _TMP51 = _TMP46 + _ma15.z*(_TMP50 - _TMP46);
        } else {
            if (_edr60.z && _fx60.z) {
                _TMP52 = _TMP46 + _ma60.z*(_TMP50 - _TMP46);
            } else {
                if (_edr45.z && _fx45.z) {
                    _TMP53 = _TMP46 + _ma45.z*(_TMP50 - _TMP46);
                } else {
                    if (_edrrn.z && _fxrn.z) {
                        _TMP54 = _TMP46 + _marn.z*(_TMP50 - _TMP46);
                    } else {
                        _TMP54 = _TMP46;
                    }
                    _TMP53 = _TMP54;
                }
                _TMP52 = _TMP53;
            }
            _TMP51 = _TMP52;
        }
        if (_px.w) {
            _TMP55 = _TMP17.xyz;
        } else {
            _TMP55 = _TMP11.xyz;
        }
        if (_edr15.w && _fx15.w) {
            _TMP56 = _TMP51 + _ma15.w*(_TMP55 - _TMP51);
        } else {
            if (_edr60.w && _fx60.w) {
                _TMP57 = _TMP51 + _ma60.w*(_TMP55 - _TMP51);
            } else {
                if (_edr45.w && _fx45.w) {
                    _TMP58 = _TMP51 + _ma45.w*(_TMP55 - _TMP51);
                } else {
                    if (_edrrn.w && _fxrn.w) {
                        _TMP59 = _TMP51 + _marn.w*(_TMP55 - _TMP51);
                    } else {
                        _TMP59 = _TMP51;
                    }
                    _TMP58 = _TMP59;
                }
                _TMP57 = _TMP58;
            }
            _TMP56 = _TMP57;
        }
        _res = _TMP56;
    }
    _ret_0 = vec4(_res.x, _res.y, _res.z, 1.00000000E+000);
    gl_FragColor = _ret_0;
    return;
}
