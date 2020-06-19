`ifndef CONV_IP_COVERAGE_COLLECTOR_SV
`define CONV_IP_COVERAGE_COLLECTOR_SV

`uvm_analysis_imp_decl(_axi_lite_cov)
`uvm_analysis_imp_decl(_bram_a_cov)
`uvm_analysis_imp_decl(_bram_b_cov)
`uvm_analysis_imp_decl(_bram_c_cov)

class conv_ip_coverage_collector extends uvm_scoreboard;
	`uvm_component_utils(conv_ip_coverage_collector)

  int data_a, num_of_data_a;
  int signed data_b;
  //clone items
  axi_lite_item axi_clone;
  bram_a_item bram_a_clone_item;
  bram_b_item bram_b_clone_item;
  bram_c_item bram_c_clone_item;
  
	uvm_analysis_imp_axi_lite_cov#(axi_lite_item, conv_ip_coverage_collector) m_axi_lite_cov;
	uvm_analysis_imp_bram_a_cov#(bram_a_item, conv_ip_coverage_collector) m_bram_a_cov;
  uvm_analysis_imp_bram_b_cov#(bram_b_item, conv_ip_coverage_collector) m_bram_b_cov;
  uvm_analysis_imp_bram_c_cov#(bram_c_item, conv_ip_coverage_collector) m_bram_c_cov;
	

	 extern function new(string name = "conv_ip_coverage_collector", uvm_component parent);
	 extern virtual function void build_phase(uvm_phase phase);	
 	 extern virtual function void write_axi_lite_cov(axi_lite_item m_axi_item);
 	 extern virtual function void write_bram_a_cov(bram_a_item m_bram_a_item);
 	 extern virtual function void write_bram_b_cov(bram_b_item m_bram_b_item);
 	 extern virtual function void write_bram_c_cov(bram_c_item m_bram_c_item);

// coverage
 covergroup axi_lite_cg;
   option.per_instance = 1;

   reg_address : coverpoint axi_clone.addr{
    bins COLUMNS_REG_ADDR = {'h0};
    bins LINES_REG_ADDR = {'h4};
    bins START_REG_ADDR = {'h8};
    bins READY_REG_ADDR = {'h12};
    }
   
   axi_data: coverpoint axi_clone.data {
    bins MINIMUM = {1};
    bins DATA_SMALL = { [2:50] };
    bins DATA_MEDIUM = { [51:150] };
    bins DATA_BIG = { [151:254] };
    bins MAXIMUM = {255};
   }
   start: cross reg_address, axi_data {
    bins start_conv = binsof(reg_address.START_REG_ADDR) && binsof(axi_data) intersect {1};
    ignore_bins start_with_data_small = binsof(reg_address.START_REG_ADDR) && binsof(axi_data.DATA_SMALL);
    ignore_bins start_with_data_medium = binsof(reg_address.START_REG_ADDR) && binsof(axi_data.DATA_MEDIUM);
    ignore_bins start_with_data_big = binsof(reg_address.START_REG_ADDR) && binsof(axi_data.DATA_BIG);
    ignore_bins start_with_data_max = binsof(reg_address.START_REG_ADDR) && binsof(axi_data.MAXIMUM);
    ignore_bins ready_data = binsof(reg_address.READY_REG_ADDR) && binsof(axi_data);
    ignore_bins colums_with_one = binsof(reg_address.COLUMNS_REG_ADDR) && binsof(axi_data.MINIMUM);
    ignore_bins lines_with_one = binsof(reg_address.LINES_REG_ADDR) && binsof(axi_data.MINIMUM);
   }
   write: coverpoint axi_clone.write {
    bins write_data = {1};
   }
   read: coverpoint axi_clone.read {
    bins read_data = {1};
   }
   write_in_regs: cross reg_address, write {
    ignore_bins ready_write = binsof(reg_address) intersect {READY_REG_ADDR} && binsof(write) intersect {1};
   }
   read_from_regs: cross reg_address, read; //automatski binovi za sve kombinacije
  endgroup : axi_lite_cg


  covergroup bram_b_cg;
   option.per_instance = 1;

   b_address : coverpoint bram_b_clone_item.m_addr_b_out{
      bins zero = {'h0};
      bins one = {'h4};
      bins two = {'h8};
      bins three = {'hc};
      bins four = {'h10};
      bins five = {'h14};
      bins six = {'h18};
      bins seven = {'h1c};
      bins eight = {'h20};
    }
   
   bram_b_data: coverpoint data_b { 
    bins IDENTITY = (0 => 0 => 0 => 0 => 1 => 0 => 0 => 0);
    bins EDGE = (-1 => -1 => -1 => -1 => 8 => -1 => -1 => -1);
    bins SHARPEN = (0 => -1 => 0 => -1 => 5 => -1 => 0 => -1);
    bins BOX_BLUR = (1 => 1 => 1 => 1 => 1 => 1 => 1 => 1);
    bins GAUSSIAN_BLUR = (1 => 2 => 1 => 2 => 4 => 2 => 1 => 2);
   }
  endgroup : bram_b_cg

  covergroup bram_c_cg;
   option.per_instance = 1;
   
   bram_c_data: coverpoint bram_c_clone_item.m_data_c_out {
    bins C_DATA_SMALL = { [1:50] };
    bins C_DATA_MEDIUM = { [51:150] };
    bins C_DATA_BIG = { [151:255] };
   }
   
  endgroup : bram_c_cg

  covergroup bram_a_cg;
   option.per_instance = 1;

   bram_a_data: coverpoint data_a {
    bins DATA_SMALL = { [1:50] };
    bins DATA_MEDIUM = { [51:150] };
    bins DATA_BIG = { [151:255] };
   }
   
  endgroup : bram_a_cg

endclass

// constructor
function conv_ip_coverage_collector::new(string name = "conv_ip_coverage_collector", uvm_component parent);

  super.new(name,parent);
    axi_lite_cg = new();
    bram_a_cg = new();
    bram_b_cg = new();
    bram_c_cg = new();
endfunction : new

function void conv_ip_coverage_collector::build_phase(uvm_phase phase);
	super.build_phase(phase);
	m_axi_lite_cov = new("m_axi_lite_cov",this);
	m_bram_a_cov   = new("m_bram_a_cov",this);
  m_bram_b_cov   = new("m_bram_b_cov",this);
	m_bram_c_cov   = new("m_bram_c_cov",this);
	
endfunction: build_phase

function void conv_ip_coverage_collector::write_axi_lite_cov(axi_lite_item m_axi_item);

	$cast(axi_clone,m_axi_item.clone());	
  axi_lite_cg.sample();
	
endfunction: write_axi_lite_cov

function void conv_ip_coverage_collector::write_bram_a_cov(bram_a_item m_bram_a_item);
	 $cast(bram_a_clone_item,m_bram_a_item.clone());
   data_a = bram_a_clone_item.m_data_a_in[num_of_data_a];
   num_of_data_a++; //da bi mogla da ubacim po jedan podatak??
	 bram_a_cg.sample();
endfunction: write_bram_a_cov

function void conv_ip_coverage_collector::write_bram_b_cov(bram_b_item m_bram_b_item);
  $cast(bram_b_clone_item,m_bram_b_item.clone());
  data_b = bram_b_clone_item.m_data_b_in;
  bram_b_cg.sample();
endfunction: write_bram_b_cov

function void conv_ip_coverage_collector::write_bram_c_cov(bram_c_item m_bram_c_item);

  $cast(bram_c_clone_item,m_bram_c_item.clone());
  num_of_data_a = 0;
  bram_c_cg.sample();
 
endfunction: write_bram_c_cov

`endif // CONV_IP_COVERAGE_COLLECTOR_SV
