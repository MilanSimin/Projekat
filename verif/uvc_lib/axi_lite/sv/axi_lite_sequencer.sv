`ifndef AXI_LITE_SEQUENCER_SV
`define AXI_LITE_SEQUENCER_SV

class axi_lite_sequencer extends uvm_sequencer #(axi_lite_item);
  
  // registration macro
  `uvm_component_utils(axi_lite_sequencer)
    
  // configuration reference
  axi_lite_agent_cfg m_cfg;
  
  // constructor    
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : axi_lite_sequencer

// constructor
function axi_lite_sequencer::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void axi_lite_sequencer::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

`endif // AXI_LITE_SEQUENCER_SV
