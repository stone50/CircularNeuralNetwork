# Circular Neural Network
## A neural network library designed for asynchronous behavior.

This library is for the use of a special kind of neural network. In this neural network, all hidden nodes (middle nodes) are connected. This produces a few effects that make it different from a feed-forward neural network. First, given a complex enough network, outputs produced from a set of inputs will not necessarily be the same as outputs produced from the same set of inputs after any number of iterations. Second, the network does not require inputs to produce outputs. These two effects combined, provide for a network whose decisions can change based on past decisions. While this kind of network does not exactly imitate the functions of a brain, it is a slightly more accurate representation of how brains think.

A circular neural network can be used either with a single thread or multiple threads. Here are examples of how to use a network both ways.

Single Thread
```
#include <iostream>
#inlcude <CNeuralNet.h>

using namespace std;

int main() {
  srand((unsigned int)time(NULL));
  
  Network net = Network::createRandom(1, 10, 1);
  float inputs[] = { 3.14 };
  net.sendInputs(inputs);
  net.step();
  
  cout << "Output: " << net.getOutputs()[0] << endl;
  return 0;
}
```
Multiple Threads
```
#include <iostream>
#include <thread>
#include <Windows.h>
#include <CNeuralNet.h>

using namespace std;

int main() {
  srand((unsigned int)time(NULL));
  
  Network net = Network::createRandom(1, 10, 1);
  thread thinking_thread([&]() {net.beginThinking()});
  
  for (int i = 0; i < 10; i++) {
    float inputs[] = { 3.14 };
    net.sendInputs(inputs);
    Sleep(10);
    cout << "Output: " << net.getOutputs()[0] << endl;
  }
  
  net.endThinking();
  thinking_thread.join();
    
  return 0;
}
```
