`ifndef BRAM_B_SEQUENCER_SV
`define BRAM_B_SEQUENCER_SV

class bram_b_sequencer extends uvm_sequencer #(bram_b_item);
  
  // registration macro
  `uvm_component_utils(bram_b_sequencer)
    
  // configuration reference
  bram_b_agent_cfg m_cfg;
  
  // constructor    
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : bram_b_sequencer

// constructor
function bram_b_sequencer::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_b_sequencer::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

`endif // BRAM_B_SEQUENCER_SV
