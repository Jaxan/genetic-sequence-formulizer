#include "genome.hpp"
#include "evolve.hpp"

#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, char** argv){
	namespace po = boost::program_options;

	po::options_description opts;
	opts.add_options()
		("input", po::value<std::vector<int>>()->multitoken(), "sequence to generate a formula for (e.g. 1 2 4 8)")
		("help,h", po::bool_switch(), "show this help");

	po::positional_options_description file_opts;
	file_opts.add("input", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opts).positional(file_opts).run(), vm);
	po::notify(vm);

	if(vm["help"].as<bool>()){
		std::cout << "Genetic Sequence Formulizer, version " << __DATE__ << "\n";
		std::cout << "seq2form [-h] <sequence of non-negative integers>\n";
		std::cout << opts << std::endl;
		return 0;
	}

	// TODO: make these program options
	const auto genome_size = 16;
	const auto population = 10;
	const auto generations = 2000001 / population;

	auto evolver = create_evolver(population, genome_size);
	evolver.goal = vm["input"].as<std::vector<int>>();

	std::cout << "your input:\t";
	for(auto&& x : evolver.goal) std::cout << x << ", ";
	std::cout << std::endl;

	// GO! (until we find a perfect solution, or we hit the generations-bound)
	for(int i = 0; i < generations; ++i){
		evolver.next_generation();
		if(evolver.current_generation[0].second <= 0) {
			break;
		}
	}

	// SHOW!
	auto & best_genome = evolver.current_generation[0].first;
	std::cout << "formula:\t" << best_genome.as_formula() << std::endl;
	std::cout << "continuation:\n";
	static const std::vector<const int> inputs = {2003, 1337, 7877, 5732, 8315, 4466, 0354, 8923, 8888, 0000, 4059, 2403};
	static const std::vector<const int> outputs = {2, 4, 3, 4, 3, 0, 2, 2, 0, 0, 1, -1};
	for(int i = 0; i < inputs.size(); ++i){
		std::cout << inputs[i] << " = " << outputs[i] << " = " << best_genome.evaluate_on(i) << ", " << std::endl;
	}
	std::cout << std::endl;
}
