#include "Line.h"


	Line::Line() 
		:shader(0), vbo(0), vao(0)
	{



	}

	void Line::init(CShader* shade) {
		shader = shade;
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(vbo, points.size(), points.data(), GL_STATIC_DRAW);
		unsigned vertexLocation = glGetAttribLocation(shader->GetProgramObjID(), "in_Position");
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(vertexLocation);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Line::render() {

		//glUseProgram(shader->GetProgramObjID());
		//glBindVertexArray(vao);
		//glLineWidth(10.f);
		//glDrawArrays(GL_LINE_LOOP, 0, points.size());
		//glUseProgram(0);
		//glBindVertexArray(0);
	}