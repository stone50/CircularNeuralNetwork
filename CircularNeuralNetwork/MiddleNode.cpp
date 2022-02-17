#include "pch.h"
#include "Network.h"

using namespace std;

Network::MiddleNode::MiddleNode() :
	current_value(0),
	weights(vector<float>()),
	bias(0),
	inputSum(0)
{}

Network::MiddleNode::~MiddleNode() {
	weights.~vector<float>();
}

Network::MiddleNode::MiddleNode(float _current_value, const vector<float>& _weights, float _bias) :
	current_value(_current_value),
	weights(_weights),
	bias(_bias),
	inputSum(_bias)
{}

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
	for (unsigned int i = 0; i < weights_size; i++) {
		float weight;
		in_stream >> weight;
		node.weights.push_back(weight);
	}
	in_stream >> node.bias;
	return in_stream;
}

void Network::MiddleNode::randomize() {
	current_value = random(-1, 1);
	unsigned int weights_size = (unsigned int)weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		weights.at(i) = random(-1, 1);
	}
	bias = random(-1, 1);
	inputSum = bias;
}

void Network::MiddleNode::mutate(float scale) {
	for (float& weight : weights) {
		weight = clamp(weight + random(-scale, scale), -1, 1);
	}
	bias = clamp(bias + random(-1, 1), -1, 1);
	inputSum = bias;
}