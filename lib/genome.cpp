#include "genome.hpp"
#include "utilities.hpp"

#include <random>
#include <functional>
#include <iostream>
#include <stack>
#include <cassert>

// Pretty printing nowadays obviously needs colors
// blue == constant. red == node or variable
// gray background == inactive gene
static std::string color(int n, bool node){
	if(node && n == -1) return "\x1B[31mx\x1b[39m";
	if(node) return "\x1B[31m" + std::to_string(n) + "\x1b[39m";
	return "\x1B[36m" + std::to_string(n) + "\x1b[39m";
}

std::ostream & operator<<(std::ostream& out, const Value& v){
	if (v.is_node) {
		return out << color(v.value.node, true);
	} else {
		return out << color(v.value.constant, false);
	}
}

std::ostream & operator<<(std::ostream& out, Gen const & g){
	switch (g.type) {
		case Gen::kAdd: return out << "[+ " << g.x << ' ' << g.y << ']';
		case Gen::kSub: return out << "[- " << g.x << ' ' << g.y << ']';
		case Gen::kMult: return out << "[* " << g.x << ' ' << g.y << ']';
		case Gen::kDiv: return out << "[/ " << g.x << ' ' << g.y << ']';
		case Gen::kPow: return out << "[^ " << g.x << ' ' << g.y << ']';
		default: return out << "[error]";
	}
}

std::ostream & operator<<(std::ostream& out, Genome const & g){
	for(unsigned int i = 0; i < g.genes.size(); ++i){
		if(g.active_genes[i]){
			out << g.genes[i];
		} else {
			out << "\x1B[47m" << g.genes[i] << "\x1B[49m";
		}
	}
	return out << g.output;
}

void Genome::calculate_active_genes(){
	active_genes.assign(genes.size(), false);

	// NOTE: prohibits concurrency, but it's a speedup for now
	static std::stack<Node> nodes;
	nodes.push(output);
	while(!nodes.empty()){
		const auto n = nodes.top();
		nodes.pop();
		if(n == -1 || is_active(n)) continue;
		active_genes[static_cast<size_t>(n)] = true;

		auto const & g = gene(n);
		if (g.x.is_node) {
			nodes.push(g.x.value.node);
		}
		if (g.y.is_node) {
			nodes.push(g.y.value.node);
		}
	}
}

static int calc_op(Gen::Type const & op, int const & x, int const & y){
	switch (op) {
		case Gen::kAdd: return x + y;
		case Gen::kSub: return x - y;
		case Gen::kMult: return x * y;
		case Gen::kDiv: return divi(x, y);
		case Gen::kPow: return powi(x, y);
		default: return 0;
	}
}

int Genome::evaluate_on(int x) const{
	// NOTE: prohibits concurrency, but it's a speedup for now
	static std::vector<int> results;
	results.assign(output + 1, 0);

	for(int i = 0; i <= output; ++i){
		if(!is_active(i)) continue;

		auto const & g = gene(i);
		auto const xv = g.x.is_node ? (g.x.value.node == -1 ? x : results[g.x.value.node]) : g.x.value.constant;
		auto const yv = g.y.is_node ? (g.y.value.node == -1 ? x : results[g.y.value.node]) : g.y.value.constant;

		results[i] = calc_op(g.type, xv, yv);
	}
	return results[output];
}

// FIXME: encapsulate all this
using Generator = std::mt19937;
using Uniform = std::uniform_int_distribution<int>;
using Params = Uniform::param_type;

static Generator generator{0};
static Uniform distribution;
static auto random_type = std::bind(distribution, generator, Params(0, Gen::kTypes-1));
static auto random_const = std::bind(distribution, generator, Params(-20, 20));
static auto random_node = [](int n){ return distribution(generator, Params(0, n-1)); };

static Value random_value(Node n){
	Value v;
	switch(distribution(generator, Params(0, n ? 2 : 1))){
		case 0:	v.is_node = false;
			v.value.constant = random_const();
			break;
		case 1:	v.is_node = true;
			v.value.node = -1;
			break;
		case 2:	v.is_node = true;
			v.value.node = random_node(n);
			break;
	}

	return v;
}

static Gen random_gen(Node n){
	Gen g;
	g.type = Gen::Type(random_type());
	g.x = random_value(n);
	g.y = random_value(n);
	return g;
}

Genome random_genome(size_t number_of_genes){
	Genome g;
	// FIXME: don't silence the warning, fix the node-numbering
	const int nn = static_cast<int>(number_of_genes);
	for(int i = 0; i < nn; ++i){
		g.genes.emplace_back(random_gen(i));
	}
	g.output = random_node(nn);
	g.calculate_active_genes();
	return g;
}

bool Genome::mutate_random_bit(){
	// FIXME: make this more robust
	const auto fields_per_gen = 3;
	const auto bit = distribution(generator, Params(0, fields_per_gen * genes.size())); // inclusive
	if(bit == fields_per_gen * genes.size()){
		output = random_node(genes.size());
		calculate_active_genes();
		assert(invariant());
		return true;
	} else {
		const auto n = bit / fields_per_gen;
		const auto field = bit % fields_per_gen;
		auto & g = gene(n);
		switch(field){
			case 0:
				g.type = Gen::Type(random_type());
				break;
			case 1:
				g.x = random_value(n);
				break;
			case 2:
				g.y = random_value(n);
				break;
		}
		if(is_active(n)){
			calculate_active_genes();
			assert(invariant());
			return true;
		} else {
			assert(invariant());
			return false;
		}
	}
}

bool Genome::invariant() const{
	for(int n = 0; n < genes.size(); ++n){
		Gen const & g = gene(n);
		if(g.x.is_node && g.x.value.node >= n) return false;
		if(g.y.is_node && g.y.value.node >= n) return false;
	}
	return true;
}

std::string Genome::as_formula() const{
	static std::vector<std::string> results;
	results.assign(output+1, "err");

	for(int i = 0; i <= output; ++i){
		if(!is_active(i)) continue;

		auto const & g = gene(i);
		std::string x;
		std::string y;

		if(g.x.is_node){
			if(g.x.value.node == -1) x = "x";
			else x = "(" + results[g.x.value.node] + ")";
		} else {
			x = std::to_string(g.x.value.constant);
		}

		if(g.y.is_node){
			if(g.y.value.node == -1) y = "x";
			else y = "(" + results[g.y.value.node] + ")";
		} else {
			y = std::to_string(g.y.value.constant);
		}

		switch (g.type) {
			case Gen::kAdd: results[i] = x + "+" + y; break;
			case Gen::kSub: results[i] = x + "-" + y; break;
			case Gen::kMult: results[i] = x + "*" + y; break;
			case Gen::kDiv: results[i] = x + "/" + y; break;
			case Gen::kPow: results[i] = x + "^" + y; break;
			default: results[i] = "err";
		}
	}
	return results[output];
}

