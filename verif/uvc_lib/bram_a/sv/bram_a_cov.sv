//Coverage za bram A je prebacen u coverage_collector
`ifndef BRAM_A_COV_SV
`define BRAM_A_COV_SV

class bram_a_cov extends uvm_subscriber #(bram_a_item);
  
  // registration macro
  `uvm_component_utils(bram_a_cov)
  
  // configuration reference
  bram_a_agent_cfg m_cfg;
  
  // coverage fields 
 
  // constructor
  extern function new(string name, uvm_component parent);
  // analysis implementation port function
  extern virtual function void write(bram_a_item t);

endclass : bram_a_cov

// constructor
function bram_a_cov::new(string name, uvm_component parent);
  super.new(name, parent);

endfunction : new

// analysis implementation port function
function void bram_a_cov::write(bram_a_item t);
 // bram_a_cg.sample();
endfunction : write

`endif // BRAM_A_COV_SV
