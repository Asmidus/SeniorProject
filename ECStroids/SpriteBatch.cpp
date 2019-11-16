#include "SpriteBatch.h"


#include "SpriteBatch.h"

#include <algorithm>



Glyph::Glyph(Sprite& sprite) {
	topLeft.position = sf::Vector2f(0, 0);
	topRight.position = sf::Vector2f(sprite.getLocalBounds().width, 0);
	bottomLeft.position = sf::Vector2f(0, sprite.getLocalBounds().height);
	bottomRight.position = sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height);

	topLeft.position = sprite.getTransform().transformPoint(topLeft.position);
	topRight.position = sprite.getTransform().transformPoint(topRight.position);
	bottomLeft.position = sprite.getTransform().transformPoint(bottomLeft.position);
	bottomRight.position = sprite.getTransform().transformPoint(bottomRight.position);

	topLeft.color = sprite.getColor();
	topRight.color = sprite.getColor();
	bottomLeft.color = sprite.getColor();
	bottomRight.color = sprite.getColor();

	topLeft.texCoords = sf::Vector2f(sprite.getTextureRect().left, sprite.getTextureRect().top);
	topRight.texCoords = sf::Vector2f(sprite.getTextureRect().left + sprite.getTextureRect().width, sprite.getTextureRect().top);
	bottomLeft.texCoords = sf::Vector2f(sprite.getTextureRect().left, sprite.getTextureRect().top + sprite.getTextureRect().height);
	bottomRight.texCoords = sf::Vector2f(sprite.getTextureRect().left + sprite.getTextureRect().width, sprite.getTextureRect().top + sprite.getTextureRect().height);

	texture = sprite.getTexture();
	depth = sprite.z;
}

SpriteBatch::SpriteBatch() {}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
	_sortType = sortType;
	_renderBatches.clear();

	// Makes _glpyhs.size() == 0, however it does not free internal memory.
	// So when we later call emplace_back it doesn't need to internally call new.
	_glyphs.clear();
}

void SpriteBatch::end() {
	// Set up all pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());
	for (size_t i = 0; i < _glyphs.size(); i++) {
		_glyphPointers[i] = &_glyphs[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(Sprite& sprite) {
	_glyphs.emplace_back(sprite);
}

void SpriteBatch::renderBatch(sf::RenderWindow& window) {
	for (size_t i = 0; i < _renderBatches.size(); i++) {
		window.draw(&_vertices[_renderBatches[i].offset], _renderBatches[i].numVertices, sf::Triangles, _renderBatches[i].texture);
	}
}

void SpriteBatch::createRenderBatches() {
	// This will store all the vertices that we need to upload
	// Resize the buffer to the exact size we need so we can treat
	// it like an array
	unsigned int numVertices = 6*_glyphPointers.size();
	if (_vertices.size() < numVertices) {
		_vertices.resize(numVertices);
	}

	if (_glyphPointers.empty()) {
		return;
	}

	int offset = 0; // current offset
	int cv = 0; // current vertex

	//Add the first batch
	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
	_vertices[cv++] = _glyphPointers[0]->topLeft;
	_vertices[cv++] = _glyphPointers[0]->bottomLeft;
	_vertices[cv++] = _glyphPointers[0]->bottomRight;
	_vertices[cv++] = _glyphPointers[0]->bottomRight;
	_vertices[cv++] = _glyphPointers[0]->topRight;
	_vertices[cv++] = _glyphPointers[0]->topLeft;
	offset++;

	//Add all the rest of the glyphs
	for (size_t cg = 1; cg < _glyphPointers.size(); cg++) {

		// Check if this glyph can be part of the current batch
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
			// Make a new batch
			_renderBatches.emplace_back(6*offset, 6, _glyphPointers[cg]->texture);
		} else {
			// If its part of the current batch, just increase numVertices
			_renderBatches.back().numVertices += 6;
		}
		_vertices[cv++] = _glyphPointers[cg]->topLeft;
		_vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		_vertices[cv++] = _glyphPointers[cg]->bottomRight;
		_vertices[cv++] = _glyphPointers[cg]->bottomRight;
		_vertices[cv++] = _glyphPointers[cg]->topRight;
		_vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset++;
	}
}

void SpriteBatch::sortGlyphs() {

	switch (_sortType) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	case GlyphSortType::BTF_TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return (a->texture < b->texture);
}

bool SpriteBatch::compareBTFTexture(Glyph* a, Glyph* b) {
	bool c = compareBackToFront(a, b);
	if (c) return c;
	return compareTexture(a, b);
}