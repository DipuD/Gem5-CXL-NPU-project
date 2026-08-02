#ifndef __MATRIX_ENGINE_HH__
#define __MATRIX_ENGINE_HH__

#include "dev/io_device.hh"
#include "params/MatrixEngine.hh"

namespace gem5
{

class MatrixEngine : public BasicPioDevice
{
  public:
    MatrixEngine(const MatrixEngineParams &p);

    // Tell the memory bus our exact MMIO location
    AddrRangeList getAddrRanges() const override;

    Tick read(PacketPtr pkt) override;
    Tick write(PacketPtr pkt) override;
};

} // namespace gem5

#endif // __MATRIX_ENGINE_HH__