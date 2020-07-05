`ifndef AXI_LITE_MONITOR_SV
`define AXI_LITE_MONITOR_SV

class axi_lite_monitor extends uvm_monitor;
  
  // registration macro
  `uvm_component_utils(axi_lite_monitor)
  
  // analysis port
  uvm_analysis_port #(axi_lite_item) m_aport;
  
  // virtual interface reference
  virtual interface axi_lite_if m_vif;
  
  // configuration reference
  axi_lite_agent_cfg m_cfg;
  
  // monitor item
  axi_lite_item m_item;
  
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
  // print item
  extern virtual function void print_item(axi_lite_item item);

endclass : axi_lite_monitor

// constructor
function axi_lite_monitor::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

// build phase
function void axi_lite_monitor::build_phase(uvm_phase phase);
  super.build_phase(phase);
  // create port
  m_aport = new("m_aport", this);
  
  // create item
  m_item = axi_lite_item::type_id::create("m_item", this);
endfunction : build_phase

// connect phase
task axi_lite_monitor::run_phase(uvm_phase phase);
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
task axi_lite_monitor::handle_reset();
  // wait reset assertion
  @(m_vif.clock iff m_vif.reset_n == 0);
  `uvm_info(get_type_name(), "Reset asserted.", UVM_HIGH)
endtask : handle_reset

// collect item
task axi_lite_monitor::collect_item();  
  // wait until reset is de-asserted
  
  @(posedge m_vif.clock iff m_vif.reset_n == 1);
  `uvm_info(get_type_name(), "Reset de-asserted. Starting to collect items...", UVM_LOW)
  
    // WRITE
   fork
    begin
     forever begin
       @(posedge m_vif.clock iff m_vif.s_axi_awready === 1);
        m_item.addr = m_vif.s_axi_awaddr;
        m_item.data = m_vif.s_axi_wdata;
        m_item.write = 1;
        `uvm_info(get_type_name(), " --- WRITE --- ", UVM_LOW)
        `uvm_info(get_type_name(), $sformatf("Item collected: \n%s", m_item.sprint()), UVM_HIGH)
        m_aport.write(m_item);
     end
    end
    // READ
    begin
     forever begin
       @(posedge m_vif.clock iff m_vif.s_axi_arready === 1);
       m_item.addr = m_vif.s_axi_araddr;
       @(posedge m_vif.clock iff m_vif.s_axi_rvalid === 1);
       m_item.data = m_vif.s_axi_rdata;
       m_item.read = 1; 
       `uvm_info(get_type_name(), " --- READ --- ", UVM_LOW)
       `uvm_info(get_type_name(), $sformatf("Item collected: \n%s", m_item.sprint()), UVM_HIGH)
       m_aport.write(m_item);
     end
    end
    join
endtask : collect_item

// print item
function void axi_lite_monitor::print_item(axi_lite_item item);
  `uvm_info(get_type_name(), $sformatf("Item collected: \n%s", item.sprint()), UVM_HIGH)
endfunction : print_item

`endif // AXI_LITE_MONITOR_SV
