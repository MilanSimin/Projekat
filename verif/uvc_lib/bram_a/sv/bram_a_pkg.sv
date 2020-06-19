`ifndef BRAM_A_PKG_SV
`define BRAM_A_PKG_SV

package bram_a_pkg;

`include "uvm_macros.svh"
import uvm_pkg::*;

`include "bram_a_agent_cfg.sv"
`include "bram_a_cfg.sv"
`include "bram_a_item.sv"
`include "bram_a_driver.sv"
`include "bram_a_sequencer.sv"
`include "bram_a_monitor.sv"
`include "bram_a_cov.sv"
`include "bram_a_agent.sv"
`include "bram_a_env.sv"
`include "bram_a_basic_seq.sv"

endpackage : bram_a_pkg

`include "bram_a_if.sv"

`endif // BRAM_A_PKG_SV
