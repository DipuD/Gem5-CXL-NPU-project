import m5
from m5.objects import *
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.single_channel import SingleChannelDDR4_2400
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.isas import ISA

from m5.objects.MatrixEngine import MatrixEngine

# 1. Setup the Cache Hierarchy
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size="32kB",
    l1i_size="32kB",
    l2_size="256kB"
)

# 2. Setup the Memory System
memory = SingleChannelDDR4_2400(size="2GB")

# 3. Setup the Processor (Out of Order Core)
processor = SimpleProcessor(
    cpu_type=CPUTypes.O3,
    num_cores=1,
    isa=ISA.X86
)
npu = MatrixEngine(pio_addr=0x80000000)

# 4. Setup the Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)
board.npu = npu
board.npu.pio = cache_hierarchy.membus.mem_side_ports
board.npu.dma = cache_hierarchy.membus.cpu_side_ports 

# 5. Setup the Workload (Using the proper gem5 stdlib method!)
binary = BinaryResource("/home/dipu3d/gem5-cxl-npu-project/workloads/mmult")
board.set_se_binary_workload(binary)

# 6. Initialize Simulator
simulator = Simulator(board=board)

# 7. Force C++ Instantiation FIRST
simulator._instantiate()

# 8. Safely extract the Process and Map MMIO
process = board.get_processor().get_cores()[0].core.workload[0]
process.map(0x80000000, 0x80000000, 0x1000, False)

# 9. Run the Simulation
print("Running Baseline O3 Simulation...")
simulator.run()
print(f"Simulation finished at tick {simulator.get_current_tick()}!")