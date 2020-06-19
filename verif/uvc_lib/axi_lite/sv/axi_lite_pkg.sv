`ifndef AXI_LITE_PKG_SV
`define AXI_LITE_PKG_SV

package axi_lite_pkg;

`include "uvm_macros.svh"
import uvm_pkg::*;

`include "axi_lite_common.sv"
`include "axi_lite_agent_cfg.sv"
`include "axi_lite_cfg.sv"
`include "axi_lite_item.sv"
`include "axi_lite_driver.sv"
`include "axi_lite_sequencer.sv"
`include "axi_lite_monitor.sv"
`include "axi_lite_cov.sv"
`include "axi_lite_agent.sv"
`include "axi_lite_env.sv"
`include "axi_lite_seq_lib.sv"

endpackage : axi_lite_pkg

`include "axi_lite_if.sv"

`endif // AXI_LITE_PKG_SV
