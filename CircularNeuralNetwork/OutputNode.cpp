#include "pch.h"
#include <mutex>
#include "Network.h"
#include "helpers.h"

using namespace std;

mutex output_node_thread_lock;

Network::OutputNode::OutputNode() :
	current_value(0),
	inputs(vector<float>()),
	bias(0)
{}

Network::OutputNode::OutputNode(float _current_value, float _bias) :
	current_value(_current_value),
	inputs(vector<float>()),
	bias(_bias)
{}

ostream& operator<<(ostream& out_stream, const Network::OutputNode& node) {
	out_stream << node.current_value << endl << node.bias;
	return out_stream;
}

istream& operator>>(istream& in_stream, Network::OutputNode& node) {
	output_node_thread_lock.lock();
	node = Network::OutputNode();
	in_stream >> node.current_value;
	in_stream >> node.bias;
	output_node_thread_lock.unlock();
	return in_stream;
}

void Network::OutputNode::calcCurrentValue() {
	float sum = bias;
	for (float input : inputs) {
		sum += input;
	}
	output_node_thread_lock.lock();
	current_value = sigmoid(sum);
	inputs.clear();
	output_node_thread_lock.unlock();
}

void Network::OutputNode::randomize() {
	output_node_thread_lock.lock();
	current_value = random(-1, 1);
	inputs = vector<float>();
	bias = random(-1, 1);
	output_node_thread_lock.unlock();
}

void Network::OutputNode::OutputNode::addInput(float input) {
	output_node_thread_lock.lock();
	inputs.push_back(input);
	output_node_thread_lock.unlock();
}

float Network::OutputNode::getCurrentValue() {
	return current_value;
}

void Network::OutputNode::mutate(float scale) {
	output_node_thread_lock.lock();
	bias = clamp(bias + random(-1, 1), -1, 1);
	output_node_thread_lock.unlock();
}