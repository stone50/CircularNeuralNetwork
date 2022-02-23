#pragma once

#include <vector>
#include <iostream>
#include "api.h"

class Network {
	struct InputNode {
		float current_value;
		std::vector<float> weights;

		InputNode();

		InputNode(const InputNode& other);

		InputNode(const float _current_value, const std::vector<float>& _weights);

		~InputNode();

		InputNode& operator=(const InputNode& other);

		void randomize();

		void mutate(const float scale);
	};
	struct MiddleNode {
		float current_value;
		std::vector<float> weights;
		float bias;
		float inputSum;

		MiddleNode();

		MiddleNode(const MiddleNode& other);

		MiddleNode(const float _current_value, const std::vector<float>& _weights, float _bias);

		~MiddleNode();

		MiddleNode& operator=(const MiddleNode& other);

		void randomize();

		void mutate(const float scale);
	};
	struct OutputNode {
		float current_value;
		float bias;
		float inputSum;
		
		OutputNode();

		OutputNode(const OutputNode& other);

		OutputNode(const float _current_value, const float _bias);

		OutputNode& operator=(const OutputNode& other);

		void randomize();

		void mutate(const float scale);
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

	NETWORK_API Network(const Network& other);

	NETWORK_API Network(const unsigned int input_node_count, const unsigned int middle_node_count, const unsigned int output_node_count);

	NETWORK_API ~Network();

	NETWORK_API Network& operator=(const Network& other);

	friend std::ostream& operator<<(std::ostream& out_stream, const InputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, InputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const MiddleNode& node);

	friend std::istream& operator>>(std::istream& in_stream, MiddleNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const OutputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, OutputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const Network& net);

	friend std::istream& operator>>(std::istream& in_stream, Network& net);

	NETWORK_API static Network createRandom(const unsigned int input_node_count, const unsigned int middle_node_count, const unsigned int output_node_count);

	NETWORK_API bool save(const char* filename);

	NETWORK_API static bool load(const char* filename, Network& net);

	NETWORK_API bool randomize();

	NETWORK_API bool step();

	NETWORK_API bool beginThinking();

	NETWORK_API void endThinking();

	NETWORK_API float* getOutputs();

	NETWORK_API void sendInputs(const float inputs[]);

	NETWORK_API bool mutate(const float scale);
};