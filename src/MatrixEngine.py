from m5.params import *
from m5.proxy import *
from m5.objects.Device import BasicPioDevice

class MatrixEngine(BasicPioDevice):
    type = 'MatrixEngine'
    cxx_header = "matrix_engine.hh"
    cxx_class = 'gem5::MatrixEngine'
    
    # Add the new Master Port!
    dma = RequestPort("DMA port for memory access")