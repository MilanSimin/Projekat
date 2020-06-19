//Ovaj agent je pasiv i sekvencer se ne koristi u tom slucaju
`ifndef BRAM_C_SEQUENCER_SV
`define BRAM_C_SEQUENCER_SV

class bram_c_sequencer extends uvm_sequencer #(bram_c_item);
  
  // registration macro
  `uvm_component_utils(bram_c_sequencer)
    
  // configuration reference
  bram_c_agent_cfg m_cfg;
  
  // constructor    
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : bram_c_sequencer

// constructor
function bram_c_sequencer::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_c_sequencer::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

`endif // BRAM_C_SEQUENCER_SV
