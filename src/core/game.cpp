#include "game.h"
#include "renderer.h"
#include <cstdlib>  // for rand()
#include <ctime>    // for time()
#include <vector>
#include "random.h"
#include <algorithm>
#include <map>



void Game::Init(int W, int H, int fx, int fw)
{
	screenW = W;
	screenH = H;
	fieldX = fx;
	fieldW = fw;
	fastSpeed = 600.f;
	score = 0;
	
	Random::Init();
	nextType = RandomType();
	SpawnPiece(nextType);
	nextType = RandomType();
}
void Game::Update(float dt)
{
	if (entities.empty() || gameOver) return;//safety

	Entity& active = GetActive();
	levelTimer += dt;
	if (levelTimer >= levelInterval)
	{
		level++;
		levelTimer = 0.f;
	}

	//Move fast if hold
	if (holdLeft || holdRight)
	{
		dasTimer += dt;
		if (dasTimer >= dasDelay)
		{
			dasActive = true;
			arrTimer += dt;
			if (arrTimer >= arrDelay)
			{
				arrTimer = 0.f;
				if (holdLeft)  MoveActive(-1);
				if (holdRight) MoveActive(1);
			}
		}
	}
	else
	{
		dasTimer = 0.f;
		arrTimer = 0.f;
		dasActive = false;
	}


	if (IsGrounded(active))
	{
		LockActive();
		CheckRows();
		SpawnPiece(nextType);
		nextType = RandomType(); // spawn the next one
		if (IsGrounded(GetActive())) // new piece already grounded = game over
		{
			gameOver = true;
			return;
		}
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
void Game::DrawPreview(Renderer& renderer)
{
	int blockSize = 20;
	int r, g, b;
	std::vector<glm::ivec2> blocks;
	switch (nextType)
	{
	case EntityType::Square:
		r = 255;
		g = 255;
		b = 0;
		blocks = { {0,0},{1,0},{0,1},{1,1} };
		break;
	case EntityType::L:
		r = 255;
		g = 127;
		b = 0;
		blocks = { {0,0},{0,1},{0,2},{1,2} };
		break;
	case EntityType::J:
		r = 0;
		g = 0;
		b = 255;
		blocks = { {1,0},{1,1},{0,2},{1,2} };
		break;
	case EntityType::Line:
		r = 0;
		g = 255;
		b = 255;
		blocks = { {0,0},{0,1},{0,2},{0,3} };
		break;
	case EntityType::S:
		r = 0;
		g = 255;
		b = 0;
		blocks = { {1,0},{2,0},{0,1},{1,1} };
		break;
	case EntityType::Z:
		r = 255;
		g = 0;
		b = 0;
		blocks = { {0,0},{1,0},{1,1},{2,1} };
		break;
	case EntityType::T:
		r = 160;
		g = 0;
		b = 240;
		blocks = { {0,0},{1,0},{2,0},{1,1} };
		break;
	}
	int maxX = 0, maxY = 0;
	for (auto& bl : blocks) { maxX = std::max(maxX, bl.x); maxY = std::max(maxY, bl.y); }
	int pieceW = (maxX + 1) * blockSize;
	int pieceH = (maxY + 1) * blockSize;

	// center of the preview box
	int sides = screenW - fieldW;
	int boxX = screenW - sides / 2 + 10;
	int boxY = 10;
	int boxSize = 100;

	// offset to center piece inside box
	int offsetX = boxX + (boxSize - pieceW) / 2;
	int offsetY = boxY + (boxSize - pieceH) / 2;

	for (auto& bl : blocks)
		renderer.DrawRect(
			offsetX + bl.x * blockSize,
			offsetY + bl.y * blockSize,
			blockSize, blockSize,
			r, g, b
		);
}
void Game::OnInput(int key)
{
	Entity& active = GetActive();
	if (key == GLFW_KEY_SPACE) active.velY = fastSpeed;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) MoveActive(-1);
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) MoveActive(1);
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
	{
		RotateActive(true);
		if (CollidesHorizontal(GetActive()) || IsGrounded(GetActive()))
			RotateActive(false);
	}
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
	{
		RotateActive(false);
		if (CollidesHorizontal(GetActive()) || IsGrounded(GetActive()))
			RotateActive(true);
	}
}

