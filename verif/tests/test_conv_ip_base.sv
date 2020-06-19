`ifndef TEST_CONV_IP_BASE_SV
`define TEST_CONV_IP_BASE_SV

class test_conv_ip_base extends uvm_test;
  
  // registration macro
  `uvm_component_utils(test_conv_ip_base)
  
  // component instance
  conv_ip_env_top m_conv_ip_env_top;
  //wb2uart_vir_sequence m_vseq;
  
  // configuration instance
  conv_ip_top_cfg m_cfg;
  
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  // end_of_elaboration phase
  extern virtual function void end_of_elaboration_phase(uvm_phase phase);
  // set default configuration
  extern virtual function void set_default_configuration();
    
endclass : test_conv_ip_base 

// constructor
function test_conv_ip_base::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void test_conv_ip_base::build_phase(uvm_phase phase);
  super.build_phase(phase);    
  
  // create component
  m_conv_ip_env_top = conv_ip_env_top::type_id::create("m_conv_ip_env_top", this);
 
   
  // create and set configuration
  m_cfg = conv_ip_top_cfg::type_id::create("m_cfg", this);
  set_default_configuration();
  
  // set configuration in DB
  uvm_config_db#(conv_ip_top_cfg)::set(this, "m_conv_ip_env_top", "m_cfg", m_cfg);
  uvm_config_db#(conv_ip_top_cfg)::set(this, "m_conv_ip_env_top.m_scoreboard", "m_cfg", m_cfg);

  // enable monitor item recording
  set_config_int("*", "recording_detail", 1);
  
  // define verbosity
  uvm_top.set_report_verbosity_level_hier(UVM_HIGH);
endfunction : build_phase

// end_of_elaboration phase
function void test_conv_ip_base::end_of_elaboration_phase(uvm_phase phase);
  super.end_of_elaboration_phase(phase);

  // allow additional time before stopping
  uvm_test_done.set_drain_time(this, 10us);
endfunction : end_of_elaboration_phase

// set default configuration
function void test_conv_ip_base::set_default_configuration();

	if(!m_cfg.randomize())begin
		`uvm_fatal(get_type_name(), "Failed to randomize.")
	end
  // define default configuration :

   //INIT Configuration
    m_cfg.m_axi_lite_cfg.m_agent_cfg.m_is_active = UVM_ACTIVE;
    m_cfg.m_axi_lite_cfg.m_agent_cfg.m_has_checks = 1;
    m_cfg.m_axi_lite_cfg.m_agent_cfg.m_has_coverage = 1;
    
    //MATRIX A Configuration
    m_cfg.m_bram_a_cfg.m_agent_cfg.m_is_active = UVM_ACTIVE;
    m_cfg.m_bram_a_cfg.m_agent_cfg.m_has_checks = 1;
    m_cfg.m_bram_a_cfg.m_agent_cfg.m_has_coverage = 1;
    
    //MATRIX B Configuration
    m_cfg.m_bram_b_cfg.m_agent_cfg.m_is_active = UVM_ACTIVE;
    m_cfg.m_bram_b_cfg.m_agent_cfg.m_has_checks = 1;
    m_cfg.m_bram_b_cfg.m_agent_cfg.m_has_coverage = 1;
    
    //MATRIX C Configuration
    m_cfg.m_bram_c_cfg.m_agent_cfg.m_is_active = UVM_ACTIVE;
    m_cfg.m_bram_c_cfg.m_agent_cfg.m_has_checks = 1;
    m_cfg.m_bram_c_cfg.m_agent_cfg.m_has_coverage = 1;

   //reset field
    m_cfg.reset_happend = 0;
    
endfunction : set_default_configuration

`endif // TEST_CONV_IP_BASE_SV
