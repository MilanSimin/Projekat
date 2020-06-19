`ifndef CONV_IP_VIR_SEQUENCE_SV
`define CONV_IP_VIR_SEQUENCE_SV

class conv_ip_vir_sequence extends conv_ip_base_vir;
  
	`uvm_object_utils (conv_ip_vir_sequence)

  rand int unsigned data_que_a [$];
  rand bit [31:0] columns;
  rand bit [31:0] lines;
  rand kernel_t kernel_type;

  constraint c_data_a { data_que_a.size() == columns*lines; 
                        foreach(data_que_a[i]) {data_que_a[i] >= 0; data_que_a[i] < 256;
                      }}
  constraint c_data_a_dist { foreach(data_que_a[i]) { data_que_a[i] dist {[1:55] := 70, 
                                                                        [56:150] := 20, 
                                                                       [151:255] := 10}; 
                                                    }}
  constraint c_size_of_lines { lines >= 3; lines < 256;}
  constraint c_size_of_colums {columns >= 3; columns < 256;}
	
function new (string name = "conv_ip_vir_sequence");
	super.new(name);
endfunction

	bram_a_basic_seq m_a_seq;
	bram_b_basic_seq m_b_seq;
	axi_lite_write_seq m_axi_lite_seq;
	
task pre_body();
	super.pre_body();
	
	m_a_seq = bram_a_basic_seq::type_id::create ("m_a_seq");
	m_b_seq = bram_b_basic_seq::type_id::create ("m_b_seq");
	m_axi_lite_seq = axi_lite_write_seq::type_id::create ("m_axi_lite_seq");
	
endtask: pre_body

task body();
	
	if(!m_axi_lite_seq.randomize() with {addr == LINES_REG_ADDR; data == lines;}) begin
	 `uvm_fatal(get_type_name(), "Failed to randomize.")
  end
	m_axi_lite_seq.start(p_sequencer.m_axi_lite_sequencer);

  if(!m_axi_lite_seq.randomize() with {addr == COLUMNS_REG_ADDR; data == columns;}) begin
	 `uvm_fatal(get_type_name(), "Failed to randomize.")
  end
	m_axi_lite_seq.start(p_sequencer.m_axi_lite_sequencer);

  if(!m_axi_lite_seq.randomize() with {addr == START_REG_ADDR; data == 'h1;}) begin //start
	   `uvm_fatal(get_type_name(), "Failed to randomize.")
    end
	  m_axi_lite_seq.start(p_sequencer.m_axi_lite_sequencer);
  
  fork 
   begin
    
    #1us;
    if(!m_axi_lite_seq.randomize() with {addr == START_REG_ADDR; data =='h0;}) begin //obrisi start registar
	   `uvm_fatal(get_type_name(), "Failed to randomize.")
    end
	  m_axi_lite_seq.start(p_sequencer.m_axi_lite_sequencer);
    #50us;
   end
   begin
      if(!m_a_seq.randomize() with { m_data_a.size() == data_que_a.size(); 
                                    foreach (m_data_a[i]) {m_data_a[i]== data_que_a[i];} 
                                    columns==local::columns;
                                    lines==local::lines;
                                   }) 
       begin
				 `uvm_fatal(get_type_name(), "Failed to randomize.")
	     end
       `uvm_info(get_type_name(), $sformatf("Size is %d, queue is : %p.",data_que_a.size(),data_que_a),UVM_LOW)
		    m_a_seq.start(p_sequencer.m_bram_a_seq);
   end
   begin 
       repeat( (columns-2)*(lines-2)) begin
				  if(!m_b_seq.randomize() with {m_type_of_kernel == kernel_type;}) begin
				    `uvm_fatal(get_type_name(), "Failed to randomize.")
				  end
				  m_b_seq.start(p_sequencer.m_bram_b_seq);
     end
   end
 join

endtask: body
endclass

`endif // CONV_IP_VIR_SRQUENCE_SV
