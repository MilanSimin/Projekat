`ifndef BRAM_A_MONITOR_SV
`define BRAM_A_MONITOR_SV

class bram_a_monitor extends uvm_monitor;
  
  // registration macro
  `uvm_component_utils(bram_a_monitor)
  
  // analysis port
  uvm_analysis_port #(bram_a_item) m_aport;
  
  // virtual interface reference
  virtual interface bram_a_if m_vif;
  
  // configuration reference
  bram_a_agent_cfg m_cfg;
  
  // monitor item
  bram_a_item m_item;
  
  // constructor
  extern function new(string name, uvm_component parent);
  // build phase
  extern virtual function void build_phase(uvm_phase phase);
  // run phase
  extern virtual task run_phase(uvm_phase phase);
  // handle reset
  extern virtual task handle_reset();
  // collect item
  extern virtual task collect_item();


endclass : bram_a_monitor

// constructor
function bram_a_monitor::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void bram_a_monitor::build_phase(uvm_phase phase);
  super.build_phase(phase);
  
  // create port
  m_aport = new("m_aport", this);
  
  // create item
  m_item = bram_a_item::type_id::create("m_item", this);
endfunction : build_phase

// connect phase
task bram_a_monitor::run_phase(uvm_phase phase);
  super.run_phase(phase);
  
  forever begin
    fork : run_phase_fork_block
      begin
        handle_reset();
      end
      begin
        collect_item();    
      end
    join_any // run_phase_fork_block
    disable fork;
  end

endtask : run_phase

// handle reset
task bram_a_monitor::handle_reset();
  // wait reset assertion
  @(m_vif.clock iff m_vif.reset_n == 0);
  `uvm_info(get_type_name(), "Reset asserted.", UVM_HIGH)
endtask : handle_reset

// collect item
task bram_a_monitor::collect_item();  
  // wait until reset is de-asserted
  wait (m_vif.reset_n == 1);
//  `uvm_info(get_type_name(), "Reset de-asserted. Starting to collect items...", UVM_HIGH)
  
  forever begin    
   
   @(posedge m_vif.clock iff m_vif.enable_a === 1'b1); 
     m_item.m_addr_a_out = m_vif.addr_a; 
     m_item.m_data_a_in.push_back(m_vif.data_a_in); 
     m_item.en_a = m_vif.enable_a;
  
   //print item
   `uvm_info(get_type_name(), $sformatf("Address of A is: \t%d, data is : \t%d", m_item.m_addr_a_out, m_vif.data_a_in), UVM_HIGH)
 
    // write analysis port
    m_aport.write(m_item);    
  end // forever begin  
endtask : collect_item

`endif // BRAM_A_MONITOR_SV
