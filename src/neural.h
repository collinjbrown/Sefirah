#ifndef NEURAL_H
#define NEURAL_H

#include "world.h"

namespace Sefirah
{
	class Neural
	{
	public:
		void Recurse(std::vector<Neuron*> activatedNeurons, std::vector<Neuron*> neurons, std::vector<Dendron> dendrons, int depth, int maxDepth);
		void RunBrain(Organism* organism, World* world, int maxRecurseDepth);
	};
}

#endif