`ifndef AXI_LITE_ITEM_SV
`define AXI_LITE_ITEM_SV

class axi_lite_item extends uvm_sequence_item;
  
   // item fields
  rand bit [3:0] addr;
  rand bit [31:0] data;
  rand bit write; //pomocni signal za write operaciju
  rand bit read; //pomocni signal za read

  //output
  // registration macro    
  `uvm_object_utils_begin(axi_lite_item)
    `uvm_field_int(addr, UVM_ALL_ON)
    `uvm_field_int(data, UVM_ALL_ON)
    `uvm_field_int(read, UVM_ALL_ON)
    `uvm_field_int(write, UVM_ALL_ON)
  `uvm_object_utils_end

  // constraints
 constraint c_data {
   soft data < 255; data >= 0; 
  }

  constraint c_addr {
      (addr == 0 || addr == 4 || addr == 8 || addr == 12);
  }

  // constructor  
  extern function new(string name = "axi_lite_item");
  
endclass : axi_lite_item

// constructor
function axi_lite_item::new(string name = "axi_lite_item");
  super.new(name);
endfunction : new

`endif // AXI_LITE_ITEM_SV
