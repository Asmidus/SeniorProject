#include "LightEngine.h"
#include "TextureManager.h"
#include "Vertex.h"
#include "Sprite.h"
#include "Light.h"
#include "Transform.h"
#include <iostream>



LightEngine::LightEngine(void) {
	_vbo = 0;
	LoadShaders();
}

LightEngine::~LightEngine(void) {}

void LightEngine::LoadShaders(void) {
	hullShader.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	hullShader.addAttribute("vertexPosition");
	hullShader.addAttribute("vertexColor");
	hullShader.addAttribute("vertexUV");
	hullShader.link();
	hPos = hullShader.getUniformLocation("P");

	// Distort shader
	distortShader.compileShaders("Shaders/textureShading.vert", "Shaders/distort.fx");
	distortShader.addAttribute("vertexPosition");
	distortShader.addAttribute("vertexColor");
	distortShader.addAttribute("vertexUV");
	distortShader.link();
	dPos = distortShader.getUniformLocation("P");
	//distortShader.use();
	//glUniform1i(distortShader.getUniformLocation("texture"), lightNum);

	//_program.use();
	//distortShader.loadFromFile("distort.fx", sf::Shader::Fragment);
	//distortShader.setParameter("texture", sf::Shader::CurrentTexture);

	// Reduce shader
	reduceShader.compileShaders("Shaders/textureShading.vert", "Shaders/reduce.fx");
	reduceShader.addAttribute("vertexPosition");
	reduceShader.addAttribute("vertexColor");
	reduceShader.addAttribute("vertexUV");
	reduceShader.link();
	rPos = reduceShader.getUniformLocation("P");
	//reduceShader.loadFromFile("reduce.fx", sf::Shader::Fragment);
	//reduceShader.setParameter("texture", sf::Shader::CurrentTexture);

	// Shadow shader
	shadowShader.compileShaders("Shaders/textureShading.vert", "Shaders/shadow.fx");
	shadowShader.addAttribute("vertexPosition");
	shadowShader.addAttribute("vertexColor");
	shadowShader.addAttribute("vertexUV");
	shadowShader.link();
	sPos = shadowShader.getUniformLocation("P");
	//shadowShader.loadFromFile("shadow.fx", sf::Shader::Fragment);

	// BlurV shader
	blurVShader.compileShaders("Shaders/textureShading.vert", "Shaders/blurV.fx");
	blurVShader.addAttribute("vertexPosition");
	blurVShader.addAttribute("vertexColor");
	blurVShader.addAttribute("vertexUV");
	blurVShader.link();
	bvPos = blurVShader.getUniformLocation("P");
	//blurVShader.loadFromFile("blurV.fx", sf::Shader::Fragment);
	//blurVShader.setParameter("texture", sf::Shader::CurrentTexture);

	// BlurH shader
	blurHShader.compileShaders("Shaders/textureShading.vert", "Shaders/blurH.fx");
	blurHShader.addAttribute("vertexPosition");
	blurHShader.addAttribute("vertexColor");
	blurHShader.addAttribute("vertexUV");
	blurHShader.link();
	bhPos = blurHShader.getUniformLocation("P");
	_camera.init(1, 1);
	//_camera.setPosition();
	//blurHShader.loadFromFile("blurH.fx", sf::Shader::Fragment);
	//blurHShader.setParameter("texture", sf::Shader::CurrentTexture);
}

void LightEngine::Begin(const Light& light,  const Transform& transform) {
	unsigned int size = light.radius * 2;
	hullShader.use();
	_camera.init(size, size);
	//_camera.setPosition(0, 0);
	//_camera.setScale(size/750);
	_camera.setPosition(glm::vec2(transform.rect.x + transform.rect.w / 2, size - transform.rect.y - transform.rect.h / 2));
	//_camera.setPosition(glm::vec2(size / 2.0, size / 2.0));
	//_camera.setPosition(glm::vec2( -transform.rect.x - transform.rect.w / 2 + size / 2, -transform.rect.y - transform.rect.h / 2 + size / 2));
	_camera.update();
	auto& matrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(hPos, 1, GL_FALSE, &matrix[0][0]);
	//glViewport(-transform.rect.x - transform.rect.w / 2 + size / 2, -transform.rect.y - transform.rect.h / 2 + size / 2, size, size);
	auto info = TextureManager::GetRenderTextures(size);
	glBindFramebuffer(GL_FRAMEBUFFER, info[0].first);
}