//PRIVATE-------------------------------------
void Game::RotateActive(bool clockwise)
{
	Entity& e = GetActive();
	for (auto& b : e.blocks)
	{
		int oldX = b.x;
		int oldY = b.y;
		if (clockwise)
		{
			b.x = oldY;
			b.y = -oldX;
		}
		else
		{
			b.x = -oldY;
			b.y = oldX;
		}
	}
	// normalize
	int minX = INT_MAX, minY = INT_MAX;
	for (auto& b : e.blocks) { minX = std::min(minX, b.x); minY = std::min(minY, b.y); }
	for (auto& b : e.blocks) { b.x -= minX; b.y -= minY; }
}
void Game::SpawnPiece(EntityType type)
{
	Entity e;
	e.type = type;
	e.y = 0;
	int blockSize = 20;
	int cols = fieldW / blockSize;
	e.x = fieldX + Random::Int(0, cols - 3) * blockSize; // -3 gives room for widest pieces
	e.velY = 60.f + (level - 1) * 5.f;
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
		int bx = (int)round(e.x) + b.x * e.blockSize; // needs round
		int by = (int)round(e.y) + b.y * e.blockSize;

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

void Game::CheckRows()
{
	std::map<int, int> rowCount; // y position -> block count
	std::vector<int> fullRows;

	for (auto& e : entities)
	{
		if (!e.locked) continue;
		for (auto& b : e.blocks)
		{
			int blockY = (int)round(e.y) + b.y * e.blockSize;
			rowCount[blockY]++;
		}
	}

	int blocksPerRow = fieldW / 20; // 15 for 300px field with 20px blocks

	for (auto& [y, count] : rowCount)
	{
		if (count == blocksPerRow)
		{
			fullRows.emplace_back(y);
		}
	}
	//delete
	for (int targetY : fullRows)
	{
		for (auto& e : entities)
		{
			if (!e.locked) continue;
			e.blocks.erase(
				std::remove_if(e.blocks.begin(), e.blocks.end(), [&](auto& b) {
					int blockY = (int)round(e.y) + b.y * e.blockSize;
					return blockY == targetY;
					}),
				e.blocks.end()
			);
		}
	}

	//remove empty entities
	entities.erase(
		std::remove_if(entities.begin(), entities.end(), [](auto& e) {
			return e.locked && e.blocks.empty();
			}),
		entities.end()
	);

	//shift rest down
	std::sort(fullRows.begin(), fullRows.end()); // smallest y first
	for (int targetY : fullRows)
	{
		for (auto& e : entities)
		{
			if (!e.locked) continue;
			for (auto& b : e.blocks)
			{
				int blockY = (int)round(e.y) + b.y * e.blockSize;
				if (blockY < targetY) // only shift blocks above the cleared row
					b.y += 1; // shift down by one block
			}
		}
	}
	// scoring - more lines at once = more points
	if (fullRows.size() == 1) score += 100;
	else if (fullRows.size() == 2) score += 300;
	else if (fullRows.size() == 3) score += 500;
	else if (fullRows.size() == 4) score += 800; // tetris!
}
void Game::Reset()
{
	entities.clear();
	score = 0;
	level = 1;
	levelTimer = 0.f;
	gameOver = false;
	nextType = RandomType();
	SpawnPiece(nextType);
	nextType = RandomType();
}
void Game::MoveActive(int dir) // dir = -1 left, +1 right
{
	Entity& a = GetActive();
	a.x += dir * a.blockSize;
	if (CollidesHorizontal(a)) a.x -= dir * a.blockSize;
	for (auto& b : a.blocks)
	{
		if (a.x + b.x * a.blockSize < fieldX) a.x += a.blockSize;
		if (a.x + (b.x + 1) * a.blockSize > fieldX + fieldW) a.x -= a.blockSize;
	}
}