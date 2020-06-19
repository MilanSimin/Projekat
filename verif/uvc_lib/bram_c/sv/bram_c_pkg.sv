`ifndef BRAM_C_PKG_SV
`define BRAM_C_PKG_SV

package bram_c_pkg;

`include "uvm_macros.svh"
import uvm_pkg::*;

`include "bram_c_agent_cfg.sv"
`include "bram_c_cfg.sv"
`include "bram_c_item.sv"
`include "bram_c_driver.sv"
`include "bram_c_sequencer.sv"
`include "bram_c_monitor.sv"
`include "bram_c_cov.sv"
`include "bram_c_agent.sv"
`include "bram_c_env.sv"

endpackage : bram_c_pkg

`include "bram_c_if.sv"

`endif // BRAM_C_PKG_SV
