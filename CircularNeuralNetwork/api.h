#pragma once

#include <vector>
#include "InputNode.h"
#include "MiddleNode.h"
#include "OutputNode.h"

#ifdef CIRCULARNEURALNETWORK_EXPORTS
#define NETWORK_API __declspec(dllexport)
#else
#define NETWORK_API __declspec(dllimport)
#endif

#pragma warning( push )
#pragma warning( disable : 4251 )
template class NETWORK_API std::vector<float>;
template class NETWORK_API std::vector<InputNode>;
template class NETWORK_API std::vector<MiddleNode>;
template class NETWORK_API std::vector<OutputNode>;
#pragma warning( pop )