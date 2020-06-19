//------------------------------------------------------------------------------
// Copyright (c) 2019 Elsys Eastern Europe
`ifndef TEST_CONV_IP_BOX_BLUR_SV
`define TEST_CONV_IP_BOX_BLUR_SV

// box_blur test
class test_conv_ip_box_blur extends test_conv_ip_base;
  
  // registration macro
  `uvm_component_utils(test_conv_ip_box_blur)
  
 conv_ip_vir_sequence m_vir_seq;

  // constructor
  extern function new(string name, uvm_component parent);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // set default configuration
  extern function void set_default_configuration();
  
endclass : test_conv_ip_box_blur

// constructor
function test_conv_ip_box_blur::new(string name, uvm_component parent);
  super.new(name, parent);
  m_vir_seq = conv_ip_vir_sequence::type_id::create("m_vir_seq", this);
endfunction : new

// run phase
task test_conv_ip_box_blur::run_phase(uvm_phase phase);
  super.run_phase(phase);
  
  uvm_test_done.raise_objection(this, get_type_name());    
  `uvm_info(get_type_name(), "TEST STARTED", UVM_LOW)
 

  assert(m_vir_seq.randomize() with {kernel_type == BOX_BLUR; columns inside {[5:15]}; lines inside {[5:15]};})
  m_vir_seq.start(m_conv_ip_env_top.m_virt_seqr);
  #200ns;
     
  uvm_test_done.drop_objection(this, get_type_name());    
  `uvm_info(get_type_name(), "TEST FINISHED", UVM_LOW)
endtask : run_phase

// set default configuration
function void test_conv_ip_box_blur::set_default_configuration();
  super.set_default_configuration();
  
  // redefine configuration
endfunction : set_default_configuration

`endif // TEST_CONV_IP_BOX_BLUR_SV
