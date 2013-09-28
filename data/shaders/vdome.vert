#version 150

uniform mat4 modelViewProjectionMatrix;
 
in vec4 position;
in vec2 texcoord;
 
out vec2 vtexcoord;
   
void main()
{
	vec2 p = vec2(position.x, position.y);

	// interpolate bottom edge x coordinate
	vec2 x1 = mix(vec2(0,0), vec2(1,0), p.x);

	// interpolate top edge x coordinate
	vec2 x2 = mix(vec2(0,1), vec2(1,1), p.x);

	// interpolate y position
	p = mix(x1, x2, p.y);

    gl_Position = modelViewProjectionMatrix * vec4(p, 0, 1);

	vtexcoord = texcoord;
}