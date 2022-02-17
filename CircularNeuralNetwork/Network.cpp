#include "pch.h"
#include <fstream>
#include <mutex>
#include "Network.h"

using namespace std;

mutex network_thread_lock;

Network::Network() :
	input_nodes(vector<InputNode>()),
	middle_nodes(vector<MiddleNode>()),
	output_nodes(vector<OutputNode>()),
	input_nodes_size(0),
	middle_nodes_size(0),
	output_nodes_size(0),
	outputs(vector<float>()),
	thinking(false)
{}

Network::Network(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count) :
	input_nodes(vector<InputNode>(input_node_count, InputNode(0, vector<float>(middle_node_count, 0)))),
	middle_nodes(vector<MiddleNode>(middle_node_count, MiddleNode(0, vector<float>(middle_node_count + output_node_count - 1), 0))),
	output_nodes(vector<OutputNode>(output_node_count, OutputNode(0, 0))),
	input_nodes_size(input_node_count),
	middle_nodes_size(middle_node_count),
	output_nodes_size(output_node_count),
	outputs(vector<float>(output_node_count, 0)),
	thinking(false)
{}

Network::~Network() {
	input_nodes.~vector<InputNode>();
	middle_nodes.~vector<MiddleNode>();
	output_nodes.~vector<OutputNode>();
	outputs.~vector<float>();
}

Network::Network(const vector<InputNode>& _input_nodes, const vector<MiddleNode>& _middle_nodes, const vector<OutputNode>& _output_nodes) :
	input_nodes(_input_nodes),
	middle_nodes(_middle_nodes),
	output_nodes(_output_nodes),
	input_nodes_size((unsigned int)_input_nodes.size()),
	middle_nodes_size((unsigned int)_middle_nodes.size()),
	output_nodes_size((unsigned int)_output_nodes.size()),
	outputs(vector<float>()),
	thinking(false)
{
	for (OutputNode& output_node : output_nodes) {
		outputs.push_back(output_node.current_value);
	}
}

Network Network::createRandom(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count) {
	vector<InputNode> random_input_nodes;
	for (unsigned int input_node_index = 0; input_node_index < input_node_count; input_node_index++) {
		vector<float> input_weights;
		for (unsigned int weight_index = 0; weight_index < middle_node_count; weight_index++) {
			input_weights.push_back(random(-1, 1));
		}

		random_input_nodes.push_back(InputNode(random(-1, 1), input_weights));
	}

	vector<MiddleNode> random_middle_nodes;
	for (unsigned int middle_node_index = 0; middle_node_index < middle_node_count; middle_node_index++) {
		vector<float> middle_weights;
		unsigned int weight_count = middle_node_count + output_node_count - 1;
		for (unsigned int weight_index = 0; weight_index < weight_count; weight_index++) {
			middle_weights.push_back(random(-1, 1));
		}

		random_middle_nodes.push_back(MiddleNode(random(-1, 1), middle_weights, random(-1, 1)));
	}

	vector<OutputNode> random_output_nodes;
	for (unsigned int output_node_index = 0; output_node_index < output_node_count; output_node_index++) {
		random_output_nodes.push_back(OutputNode(random(-1, 1), random(-1, 1)));
	}

	return Network(random_input_nodes, random_middle_nodes, random_output_nodes);
}

ostream& operator<<(ostream& out_stream, const Network& net){
	out_stream << net.input_nodes_size << endl;
	for (const Network::InputNode& input_node : net.input_nodes) {
		out_stream << input_node << endl;
	}
	out_stream << net.middle_nodes_size << endl;
	for (const Network::MiddleNode& middle_node : net.middle_nodes) {
		out_stream << middle_node << endl;
	}
	out_stream << net.output_nodes_size << endl;
	for (const Network::OutputNode& output_node : net.output_nodes) {
		out_stream << output_node << endl;
	}
	unsigned int outputs_size = (unsigned int)net.outputs.size();
	for (unsigned int i = 0; i < outputs_size - 1; i++) {
		out_stream << net.outputs.at(i) << endl;
	}
	return out_stream;
}

istream& operator>>(istream& in_stream, Network& net) {
	in_stream >> net.input_nodes_size;
	for (unsigned int i = 0; i < net.input_nodes_size; i++) {
		Network::InputNode input_node;
		in_stream >> input_node;
		net.input_nodes.push_back(input_node);
	}
	in_stream >> net.middle_nodes_size;
	for (unsigned int i = 0; i < net.middle_nodes_size; i++) {
		Network::MiddleNode middle_node;
		in_stream >> middle_node;
		net.middle_nodes.push_back(middle_node);
	}
	in_stream >> net.output_nodes_size;
	for (unsigned int i = 0; i < net.output_nodes_size; i++) {
		Network::OutputNode output_node;
		in_stream >> output_node;
		net.output_nodes.push_back(output_node);
	}
	for (Network::OutputNode& output_node : net.output_nodes) {
		net.outputs.push_back(output_node.current_value);
	}
	return in_stream;
}

bool Network::save(const char* filename) {
	if (thinking) {
		return false;
	}
	ofstream file_stream(filename, ios::trunc);
	file_stream << *this;
	return true;
}

bool Network::load(const char* filename, Network& net) {
	if (net.thinking) {
		return false;
	}
	ifstream file_stream(filename);
	file_stream >> net;
	return file_stream.good();
}

