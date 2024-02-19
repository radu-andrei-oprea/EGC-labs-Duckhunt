#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int switchLight;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


//vec3 point_light_contribution(vec3 light_pos, vec3 light_color)
//{
//	vec3 color;
//	//calculele componentelor difuze si speculare din modelul Phong de iluminare pentru lumina punctiforma.
//	return color;
//}   


void main()
{   
    vec3 N = normalize(world_normal);
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position);
    vec3 H = normalize( L + V );
    vec3 R = reflect (-L, N);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = material_kd * 0.25;

    float diffuse_light = material_kd * max(dot(N, L), 0);
    
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
    }

    float total_light = 0;

    if(switchLight == 0) {

        float kc = 0.5, kl = 0.25, kq = 0.1; //constante pentru atenuare
        float d = distance(light_position, world_position);
        float factorAtenuare = 1/(kc + d*kl + d*d*kq);
        total_light = ambient_light + factorAtenuare * (diffuse_light + specular_light);
    } else {
        // TODO(student): If (and only if) the light is a spotlight, we need to do
        // some additional things.
        float cut_off = radians(30.f);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        float linear_att;
        float light_att_factor;


        if (spot_light > cos(cut_off)){
            // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
            linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            // se calculeaza atenuarea luminii
            light_att_factor = pow(linear_att, 2);
        }

        // TODO(student): Compute the total light. You can just add the components
        // together, but if you're feeling extra fancy, you can add individual
        // colors to the light components. To do that, pick some vec3 colors that
        // you like, and multiply them with the respective light components.
        total_light = ambient_light + linear_att + light_att_factor * (diffuse_light + specular_light);
    }

    // TODO(student): Write pixel out color
    vec3 additional_color = vec3(1,0.7,0.7);

    if(switchLight == 1)
        out_color = vec4(object_color * total_light * additional_color , 1);
    else 
        out_color = vec4(object_color * total_light , 1);

}
