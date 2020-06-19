`ifndef BRAM_C_ITEM_SV
`define BRAM_C_ITEM_SV

class bram_c_item extends uvm_sequence_item;
  
  // item fields
  rand bit [15:0] m_data_c_out;
  rand bit [31:0] m_addr_c;
  bit en_c;
  
  // registration macro    
  `uvm_object_utils_begin(bram_c_item)
    `uvm_field_int(m_data_c_out, UVM_ALL_ON)
    `uvm_field_int(m_addr_c, UVM_ALL_ON)
    `uvm_field_int(en_c, UVM_ALL_ON)
  `uvm_object_utils_end
  
  // constructor  
  extern function new(string name = "bram_c_item");
  
endclass : bram_c_item

// constructor
function bram_c_item::new(string name = "bram_c_item");
  super.new(name);
endfunction : new

`endif // BRAM_C_ITEM_SV
