`ifndef TEST_CONV_IP_READ_RESET_VALUES_SV
`define TEST_CONV_IP_READ_RESET_VALUES_SV

// reset_values test
class test_conv_ip_read_reset_values extends test_conv_ip_base;
  
  // registration macro
  `uvm_component_utils(test_conv_ip_read_reset_values)
 
 axi_lite_read_seq axi_read_seq;
  // constructor
  extern function new(string name, uvm_component parent);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
   // set default configuration
  extern function void set_default_configuration();
  
endclass : test_conv_ip_read_reset_values

// constructor
function test_conv_ip_read_reset_values::new(string name, uvm_component parent);
  super.new(name, parent);
  axi_read_seq = axi_lite_read_seq::type_id::create("axi_read_seq", this);
endfunction : new

// run phase
task test_conv_ip_read_reset_values::run_phase(uvm_phase phase);
  super.run_phase(phase);

  uvm_test_done.raise_objection(this, get_type_name());   
 
  `uvm_info(get_type_name(), "TEST STARTED", UVM_LOW)
 
   #600ns;
   assert(axi_read_seq.randomize() with {addr == COLUMNS_REG_ADDR;});
   axi_read_seq.start(m_conv_ip_env_top.m_virt_seqr.m_axi_lite_sequencer);
   #10ns;
   assert(axi_read_seq.randomize() with {addr == LINES_REG_ADDR;});
   axi_read_seq.start(m_conv_ip_env_top.m_virt_seqr.m_axi_lite_sequencer);
   #10ns;
   assert(axi_read_seq.randomize() with {addr == START_REG_ADDR;});
   axi_read_seq.start(m_conv_ip_env_top.m_virt_seqr.m_axi_lite_sequencer);
   #10ns;
   assert(axi_read_seq.randomize() with {addr == READY_REG_ADDR;});
   axi_read_seq.start(m_conv_ip_env_top.m_virt_seqr.m_axi_lite_sequencer);
   #200ns;
     
  uvm_test_done.drop_objection(this, get_type_name());    
  `uvm_info(get_type_name(), "TEST FINISHED", UVM_LOW)
endtask : run_phase

// set default configuration
function void test_conv_ip_read_reset_values::set_default_configuration();
  super.set_default_configuration();
  m_cfg.reset_happend = 1;
  // redefine configuration
endfunction : set_default_configuration

`endif // TEST_CONV_IP_READ_RESET_VALUES_SV
