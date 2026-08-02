from m5.params import *
from m5.proxy import *
from m5.objects.Device import BasicPioDevice

class MatrixEngine(BasicPioDevice):
    type = 'MatrixEngine'
    cxx_header = "matrix_engine.hh"
    cxx_class = "gem5::MatrixEngine"

    # The physical address range this device responds to
    pio_addr = Param.Addr(0x80000000, "MMIO base address")
    pio_size = Param.Addr(0x1000, "MMIO size")