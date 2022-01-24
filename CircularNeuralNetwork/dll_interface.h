#pragma once

#include "Network.h"

#pragma warning( push )
#pragma warning( disable : 4251 )
template class NETWORK_API std::vector<float>;
template class NETWORK_API std::vector<Network::InputNode>;
template class NETWORK_API std::vector<Network::MiddleNode>;
template class NETWORK_API std::vector<Network::OutputNode>;
#pragma warning( pop )