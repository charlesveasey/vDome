#version 150
 
uniform float saturation;
uniform float brightness;
uniform float contrast;
													 
uniform float red;
uniform float green;
uniform float blue;

uniform sampler2DRect texsampler;

uniform float scale;

in vec2 vtexcoord;

const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

vec3 sat;
vec3 intensity;
vec3 con;
vec3 avgluma;
vec3 brt;
vec4 t;

out vec4 outputColor;

void main() {
    // scale
    float offsetx = (1024/2) - (1024/2) * (1/scale);
    float offsety = (768/2) - (768/2) * (1/scale);
    float vx = (vtexcoord.x * (1/scale) ) + offsetx;
    float vy = (vtexcoord.y * (1/scale) ) + offsety;
    
    t = texture(texsampler, vec2(vx,vy) );
    
    // color
	brt = t.rgb * brightness;
		
	avgluma = vec3(0.5, 0.5, 0.5);
    con = mix(avgluma, brt, contrast);
    
    intensity = vec3 (dot(con, LumCoeff));
    sat = mix(intensity, con, saturation);

	outputColor = vec4(sat.rgb, t.a);
}