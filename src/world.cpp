#include "world.h"
#include <time.h>

namespace Sefirah
{
	float World::CheckSight(glm::vec2 pos, glm::vec2 forward, int w, int r)
	{
		bool found = false;
		int depth = 0;

		glm::vec2 perp = glm::vec2(forward.y, -forward.x);

		for (int x = -w; x < w; x++)
		{
			glm::vec2 startPos = pos + (perp * (float)x);

			int nx = startPos.x;
			int ny = startPos.y;
			depth = 0;

			for (int y = 0; y < r; y++)
			{
				nx += forward.x;
				ny += forward.y;
				depth++;

				Organism* o = GetOrganism(nx, ny);

				if (o != nullptr)
				{
					found = true;
					break;
				}
			}
		}

		if (!found) return 0.0f;
		return (1.0f - (depth / (float)(r)));
	}

	float World::CheckSmell(glm::vec2 pos, int r)
	{
		std::vector<Organism*> contenders;

		int x = pos.x;
		int y = pos.y;

		for (int i = x - r; i < x + r; i++)
		{
			for (int j = y - r; j < y + r; j++)
			{
				if (i >= 0 && i < width &&
					j >= 0 && j < height &&
					(i != x || j != y))
				{
					Organism* o = GetOrganism(i, j);
					if (o != nullptr) contenders.push_back(o);
				}
			}
		}

		float minDist = 0;

		for (int i = 0; i < contenders.size(); i++)
		{
			Organism* o = contenders[i];
			float dist = glm::distance(glm::vec2(x, y), pos);

			if (dist < minDist)
			{
				minDist = dist;
			}
		}

		float ret = 1 - (minDist / (float)(r));

		if (ret < 0.0f) return 0.0f;
		return ret;
	}

	float World::CheckSounds(glm::vec2 pos, int r)
	{
		int count = 0;
		int x = pos.x;
		int y = pos.y;

		for (int i = x - r; i < x + r; i++)
		{
			for (int j = y - r; j < y + r; j++)
			{
				if (i >= 0 && i < width &&
					j >= 0 && j < height)
				{
					if (ripple[(j * width) + i]) count++;
				}
			}
		}

		float ret = count / (float)(r);

		if (ret > 1.0f) return 1.0f;
		return ret;
	}

	float World::CheckTouch(glm::vec2 pos)
	{
		int x = (int)pos.x;
		int y = (int)pos.y;

		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				Organism* o = GetOrganism(x + i, y + j);
				if (o != nullptr) return 1.0f;
			}
		}

		return 0.0f;
	}

	Organism* World::GetOrganism(int x, int y)
	{
		if (x >= 0 && x < width &&
			y >= 0 && y < width)
		{
			return organismSpace[(y * width) + x];
		}

		return nullptr;
	}

	void World::ReproduceOrganism(Organism* organism)
	{
		Organism* newOrganism = new Organism(organism);
		organisms.push_back(newOrganism);
		RandomlyPlaceOrganism(newOrganism);

		srand(time(NULL));

		if ((rand() % 10000 / 100.0f) <= evolutionRate) newOrganism->Evolve();
	}

	void World::KillOrganism(Organism* organism)
	{
		glm::vec2 pos = organism->GetPosition();
		organisms.erase(std::remove(organisms.begin(), organisms.end(), organism), organisms.end());
		organismSpace[((int)pos.y * width) + (int)pos.x] = nullptr;
		delete organism;
	}

	void World::MoveOrganism(Organism* o, int dx, int dy)
	{
		// This shouldn't happen, but just in case.
		if (dx == 0 && dy == 0) return;

		glm::vec2 position = o->GetPosition();

		int px = (int)position.x;
		int py = (int)position.y;

		// We also need to check to be sure the place
		// the organism wants to move isn't off the grid.
		if (px + dx < 0 || px + dx >= width ||
			py + dy < 0 || py + dy >= height) return;

		// Every time something moves, it creates a "ripple"
		// that is really just a bool that tells organisms with
		// the right sensory neurons that something nearby is
		// moving.
		rippleBuffer[(py * width) + px] = true;

		Organism* target = GetOrganism(px + dx, py + dy);

		if (target != nullptr)
		{
			KillOrganism(target);
			o->Consume(100);
		}

		organismSpace[(py * width) + px] = nullptr;
		organismSpace[((py + dy) * width) + (px + dx)] = o;
		o->Move(glm::vec2(dx, dy));
	}

	void World::RandomlyPlaceOrganism(Organism* o)
	{
		int x = rand() % width;
		int y = rand() % height;

		if (GetOrganism(x, y) == nullptr)
		{
			o->SetPosition(glm::vec2(x, y));

			organismSpace[(y * width) + x] = o;

			return;
		}

		RandomlyPlaceOrganism(o);
	}

	void World::Terminate()
	{
		for (int i = 0; i < organisms.size(); i++)
		{
			delete organisms[i];
		}

		delete this;
	}
}
