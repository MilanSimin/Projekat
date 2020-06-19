`ifndef BRAM_C_CFG_SV
`define BRAM_C_CFG_SV

class bram_c_cfg extends uvm_object;
  
  // agent configuration
  bram_c_agent_cfg m_agent_cfg;
  
  // registration macro
  `uvm_object_utils_begin(bram_c_cfg)
    `uvm_field_object(m_agent_cfg, UVM_ALL_ON)
  `uvm_object_utils_end
  
  // constructor   
  extern function new(string name = "bram_c_cfg");
    
endclass : bram_c_cfg

// constructor
function bram_c_cfg::new(string name = "bram_c_cfg");
  super.new(name);
  
  // create agent configuration
  m_agent_cfg = bram_c_agent_cfg::type_id::create("m_agent_cfg");
endfunction : new

`endif // BRAM_C_CFG_SV
