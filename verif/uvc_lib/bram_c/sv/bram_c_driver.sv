`ifndef BRAM_C_DRIVER_SV
`define BRAM_C_DRIVER_SV

class bram_c_driver extends uvm_driver #(bram_c_item);
  
  // registration macro
  `uvm_component_utils(bram_c_driver)
  
  // virtual interface reference
  virtual interface bram_c_if m_vif;
  
  // configuration reference
  bram_c_agent_cfg m_cfg;
  
  // request item
  REQ m_req;
   
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // process item
  extern virtual task process_item(bram_c_item item);

endclass : bram_c_driver

// constructor
function bram_c_driver::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_c_driver::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

// run phase
task bram_c_driver::run_phase(uvm_phase phase);
  super.run_phase(phase);

 // init signals
  m_vif.addr_c <= 0;
  m_vif.data_c_out <= 0;
  
  forever begin
    seq_item_port.get_next_item(m_req);
    process_item(m_req);
    seq_item_port.item_done();
  end
endtask : run_phase

// process item
task bram_c_driver::process_item(bram_c_item item);
  // print item
  `uvm_info(get_type_name(), $sformatf("Item to be driven: \n%s", item.sprint()), UVM_HIGH)
  
  // wait until reset is de-asserted
  wait (m_vif.reset_n == 1);
  
  // drive signals
 // @(posedge m_vif.clock iff m_vif.enable_c === 1'b1);
  //  m_vif.addr_c <= item.m_addr_c;
    
endtask : process_item

`endif // BRAM_C_DRIVER_SV