void LightEngine::End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	hullShader.unuse();
}

glm::vec2 rotatePoint(const glm::vec2& pos, float angle) {
	glm::vec2 newv;
	newv.x = pos.x * cos(angle) - pos.y * sin(angle);
	newv.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newv;
}

void LightEngine::DrawHull(Light* light, Transform* lightTransform, Sprite* sprite, Transform* transform) {
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}

	static glm::vec2 tlDims, brDims, tl, bl, br, tr;
	static Vertex topLeft, topRight, bottomLeft, bottomRight;
	tlDims.x = -transform->rect.w * transform->center.x;
	tlDims.y = -transform->rect.h * transform->center.y;
	brDims.x = transform->rect.w * (1 - transform->center.x);
	brDims.y = transform->rect.h * (1 - transform->center.y);

	// Get points centered around origin
	tl.x = tlDims.x;
	tl.y = tlDims.y;

	bl.x = tlDims.x;
	bl.y = brDims.y;

	br.x = brDims.x;
	br.y = brDims.y;

	tr.x = brDims.x;
	tr.y = tlDims.y;

	// Rotate the points
	tl = rotatePoint(tl, transform->angle) - tlDims;
	bl = rotatePoint(bl, transform->angle) - tlDims;
	br = rotatePoint(br, transform->angle) - tlDims;
	tr = rotatePoint(tr, transform->angle) - tlDims;
	auto info = TextureManager::GetRenderTextures(light->radius * 2);
	static Vertex vertexData[6];
	//int width = transform->rect.w, height = transform->rect.h;
	int size = light->radius * 2;
	//int x = transform->rect.x, y = size - transform->rect.y - transform->rect.h;

	auto uv = sprite->getUV();
	topLeft.pos.x = transform->rect.x + tl.x;
	topLeft.pos.y = size - transform->rect.y - tl.y;
	topLeft.uv.x = uv.x;
	topLeft.uv.y = uv.y + uv.w;

	bottomLeft.pos.x = transform->rect.x + bl.x;
	bottomLeft.pos.y = size - transform->rect.y - bl.y;
	bottomLeft.uv.x = uv.x;
	bottomLeft.uv.y = uv.y;

	bottomRight.pos.x = transform->rect.x + br.x;
	bottomRight.pos.y = size - transform->rect.y - br.y;
	bottomRight.uv.x = uv.x + uv.z;
	bottomRight.uv.y = uv.y;

	topRight.pos.x = transform->rect.x + tr.x;
	topRight.pos.y = size - transform->rect.y - tr.y;
	topRight.uv.x = uv.x + uv.z;
	topRight.uv.y = uv.y + uv.w;


	vertexData[0] = bottomRight;
	vertexData[1] = bottomLeft;
	vertexData[2] = topLeft;
	vertexData[3] = topLeft;
	vertexData[4] = topRight;
	vertexData[5] = bottomRight;

	//glBindFramebuffer(GL_FRAMEBUFFER, info[0].first);
	//glViewport(-lightTransform->rect.x - lightTransform->rect.w / 2 + size / 2, -lightTransform->rect.y - lightTransform->rect.h / 2 + size / 2, size, size);
	//Tell opengl to bind our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//Upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, sprite->texture);

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Draw the 6 vertices to the screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disable the vertex attrib arrays. This is not optional.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightEngine::Draw(GLuint& fbo, Light* light, GLuint texture) {
	//This array will hold our vertex data.
	//We need 6 vertices, and each vertex has 2
	//floats for X and Y
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}
	static Vertex vertexData[6];
	int size = light->radius * 2;
	int width = size, height = size;
	int x = 0, y = 0;
	//First Triangle
	if (fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	} else {
		//glViewport(0, 0, 500, 500);
		size = 100;
		width = size, height = size;
	}
	vertexData[0].pos = glm::vec2(x + width, y + height);
	vertexData[0].setUV(1.0f, 0.0f);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 0.0f);

	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 1.0f);

	//Second Triangle
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 1.0f);

	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 1.0f);

	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 0.0f);
	//Tell opengl to bind our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//Upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texture);

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Draw the 6 vertices to the screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disable the vertex attrib arrays. This is not optional.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (fbo) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void LightEngine::Draw(Light* light, Transform* lightTransform) {
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}
	//This array will hold our vertex data.
