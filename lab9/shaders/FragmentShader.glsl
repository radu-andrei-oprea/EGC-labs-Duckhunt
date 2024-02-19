#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform float time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color_1 = texture2D(texture_1, texcoord);
    vec4 color_2 = texture2D(texture_2, texcoord);
	
    //out_color = color;
    out_color = mix(color_1, color_2, 0.5f);

    float alpha = out_color.a;
    if(alpha < 0.5f){
        discard;
    }

}
