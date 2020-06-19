`ifndef BRAM_A_AGENT_CFG_SV
`define BRAM_A_AGENT_CFG_SV

class bram_a_agent_cfg extends uvm_object;
  
  // configuration fields
  uvm_active_passive_enum m_is_active = UVM_ACTIVE;
  bit m_has_checks;
  bit m_has_coverage;  
  
  // registration macro
  `uvm_object_utils_begin(bram_a_agent_cfg)
    `uvm_field_enum(uvm_active_passive_enum, m_is_active, UVM_ALL_ON)
    `uvm_field_int(m_has_checks, UVM_ALL_ON)
    `uvm_field_int(m_has_coverage, UVM_ALL_ON)
  `uvm_object_utils_end
  
  // constructor   
  extern function new(string name = "bram_a_agent_cfg");
    
endclass : bram_a_agent_cfg

// constructor
function bram_a_agent_cfg::new(string name = "bram_a_agent_cfg");
  super.new(name);
endfunction : new

`endif // BRAM_A_AGENT_CFG_SV
