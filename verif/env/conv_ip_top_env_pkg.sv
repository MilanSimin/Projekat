`ifndef CONV_IP_TOP_ENV_PKG_SV
`define CONV_IP_TOP_ENV_PKG_SV

package conv_ip_top_env_pkg;

`include "uvm_macros.svh"
import uvm_pkg::*;

// import UVC's packages
import axi_lite_pkg::*;
import bram_a_pkg::*;
import bram_b_pkg::*;
import bram_c_pkg::*;

// include env files
`include "conv_ip_top_cfg.sv"
`include "conv_ip_virtual_sequencer.sv"
`include "conv_ip_base_sequence.sv"
`include "conv_ip_vir_sequence.sv"
`include "conv_ip_scoreboard.sv"
`include "conv_ip_coverage_collector.sv"
`include "conv_ip_env_top.sv"

endpackage : conv_ip_top_env_pkg

`endif // CONV_IP_TOP_ENV_PKG_SV
