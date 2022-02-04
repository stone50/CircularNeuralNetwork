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
	input_nodes_size(0), middle_nodes_size(0),
	output_nodes_size(0), outputs(vector<float>()),
	thinking(false)
{}

Network::Network(vector<InputNode> _input_nodes, vector<MiddleNode> _middle_nodes, vector<OutputNode> _output_nodes) :
	input_nodes(_input_nodes),
	middle_nodes(_middle_nodes),
	output_nodes(_output_nodes),
	input_nodes_size(_input_nodes.size()),
	middle_nodes_size(_middle_nodes.size()),
	output_nodes_size(_output_nodes.size()),
	outputs(vector<float>()),
	thinking(false)
{
	for (OutputNode& output_node : output_nodes) {
		network_thread_lock.lock();
		outputs.push_back(output_node.getCurrentValue());
		network_thread_lock.unlock();
	}
}

Network Network::createRandom(unsigned int input_node_count, unsigned int middle_node_count, unsigned int output_node_count) {
	vector<InputNode> random_input_nodes = vector<InputNode>();
	for (unsigned int input_node_index = 0; input_node_index < input_node_count; input_node_index++) {
		vector<float> input_weights = vector<float>();
		for (unsigned int weight_index = 0; weight_index < middle_node_count; weight_index++) {
			input_weights.push_back(random(-1, 1));
		}

		random_input_nodes.push_back(InputNode(random(-1, 1), input_weights));
	}

	vector<MiddleNode> random_middle_nodes = vector<MiddleNode>();
	for (unsigned int middle_node_index = 0; middle_node_index < middle_node_count; middle_node_index++) {
		vector<float> middle_weights = vector<float>();
		unsigned int weight_count = middle_node_count + output_node_count - 1;
		for (unsigned int weight_index = 0; weight_index < weight_count; weight_index++) {
			middle_weights.push_back(random(-1, 1));
		}

		random_middle_nodes.push_back(MiddleNode(random(-1, 1), middle_weights, random(-1, 1)));
	}

	vector<OutputNode> random_output_nodes = vector<OutputNode>();
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
	unsigned int outputs_size = net.outputs.size();
	for (unsigned int i = 0; i < outputs_size - 1; i++) {
		out_stream << net.outputs.at(i) << endl;
	}
	return out_stream;
}

istream& operator>>(istream& in_stream, Network& net) {
	network_thread_lock.lock();
	net = Network();
	in_stream >> net.input_nodes_size;
	network_thread_lock.unlock();
	for (unsigned int i = 0; i < net.input_nodes_size; i++) {
		Network::InputNode input_node;
		in_stream >> input_node;
		network_thread_lock.lock();
		net.input_nodes.push_back(input_node);
		network_thread_lock.unlock();
	}
	network_thread_lock.lock();
	in_stream >> net.middle_nodes_size;
	network_thread_lock.unlock();
	for (unsigned int i = 0; i < net.middle_nodes_size; i++) {
		Network::MiddleNode middle_node;
		in_stream >> middle_node;
		network_thread_lock.lock();
		net.middle_nodes.push_back(middle_node);
		network_thread_lock.unlock();
	}
	network_thread_lock.lock();
	in_stream >> net.output_nodes_size;
	network_thread_lock.unlock();
	for (unsigned int i = 0; i < net.output_nodes_size; i++) {
		Network::OutputNode output_node;
		in_stream >> output_node;
		network_thread_lock.lock();
		net.output_nodes.push_back(output_node);
		network_thread_lock.unlock();
	}
	for (Network::OutputNode& output_node : net.output_nodes) {
		network_thread_lock.lock();
		net.outputs.push_back(output_node.getCurrentValue());
		network_thread_lock.unlock();
	}
	return in_stream;
}

void Network::save(string filename) {
	ofstream file_stream(filename, ios::trunc);
	file_stream << *this;
	file_stream.close();
}

Network Network::load(string filename) {
	ifstream file_stream(filename);
	Network net;
	file_stream >> net;
	file_stream.close();
	return net;
}

void Network::randomize() {
	for (unsigned int i = 0; i < input_nodes_size; i++) {
		input_nodes.at(i).randomize();
	}
	for (unsigned int i = 0; i < middle_nodes_size; i++) {
		middle_nodes.at(i).randomize();
	}
	for (unsigned int i = 0; i < output_nodes_size; i++) {
		output_nodes.at(i).randomize();
	}
}

void Network::step() {
	// step() should not be called while the network is already thinking
	if (thinking) {
		return;
	}

	// input nodes send outputs
	for (InputNode& input_node : input_nodes) {
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			middle_nodes.at(middle_node_index).addInput(input_node.getCurrentValue() * input_node.getWeightAt(middle_node_index));
		}
		input_node.setCurrentValue(0);
	}

	// middle nodes send outputs
	for (unsigned int sender_index = 0; sender_index < middle_nodes_size; sender_index++) {
		bool sender_reached = false;
		MiddleNode sender = middle_nodes.at(sender_index);
		float sender_current_value = sender.getCurrentValue();
		for (unsigned int middle_node_index = 0; middle_node_index < middle_nodes_size; middle_node_index++) {
			if (sender_index != middle_node_index) {
				middle_nodes.at(middle_node_index).addInput(sender_current_value * sender.getWeightAt(middle_node_index - sender_reached));
			}
			else {
				sender_reached = true;
			}
		}
		for (unsigned int output_node_index = 0; output_node_index < output_nodes_size; output_node_index++) {
			output_nodes.at(output_node_index).addInput(sender_current_value * sender.getWeightAt(output_node_index + middle_nodes_size - 1));
		}
	}

	// middle nodes calculate current value
	for (MiddleNode& middle_node : middle_nodes) {
		middle_node.calcCurrentValue();
	}

	// output nodes calculate current value
	for (OutputNode& output_node : output_nodes) {
		output_node.calcCurrentValue();
	}

	// replace elements of outputs using output_nodes
	for (unsigned int output_index = 0; output_index < output_nodes_size; output_index++) {
		network_thread_lock.lock();
		outputs.at(output_index) = output_nodes.at(output_index).getCurrentValue();
		network_thread_lock.unlock();
	}
}

void Network::beginThinking() {
	network_thread_lock.lock();
	thinking = true;
	network_thread_lock.unlock();
	while (thinking) {
		step();
	}
}

void Network::endThinking() {
	network_thread_lock.lock();
	thinking = false;
	network_thread_lock.unlock();
}

float Network::getOutputAt(int index) {
	return outputs.at(index);
}

void Network::sendInputs(vector<float> inputs) {
	for (unsigned int i = 0; i < input_nodes_size; i++) {
		input_nodes.at(i).setCurrentValue(inputs.at(i));
	}
}

void Network::mutate(float scale) {
	for (InputNode& input_node : input_nodes) {
		input_node.mutate(scale);
	}
	for (MiddleNode& middle_node : middle_nodes) {
		middle_node.mutate(scale);
	}
	for (OutputNode& output_node : output_nodes) {
		output_node.mutate(scale);
	}
}