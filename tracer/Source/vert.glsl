attribute vec4 position;
attribute vec4 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying vec3 ec_pos;

void main (void)
{
	ec_pos = (projectionMatrix * viewMatrix * position).xyz;
    
    gl_Position = projectionMatrix * viewMatrix * position;
}
