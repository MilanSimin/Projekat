`ifndef BRAM_B_DRIVER_SV
`define BRAM_B_DRIVER_SV

class bram_b_driver extends uvm_driver #(bram_b_item);
  
  // registration macro
  `uvm_component_utils(bram_b_driver)
  
  // virtual interface reference
  virtual interface bram_b_if m_vif;
  
  // configuration reference
  bram_b_agent_cfg m_cfg;
  
  // request item
  REQ m_req;
   
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // process item
  extern virtual task process_item(bram_b_item item);

endclass : bram_b_driver

// constructor
function bram_b_driver::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_b_driver::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

// run phase
task bram_b_driver::run_phase(uvm_phase phase);
  super.run_phase(phase);

  // init signals
  m_vif.addr_b <= 0;
  m_vif.data_b_in <= 0;
  
  forever begin
    seq_item_port.get_next_item(m_req);
    process_item(m_req);
    seq_item_port.item_done();
  end
endtask : run_phase

// process item
task bram_b_driver::process_item(bram_b_item item);

 // wait until reset is de-asserted
  wait (m_vif.reset_n == 1);
  
  // drive signals
  
  @(posedge m_vif.clock iff m_vif.reset_n === 1'b1);
  @(posedge m_vif.enable_b);
    m_vif.data_b_in <= item.m_data_b_in;
   `uvm_info(get_type_name(), $sformatf("Item to be driven: \n%d", item.m_data_b_in), UVM_HIGH)
    
endtask : process_item

`endif // BRAM_B_DRIVER_SV
