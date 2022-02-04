#include "pch.h"
#include <mutex>
#include "Network.h"

using namespace std;

mutex middle_node_thread_lock;

Network::MiddleNode::MiddleNode() :
	current_value(0),
	inputs(vector<float>()),
	weights(vector<float>()),
	bias(0)
{}

Network::MiddleNode::MiddleNode(float _current_value, vector<float> _weights, float _bias) :
	current_value(_current_value),
	inputs(vector<float>()),
	weights(_weights),
	bias(_bias)
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
	middle_node_thread_lock.lock();
	node = Network::MiddleNode();
	in_stream >> node.current_value;
	middle_node_thread_lock.unlock();
	unsigned int weights_size;
	in_stream >> weights_size;
	for (unsigned int i = 0; i < weights_size; i++) {
		float weight;
		in_stream >> weight;
		middle_node_thread_lock.lock();
		node.weights.push_back(weight);
		middle_node_thread_lock.unlock();
	}
	middle_node_thread_lock.lock();
	in_stream >> node.bias;
	middle_node_thread_lock.unlock();
	return in_stream;
}

void Network::MiddleNode::calcCurrentValue() {
	float sum = bias;
	for (float input : inputs) {
		sum += input;
	}
	middle_node_thread_lock.lock();
	current_value = sigmoid(sum);
	inputs.clear();
	middle_node_thread_lock.unlock();
}

void Network::MiddleNode::randomize() {
	middle_node_thread_lock.lock();
	current_value = random(-1, 1);
	inputs = vector<float>();
	middle_node_thread_lock.unlock();
	unsigned int weights_size = weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		middle_node_thread_lock.lock();
		weights.at(i) = random(-1, 1);
		middle_node_thread_lock.unlock();
	}
	middle_node_thread_lock.lock();
	bias = random(-1, 1);
	middle_node_thread_lock.unlock();
}

void Network::MiddleNode::addInput(float input) {
	middle_node_thread_lock.lock();
	inputs.push_back(input);
	middle_node_thread_lock.unlock();
}

float Network::MiddleNode::getWeightAt(int index) {
	return weights.at(index);
}

float Network::MiddleNode::getCurrentValue() {
	return current_value;
}

void Network::MiddleNode::mutate(float scale) {
	for (float& weight : weights) {
		middle_node_thread_lock.lock();
		weight = clamp(weight + random(-scale, scale), -1, 1);
		middle_node_thread_lock.unlock();
	}
	middle_node_thread_lock.lock();
	bias = clamp(bias + random(-1, 1), -1, 1);
	middle_node_thread_lock.unlock();
}