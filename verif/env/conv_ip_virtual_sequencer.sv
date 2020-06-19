`ifndef CONV_IP_VIRTUAL_SEQUENCER_SV
`define CONV_IP_VIRTUAL_SEQUENCER_SV

class conv_ip_virtual_sequencer extends uvm_sequencer;

	`uvm_component_utils (conv_ip_virtual_sequencer)
	
	
	function new(string name = "conv_ip_virtual_sequencer", uvm_component parent);
		super.new(name, parent);
	endfunction
	
	axi_lite_sequencer m_axi_lite_sequencer;
	bram_a_sequencer m_bram_a_seq;
	bram_b_sequencer m_bram_b_seq;
	
endclass : conv_ip_virtual_sequencer

`endif // CONV_IP_VIRTUAL_SEQUENCER_SV
