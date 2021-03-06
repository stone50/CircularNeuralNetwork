#include <fstream>
#include <mutex>
#include "Network.h"
#include "helpers.h"

using namespace std;

mutex mutex_lock;

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

Network::Network(const Network& other) {
	if (!other.thinking) {
		input_nodes = other.input_nodes;
		middle_nodes = other.middle_nodes;
		output_nodes = other.output_nodes;
		input_nodes_size = other.input_nodes_size;
		middle_nodes_size = other.middle_nodes_size;
		output_nodes_size = other.output_nodes_size;
		outputs = other.outputs;
		thinking = false;
	} else {
		*this = Network();
	}
}

Network::Network(const unsigned int input_node_count, const unsigned int middle_node_count, const unsigned int output_node_count) :
	input_nodes(vector<InputNode>(input_node_count, InputNode(0, vector<float>(middle_node_count)))),
	middle_nodes(vector<MiddleNode>(middle_node_count, MiddleNode(0, vector<float>(middle_node_count + output_node_count - 1), 0))),
	output_nodes(vector<OutputNode>(output_node_count)),
	input_nodes_size(input_node_count),
	middle_nodes_size(middle_node_count),
	output_nodes_size(output_node_count),
	outputs(vector<float>(output_node_count)),
	thinking(false)
{}

Network::Network(const vector<InputNode>& _input_nodes, const vector<MiddleNode>& _middle_nodes, const vector<OutputNode>& _output_nodes) :
	input_nodes(_input_nodes),
	middle_nodes(_middle_nodes),
	output_nodes(_output_nodes),
	input_nodes_size((unsigned int)_input_nodes.size()),
	middle_nodes_size((unsigned int)_middle_nodes.size()),
	output_nodes_size((unsigned int)_output_nodes.size()),
	outputs(vector<float>(_output_nodes.size())),
	thinking(false)
{}

Network::~Network() {
	input_nodes.~vector<InputNode>();
	middle_nodes.~vector<MiddleNode>();
	output_nodes.~vector<OutputNode>();
	outputs.~vector<float>();
}

Network& Network::operator=(const Network& other) {
	if (!other.thinking) {
		input_nodes = other.input_nodes;
		middle_nodes = other.middle_nodes;
		output_nodes = other.output_nodes;
		input_nodes_size = other.input_nodes_size;
		middle_nodes_size = other.middle_nodes_size;
		output_nodes_size = other.output_nodes_size;
		outputs = other.outputs;
		thinking = false;
	}
	return *this;
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
	out_stream << net.output_nodes_size;
	for (const Network::OutputNode& output_node : net.output_nodes) {
		out_stream << endl << output_node;
	}
	return out_stream;
}

istream& operator>>(istream& in_stream, Network& net) {
	in_stream >> net.input_nodes_size;
	net.input_nodes = vector<Network::InputNode>(net.input_nodes_size);
	for (Network::InputNode& input_node : net.input_nodes) {
		in_stream >> input_node;
	}
	in_stream >> net.middle_nodes_size;
	net.middle_nodes = vector<Network::MiddleNode>(net.middle_nodes_size);
	for (Network::MiddleNode& middle_node : net.middle_nodes) {
		in_stream >> middle_node;
	}
	in_stream >> net.output_nodes_size;
	net.output_nodes = vector<Network::OutputNode>(net.output_nodes_size);
	for (Network::OutputNode& output_node : net.output_nodes) {
		in_stream >> output_node;
	}
	return in_stream;
}

Network Network::createRandom(const unsigned int input_node_count, const unsigned int middle_node_count, const unsigned int output_node_count) {
	srand((unsigned int)time(NULL));

	vector<InputNode> random_input_nodes;
	random_input_nodes.reserve(input_node_count);
	for (unsigned int input_node_index = 0; input_node_index < input_node_count; input_node_index++) {
		vector<float> input_weights(middle_node_count);
		for (unsigned int weight_index = 0; weight_index < middle_node_count; weight_index++) {
			input_weights.at(weight_index) = random(-1, 1);
		}
		random_input_nodes.push_back(InputNode(random(-1, 1), input_weights));
	}

	vector<MiddleNode> random_middle_nodes;
	random_middle_nodes.reserve(middle_node_count);
	for (unsigned int middle_node_index = 0; middle_node_index < middle_node_count; middle_node_index++) {
		const unsigned int weight_count = middle_node_count + output_node_count - 1;
		vector<float> middle_weights(weight_count);
		for (unsigned int weight_index = 0; weight_index < weight_count; weight_index++) {
			middle_weights.at(weight_index) = random(-1, 1);
		}
		random_middle_nodes.push_back(MiddleNode(random(-1, 1), middle_weights, random(-1, 1)));
	}

	vector<OutputNode> random_output_nodes;
	random_output_nodes.reserve(output_node_count);
	for (unsigned int output_node_index = 0; output_node_index < output_node_count; output_node_index++) {
		random_output_nodes.push_back(OutputNode(random(-1, 1), random(-1, 1)));
	}

	return Network(random_input_nodes, random_middle_nodes, random_output_nodes);
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
	return !file_stream.fail();
}

bool Network::randomize() {
	if (thinking) {
		return false;
	}
	srand((unsigned int)time(NULL));
	for (Network::InputNode& input_node : input_nodes) {
		input_node.randomize();
	}
	for (Network::MiddleNode& middle_node : middle_nodes) {
		middle_node.randomize();
	}
	for (Network::OutputNode& output_node : output_nodes) {
		output_node.randomize();
	}
	return true;
}

void Network::baseStep() {
	mutex_lock.lock();

	// input nodes send outputs
	for (InputNode& input_node : input_nodes) {
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			middle_nodes.at(middle_node_index).inputSum += input_node.current_value * input_node.weights.at(middle_node_index);
		}
	}

	// middle nodes send outputs
	for (unsigned int sender_index = 0; sender_index < middle_nodes_size; sender_index++) {
		bool sender_reached = false;
		const MiddleNode& sender = middle_nodes.at(sender_index);
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
	mutex_lock.unlock();
}

bool Network::step() {
	if (thinking) {
		return false;
	}
	baseStep();
	return true;
}

bool Network::beginThinking() {
	if (thinking) {
		return false;
	}
	thinking = true;
	while (thinking) {
		baseStep();
	}
	return true;
}

void Network::endThinking() {
	mutex_lock.lock();
	thinking = false;
	mutex_lock.unlock();
}

float* Network::getOutputs() {
	mutex_lock.lock();
	for (unsigned int i = 0; i < output_nodes_size; i++) {
		outputs.at(i) = output_nodes.at(i).current_value;
	}
	mutex_lock.unlock();
	return outputs.data();
}

void Network::sendInputs(const float inputs[]) {
	mutex_lock.lock();
	for (unsigned int i = 0; i < input_nodes_size; i++) {
		input_nodes.at(i).current_value = sigmoid(inputs[i]);
	}
	mutex_lock.unlock();
}

bool Network::mutate(const float scale) {
	if (thinking) {
		return false;
	}
	srand((unsigned int)time(NULL));
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