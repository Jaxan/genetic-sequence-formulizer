#pragma once

#include "genome.hpp"
#include <vector>

using Score = double;

struct Evolver{
	// stores the genome with its score
	// first element is the best one (after calling next_generation()).
	std::vector<std::pair<Genome, Score>> current_generation;

	// goal which we are trying to achieve
	std::vector<int> goal;

	// evaluates current generation, picks the best one, and generate new generation
	void next_generation();
};

// reate a random generation to start with
Evolver create_evolver(size_t population_size, size_t genome_size);
