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
	void RotateActive(bool clockwise);
	void CheckRows();
	
	bool IsGrounded(const Entity& e);
	bool CollidesHorizontal(const Entity& e);
	bool gameOver = false;
	EntityType RandomType();
	int fieldX, fieldW;
	float fastSpeed;
	EntityType nextType;
	int level = 1;
	float levelTimer = 0.f;
	float levelInterval = 30.f; // seconds per leve

	int score;
	bool holdLeft = false, holdRight = false;
	float dasTimer = 0.f;     // time held before repeat starts
	float arrTimer = 0.f;     // repeat speed once DAS triggers
	float dasDelay = 0.2f;    // seconds before repeat kicks in
	float arrDelay = 0.05f;   // seconds between repeats after that
	bool dasActive = false;   // has DAS triggered yet?
	void MoveActive(int dir);
	
	

public:
	void Init(int screenW, int screenH, int fieldX, int fieldWH);
	void Update(float dt);
	void Draw(Renderer& renderer);
	void OnInput(int key);
	bool IsOver() { return gameOver; }
	void DrawPreview(Renderer& renderer);
	void Reset();
	int GetScore() { return score; }
	void SetHold(bool left, bool right) { holdLeft = left; holdRight = right; }
	int GetLevel() { return level; }
};