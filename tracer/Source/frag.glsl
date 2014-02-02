uniform vec4 lightPosition;

varying vec3 ec_pos;

void main (void)
{
	vec3 ec_normal = normalize (cross (dFdx (ec_pos), dFdy (ec_pos)));
    
	float intensity = dot (gl_ModelViewMatrix * lightPosition, vec4 (ec_normal, 1.0));
    
	gl_FragColor = gl_Color * vec4 (vec3 (intensity), 1.0) + vec4 (0.0, 1.0, 1.0, 1.0);
}
