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
		glm::vec3 pos;
		glm::vec3 vel;
		GLfloat TTL; // time to live
		GLint type;
	};

	const int EMITTER = 0;
	const int PARTICLE = 1;

	GLuint tfb[2];
	GLuint vbo[2];
	GLuint vao;

	unsigned int currentBuffer;

	Shader emitShader, processShader;

};

