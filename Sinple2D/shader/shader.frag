#version 330 core

out vec4 Color;

in vec2 texcoord;
in vec4 color;

uniform sampler2D defaultTexture;
uniform int bRenderTexture;

void main()
{
    if( bRenderTexture != 0 ){
        Color = texture(defaultTexture, texcoord) * color * color.w;
    }
    else{
        Color = color;
    }
}