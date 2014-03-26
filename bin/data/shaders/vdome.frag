#version 150
 
uniform float saturation;
uniform float brightness;
uniform float contrast;
													 
uniform sampler2DRect texsampler;

in vec2 vtexcoord;

const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

vec3 sat;
vec3 intensity;
vec3 con;
vec3 avgluma;
vec3 brt;
vec4 t;

// mask
uniform sampler2DRect maskTex;


out vec4 outputColor;

void main() {
    t = texture(texsampler, vtexcoord);
    
    // color
	brt = t.rgb * brightness;
		
	avgluma = vec3(0.5, 0.5, 0.5);
    con = mix(avgluma, brt, contrast);
    
    intensity = vec3 (dot(con, LumCoeff));
    sat = mix(intensity, con, saturation);


   // get rgb from tex0
   // vec3 src = texture(tex0, texCoordVarying).rgb;

    // get alpha from mask
    float mask = texture(maskTex, vtexcoord).r;
    
    //mix the rgb from tex0 with the alpha of the mask
    outputColor = vec4(sat.rgb , mask);
    
    //outputColor = vec4(sat.rgb, t.a);
}
