#include "matrix_engine.hh"
#include <iostream>

namespace gem5
{

// NEW: Initialize memPort in the constructor list
MatrixEngine::MatrixEngine(const MatrixEngineParams &p) : 
    SimObject(p),
    memPort(p.name + ".mem_side", this)
{
    std::cout << "MatrixEngine Hardware: Powered On and Initialized!" << std::endl;
}

void MatrixEngine::startup()
{
    std::cout << "MatrixEngine Hardware: Simulation Started!" << std::endl;
}

// NEW: Expose the port to the gem5 system
Port &MatrixEngine::getPort(const std::string &if_name, PortID idx)
{
    if (if_name == "mem_side") {
        return memPort;
    } else {
        return SimObject::getPort(if_name, idx);
    }
}

// NEW: Handle incoming memory data (Dummy implementation for now)
bool MatrixEngine::MemPort::recvTimingResp(PacketPtr pkt)
{
    // The RAM has sent us data!
    return true; 
}

// NEW: Handle bus retry (Dummy implementation for now)
void MatrixEngine::MemPort::recvReqRetry()
{
    // If the memory bus is busy, it tells us to try again later.
}

} // namespace gem5