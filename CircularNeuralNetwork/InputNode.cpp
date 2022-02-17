#include "pch.h"
#include "Network.h"

using namespace std;

Network::InputNode::InputNode() :
	current_value(0),
	weights(vector<float>())
{}

Network::InputNode::~InputNode() {
	weights.~vector<float>();
}

Network::InputNode::InputNode(float _current_value, const vector<float>& _weights) :
	current_value(_current_value),
	weights(_weights)
{}

ostream& operator<<(ostream& out_stream, const Network::InputNode& node) {
	unsigned int weights_size = (unsigned int)node.weights.size();
	out_stream << node.current_value << endl << weights_size << endl;
	for (unsigned int i = 0; i < weights_size - 1; i++) {
		out_stream << node.weights.at(i) << endl;
	}
	out_stream << node.weights.at(weights_size - 1);
	return out_stream;
}

istream& operator>>(istream& in_stream, Network::InputNode& node) {
	in_stream >> node.current_value;
	unsigned int weights_size;
	in_stream >> weights_size;
	for (unsigned int i = 0; i < weights_size; i++) {
		float weight;
		in_stream >> weight;
		node.weights.push_back(weight);
	}
	return in_stream;
}

void Network::InputNode::randomize() {
	current_value = random(-1, 1);
	unsigned int const weights_size = (unsigned int)weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		weights.at(i) = random(-1, 1);
	}
}

void Network::InputNode::mutate(float scale) {
	for (float& weight : weights) {
		weight = clamp(weight + random(-scale, scale), -1, 1);
	}
}