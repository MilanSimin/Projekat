`ifndef BRAM_C_IF_SV
`define BRAM_C_IF_SV

interface bram_c_if(input clock, input reset_n);
  
  `include "uvm_macros.svh"
  import uvm_pkg::*;
  
   // signals
  logic [15:0] data_c_out;
  logic [31:0] addr_c;
  logic enable_c;
  logic write_en_c;
  
endinterface : bram_c_if

`endif // BRAM_C_IF_SV
