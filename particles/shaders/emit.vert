#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vel;
layout(location = 2) in float TTL; // time to live
layout(location = 3) in uint type;

void main() {
	gl_Position = vec4(pos, 1.0);
}