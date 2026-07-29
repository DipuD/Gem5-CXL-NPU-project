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

# 3.5 Setup our Custom Hardware
npu = MatrixEngine()

# 4. Setup the Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# Attach the NPU to the board so the simulator knows it exists
board.npu = npu 

board.npu.mem_side = cache_hierarchy.membus.cpu_side_ports

# 5. Set the Workload (Our compiled C program)
binary_path = "/home/dipu3d/gem5-cxl-npu-project/workloads/mmult"
board.set_se_binary_workload(BinaryResource(local_path=binary_path))

# 6. Run the Simulation
print("Running Baseline O3 Simulation...")
simulator = Simulator(board=board)
simulator.run()
print(f"Simulation finished at tick {simulator.get_current_tick()}!")
