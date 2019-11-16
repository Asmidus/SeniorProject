#include "SpriteBatch.h"
#include <iostream>
#include <algorithm>
#include <execution>



Glyph::Glyph(Sprite& sprite) {
	static const sf::FloatRect* localBounds;
	localBounds = &sprite.getLocalBounds();
	topLeft.position.x = 0;
	topLeft.position.y = 0;
	topRight.position.x = localBounds->width;
	topRight.position.y = 0;
	bottomLeft.position.x = 0;
	bottomLeft.position.y = localBounds->height;
	bottomRight.position.x = localBounds->width;
	bottomRight.position.y = localBounds->height;

	static const sf::Transform* transform;
	transform = &sprite.getTransform();
	topLeft.position = transform->transformPoint(topLeft.position);
	topRight.position = transform->transformPoint(topRight.position);
	bottomLeft.position = transform->transformPoint(bottomLeft.position);
	bottomRight.position = transform->transformPoint(bottomRight.position);

	topLeft.color = sprite.getColor();
	topRight.color = sprite.getColor();
	bottomLeft.color = sprite.getColor();
	bottomRight.color = sprite.getColor();

	static const sf::IntRect* texRect;
	texRect = &sprite.getTextureRect();
	topLeft.texCoords.x = texRect->left;
	topLeft.texCoords.y = texRect->top;
	topRight.texCoords.x = texRect->left + texRect->width;
	topRight.texCoords.y = texRect->top;
	bottomLeft.texCoords.x = texRect->left;
	bottomLeft.texCoords.y = texRect->top + texRect->height;
	bottomRight.texCoords.x = texRect->left + texRect->width;
	bottomRight.texCoords.y = texRect->top + texRect->height;

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

	//sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(Sprite& sprite) {
	_glyphs.emplace_back(sprite);
}

void SpriteBatch::renderBatch(sf::RenderWindow& window) {
	//std::cout << sizeof(_vertices) + (sizeof(sf::Vertex) * _vertices.size()) << "\n";
	static sf::VertexBuffer triangles(sf::Triangles);
	if (triangles.getVertexCount() < _vertices.size()) {
		triangles.create(_vertices.size());
	}
	triangles.update(_vertices.data());
	window.draw(triangles, _renderBatches[0].texture);
	//for (size_t i = 0; i < _renderBatches.size(); i++) {
		//window.draw(&_vertices[_renderBatches[i].offset], _renderBatches[i].numVertices, sf::Triangles, _renderBatches[i].texture);
	//}
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
		std::stable_sort(std::execution::par_unseq, _glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(std::execution::par_unseq, _glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(std::execution::par_unseq, _glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	case GlyphSortType::BTF_TEXTURE:
		std::stable_sort(std::execution::par_unseq, _glyphPointers.begin(), _glyphPointers.end(), compareBTFTexture);
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
	if (a->depth == b->depth) {
		return compareTexture(a, b);
	} else {
		return compareBackToFront(a, b);
	}
}