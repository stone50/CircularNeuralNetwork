#pragma once

#include <string>
#include <iostream>
#include "api.h"

class NETWORK_API Network {
public:
	Network();

	Network(std::vector<InputNode> _input_nodes, std::vector<MiddleNode> _middle_nodes, std::vector<OutputNode> _output_nodes);

	static Network createRandom(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count);

	friend std::ostream& operator<<(std::ostream& out_stream, const Network& net);

	friend std::istream& operator>>(std::istream& in_stream, Network& net);

	void save(std::string filename);

	static Network load(std::string filename);

	void randomize();

	void step();

	void beginThinking();

	void endThinking();

	// returns the value of the output at the given index
	float getOutputAt(int index);

	void sendInputs(std::vector<float> inputs);

	void mutate(float scale);

private:
	std::vector<InputNode> input_nodes;
	std::vector<MiddleNode> middle_nodes;
	std::vector<OutputNode> output_nodes;
	unsigned int input_nodes_size;
	unsigned int middle_nodes_size;
	unsigned int output_nodes_size;
	std::vector<float> outputs;
	bool thinking;
};