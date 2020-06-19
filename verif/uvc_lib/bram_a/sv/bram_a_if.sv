`ifndef BRAM_A_IF_SV
`define BRAM_A_IF_SV

interface bram_a_if(input clock, input reset_n);
  
  `include "uvm_macros.svh"
  import uvm_pkg::*;
  
  // signals
  logic [8:0] data_a_in;
  logic [31:0] addr_a;
  logic enable_a;
  logic write_en_a;
  
endinterface : bram_a_if

`endif // BRAM_A_IF_SV
