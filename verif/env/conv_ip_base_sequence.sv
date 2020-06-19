`ifndef CONV_IP_BASE_SEQUENCE_SV
`define CONV_IP_BASE_SEQUENCE_SV

class conv_ip_base_vir extends uvm_sequence;

	`uvm_object_utils (conv_ip_base_vir)
	`uvm_declare_p_sequencer (conv_ip_virtual_sequencer)
	
function new (string name = "conv_ip_base_vir");
	super.new(name);
endfunction
endclass

`endif // CONV_IP_BASE_SRQUENCE_SV
