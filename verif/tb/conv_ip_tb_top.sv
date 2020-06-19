`ifndef CONV_IP_TB_TOP_SV
`define CONV_IP_TB_TOP_SV

// define timescale
`timescale 1ns/1ns

module conv_ip_tb_top;
  
  `include "uvm_macros.svh"  
  import uvm_pkg::*;
  
  // import test package
  import conv_ip_test_pkg::*;
    
  // signals
  reg reset_n;
  reg clock;
  
  
  // UVC interface instance
  axi_lite_if axi_lite_if_inst(clock, reset_n);
  bram_a_if   bram_a_if_inst(clock, reset_n);
  bram_b_if   bram_b_if_inst(clock, reset_n);
  bram_c_if   bram_c_if_inst(clock, reset_n);
   	
  	
  // DUT instance
  image_convolution_new_v1_0 dut(
    .s00_axi_aresetn (reset_n),
    .s00_axi_aclk    (clock),
    .s00_axi_awaddr  (axi_lite_if_inst.s_axi_awaddr),
    .s00_axi_awprot  (0),
    .s00_axi_awvalid (axi_lite_if_inst.s_axi_awvalid),
    .s00_axi_awready (axi_lite_if_inst.s_axi_awready),
    .s00_axi_wdata   (axi_lite_if_inst.s_axi_wdata),
    .s00_axi_wstrb   (axi_lite_if_inst.s_axi_wstrb),
    .s00_axi_wvalid  (axi_lite_if_inst.s_axi_wvalid),
    .s00_axi_wready  (axi_lite_if_inst.s_axi_wready),
    .s00_axi_bresp   (axi_lite_if_inst.s_axi_bresp),
    .s00_axi_bvalid  (axi_lite_if_inst.s_axi_bvalid),
    .s00_axi_bready  (axi_lite_if_inst.s_axi_bready),
    .s00_axi_araddr  (axi_lite_if_inst.s_axi_araddr),
    .s00_axi_arvalid (axi_lite_if_inst.s_axi_arvalid),
    .s00_axi_arready (axi_lite_if_inst.s_axi_arready),
    .s00_axi_rdata   (axi_lite_if_inst.s_axi_rdata),
    .s00_axi_rresp   (axi_lite_if_inst.s_axi_rresp),
    .s00_axi_rvalid  (axi_lite_if_inst.s_axi_rvalid),
    .s00_axi_rready  (axi_lite_if_inst.s_axi_rready),
    .clka            (open),
    .clkb            (open),
    .clkc            (open),
    .reseta          (open),
    .resetb          (open),
    .resetc          (open),
    .ena             (bram_a_if_inst.enable_a),
    .enb             (bram_b_if_inst.enable_b),
    .enc             (bram_c_if_inst.enable_c),
    .addra           (bram_a_if_inst.addr_a),
    .addrb           (bram_b_if_inst.addr_b),
    .addrc           (bram_c_if_inst.addr_c),
    .dina            (open),
    .douta           (bram_a_if_inst.data_a_in),
    .dinb            (open),
    .doutb           (bram_b_if_inst.data_b_in),
    .wea             (bram_a_if_inst.write_en_a),
    .web             (bram_b_if_inst.write_en_b),
    .wec             (bram_c_if_inst.write_en_c),
    .dinc            (bram_c_if_inst.data_c_out),
    .doutc           (0)
  );
  

  // configure UVC's virtual interface in DB
  initial begin : config_if_block
    uvm_config_db#(virtual axi_lite_if)::set(uvm_root::get(), "uvm_test_top.m_conv_ip_env_top.m_axi_lite_env.m_agent", "m_vif", axi_lite_if_inst);
    uvm_config_db#(virtual bram_a_if)::set(uvm_root::get(), "uvm_test_top.m_conv_ip_env_top.m_bram_a_env.m_agent", "m_vif", bram_a_if_inst);
    uvm_config_db#(virtual bram_b_if)::set(uvm_root::get(), "uvm_test_top.m_conv_ip_env_top.m_bram_b_env.m_agent", "m_vif", bram_b_if_inst);
    uvm_config_db#(virtual bram_c_if)::set(uvm_root::get(), "uvm_test_top.m_conv_ip_env_top.m_bram_c_env.m_agent", "m_vif", bram_c_if_inst);
  end
    
  // define initial clock value and generate reset
  initial begin : clock_and_rst_init_block
    reset_n <= 1'b0;
    clock <= 1'b1;
    #501 reset_n <= 1'b1;
  end
  
  // generate clock
  always begin : clock_gen_block
    #50 clock <= ~clock;
  end
  
  initial begin : timeformat
    $timeformat(-9,0,"ns",5);
  end
  
  // run test
  initial begin : run_test_block
    run_test();
  end
  
endmodule : conv_ip_tb_top

`endif // CONV_IP_TB_TOP_SV
