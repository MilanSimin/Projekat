`ifndef BRAM_A_DRIVER_SV
`define BRAM_A_DRIVER_SV

class bram_a_driver extends uvm_driver #(bram_a_item);
  
  // registration macro
  `uvm_component_utils(bram_a_driver)
  
  // virtual interface reference
  virtual interface bram_a_if m_vif;
  
  // configuration reference
  bram_a_agent_cfg m_cfg;
  
  // request item
  REQ m_req;
   
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // process item
  extern virtual task process_item(bram_a_item item);

endclass : bram_a_driver

// constructor
function bram_a_driver::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_a_driver::build_phase(uvm_phase phase);
  super.build_phase(phase);
endfunction : build_phase

// run phase
task bram_a_driver::run_phase(uvm_phase phase);
  super.run_phase(phase);

  m_vif.data_a_in <= 0;

  forever begin
    seq_item_port.get_next_item(m_req);
    process_item(m_req);
    seq_item_port.item_done();
  end
endtask : run_phase

// process item
task bram_a_driver::process_item(bram_a_item item);

  // wait until reset is de-asserted
  //wait (m_vif.reset_n == 1);
  
  // drive signals
  @(posedge m_vif.clock iff m_vif.reset_n == 1);
   for(int i = 0; i< (9*(item.columns-2)*(item.lines-2)); i++) begin
    @(posedge m_vif.enable_a);
    item.read_address = (m_vif.addr_a/4);
    m_vif.data_a_in <= item.m_data_a_in[item.read_address];
    `uvm_info(get_type_name(), $sformatf("Data is %d, adress is %d",m_vif.data_a_in,item.read_address),UVM_LOW)
   end

endtask : process_item

`endif // BRAM_A_DRIVER_SV
