#pragma once

#include <glm/glm.hpp>
#include <vector>



//// Determines how we should sort the glyphs
//enum class GlyphSortType {
//	NONE,
//	FRONT_TO_BACK,
//	BACK_TO_FRONT,
//	TEXTURE,
//	BTF_TEXTURE
//};
//
//// A glyph is a single quad. These are added via SpriteBatch::draw
//class Glyph {
//public:
//	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color);
//	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color, float angle, const glm::vec2& center = { 0.5, 0.5 });
//
//	GLuint texture;
//	float depth;
//
//	Vertex topLeft;
//	Vertex bottomLeft;
//	Vertex topRight;
//	Vertex bottomRight;
//private:
//	// Rotates a point about (0,0) by angle
//	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
//};
//
//// Each render batch is used for a single draw call
//class RenderBatch {
//public:
//	RenderBatch(unsigned int Offset, unsigned int NumVertices, sf::Texture* Texture) : offset(Offset),
//		numVertices(NumVertices), texture(Texture) {}
//	unsigned int offset;
//	unsigned int numVertices;
//	sf::Texture* texture;
//};
//
//// The SpriteBatch class is a more efficient way of drawing sprites
//class SpriteBatch {
//public:
//	SpriteBatch();
//	~SpriteBatch();
//
//	// Initializes the sprite batch
//	void init();
//	void dispose();
//
//	// Begins the sprite batch
//	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
//
//	// Ends the sprite batch
//	void end();
//
//	// Adds a glyph to the sprite batch
//	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
//	// Adds a glyph to the sprite batch with rotation
//	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle, const glm::vec2& center = { 0.5, 0.5 });
//	// Adds a glyph to the sprite batch with rotation
//	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, const glm::vec2& dir);
//
//	// Renders the entire SpriteBatch to the screen
//	void renderBatch();
//
//private:
//	// Creates all the needed RenderBatches
//	void createRenderBatches();
//
//	// Generates our VAO and VBO
//	void createVertexArray();
//
//	// Sorts glyphs according to _sortType
//	void sortGlyphs();
//
//	// Comparators used by sortGlyphs()
//	static bool compareFrontToBack(Glyph* a, Glyph* b);
//	static bool compareBackToFront(Glyph* a, Glyph* b);
//	static bool compareTexture(Glyph* a, Glyph* b);
//	static bool compareBTFTexture(Glyph* a, Glyph* b);
//
//	GlyphSortType _sortType;
//
//	std::vector<Sprite*> _sprites;
//	std::vector<RenderBatch> _renderBatches;
//};