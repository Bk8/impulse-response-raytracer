attribute vec4 position;
attribute vec4 normal;
attribute vec4 sourceColour;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying vec4 destinationColour;

void main (void)
{
    destinationColour = sourceColour;
    gl_Position = projectionMatrix * viewMatrix * position;
}
