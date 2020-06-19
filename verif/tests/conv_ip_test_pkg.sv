`ifndef CONV_IP_TEST_PKG_SV
`define CONV_IP_TEST_PKG_SV

package conv_ip_test_pkg;

`include "uvm_macros.svh"
import uvm_pkg::*;

// import UVC's packages
import bram_a_pkg::*;
import bram_b_pkg::*;
import bram_c_pkg::*;
import axi_lite_pkg::*;

// import env package
import conv_ip_top_env_pkg::*;

// include tests
`include "test_conv_ip_base.sv"
`include "test_conv_ip_read_reset_values.sv" //reset values
`include "test_conv_ip_example.sv"
`include "test_conv_ip_edge_detection.sv"
`include "test_conv_ip_sharpen.sv"
`include "test_conv_ip_box_blur.sv"
`include "test_conv_ip_gaussian_blur.sv"
`include "test_conv_ip_multiple.sv"
`include "test_conv_ip_maximum.sv"
`include "test_conv_ip_minimum.sv"
endpackage : conv_ip_test_pkg

`endif // CONV_IP_TEST_PKG_SV
