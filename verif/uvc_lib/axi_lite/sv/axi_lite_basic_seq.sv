`ifndef AXI_LITE_BASIC_SEQ_SV
`define AXI_LITE_BASIC_SEQ_SV

class axi_lite_basic_seq extends uvm_sequence #(axi_lite_item);
  
  // registration macro
  `uvm_object_utils(axi_lite_basic_seq)
  
  // sequencer pointer macro
  `uvm_declare_p_sequencer(axi_lite_sequencer)
  rand bit [3:0] addr;
  rand bit [31:0] data;
  // constructor
  extern function new(string name = "axi_lite_basic_seq");
  // body task
  extern virtual task body();
  
   constraint c_data {
       data < 256; data >= 0; 
  }
  
   constraint c_addr {
      (addr == 0 || addr == 4 || addr == 8 || addr == 12);
  }

endclass : axi_lite_basic_seq

// constructor
function axi_lite_basic_seq::new(string name = "axi_lite_basic_seq");
  super.new(name);
endfunction : new

// body task
task axi_lite_basic_seq::body();

  req = axi_lite_item::type_id::create("req");
  
  start_item(req);
  
  if(!req.randomize() with {addr == local::addr; data == local::data;}) begin
    `uvm_fatal(get_type_name(), "Failed to randomize.")
  end  
  
  finish_item(req);

endtask : body

`endif // AXI_LITE_BASIC_SEQ_SV
