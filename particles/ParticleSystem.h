#pragma once

#include "Shader.h"

#include <glm/vec3.hpp>
#include "GL/glew.h"

class ParticleSystem
{
public:
	ParticleSystem(float x, float y, float z);
	~ParticleSystem();

	void render(const float & DELTA_TIME);

private:
	void updateParticles(const float & DELTA_TIME);

	struct Particle {
		GLfloat pos[3];
		GLfloat vel[3];
		GLfloat TTL; // time to live
		GLuint type;
	};

	const int EMITTER = 0;
	const int PARTICLE = 1;

	GLuint tfb[2];
	GLuint vbo[2];
	GLuint vao;

	unsigned int currentBuffer;

	Shader emitShader, processShader;

};

