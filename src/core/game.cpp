#include "game.h"
#include "renderer.h"
#include <cstdlib>  // for rand()
#include <ctime>    // for time()
#include <vector>
#include "random.h"


void Game::Init(int W, int H, int fx, int fw)
{
	screenW = W;
	screenH = H;
	fieldX = fx;
	fieldW = fw;
	fastSpeed = 600.f;
	Random::Init();
	SpawnPiece(RandomType());
}
void Game::Update(float dt)
{
	if (entities.empty()) return; // safety check

	Entity& active = GetActive();

	if (IsGrounded(active))
	{
		LockActive();
		SpawnPiece(RandomType()); // spawn the next one
	}
	else
	{
		active.y += active.velY * dt; // only move if not grounded
	}
}
void Game::Draw(Renderer& renderer)
{
	for (auto& e : entities)
		for (auto& b : e.blocks)
			renderer.DrawRect(
				(int)round(e.x) + b.x * e.blockSize,
				(int)round(e.y) + b.y * e.blockSize,
				e.blockSize, e.blockSize,
				e.r, e.g, e.b
			);
}
void Game::OnInput(int key)
{
	Entity& active = GetActive();
	if (key == GLFW_KEY_SPACE) active.velY = fastSpeed;
	if (key == GLFW_KEY_LEFT)  active.x -= active.blockSize;
	if (key == GLFW_KEY_LEFT && CollidesHorizontal(active)) active.x += active.blockSize; // undo left
	if (key == GLFW_KEY_RIGHT) active.x += active.blockSize;
	if (key == GLFW_KEY_RIGHT && CollidesHorizontal(active)) active.x -= active.blockSize; // undo right
	// clamp each block to field bounds
	for (auto& b : active.blocks)
	{
		int blockLeft = active.x + b.x * active.blockSize;
		int blockRight = blockLeft + active.blockSize;
		if (blockLeft < fieldX)
			active.x += fieldX - blockLeft;
		if (blockRight > fieldX + fieldW)
			active.x -= blockRight - (fieldX + fieldW);
	}
	
}

//PRIVATE-------------------------------------

void Game::SpawnPiece(EntityType type)
{
	Entity e;
	e.type = type;
	e.y = 0;
	int blockSize = 20;
	int cols = fieldW / blockSize;
	e.x = fieldX + Random::Int(0, cols - 3) * blockSize; // -3 gives room for widest pieces
	e.velY = 60.f;
	e.locked = false;

	switch (type)
	{
	case EntityType::Square: 
		e.r = 255;
		e.g = 255;
		e.b = 0;
		e.blocks = { {0,0},{1,0},{0,1},{1,1} };
		break;
	case EntityType::L: 
		e.r = 255;
		e.g = 127;
		e.b = 0;
		e.blocks = { {0,0},{0,1},{0,2},{1,2} };
		break;
	case EntityType::J:  
		e.r = 0;
		e.g = 0;
		e.b = 255;
		e.blocks = { {1,0},{1,1},{0,2},{1,2} };
		break;
	case EntityType::Line: 
		e.r = 0;
		e.g = 255;
		e.b = 255;
		e.blocks = { {0,0},{0,1},{0,2},{0,3} };
		break;
	case EntityType::S:   
		e.r = 0;
		e.g = 255;
		e.b = 0;
		e.blocks = { {1,0},{2,0},{0,1},{1,1} };
		break;
	case EntityType::Z: 
		e.r = 255;
		e.g = 0;
		e.b = 0;
		e.blocks = { {0,0},{1,0},{1,1},{2,1} };
		break;
	case EntityType::T:  
		e.r = 160;
		e.g = 0;
		e.b = 240;
		e.blocks = { {0,0},{1,0},{2,0},{1,1} };
		break;
	}
	entities.push_back(e);
}
void Game::LockActive()
{
	Entity& e = GetActive();
	e.x = fieldX + round((e.x - fieldX) / e.blockSize) * e.blockSize;
	e.y = round(e.y / e.blockSize) * e.blockSize;
	e.locked = true;
}
bool Game::IsGrounded(const Entity& e)
{
	for (auto& b : e.blocks)
	{
		// check floor
		if (e.y + (b.y + 1) * e.blockSize >= screenH) return true;

		// check against all locked pieces
		for (auto& other : entities)
		{
			if (!other.locked) continue; // skip active piece

			for (auto& ob : other.blocks)
			{
				int ex = (int)round(e.x) + b.x * e.blockSize;
				int ey = (int)round(e.y) + (b.y + 1) * e.blockSize;

				int ox = (int)round(other.x) + ob.x * e.blockSize;
				int oy = (int)round(other.y) + ob.y * e.blockSize;

				if (ex == ox && ey >= oy && ey <= oy + e.blockSize)
					return true;
			}
		}
	}
	return false;
}

EntityType Game::RandomType()
{
	int r = Random::Int(0,6); // 7 piece types
	return static_cast<EntityType>(r);
}
bool Game::CollidesHorizontal(const Entity& e)
{
	for (auto& b : e.blocks)
	{
		int bx = e.x + b.x * e.blockSize;
		int by = e.y + b.y * e.blockSize;

		for (auto& other : entities)
		{
			if (!other.locked) continue;

			for (auto& ob : other.blocks)
			{
				int ox = (int)round(other.x) + ob.x * e.blockSize;
				int oy = (int)round(other.y) + ob.y * e.blockSize;

				// same x = overlap, check y range to confirm they're on same row
				if (bx == ox && by < oy + e.blockSize && by + e.blockSize > oy)
					return true;
			}
		}
	}
	return false;
}