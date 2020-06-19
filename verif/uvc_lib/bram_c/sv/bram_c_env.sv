`ifndef BRAM_C_ENV_SV
`define BRAM_C_ENV_SV

class bram_c_env extends uvm_env;
  
  // registration macro
  `uvm_component_utils(bram_c_env)  
  
  // configuration instance
  bram_c_cfg m_cfg;  

  // agent instance
  bram_c_agent m_agent;
  
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  
endclass : bram_c_env

// constructor
function bram_c_env::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_c_env::build_phase(uvm_phase phase);
  super.build_phase(phase);
  
  // get configuration
  if(!uvm_config_db #(bram_c_cfg)::get(this, "", "m_cfg", m_cfg)) begin
    `uvm_fatal(get_type_name(), "Failed to get configuration object from config DB!")
  end

  // create agent
  m_agent = bram_c_agent::type_id::create("m_agent", this);

  // set agent configuration
  uvm_config_db#(bram_c_agent_cfg)::set(this, "m_agent", "m_cfg", m_cfg.m_agent_cfg);
endfunction : build_phase

`endif // BRAM_C_ENV_SV
