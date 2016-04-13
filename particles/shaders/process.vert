#version 450
layout(points) in;
layout(points, max_vertices = 1) out;
layout(invocations = 32) in; // run 32 instances for all slices in the 3D texture

out vec4 frag_color;
void main() {
	frag_color = vec4(1.0, 1.0, 0.0, 1.0);
}