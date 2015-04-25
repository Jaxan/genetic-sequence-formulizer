#pragma once

#include <vector>
#include <iosfwd>

// TODO: make this a decent type, and also make the variable "x" a decent type
using Node = int;

struct Value {
	bool is_node;
	union {
		int constant;
		Node node; // -1 reserved for "x"
	} value;
};

struct Gen {
	enum Type {
		kAdd,
		kSub,
		kMult,
		kMod,
		kDiv,
		kPow,
		kTypes
	} type;
	Value x, y;
};

struct Genome {
	std::vector<Gen> genes;
	std::vector<bool> active_genes;
	Node output;

	// calculates which genes are active (used internally)
	void calculate_active_genes();

	// evaluates expression with input x
	int evaluate_on(int x) const;

	// mutate a single bit of some gene, returns true when a active gene was altered
	bool mutate_random_bit();

	// checks whether the invariant still holds (for debugging)
	bool invariant() const;

	// returns a string in a (more or less) readable format
	std::string as_formula() const;

	// some trivial getters (to silence warnings)
	Gen & gene(int n) { return genes[static_cast<size_t>(n)]; }
	Gen const & gene(int n) const { return genes[static_cast<size_t>(n)]; }
	bool is_active(int n) const { return active_genes[static_cast<size_t>(n)]; }
};

// the obvious I/O routines
std::ostream & operator<<(std::ostream& out, Value const & g);
std::ostream & operator<<(std::ostream& out, Gen const & g);
std::ostream & operator<<(std::ostream& out, Genome const & g);

// generates a random genome with the specified number of genes/nodes
Genome random_genome(size_t number_of_genes);
