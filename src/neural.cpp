#include "neural.h"
#include <iostream>
#include <string>

namespace Sefirah
{
	void Neural::Recurse(std::vector<Neuron*> activatedNeurons, std::vector<Neuron*> neurons, std::vector<Dendron> dendrons , int depth, int maxDepth)
	{
		if (depth > maxDepth) return;
		std::vector<Neuron*> newlyActivatedNeurons;
		
		for (int i = 0; i < activatedNeurons.size(); i++)
		{
			Neuron* n = activatedNeurons[i];
			n->value = tanhf(n->buffer);
			n->buffer = 0;
		}

		for (int i = 0; i < activatedNeurons.size(); i++)
		{
			Neuron* n = activatedNeurons[i];

			if (n->neuronType == NeuronType::compute)
			{
				for (int j = 0; j < n->dendronIndices.size(); j++)
				{
					Dendron d = dendrons[n->dendronIndices[j]];

					if (d.outputIndex == -1) continue;

					Neuron* out = neurons[d.outputIndex];

					out->buffer += n->value * d.strength;

					if (out->neuronType == NeuronType::compute)
					{
						if (std::find(newlyActivatedNeurons.begin(), newlyActivatedNeurons.end(), out) == newlyActivatedNeurons.end())
						{
							newlyActivatedNeurons.push_back(out);
						}
					}
				}
			}
		}
		
		Recurse(newlyActivatedNeurons, neurons, dendrons, depth + 1, maxDepth);
	}

	void Neural::RunBrain(Organism* organism, World* world, int maxRecurseDepth)
	{
		std::vector<Neuron> neurons = organism->GetNeurons();
		std::vector<Dendron> dendrons = organism->GetDendrons();

		glm::vec2 position = organism->GetPosition();
		glm::vec2 forward = organism->GetForward();

		std::vector<Neuron*> neuronPointers;

		std::vector<Neuron*> activatedNeurons;

		// First, we run through each input neuron and get its value.

		for (int i = 0; i < neurons.size(); i++)
		{
			Neuron n = neurons[i];
			neuronPointers.push_back(&neurons[i]);

			if (n.neuronType == NeuronType::sense)
			{
				float value = 0;

				if (n.senseType == SenseNeuronType::sight)
				{
					value = world->CheckSight(position, forward, 2, 10);
				}
				else if (n.senseType == SenseNeuronType::smell)
				{
					value = world->CheckSmell(position, 10);
				}
				else if (n.senseType == SenseNeuronType::sound)
				{
					value = world->CheckSounds(position, 10);
				}
				else
				{
					value = world->CheckTouch(position);
				}

				for (int j = 0; j < n.dendronIndices.size(); j++)
				{
					Dendron d = dendrons[n.dendronIndices[j]];
					
					if (d.outputIndex == -1) continue;

					Neuron* out = &neurons[d.outputIndex];
					
					if (out->neuronType != NeuronType::sense)
					{
						out->buffer += value * d.strength;
						activatedNeurons.push_back(out);
					}
				}
			}
		}

		// Cool, now that we've done that we can recursively run
		// the brain until we either hit our max recursion depth
		// or we run out of neurons to activate.
		Recurse(activatedNeurons, neuronPointers, dendrons, 0, maxRecurseDepth);

		// And now we go through all the action neurons to see if
		// any of them actually activate.
		for (int i = 0; i < neurons.size(); i++)
		{
			Neuron n = neurons[i];
			
			if (n.neuronType == NeuronType::action)
			{
				if (n.actionType == ActionNeuronType::moveForward)
				{
					if (n.buffer > 0) world->MoveOrganism(organism, forward.x, forward.y);
				}
				else if (n.actionType == ActionNeuronType::moveBack)
				{
					if (n.buffer > 0) world->MoveOrganism(organism, -forward.x, -forward.y);
				}
				else if (n.actionType == ActionNeuronType::moveRandom)
				{
					if (n.buffer > 0)
					{
						int rx = 0;
						int ry = 0;

						float r1 = 1 + (rand() % 100);
						float r2 = 1 + (rand() % 100);

						if (r1 < 33) rx = -1;
						else if (r1 < 66) rx = 1;

						if (r2 < 33) ry = -1;
						else if (r2 < 66) ry = 1;

						world->MoveOrganism(organism, rx, ry);
					}
				}
				else if (n.actionType == ActionNeuronType::moveX)
				{
					if (n.buffer < 0) world->MoveOrganism(organism, -1, 0);
					else if (n.buffer > 0) world->MoveOrganism(organism, 1, 0);
				}
				else
				{
					if (n.buffer < 0) world->MoveOrganism(organism, 0, -1);
					else if (n.buffer > 0) world->MoveOrganism(organism, 0, 1);
				}
			}
		}
	}
}
