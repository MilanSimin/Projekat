`ifndef TEST_CONV_IP_MULTIPLE_SV
`define TEST_CONV_IP_MULTIPLE_SV

// multiple test
class test_conv_ip_multiple extends test_conv_ip_base;
  
  // registration macro
  `uvm_component_utils(test_conv_ip_multiple)
 
 rand int num_of_conv;
 conv_ip_vir_sequence m_vir_seq;
 constraint convolution_number { (num_of_conv > 1 && num_of_conv < 10); }
  // constructor
  extern function new(string name, uvm_component parent);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // set default configuration
  extern function void set_default_configuration();
  
endclass : test_conv_ip_multiple

// constructor
function test_conv_ip_multiple::new(string name, uvm_component parent);
  super.new(name, parent);
  m_vir_seq = conv_ip_vir_sequence::type_id::create("m_vir_seq", this);
endfunction : new

// run phase
task test_conv_ip_multiple::run_phase(uvm_phase phase);
  super.run_phase(phase);

  this.randomize(num_of_conv);
  uvm_test_done.raise_objection(this, get_type_name());    
  `uvm_info(get_type_name(), "TEST STARTED", UVM_LOW)
 
  `uvm_info(get_type_name(), $sformatf("Number of convolution: %d", num_of_conv), UVM_LOW)
  repeat( num_of_conv) begin
   assert(m_vir_seq.randomize() with {columns == 5; lines == 6;}) //inside {[5:15]}; lines inside {[5:20]};})
   m_vir_seq.start(m_conv_ip_env_top.m_virt_seqr);
   #200ns;
  end
     
  uvm_test_done.drop_objection(this, get_type_name());    
  `uvm_info(get_type_name(), "TEST FINISHED", UVM_LOW)
endtask : run_phase

// set default configuration
function void test_conv_ip_multiple::set_default_configuration();
  super.set_default_configuration();
  
  // redefine configuration
endfunction : set_default_configuration

`endif // TEST_CONV_IP_MULTIPLE_SV
