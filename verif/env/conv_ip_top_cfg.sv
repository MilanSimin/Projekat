`ifndef CONV_IP_TOP_CFG_SV
`define CONV_IP_TOP_CFG_SV

class conv_ip_top_cfg extends uvm_object;
    
  // UVC configuration
   bit reset_happend;
   axi_lite_cfg m_axi_lite_cfg;
   bram_a_cfg m_bram_a_cfg;
   bram_b_cfg m_bram_b_cfg;
   bram_c_cfg m_bram_c_cfg;
  
  // registration macro
  `uvm_object_utils_begin(conv_ip_top_cfg)
    `uvm_field_int(reset_happend, UVM_ALL_ON)
    `uvm_field_object(m_axi_lite_cfg, UVM_ALL_ON)
    `uvm_field_object(m_bram_a_cfg, UVM_ALL_ON)
    `uvm_field_object(m_bram_b_cfg, UVM_ALL_ON)
    `uvm_field_object(m_bram_c_cfg, UVM_ALL_ON)
  `uvm_object_utils_end
    
  // constructor
  extern function new(string name = "conv_ip_top_cfg");
  
endclass : conv_ip_top_cfg

// constructor
function conv_ip_top_cfg::new(string name = "conv_ip_top_cfg");
  super.new(name);
  
  // create UVC configuration
  m_axi_lite_cfg = axi_lite_cfg::type_id::create("m_axi_lite_cfg");
  m_bram_a_cfg = bram_a_cfg::type_id::create("m_bram_a_cfg");
  m_bram_b_cfg = bram_b_cfg::type_id::create("m_bram_b_cfg");
  m_bram_c_cfg = bram_c_cfg::type_id::create("m_bram_c_cfg");
  
endfunction : new

`endif // CONV_IP_TOP_CFG_SV
