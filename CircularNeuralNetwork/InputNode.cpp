#include "pch.h"
#include <mutex>
#include "InputNode.h"
#include "helpers.h"

using namespace std;

mutex input_node_thread_lock;

InputNode::InputNode() :
	current_value(0),
	weights(vector<float>())
{}

InputNode::InputNode(float _current_value, vector<float> _weights) :
	current_value(_current_value),
	weights(_weights)
{}

ostream& operator<<(ostream& out_stream, const InputNode& node) {
	unsigned int weights_size = node.weights.size();
	out_stream << node.current_value << endl << weights_size << endl;
	for (unsigned int i = 0; i < weights_size - 1; i++) {
		out_stream << node.weights.at(i) << endl;
	}
	out_stream << node.weights.at(weights_size - 1);
	return out_stream;
}

istream& operator>>(istream& in_stream, InputNode& node) {
	input_node_thread_lock.lock();
	node = InputNode();
	in_stream >> node.current_value;
	input_node_thread_lock.unlock();
	unsigned int weights_size;
	in_stream >> weights_size;
	for (unsigned int i = 0; i < weights_size; i++) {
		float weight;
		in_stream >> weight;
		input_node_thread_lock.lock();
		node.weights.push_back(weight);
		input_node_thread_lock.unlock();
	}
	return in_stream;
}

void InputNode::setCurrentValue(float new_value) {
	input_node_thread_lock.lock();
	current_value = sigmoid(new_value);
	input_node_thread_lock.unlock();
}

void InputNode::randomize() {
	input_node_thread_lock.lock();
	current_value = random(-1, 1);
	input_node_thread_lock.unlock();
	unsigned int const weights_size = weights.size();
	for (unsigned int i = 0; i < weights_size; i++) {
		input_node_thread_lock.lock();
		weights.at(i) = random(-1, 1);
		input_node_thread_lock.unlock();
	}
}

float InputNode::getWeightAt(int index) {
	return weights.at(index);
}

float InputNode::getCurrentValue() {
	return current_value;
}

void InputNode::mutate(float scale) {
	for (float& weight : weights) {
		input_node_thread_lock.lock();
		weight = clamp(weight + random(-scale, scale), -1, 1);
		input_node_thread_lock.unlock();
	}
}