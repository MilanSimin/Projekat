`ifndef BRAM_A_SEQUENCER_SV
`define BRAM_A_SEQUENCER_SV

class bram_a_sequencer extends uvm_sequencer #(bram_a_item);
  
  // registration macro
  `uvm_component_utils(bram_a_sequencer)
    
  // configuration reference
  bram_a_agent_cfg m_cfg;
  
  // constructor    
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : bram_a_sequencer

// constructor
function bram_a_sequencer::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_a_sequencer::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

`endif // BRAM_A_SEQUENCER_SV
