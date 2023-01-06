#include "organism.h"
#include <time.h>
#include <iostream>

namespace Sefirah
{
	void Organism::Move(glm::vec2 dxy)
	{
		position += dxy;
		forward = dxy;
	}

	void Organism::DevelopNeuron(Gene* gene)
	{
		gene->geneType = GeneType::neuron;
		gene->index = neurons.size();

		Neuron n =
		{
			static_cast<NeuronType>(rand() % N_TYPES),
			static_cast<SenseNeuronType>(rand() % SN_TYPES),
			static_cast<ActionNeuronType>(rand() % AN_TYPES)
		};

		neurons.push_back(n);
	}

	void Organism::DevelopDendron(Gene* gene)
	{
		gene->geneType = GeneType::dendron;
		gene->index = dendrons.size();

		Dendron d =
		{
			rand() % neurons.size(),
			rand() % neurons.size(),
			((1 + (rand() % 200)) - 100) / 25.0f
		};

		Neuron* n = &neurons[d.inputIndex];
		n->dendronIndices.push_back(gene->index);

		dendrons.push_back(d);
	}

	void Organism::RemoveNeuron(int index)
	{
		Neuron* n = &neurons[index];

		for (int i = 0; i < dendrons.size(); i++)
		{
			Dendron* d = &dendrons[i];

			if (d->inputIndex == index) d->inputIndex = -1;
			if (d->outputIndex == index) d->outputIndex = -1;
		}

		neurons.erase(neurons.begin() + index);
	}

	void Organism::RemoveDendron(int index)
	{
		Dendron* d = &dendrons[index];

		for (int i = 0; i < neurons.size(); i++)
		{
			Neuron* n = &neurons[i];

			for (int j = 0; j < n->dendronIndices.size(); j++)
			{
				if (j == index)
				{
					// !!!
					n->dendronIndices.erase(n->dendronIndices.begin() + j);
				}
			}
		}

		dendrons.erase(dendrons.begin() + index);
	}

	void Organism::Evolve()
	{
		srand(time(NULL));
		
		// First, let's just change the color slightly.
		float cr = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5f;
		float cg = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5f;
		float cb = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5f;

		glm::vec4 newColor = glm::vec4(cr + color.r, cg + color.g, cb + color.b, color.a);

		if (newColor.r < 0) newColor.r = 0.0f;
		else if (newColor.r > 1) newColor.r = 1.0f;
		if (newColor.g < 0) newColor.g = 0.0f;
		else if (newColor.g > 1) newColor.g = 1.0f;
		if (newColor.b < 0) newColor.b = 0.0f;
		else if (newColor.b > 1) newColor.b = 1.0f;

		color = newColor;
		
		// We need to figure out which gene we're going to modify.

		Gene* gene = &genome[rand() % genome.size()];

		// Are we going to flip the gene.
		float r = (rand() % 1000) / 100.0f;

		if (r < 5.0f)
		{
			if (gene->geneType == GeneType::neuron)
			{
				// We're going to flip a neuron into a dendron.
				RemoveNeuron(gene->index);
				DevelopDendron(gene);
				return;
			}

			// We're going to flip a dendron into a neuron.
			RemoveDendron(gene->index);
			DevelopNeuron(gene);
			return;
		}

		// Okay, so now we know that we're not going to flip the
		// whole type of gene; we need to figure out what kind of
		// gene we have on our hands.

		r = (rand() % 1000) / 100.0f;

		if (gene->geneType == GeneType::neuron)
		{
			// We're dealing with a neuron.

			Neuron* neuron = &neurons[gene->index];

			// We have three options:
			// 1. We can flip its type.
			// 2. We can flip the sensory type.
			// 3. We can flip the action type.

			if (r < 33)
			{
				// We are going to flip its type.
				neuron->neuronType = static_cast<NeuronType>(rand() % N_TYPES);
				return;
			}
			else if (r < 66)
			{
				// We are going to flip its sense type.
				neuron->senseType = static_cast<SenseNeuronType>(rand() % SN_TYPES);
				return;
			}

			// We are going to flip its action type.
			neuron->actionType = static_cast<ActionNeuronType>(rand() % AN_TYPES);
			return;
		}
		
		// We're dealing with a dendron.

		Dendron* dendron = &dendrons[gene->index];

		// We have three options again:
		// 1. We can change its input.
		// 2. We can change its output.
		// 3. We can change its strength.

		if (r < 33)
		{
			// We're going to change the strength of the 
			// connection.
			float ds = ((1 + (rand() % 200)) - 100) / 25.0f;

			dendron->strength += ds;

			if (dendron->strength > 4.0f) dendron->strength = 4.0f;
			else if (dendron->strength < -4.0f) dendron->strength = -4.0f;
			return;
		}
		else if (r < 66)
		{
			// We're going to change the input of the connection.

			dendron->inputIndex = rand() % neurons.size();
			return;
		}

		// We're going to change the output of the connection.

		dendron->outputIndex = rand() % neurons.size();
	}

	Organism::Organism(unsigned int geneCount)
	{
		this->position = glm::vec2(0, 0);

		this->forward = glm::vec2(0, 1);

		this->color = glm::vec4(	static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
									static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
									static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
									1.0f);

		this->satiation = 50;

		// Generate new neurons and dendrons.
		
		// First, generate an initial neuron.
		Gene gene =
		{
			GeneType::neuron,
			0
		};

		this->DevelopNeuron(&gene);

		genome.push_back(gene);

		for (int i = 0; i < geneCount; i++)
		{
			int r = rand() % 100;

			if (r < 49)
			{
				Gene gene =
				{
					GeneType::neuron,
					neurons.size()
				};

				this->DevelopNeuron(&gene);

				genome.push_back(gene);

				continue;
			}

			// Generate a dendron.
			Gene gene =
			{
				GeneType::dendron,
				dendrons.size()
			};

			this->DevelopDendron(&gene);

			genome.push_back(gene);
		}
	}

	Organism::Organism(Organism* parent)
	{
		this->position = parent->position;
		this->forward = glm::vec2(0, 1);
		this->color = parent->color;

		this->satiation = 50;

		this->genome = parent->genome;
		this->neurons = parent->neurons;
		this->dendrons = parent->dendrons;
	}
}