from m5.params import *
from m5.SimObject import SimObject

class MatrixEngine(SimObject):
    type = 'MatrixEngine'
    cxx_header = "matrix_engine.hh"
    cxx_class = "gem5::MatrixEngine"
    
    # NEW: Add a port to connect to the memory system
    mem_side = RequestPort("Memory side port, connects to the memory bus")