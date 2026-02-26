// Entity.h
#pragma once
#include <glm/glm.hpp>
#include <vector>

enum class EntityType {
	L,
	J,
	Line,
	Square,
	T,
	Z,
	S,
};

struct Entity {
	EntityType type;
	float x, y;        // pivot position
	float velY;
	unsigned char r, g, b;
	bool locked;
	std::vector<glm::ivec2> blocks; // e.g. {0,0}, {1,0}, {2,0} etc.
	int blockSize = 20; // pixel size of each block
};