//We need 6 vertices, and each vertex has 2
//floats for X and Y
	static Vertex vertexData[6];
	int size = light->radius * 2;
	int width = size, height = size;
	int x = lightTransform->rect.x + lightTransform->rect.w/2 - size / 2, y = lightTransform->rect.y + lightTransform->rect.h/2 - size / 2;
	//First Triangle
	vertexData[0].pos = glm::vec2(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 1.0f);

	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);

	//Second Triangle
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 0.0f);

	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 0.0f);

	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 1.0f);
	//Tell opengl to bind our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//Upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, light->shadowTex);

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Draw the 6 vertices to the screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disable the vertex attrib arrays. This is not optional.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LightEngine::CreateShadows(Light* light) {
	auto info = TextureManager::GetRenderTextures(light->radius * 2);
	auto& matrix = _camera.getCameraMatrix();
	distortShader.use();
	glUniformMatrix4fv(dPos, 1, GL_FALSE, &matrix[0][0]);
	Draw(info[1].first, light, info[0].second);
	distortShader.unuse();

	reduceShader.use();
	static GLuint reduceRenderTargetSize = reduceShader.getUniformLocation("renderTargetSize");
	glUniformMatrix4fv(rPos, 1, GL_FALSE, &matrix[0][0]);
	glUniform1f(reduceRenderTargetSize, light->radius * 2.0f);
	Draw(info[2].first, light, info[1].second);
	reduceShader.unuse();

	static GLuint reduce = shadowShader.getUniformLocation("reduce");
	static GLuint renderTargetSize = shadowShader.getUniformLocation("renderTargetSize");
	static GLuint lightColor = shadowShader.getUniformLocation("lightColor");

	shadowShader.use();
	glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
	glBindTexture(GL_TEXTURE_2D, info[2].second);
	glActiveTexture(GL_TEXTURE0);
	static GLfloat col[4] = { 0, 0, 1, 1 };
	col[0] = light->color[0];
	col[1] = light->color[1];
	col[2] = light->color[2];
	glUniformMatrix4fv(sPos, 1, GL_FALSE, &matrix[0][0]);
	glUniform1i(reduce, 1);
	glUniform1i(renderTargetSize, light->radius * 2.f);
	glUniform4fv(lightColor, 1, col);
	TextureManager::ClearTexture(light->shadowFBO);
	Draw(light->shadowFBO, light, info[0].second);
	shadowShader.unuse();


	blurHShader.use();
	glUniformMatrix4fv(bhPos, 1, GL_FALSE, &matrix[0][0]);
	TextureManager::ClearTexture(info[3].first);
	Draw(info[3].first, light, light->shadowTex);
	blurHShader.unuse();

	blurVShader.use();
	glUniformMatrix4fv(bvPos, 1, GL_FALSE, &matrix[0][0]);
	TextureManager::ClearTexture(light->shadowFBO);
	Draw(light->shadowFBO, light, info[3].second);
	blurVShader.unuse();
	//TextureManager::ClearTexture(info[0].first);
}

void LightEngine::UpdateMatrix(glm::mat4 mat) {
	_matrix = mat;
}
