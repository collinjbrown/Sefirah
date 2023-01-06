#ifndef WORLD_H
#define WORLD_H

#include "organism.h"

namespace Sefirah
{
	class World
	{
	private:
		// World Size
		const static unsigned int width = 256;
		const static unsigned int height = 256;

		float evolutionRate = 0.01f;

		// Organisms
		Organism*				organismSpace[width * height];
		std::vector<Organism*>	organisms;

		// Ripple & Ripple Buffer
		bool		ripple[width * height];
		bool		rippleBuffer[width * height];

	public:
		// World Size 2: Electric Boogaloo
		int			Width() { return width; }
		int			Height() { return height; }

		// We need to be able to provide data about the world
		// for the sensory organs.
		float					CheckSight(glm::vec2 pos, glm::vec2 forward, int w, int r);
		float					CheckSmell(glm::vec2 pos, int r);
		float					CheckSounds(glm::vec2 pos, int r);
		float					CheckTouch(glm::vec2 pos);

		// Get or add an organism.
		void					AddOrganism(Organism* organism) { organisms.push_back(organism); }
		Organism*				GetOrganism(int x, int y);
		std::vector<Organism*>	GetOrganisms() { return organisms; }

		void					ReproduceOrganism(Organism* organism);
		void					KillOrganism(Organism* organism);
		
		void					MoveOrganism(Organism* o, int dx, int dy);
		void					RandomlyPlaceOrganism(Organism* o);

		void		Terminate();
	};
}

#endif