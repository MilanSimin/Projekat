`ifndef BRAM_A_ITEM_SV
`define BRAM_A_ITEM_SV

class bram_a_item extends uvm_sequence_item;
  
  // item fields
  rand int signed m_data_a_in[$];// za drajvovanje iz top sekvence
  int read_address;
  rand bit [31:0] columns, lines;
  //output
  bit [31:0] m_addr_a_out;
  bit en_a;
  
  // registration macro    
  `uvm_object_utils_begin(bram_a_item)
    `uvm_field_queue_int(m_data_a_in, UVM_ALL_ON)
    `uvm_field_int(read_address, UVM_ALL_ON)
    `uvm_field_int(columns, UVM_ALL_ON)
    `uvm_field_int(lines, UVM_ALL_ON)
    `uvm_field_int(m_addr_a_out, UVM_ALL_ON)
    `uvm_field_int(en_a, UVM_ALL_ON)
  `uvm_object_utils_end
  
  // constraints
  constraint c_data {foreach(m_data_a_in[i]) {soft m_data_a_in[i] >= 0; soft m_data_a_in[i] < 255;}}
  // constructor  
  extern function new(string name = "bram_a_item");
  
endclass : bram_a_item

// constructor
function bram_a_item::new(string name = "bram_a_item");
  super.new(name);
endfunction : new

`endif // BRAM_A_ITEM_SV
