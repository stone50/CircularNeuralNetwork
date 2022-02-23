#include "Network.h"
#include "helpers.h"

using namespace std;

Network::OutputNode::OutputNode() :
	current_value(0),
	bias(0),
	inputSum(0)
{}

Network::OutputNode::OutputNode(const Network::OutputNode& other) :
	current_value(other.current_value),
	bias(other.bias),
	inputSum(other.inputSum)
{}

Network::OutputNode::OutputNode(const float _current_value, const float _bias) :
	current_value(_current_value),
	bias(_bias),
	inputSum(_bias)
{}

Network::OutputNode& Network::OutputNode::operator=(const Network::OutputNode& other) {
	current_value = other.current_value;
	bias = other.bias;
	inputSum = other.inputSum;
	return *this;
}

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

void Network::OutputNode::mutate(const float scale) {
	bias = clamp(bias + random(-1, 1), -1, 1);
	inputSum = bias;
}