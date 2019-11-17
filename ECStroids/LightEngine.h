#pragma once
#include "Program.h"
#include "Camera.h"
#include <gl/glew.h>
#include <memory>
#include <list>
#include <map>

class Light;
class Sprite;
class Transform;
class LightEngine {
public:
	LightEngine(void);
	virtual ~LightEngine(void);

	void LoadShaders(void);
	void DrawHull(Light* light, Transform* lightTransform, Sprite* sprite, Transform* transform);
	void Draw(GLuint& fbo, Light* light, GLuint texture);
	void Draw(Light* light, Transform* lightTransform);
	void CreateShadows(Light* light);
	void UpdateMatrix(glm::mat4 mat);

	void Clean();

	// Shaders
	Program distortShader;
	Program reduceShader;
	Program shadowShader;
	Program blurVShader;
	Program blurHShader;

	GLuint dPos;
	GLuint rPos;
	GLuint sPos;
	GLuint bvPos;
	GLuint bhPos;

	GLuint _vbo;
	glm::mat4 _matrix;
};

