varying     float _frame_rotation;

varying     vec2 _one;

varying     float _mod_factor;

varying     vec2 _texCoord;





struct tex_coords {

    vec2 _texCoord;

    float _mod_factor;

    vec2 _one;

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

tex_coords _coords1;

uniform mat4 MVPMatrix;

input_dummy _IN1;

vec4 _r0003;

vec4 VertexCoord;

vec4 COLOR;

varying vec4 COL0;

vec4 TexCoord;

varying vec4 TEX0;

varying vec4 TEX1;

varying vec4 TEX2;



 

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



    _r0003.x = dot(MVPMatrix_[0], VertexCoord);

    _r0003.y = dot(MVPMatrix_[1], VertexCoord);

    _r0003.z = dot(MVPMatrix_[2], VertexCoord);

    _r0003.w = dot(MVPMatrix_[3], VertexCoord);

    _oPosition1 = _r0003;

    _oColor = COLOR;

    _coords1._texCoord = TexCoord.xy;

    _coords1._mod_factor = (TexCoord.x*OutputSize.x*TextureSize.x)/InputSize.x;

    _coords1._one = 1.00000000E+000/TextureSize;

    gl_Position = _r0003;

    COL0 = COLOR;

    TEX0.xy = TexCoord.xy;

    TEX1.x = _coords1._mod_factor;

    TEX2.xy = _coords1._one;

}
