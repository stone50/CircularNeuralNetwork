#include "Network.h"
#include "helpers.h"

using namespace std;

Network::InputNode::InputNode() :
	current_value(0),
	weights(vector<float>())
{}

Network::InputNode::InputNode(const Network::InputNode& other) :
	current_value(other.current_value),
	weights(other.weights)
{}

Network::InputNode::InputNode(const float _current_value, const vector<float>& _weights) :
	current_value(_current_value),
	weights(_weights)
{}

Network::InputNode::~InputNode() {
	weights.~vector<float>();
}

Network::InputNode& Network::InputNode::operator=(const Network::InputNode& other) {
	current_value = other.current_value;
	weights = other.weights;
	return *this;
}

ostream& operator<<(ostream& out_stream, const Network::InputNode& node) {
	out_stream << node.current_value << endl << node.weights.size();
	for (const float& weight : node.weights) {
		out_stream << endl << weight;
	}
	return out_stream;
}

istream& operator>>(istream& in_stream, Network::InputNode& node) {
	in_stream >> node.current_value;
	unsigned int weights_size;
	in_stream >> weights_size;
	node.weights = vector<float>(weights_size);
	for (float& weight : node.weights) {
		in_stream >> weight;
	}
	return in_stream;
}

void Network::InputNode::randomize() {
	current_value = random(-1, 1);
	const unsigned int weights_size = (unsigned int)weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		weights.at(i) = random(-1, 1);
	}
}

void Network::InputNode::mutate(const float scale) {
	for (float& weight : weights) {
		weight = clamp(weight + random(-scale, scale), -1, 1);
	}
}