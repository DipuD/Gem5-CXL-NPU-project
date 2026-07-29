#ifndef __MATRIX_ENGINE_HH__
#define __MATRIX_ENGINE_HH__

#include "params/MatrixEngine.hh"
#include "sim/sim_object.hh"
#include "mem/port.hh" // NEW: Required for hardware ports

namespace gem5
{

class MatrixEngine : public SimObject
{
  private:
    // NEW: Define our custom memory port
    class MemPort : public RequestPort
    {
      private:
        MatrixEngine *owner;
      public:
        MemPort(const std::string& name, MatrixEngine *owner) :
            RequestPort(name, owner), owner(owner) {}
      protected:
        // Required overrides for any RequestPort
        bool recvTimingResp(PacketPtr pkt) override;
        void recvReqRetry() override;
    };

    // Instantiate the port as a physical part of the NPU
    MemPort memPort;

  public:
    MatrixEngine(const MatrixEngineParams &p);
    void startup() override;
    
    // NEW: Function that allows the Python script to plug wires into this port
    Port &getPort(const std::string &if_name, PortID idx = InvalidPortID) override;
};

} // namespace gem5

#endif // __MATRIX_ENGINE_HH__
