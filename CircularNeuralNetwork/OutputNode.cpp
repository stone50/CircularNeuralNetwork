#include "pch.h"
#include "Network.h"

using namespace std;

Network::OutputNode::OutputNode() :
	current_value(0),
	bias(0),
	inputSum(0)
{}

Network::OutputNode::OutputNode(float _current_value, float _bias) :
	current_value(_current_value),
	bias(_bias),
	inputSum(_bias)
{}

ostream& operator<<(ostream& out_stream, const Network::OutputNode& node) {
	out_stream << node.current_value << endl << node.bias;
	return out_stream;
}

istream& operator>>(istream& in_stream, Network::OutputNode& node) {
	in_stream >> node.current_value;
	in_stream >> node.bias;
	return in_stream;
}

void Network::OutputNode::randomize() {
	current_value = random(-1, 1);
	bias = random(-1, 1);
	inputSum = bias;
}

void Network::OutputNode::mutate(float scale) {
	bias = clamp(bias + random(-1, 1), -1, 1);
	inputSum = bias;
}