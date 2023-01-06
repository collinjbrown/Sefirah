# Design
## Purpose
 Sefirah is meant to be a little, non-serious simulation of
 the evolution of organisms, specifically their brains. It
 uses very, very rudimentary neural networks to model the
 neurons and dendrons of a brain, each of which are controlled
 by genes that are passed down from one organism to another.
 
 This model is heavily influenced by the simulation made by
 David R. Miller which is the centerpiece of a video called
 ["I programmed some creatures. They Evolved."](https://www.youtube.com/watch?v=N3tRFayqVtk)
 Sefirah is the second version of this program that we've written;
 the first was more of a test to see if we could implement
 those features present in Miller's program, which was a
 success. In contrast, Sefirah is meant to extend some of
 these features and tack on new ones.

## Outline
 The general structure of Sefirah will look like this:
 
	Organisms are the core unit of the model. They are composed
	of a number of components, but the one we are most
	interested in is their genome: a set of rules about
	how to construct the neural network that makes up their
	brain.

	Genes can be separated into neuron and dendron genes:
	neuron genes indicate a particular neuron that an
	organism has, while dendron genes indicate a particular
	connection between neurons.

