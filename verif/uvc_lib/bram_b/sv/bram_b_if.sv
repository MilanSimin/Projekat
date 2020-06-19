`ifndef BRAM_B_IF_SV
`define BRAM_B_IF_SV

interface bram_b_if(input clock, input reset_n);
  
  `include "uvm_macros.svh"
  import uvm_pkg::*;
  
  // signals
  logic signed [8:0]  data_b_in;
  logic [7:0] addr_b;
  logic enable_b;
  logic write_en_b;
  
endinterface : bram_b_if

`endif // BRAM_B_IF_SV
