#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "api.h"

class NETWORK_API Network {
	struct InputNode {
		float current_value;
		std::vector<float> weights;

		InputNode();

		InputNode(float _current_value, std::vector<float> _weights);

		void setCurrentValue(float new_value);

		void randomize();

		float getWeightAt(int index);

		float getCurrentValue();

		void mutate(float scale);
	};
	struct MiddleNode {
		float current_value;
		std::vector<float> inputs;
		std::vector<float> weights;
		float bias;

		MiddleNode();

		MiddleNode(float _current_value, std::vector<float> _weights, float _bias);

		void calcCurrentValue();

		void randomize();

		void addInput(float input);

		float getWeightAt(int index);

		float getCurrentValue();

		void mutate(float scale);
	};
	struct OutputNode {
		float current_value;
		std::vector<float> inputs;
		float bias;
		
		OutputNode();

		OutputNode(float _current_value, float _bias);

		void calcCurrentValue();

		void randomize();

		void addInput(float input);

		float getCurrentValue();

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
	Network(std::vector<InputNode> _input_nodes, std::vector<MiddleNode> _middle_nodes, std::vector<OutputNode> _output_nodes);

public:
	Network();

	static Network createRandom(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count);

	friend std::ostream& operator<<(std::ostream& out_stream, const InputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, InputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const MiddleNode& node);

	friend std::istream& operator>>(std::istream& in_stream, MiddleNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const OutputNode& node);

	friend std::istream& operator>>(std::istream& in_stream, OutputNode& node);

	friend std::ostream& operator<<(std::ostream& out_stream, const Network& net);

	friend std::istream& operator>>(std::istream& in_stream, Network& net);

	void save(std::string filename);

	static Network load(std::string filename);

	void randomize();

	void step();

	void beginThinking();

	void endThinking();

	float getOutputAt(int index);

	void sendInputs(std::vector<float> inputs);

	void mutate(float scale);
};