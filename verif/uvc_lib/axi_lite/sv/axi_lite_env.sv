`ifndef AXI_LITE_ENV_SV
`define AXI_LITE_ENV_SV

class axi_lite_env extends uvm_env;
  
  // registration macro
  `uvm_component_utils(axi_lite_env)  
  
  // configuration instance
  axi_lite_cfg m_cfg;  

  // agent instance
  axi_lite_agent m_agent;
  
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : axi_lite_env

// constructor
function axi_lite_env::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void axi_lite_env::build_phase(uvm_phase phase);
  super.build_phase(phase);
  
  // get configuration
  if(!uvm_config_db #(axi_lite_cfg)::get(this, "", "m_cfg", m_cfg)) begin
    `uvm_fatal(get_type_name(), "Failed to get configuration object from config DB!")
  end

  // create agent
  m_agent = axi_lite_agent::type_id::create("m_agent", this);

  // set agent configuration
  uvm_config_db#(axi_lite_agent_cfg)::set(this, "m_agent", "m_cfg", m_cfg.m_agent_cfg);
endfunction : build_phase

`endif // AXI_LITE_ENV_SV
