#pragma once

#include <vector>
#include <iostream>
#include "api.h"

class Network {
	struct InputNode {
		float current_value;
		std::vector<float> weights;

		InputNode();

		~InputNode();

		InputNode(float _current_value, const std::vector<float>& _weights);

		void randomize();

		void mutate(float scale);
	};
	struct MiddleNode {
		float current_value;
		std::vector<float> weights;
		float bias;
		float inputSum;

		MiddleNode();

		~MiddleNode();

		MiddleNode(float _current_value, const std::vector<float>& _weights, float _bias);

		void randomize();

		void mutate(float scale);
	};
	struct OutputNode {
		float current_value;
		float bias;
		float inputSum;
		
		OutputNode();

		OutputNode(float _current_value, float _bias);

		void randomize();

		void mutate(float scale);
	};
	std::vector<InputNode> input_nodes;
	std::vector<MiddleNode> middle_nodes;
	std::vector<OutputNode> output_nodes;
	unsigned int input_nodes_size;
	unsigned int middle_nodes_size;
	unsigned int output_nodes_size;
	std::vector<float> outputs;
	bool thinking;
	Network(const std::vector<InputNode>& _input_nodes, const std::vector<MiddleNode>& _middle_nodes, const std::vector<OutputNode>& _output_nodes);
	void baseStep();

public:
	NETWORK_API Network();

	NETWORK_API Network(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count);

	NETWORK_API ~Network();

	NETWORK_API static Network createRandom(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count);

	friend std::ostream& operator<<(std::ostream& out_stream, const InputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, InputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const MiddleNode& node);

	friend std::istream& operator>>(std::istream& in_stream, MiddleNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const OutputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, OutputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const Network& net);

	friend std::istream& operator>>(std::istream& in_stream, Network& net);

	NETWORK_API bool save(const char* filename);

	NETWORK_API static bool load(const char* filename, Network& net);

	NETWORK_API bool randomize();

	NETWORK_API bool step();

	NETWORK_API bool beginThinking();

	NETWORK_API void endThinking();

	NETWORK_API float* getOutputs();

	NETWORK_API void sendInputs(const float inputs[]);

	NETWORK_API bool mutate(float scale);
};