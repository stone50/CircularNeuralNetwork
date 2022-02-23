#include "Network.h"
#include "helpers.h"

using namespace std;

Network::MiddleNode::MiddleNode() :
	current_value(0),
	weights(vector<float>()),
	bias(0),
	inputSum(0)
{}

Network::MiddleNode::MiddleNode(const Network::MiddleNode& other) :
	current_value(other.current_value),
	weights(other.weights),
	bias(other.bias),
	inputSum(other.inputSum)
{}

Network::MiddleNode::MiddleNode(const float _current_value, const vector<float>& _weights, const float _bias) :
	current_value(_current_value),
	weights(_weights),
	bias(_bias),
	inputSum(_bias)
{}

Network::MiddleNode::~MiddleNode() {
	weights.~vector<float>();
}

Network::MiddleNode& Network::MiddleNode::operator=(const MiddleNode& other) {
	current_value = other.current_value;
	weights = other.weights;
	bias = other.bias;
	inputSum = other.inputSum;
	return *this;
}

ostream& operator<<(ostream& out_stream, const Network::MiddleNode& node) {
	out_stream << node.current_value << endl << node.weights.size() << endl;
	for (const float& weight : node.weights) {
		out_stream << weight << endl;
	}
	out_stream << node.bias;
	return out_stream;
}

istream& operator>>(istream& in_stream, Network::MiddleNode& node) {
	in_stream >> node.current_value;
	unsigned int weights_size;
	in_stream >> weights_size;
	node.weights = vector<float>(weights_size);
	for (float& weight : node.weights) {
		in_stream >> weight;
	}
	in_stream >> node.bias;
	return in_stream;
}

void Network::MiddleNode::randomize() {
	current_value = random(-1, 1);
	const unsigned int weights_size = (unsigned int)weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		weights.at(i) = random(-1, 1);
	}
	bias = random(-1, 1);
	inputSum = bias;
}

void Network::MiddleNode::mutate(const float scale) {
	for (float& weight : weights) {
		weight = clamp(weight + random(-scale, scale), -1, 1);
	}
	bias = clamp(bias + random(-1, 1), -1, 1);
	inputSum = bias;
}