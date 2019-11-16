#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Sprite.h"



// Determines how we should sort the glyphs
enum class GlyphSortType {
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE,
	BTF_TEXTURE
};

// A glyph is a single quad. These are added via SpriteBatch::draw
class Glyph {
public:
	Glyph(Sprite& sprite);

	const sf::Texture* texture;
	float depth;

	sf::Vertex topLeft;
	sf::Vertex bottomLeft;
	sf::Vertex topRight;
	sf::Vertex bottomRight;
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
	RenderBatch(unsigned int Offset, unsigned int NumVertices, const sf::Texture* Texture) : offset(Offset),
		numVertices(NumVertices), texture(Texture) {}
	unsigned int offset;
	unsigned int numVertices;
	const sf::Texture* texture;
};

// The SpriteBatch class is a more efficient way of drawing sprites
class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();

	// Begins the sprite batch
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

	// Ends the sprite batch
	void end();

	// Adds a glyph to the sprite batch
	void draw(Sprite& sprite);

	// Renders the entire SpriteBatch to the screen
	void renderBatch(sf::RenderWindow& window);

private:
	// Creates all the needed RenderBatches
	void createRenderBatches();

	// Sorts glyphs according to _sortType
	void sortGlyphs();

	// Comparators used by sortGlyphs()
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);
	static bool compareBTFTexture(Glyph* a, Glyph* b);

	GlyphSortType _sortType;

	std::vector<Glyph*> _glyphPointers;
	std::vector<Glyph> _glyphs;
	std::vector<sf::Vertex> _vertices;
	std::vector<RenderBatch> _renderBatches;
};