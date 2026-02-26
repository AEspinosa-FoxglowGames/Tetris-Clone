#pragma once
#include "entity.h"
#include "renderer.h"

class Game {
	std::vector<Entity> entities;
	float spawnTimer = 0.f;
	int screenW, screenH;
	Entity& GetActive() { return entities.back(); }
	void SpawnPiece(EntityType type);
	void LockActive();
	bool IsGrounded(const Entity& e);
	bool CollidesHorizontal(const Entity& e);
	EntityType RandomType();
	int fieldX, fieldW;
	float fastSpeed;

public:
	void Init(int screenW, int screenH, int fieldX, int fieldWH);
	void Update(float dt);
	void Draw(Renderer& renderer);
	void OnInput(int key);
};