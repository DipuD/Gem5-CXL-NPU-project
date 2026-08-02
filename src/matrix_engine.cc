#include "matrix_engine.hh"
#include "mem/packet_access.hh"
#include <iostream>

namespace gem5
{

MatrixEngine::MatrixEngine(const MatrixEngineParams &p)
    : BasicPioDevice(p, p.pio_size)
{
    // Initialize the inherited base-class variables directly!
    pioAddr = p.pio_addr;
    pioSize = p.pio_size;

    std::cout << "MatrixEngine Hardware: MMIO configured at physical address 0x" 
              << std::hex << pioAddr << std::dec << "!" << std::endl;
}

AddrRangeList
MatrixEngine::getAddrRanges() const
{
    AddrRangeList ranges;
    ranges.push_back(RangeSize(pioAddr, pioSize));
    return ranges;
}

Tick
MatrixEngine::read(PacketPtr pkt)
{
    std::cout << "MatrixEngine: CPU read from MMIO address 0x" 
              << std::hex << pkt->getAddr() << std::dec << std::endl;
    
    pkt->makeResponse();
    memset(pkt->getPtr<uint8_t>(), 0, pkt->getSize());
    return pioDelay;
}

Tick
MatrixEngine::write(PacketPtr pkt)
{
    std::cout << "MatrixEngine: CPU wrote to MMIO address 0x" 
              << std::hex << pkt->getAddr() << std::dec << std::endl;

    if (pkt->getSize() == 4) {
        uint32_t data = pkt->getLE<uint32_t>();
        std::cout << "MatrixEngine: Received Command/Data: " << data << std::endl;
    }
    
    pkt->makeResponse();
    return pioDelay;
}

} // namespace gem5