#ifndef ORGANISM_H
#define ORGANISM_H

#include <glm/glm.hpp>
#include <vector>

#define N_TYPES 3		// DO NOT FORGET TO CHANGE THESE IF YOU ADD ANY NEW NEURON TYPES!
#define SN_TYPES 4
#define AN_TYPES 6

namespace Sefirah
{
	enum class NeuronType { sense, compute, action };
	enum class SenseNeuronType { sight, smell, sound, touch };
	enum class ActionNeuronType { reproduce, moveForward, moveBack, moveRandom, moveX, moveY };

	struct Neuron
	{
		NeuronType				neuronType;
		SenseNeuronType			senseType;
		ActionNeuronType		actionType;

		float					value = 0;
		float					buffer = 0;

		std::vector<int>		dendronIndices;
	};

	struct Dendron
	{
		int		inputIndex;
		int		outputIndex;

		float	strength;
	};

	enum class GeneType { neuron, dendron };

	struct Gene
	{
		GeneType	geneType;
		int			index;
	};

	class Organism
	{
	private:
		glm::vec2				forward;		// "Forward" is treated as the direction
											// in which the creature last moved.

		glm::vec2				position;		// Where on the grid is the organism placed?

		glm::vec4				color;			// What color should we represent the creature with?

		std::vector<Gene>		genome;			// We currently store genes in a vector for ease
												// of access. We may change this later.

		std::vector<Neuron>		neurons;		// We store neurons in the same way for the same reason.
		std::vector<Dendron>	dendrons;		// Ditto

		unsigned int			satiation;		// If this drops to 0, the creature dies.
												// This is consumed when the creature reproduces
												// and naturally each turn.

		void					DevelopNeuron(Gene* gene);
		void					DevelopDendron(Gene* gene);
		void					RemoveNeuron(int index);
		void					RemoveDendron(int index);

	public:
		glm::vec2				GetForward() { return forward; }
		glm::vec2				GetPosition() { return position; }
		void					Move(glm::vec2 dxy);

		void					SetPosition(glm::vec2 pos) { this->position = pos; }

		glm::vec4				GetColor() { return color; }

		std::vector<Gene>		GetGenome() { return genome; }
		std::vector<Neuron>		GetNeurons() { return neurons; }
		std::vector<Dendron>	GetDendrons() { return dendrons; }

		int						GetSatiation() { return satiation; }
		void					Consume(int ds) { satiation += ds; }
		void					Starve(int ds) { satiation -= ds; }

		void					Evolve();

		Organism(unsigned int geneCount);
		Organism(Organism* parent);
	};
}

#endif