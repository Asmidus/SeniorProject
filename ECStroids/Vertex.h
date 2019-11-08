#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Color {
public:
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	Color() : r(0), g(0), b(0), a(0) {}
	Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
		r(R), g(G), b(B), a(A) {}
};

struct Vertex {
	glm::vec2 pos;
	glm::vec2 uv;
	Color color;
	Vertex(glm::vec2 p, glm::vec2 u, glm::vec4 c = { 255, 255, 255, 255 }) : pos(p), uv(u), color(c[0], c[1], c[2], c[3]) {}
};

