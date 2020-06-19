`ifndef AXI_LITE_IF_SV
`define AXI_LITE_IF_SV

interface axi_lite_if(input clock, input reset_n);
  
  `include "uvm_macros.svh"
  import uvm_pkg::*;
  
 //**************adresni kanal za upis**************
  logic [3:0] s_axi_awaddr;
  logic s_axi_awvalid;
  logic s_axi_awready; //za slejv, spreman je da prihvati adresu
  //*************kanal upisanih podataka****************
  logic [31:0] s_axi_wdata;
  logic [3:0] s_axi_wstrb;
  logic s_axi_wvalid; //indikacija validnosti podatka
  logic s_axi_wready; //indikacija da je slejv spreman da prihvati podatak
  //****************statusni kanal upisa***********************
  logic [1:0] s_axi_bresp; //indikacija da li je transakcija zavrsena (slejv)
  logic s_axi_bvalid; //da li je podatak u statusu transakcije validan (slejv)
  logic s_axi_bready; //indikacija da je master spreman da prihvati podatak o statusu
  //*******************adresni kanal za citanje********************
  logic [3:0] s_axi_araddr;
  logic s_axi_arvalid;
  logic s_axi_arready;
  //******************kanal procitanih podataka********************
  logic [31:0] s_axi_rdata; //procitan podatak
  logic [1:0] s_axi_rresp; //status kompletirane transakcije citanja
  logic s_axi_rvalid; //indikacija validnosti podatka
  logic s_axi_rready;
  

endinterface : axi_lite_if

`endif // AXI_LITE_IF_SV