bool Network::randomize() {
	if (thinking) {
		return false;
	}
	for (unsigned int i = 0; i < input_nodes_size; i++) {
		input_nodes.at(i).randomize();
	}
	for (unsigned int i = 0; i < middle_nodes_size; i++) {
		middle_nodes.at(i).randomize();
	}
	for (unsigned int i = 0; i < output_nodes_size; i++) {
		output_nodes.at(i).randomize();
	}
	return true;
}

void Network::threadedStep() {
	// input nodes send outputs
	for (InputNode& input_node : input_nodes) {
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			float& middleInputSum = middle_nodes.at(middle_node_index).inputSum;
			float input = middleInputSum + (input_node.current_value * input_node.weights.at(middle_node_index));
			network_thread_lock.lock();
			middleInputSum = input;
			network_thread_lock.unlock();
		}
	}

	// middle nodes send outputs
	for (unsigned int sender_index = 0; sender_index < middle_nodes_size; sender_index++) {
		bool sender_reached = false;
		MiddleNode sender = middle_nodes.at(sender_index);
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			if (sender_index != middle_node_index) {
				float& middleInputSum = middle_nodes.at(middle_node_index).inputSum;
				float input = middleInputSum + (sender.current_value * sender.weights.at(middle_node_index - sender_reached));
				network_thread_lock.lock();
				middleInputSum = input;
				network_thread_lock.unlock();
			}
			else {
				sender_reached = true;
			}
		}
		for (unsigned int output_node_index = 0; output_node_index < output_nodes_size; output_node_index++) {
			float& outputInputSum = output_nodes.at(output_node_index).inputSum;
			float input = outputInputSum + (sender.current_value * sender.weights.at(output_node_index + middle_nodes_size - 1));
			network_thread_lock.lock();
			outputInputSum = input;
			network_thread_lock.unlock();
		}
	}

	// middle nodes calculate current value
	for (MiddleNode& middle_node : middle_nodes) {
		float value = sigmoid(middle_node.inputSum);
		network_thread_lock.lock();
		middle_node.current_value = value;
		middle_node.inputSum = middle_node.bias;
		network_thread_lock.unlock();
	}

	// output nodes calculate current value
	for (OutputNode& output_node : output_nodes) {
		float value = sigmoid(output_node.inputSum);
		network_thread_lock.lock();
		output_node.current_value = value;
		output_node.inputSum = output_node.bias;
		network_thread_lock.unlock();
	}

	// replace elements of outputs using output_nodes
	for (unsigned int output_index = 0; output_index < output_nodes_size; output_index++) {
		float& outputValue = outputs.at(output_index);
		float outputNodeValue = output_nodes.at(output_index).current_value;
		network_thread_lock.lock();
		outputValue = outputNodeValue;
		network_thread_lock.unlock();
	}
}

bool Network::step() {
	// stepBase() should not be called while the network is already thinking
	if (thinking) {
		return false;
	}

	// input nodes send outputs
	for (InputNode& input_node : input_nodes) {
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			middle_nodes.at(middle_node_index).inputSum += input_node.current_value * input_node.weights.at(middle_node_index);
		}
	}

	// middle nodes send outputs
	for (unsigned int sender_index = 0; sender_index < middle_nodes_size; sender_index++) {
		bool sender_reached = false;
		MiddleNode sender = middle_nodes.at(sender_index);
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			if (sender_index != middle_node_index) {
				middle_nodes.at(middle_node_index).inputSum += sender.current_value * sender.weights.at(middle_node_index - sender_reached);
			}
			else {
				sender_reached = true;
			}
		}
		for (unsigned int output_node_index = 0; output_node_index < output_nodes_size; output_node_index++) {
			output_nodes.at(output_node_index).inputSum += sender.current_value * sender.weights.at(output_node_index + middle_nodes_size - 1);
		}
	}

	// middle nodes calculate current value
	for (MiddleNode& middle_node : middle_nodes) {
		middle_node.current_value = sigmoid(middle_node.inputSum);
		middle_node.inputSum = middle_node.bias;
	}

	// output nodes calculate current value
	for (OutputNode& output_node : output_nodes) {
		output_node.current_value = sigmoid(output_node.inputSum);
		output_node.inputSum = output_node.bias;
	}

	// replace elements of outputs using output_nodes
	for (unsigned int output_index = 0; output_index < output_nodes_size; output_index++) {
		outputs.at(output_index) = output_nodes.at(output_index).current_value;
	}
	return true;
}

void Network::beginThinking() {
	network_thread_lock.lock();
	thinking = true;
	network_thread_lock.unlock();
	while (thinking) {
		threadedStep();
	}
}

void Network::endThinking() {
	network_thread_lock.lock();
	thinking = false;
	network_thread_lock.unlock();
}

float* Network::getOutputs() {
	return outputs.data();
}

void Network::sendInputs(const float inputs[]) {
	for (unsigned int i = 0; i < input_nodes_size; i++) {
		float& inputCurrentValue = input_nodes.at(i).current_value;
		float newCurrentValue = sigmoid(inputs[i]);
		network_thread_lock.lock();
		inputCurrentValue = newCurrentValue;
		network_thread_lock.unlock();
	}
}

bool Network::mutate(float scale) {
	if (thinking) {
		return false;
	}
	for (InputNode& input_node : input_nodes) {
		input_node.mutate(scale);
	}
	for (MiddleNode& middle_node : middle_nodes) {
		middle_node.mutate(scale);
	}
	for (OutputNode& output_node : output_nodes) {
		output_node.mutate(scale);
	}
	return true;
}