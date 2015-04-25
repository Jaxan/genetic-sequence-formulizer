#include "evolve.hpp"
#include "utilities.hpp"

#include <map>
#include <iterator>
#include <cmath>
#include <iostream>

static Score score(const std::vector<int>& goal, Genome const & genome){
	static const std::vector<const int> inputs = {2003, 1337, 7877, 5732, 8315, 4466, 0354, 8923, 8888, 0000, 4059};
	static const std::vector<const int> outputs = {2, 4, 3, 4, 3, 0, 2, 2, 0, 0, 1};

	Score ss = 0;
	for(int i = 0; i < inputs.size(); ++i) {
		Score error = outputs[i] - genome.evaluate_on(inputs[i]);
		ss += error * error;
	}
	return ss;
}

template <typename It, typename Dist>
It advance2(It it, Dist d){
	std::advance(it, d);
	return it;
}

template <typename C>
struct subrange_t {
	C & c;
	int s;

	subrange_t(C & c_, int s_)
	: c(c_), s(s_)
	{}

	auto begin(){ return advance2(c.begin(), s); }
	auto end(){ return c.end(); }

	auto begin() const { return advance2(c.begin(), s); }
	auto end() const { return c.end(); }
};

template <typename C>
auto subrange(C & c, int start){
	return subrange_t<C>(c, start);
}

void Evolver::next_generation(){
	// evaluate (if needed)
	for(auto&& g : current_generation){
		if(g.second < 0) {
			g.second = score(goal, g.first);
		}
	}

	// pick best no worse than parent
	auto best = current_generation[0];
	for(auto&& g : current_generation){
		if(g.second <= best.second){
			best = g;
		}
	}

	// continue with the best as parent
	current_generation[0] = best;
	int count = 0;
	for(auto& g : subrange(current_generation, 1)){
		count++;
		g = best;
		for(int j = 0; j < count; ++j){
			while(!g.first.mutate_random_bit()){}
		}
		g.second = -1;
	}
}

Evolver create_evolver(size_t population_size, size_t genome_size){
	Evolver e;
	e.current_generation.reserve(population_size);
	for(size_t i = 0; i < population_size; ++i){
		e.current_generation.emplace_back(random_genome(genome_size), -1);
	}
	return e;
}
