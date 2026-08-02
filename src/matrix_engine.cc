#include "matrix_engine.hh"
#include "mem/packet_access.hh"
#include <iostream>

namespace gem5
{

// Hardcode 0x1000 for the PioSize since it wasn't defined in the Python params
MatrixEngine::MatrixEngine(const MatrixEngineParams &p)
    : BasicPioDevice(p, 0x1000),
      dmaPort(name() + ".dma", this),
      state(IDLE),
      regCmdStatus(0), regAddrA(0), regAddrB(0), regAddrC(0), regSizeN(0),
      tickEvent([this]{ processTick(); }, name() + ".tickEvent")
{
    pioAddr = p.pio_addr;
    pioSize = 0x1000;
}

Port &
MatrixEngine::getPort(const std::string &if_name, PortID idx)
{
    if (if_name == "dma")
        return dmaPort;
    return BasicPioDevice::getPort(if_name, idx);
}

void 
MatrixEngine::issueDMARead(Addr addr, int size) 
{
    // Fix: Explicitly define the <Request> template type
    RequestPtr req = std::make_shared<Request>(addr, size, 0, 0);
    
    PacketPtr pkt = Packet::createRead(req);
    pkt->allocate(); 

    std::cout << "NPU [DMA]: Firing ReadReq for Address 0x" << std::hex << addr << std::dec << std::endl;
    dmaPort.sendTimingReq(pkt);
}

bool 
MatrixEngine::handleResponse(PacketPtr pkt) 
{
    std::cout << "NPU [DMA]: ReadResp received! Payload size: " << pkt->getSize() << " bytes." << std::endl;
    delete pkt; // Cleanup to prevent memory leaks
    return true;
}

void
MatrixEngine::processTick()
{
    switch(state) {
        case FETCHING:
            std::cout << "NPU [State]: FETCHING matrices from RAM..." << std::endl;
            
            // Fetch the first 64 bytes (1 cache line) of Matrix A!
            issueDMARead(regAddrA, 64);

            state = COMPUTING;
            schedule(tickEvent, curTick() + clockPeriod() * (regSizeN * regSizeN * 2));
            break;
            
        case COMPUTING:
            std::cout << "NPU [State]: COMPUTING MAC operations..." << std::endl;
            state = STORING;
            schedule(tickEvent, curTick() + clockPeriod() * (regSizeN * regSizeN * regSizeN));
            break;
            
        case STORING:
            std::cout << "NPU [State]: STORING results back to RAM..." << std::endl;
            state = DONE;
            schedule(tickEvent, curTick() + clockPeriod() * (regSizeN * regSizeN));
            break;
            
        case DONE:
            std::cout << "NPU [State]: DONE. Clearing CMD register and alerting CPU." << std::endl;
            regCmdStatus = 0x00; 
            state = IDLE;
            break;
            
        case IDLE:
            break;
    }
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
    Addr offset = pkt->getAddr() - pioAddr;
    
    if (offset == 0x00) {
        pkt->setLE<uint32_t>(regCmdStatus);
    } else {
        memset(pkt->getPtr<uint8_t>(), 0, pkt->getSize());
    }
    
    pkt->makeResponse();
    return pioDelay;
}

Tick
MatrixEngine::write(PacketPtr pkt)
{
    Addr offset = pkt->getAddr() - pioAddr;
    uint32_t data = pkt->getLE<uint32_t>();

    switch(offset) {
        case 0x00:
            regCmdStatus = data;
            if (regCmdStatus == 1 && state == IDLE) {
                state = FETCHING;
                schedule(tickEvent, curTick() + clockPeriod());
            }
            break;
        case 0x04: regAddrA = data; break;
        case 0x08: regAddrB = data; break;
        case 0x0C: regAddrC = data; break;
        case 0x10: regSizeN = data; break;
    }
    
    pkt->makeResponse();
    return pioDelay;
}

} // namespace gem5