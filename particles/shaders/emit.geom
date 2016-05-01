#version 450
layout(points) in;
layout(points, max_vertices = 1) out;

out vec3 pos;
out vec3 vel;
out float TTL;

void main() {
	
	//pos = gl_in[0].gl_Position.xyz;
	
	pos = vec3(0.0, -0.45, 0.0);
	vel = gl_in[0].gl_Position.xyz + vec3(0.0, 0.25, 0.0);
	TTL = 1;

	gl_Position = vec4(pos, 1.0);
	gl_PointSize = 1.0;
	EmitVertex();
	EndPrimitive();
}