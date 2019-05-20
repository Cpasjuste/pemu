varying     vec2 VARc22;
varying     vec2 VARc21;
varying     vec2 VARc20;
varying     vec2 VARc12;
varying     vec2 VARc11;
varying     vec2 VARc10;
varying     vec2 VARc02;
varying     vec2 VARc01;
varying     vec2 VARc00;
struct tex_coords {
    vec2 VARc00;
    vec2 VARc01;
    vec2 VARc02;
    vec2 VARc10;
    vec2 VARc11;
    vec2 VARc12;
    vec2 VARc20;
    vec2 VARc21;
    vec2 VARc22;
};
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
vec4 _oPosition1;
tex_coords _coords1;
uniform mat4 MVPMatrix;
input_dummy _IN1;
vec4 _r0009;
vec4 VertexCoord;
vec4 COLOR;
varying vec4 COL0;
vec4 TexCoord;


mat4 transpose_(mat4 matrix)
{
mat4 ret;
for (int i = 0; i != 4; i++)
for (int j = 0; j != 4; j++)
ret[i][j] = matrix[j][i];

return ret;
}

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
    VertexCoord = gl_Vertex;
    TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    COLOR = gl_Color;

    mat4 MVPMatrix_ = transpose_(MVPMatrix);
    vec4 _oColor;
    vec2 _delta;
    tex_coords _TMP7;
    _r0009.x = dot(MVPMatrix_[0], VertexCoord);
    _r0009.y = dot(MVPMatrix_[1], VertexCoord);
    _r0009.z = dot(MVPMatrix_[2], VertexCoord);
    _r0009.w = dot(MVPMatrix_[3], VertexCoord);
    _oPosition1 = _r0009;
    _oColor = COLOR;
    _delta = 5.00000000E-001/TextureSize;
    _TMP7.VARc00 = TexCoord.xy + vec2(-_delta.x, -_delta.y);
    _TMP7.VARc01 = TexCoord.xy + vec2(-_delta.x, 0.00000000E+000);
    _TMP7.VARc02 = TexCoord.xy + vec2(-_delta.x, _delta.y);
    _TMP7.VARc10 = TexCoord.xy + vec2(0.00000000E+000, -_delta.y);
    _TMP7.VARc12 = TexCoord.xy + vec2(0.00000000E+000, _delta.y);
    _TMP7.VARc20 = TexCoord.xy + vec2(_delta.x, -_delta.y);
    _TMP7.VARc21 = TexCoord.xy + vec2(_delta.x, 0.00000000E+000);
    _TMP7.VARc22 = TexCoord.xy + vec2(_delta.x, _delta.y);
    VARc00 = _TMP7.VARc00;
    VARc01 = _TMP7.VARc01;
    VARc02 = _TMP7.VARc02;
    VARc10 = _TMP7.VARc10;
    VARc11 = TexCoord.xy;
    VARc12 = _TMP7.VARc12;
    VARc20 = _TMP7.VARc20;
    VARc21 = _TMP7.VARc21;
    VARc22 = _TMP7.VARc22;
    gl_Position = _r0009;
    COL0 = COLOR;
} 