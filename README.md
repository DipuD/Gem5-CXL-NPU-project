# Energy-Aware CXL-Attached NPU for AI Inference

## Overview

This repository contains a gem5-based computer architecture research project. It models a custom Neural Processing Unit (NPU) accelerator designed for Blocked Matrix Multiplications, attached to a host CPU via a simulated Compute Express Link (CXL) memory interconnect.

The goal of this project is to explore the microarchitectural trade-offs between processing AI workloads on a general-purpose Out-of-Order (O3) CPU versus offloading them to a CXL-attached hardware accelerator, focusing on the "Memory Wall" and energy consumption.

## Current Milestones Achieved

* **Milestone 1: Baseline Profiling**
  * Modeled a 3GHz x86 O3CPU with a Private L1/L2 cache hierarchy.
  * Executed a statically compiled C-based Matrix Multiplication workload.
  * Successfully demonstrated the "Memory Wall" effect: by scaling the working set to 768KB, L1 cache misses surged to 11.8 Million, and IPC degraded severely from 2.22 down to 0.80, mathematically proving the need for a dedicated NPU.

* **Milestone 2: The NPU SimObject**
  * Developed the core C++ framework for the custom hardware accelerator (MatrixEngine).
  * Created the Python-to-C++ SimObject interface and successfully compiled the component into the gem5 binary using scons.
  * Instantiated the hardware block on the simulated motherboard alongside the CPU.

* **Milestone 3: The Interconnect**
  * Engineered a custom RequestPort (Memory Side Port) inside the MatrixEngine C++ class.
  * Physically wired the NPU's memory port to the main system coherent crossbar (memory bus) via Python configuration scripts.
  * Booted the fully integrated system, establishing the foundation for Direct Memory Access (DMA) over a simulated CXL link. 

* **Milestone 4: Memory-Mapped I/O (MMIO) and Bare-Metal Driver**
  * Refactored the accelerator to inherit from `BasicPioDevice`, allocating a dedicated physical address space (`0x80000000`).
  * Conquered complex Syscall Emulation (SE) mode memory mapping by structuring the Python instantiation lifecycle to bypass gem5 "orphan" tree restrictions.
  * Wrote a custom C-level device driver inside the matrix multiplication workload to dispatch wakeup commands directly over the system memory bus to the custom silicon.

## Directory Structure

* `configs/` - Python topology scripts for gem5 (`baseline.py`).
* `src/` - C++ SimObject definitions for the custom NPU.
* `workloads/` - C-based matrix multiplication algorithms and NPU driver code.

## How to Run the Baseline

Ensure gem5 is compiled, then run the simulation from your gem5 root directory:

```bash
build/X86/gem5.opt /path/to/gem5-cxl-npu-project/configs/baseline.py