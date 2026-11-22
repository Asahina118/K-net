#include "include/knet/endpoint.h"
#include "include/knet/reliable_udp/reliable_udp_socket.h"
#include "tests/extern/doctest.h"

TEST_CASE("socket initialization test")
{
    using namespace knet::reliable_udp;
    using namespace knet;

    auto localEndpointResult = Endpoint::from("127.0.0.1", 21116);
    REQUIRE(localEndpointResult.has_value());
    Endpoint localEndpoint = *localEndpointResult;

    auto peerEndpointResult = Endpoint::from("127.0.0.2", 17754);
    REQUIRE(peerEndpointResult.has_value());
    Endpoint peerEndpoint = *peerEndpointResult;

    auto socketResult = ReliableUDPSocket::establish(localEndpoint, peerEndpoint);
    REQUIRE(socketResult.has_value());
}
