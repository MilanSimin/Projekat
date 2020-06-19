//Coverage za bram C je prebacen u coverage_collector
`ifndef BRAM_C_COV_SV
`define BRAM_C_COV_SV

class bram_c_cov extends uvm_subscriber #(bram_c_item);
  
  // registration macro
  `uvm_component_utils(bram_c_cov)
  
  // configuration reference
  bram_c_agent_cfg m_cfg;
  // constructor
  extern function new(string name, uvm_component parent);
  // analysis implementation port function
  extern virtual function void write(bram_c_item t);

endclass : bram_c_cov

// constructor
function bram_c_cov::new(string name, uvm_component parent);
  super.new(name, parent);
 // bram_c_cg = new();
endfunction : new

// analysis implementation port function
function void bram_c_cov::write(bram_c_item t);
 // bram_c_cg.sample();
endfunction : write

`endif // BRAM_C_COV_SV
