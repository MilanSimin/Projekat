`ifndef AXI_LITE_CFG_SV
`define AXI_LITE_CFG_SV

class axi_lite_cfg extends uvm_object;
  
  // agent configuration
  axi_lite_agent_cfg m_agent_cfg;
  
  // registration macro
  `uvm_object_utils_begin(axi_lite_cfg)
    `uvm_field_object(m_agent_cfg, UVM_ALL_ON)
  `uvm_object_utils_end
  
  // constructor   
  extern function new(string name = "axi_lite_cfg");
    
endclass : axi_lite_cfg

// constructor
function axi_lite_cfg::new(string name = "axi_lite_cfg");
  super.new(name);
  
  // create agent configuration
  m_agent_cfg = axi_lite_agent_cfg::type_id::create("m_agent_cfg");
endfunction : new

`endif // AXI_LITE_CFG_SV
