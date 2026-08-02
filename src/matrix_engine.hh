#ifndef __MATRIX_ENGINE_HH__
#define __MATRIX_ENGINE_HH__

#include "dev/io_device.hh"
#include "params/MatrixEngine.hh"
#include "sim/eventq.hh" 

namespace gem5
{

class MatrixEngine : public BasicPioDevice
{
  private:
    // 1. Define the custom DMA Request Port (Warning Fixed!)
    class DMAPort : public RequestPort {
      private:
        MatrixEngine *owner;
      public:
        DMAPort(const std::string& name, MatrixEngine *owner) :
            RequestPort(name), owner(owner) {} // Removed 'owner' from RequestPort()
      protected:
        bool recvTimingResp(PacketPtr pkt) override {
            return owner->handleResponse(pkt);
        }
        void recvReqRetry() override {} 
    };

    DMAPort dmaPort;

    enum NPUState { IDLE, FETCHING, COMPUTING, STORING, DONE };
    NPUState state;

    uint32_t regCmdStatus;
    uint32_t regAddrA, regAddrB, regAddrC, regSizeN;

    EventFunctionWrapper tickEvent;
    void processTick();

  public:
    MatrixEngine(const MatrixEngineParams &p);

    Port &getPort(const std::string &if_name, PortID idx=InvalidPortID) override;
    
    void issueDMARead(Addr addr, int size);
    bool handleResponse(PacketPtr pkt);

    AddrRangeList getAddrRanges() const override;
    Tick read(PacketPtr pkt) override;
    Tick write(PacketPtr pkt) override;
};

} // namespace gem5

#endif // __MATRIX_ENGINE_HH__