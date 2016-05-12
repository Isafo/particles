#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(float x, float y, float z) 
{
	currentBuffer = 0;

	Particle particle[2000];
	particle[0].pos[0] = x; particle[0].pos[1] = y; particle[0].pos[2] = z;
	particle[0].vel[0] = 0.0; particle[0].vel[1] = 0.0; particle[0].vel[2] = 0.0;
	particle[0].TTL = 0;
	particle[0].type = EMITTER;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenTransformFeedbacks(2, tfb);
	glGenBuffers(2, vbo);

	for (unsigned int i = 0; i < 2; ++i) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfb[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle), particle, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[i]);

		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // velocity
		glEnableVertexAttribArray(2); // time to live
		glEnableVertexAttribArray(3); // type

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0); // position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(GLfloat))); // velocity
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6 * sizeof(GLfloat))); // time to live
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(GLfloat))); // type

	}
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const char* capture[] = {
		"pos",
		"vel",
		"TTL",
		"type"
	};

	emitShader.createTransformShader("shaders/emit.vert", "shaders/emit.geom", "shaders/emit.frag", capture);
	//processShader.createTransformShader("shaders/process.vert", "shaders/process.geom", "shaders/process.frag", capture);

}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::updateParticles(const float& DELTA_TIME) 
{
	
	// for debugging -----------------------
	GLuint qid;
	glGenQueries(1, &qid);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, qid);
	// -----------------------------------
	// turn off rendering
	glEnable(GL_RASTERIZER_DISCARD);
	
	glUseProgram(emitShader.programID);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[currentBuffer]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfb[currentBuffer]);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // time to live
	glEnableVertexAttribArray(3); // type

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0); // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(GLfloat))); // velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6 * sizeof(GLfloat))); // time to live
	glVertexAttribPointer(3, 1, GL_INT,   GL_FALSE, sizeof(Particle), (void*)(7 * sizeof(GLfloat))); // type

	glBeginTransformFeedback(GL_POINTS);

	if (true)
	{
		glDrawArrays(GL_POINTS, 0, 1);
	}
	else 
	{
		glDrawTransformFeedback(GL_POINTS, tfb[currentBuffer]);
	}

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	// enable rendering
	glDisable(GL_RASTERIZER_DISCARD);

	// for debugging -------------------------------------------
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint nprimitives;
	glGetQueryObjectuiv(qid, GL_QUERY_RESULT, &nprimitives);

	GLfloat feedback[7];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	for (int i = 0; i < 16; i = i + 8) {
		std::cout << "position " << 
					 feedback[i] << ", " <<
					 feedback[i + 1] << ", " <<
					 feedback[i + 2] << ", \n" <<
					 "vel " <<
					 feedback[i + 3] << ", " <<
					 feedback[i + 4] << ", " <<
					 feedback[i + 5] << ", \n" <<
					 " TTL" <<
					 feedback[i + 6] << std::endl;

		std::cout << "----------" << std::endl;
	}
	// ---------------------------------------------------------
}


void ParticleSystem::render(const float& DELTA_TIME) 
{
	
	updateParticles(DELTA_TIME);

	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0); // position

	//glDrawTransformFeedback(GL_POINTS, tfb[currentBuffer]);

	// swap to next buffer
	currentBuffer = (currentBuffer + 1) % 2;
}