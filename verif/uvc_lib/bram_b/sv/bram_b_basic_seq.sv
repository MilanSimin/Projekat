`ifndef BRAM_B_BASIC_SEQ_SV
`define BRAM_B_BASIC_SEQ_SV

class bram_b_basic_seq extends uvm_sequence #(bram_b_item);
  
  // registration macro
  `uvm_object_utils(bram_b_basic_seq)
  
  // sequencer pointer macro
  `uvm_declare_p_sequencer(bram_b_sequencer)
  
  // fields
  int signed m_kernel_data[$];
  rand kernel_t m_type_of_kernel; //kernel types are in bram_b_common file
  
  // constructor
  extern function new(string name = "bram_b_basic_seq");
  // body task
  extern virtual task body();
  //initializing kernel
  extern function void choose_kernel (kernel_t kernel);

endclass : bram_b_basic_seq

// constructor
function bram_b_basic_seq::new(string name = "bram_b_basic_seq");
  super.new(name);
endfunction : new

// body task
task bram_b_basic_seq::body();

  choose_kernel(m_type_of_kernel);

  for(int i = 0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin

   req = bram_b_item::type_id::create("req");
   start_item(req);
  
   if(!req.randomize() with {
     m_data_b_in == m_kernel_data[i];
   }) begin
     `uvm_fatal(get_type_name(), "Failed to randomize.")
   end  
  
   finish_item(req);
  end
  m_kernel_data.delete();

endtask : body

function void bram_b_basic_seq::choose_kernel (kernel_t kernel);


 case(kernel) 
  IDENTITY: begin
             for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
               m_kernel_data.push_back(8'b0);
             end
             m_kernel_data[4] = 1;
             `uvm_info(get_type_name(), $sformatf("Identity kernel is %p: .", m_kernel_data), UVM_LOW)
            end
  
  EDGE_DETECTION: begin
                    for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
                     m_kernel_data.push_back(-1);
                    end
                    m_kernel_data[4] = 8;
                    `uvm_info(get_type_name(), $sformatf("Edge detection kernel is %p: .",m_kernel_data),UVM_LOW)
                   end
  SHARPEN: begin
            for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
               m_kernel_data.push_back(8'b0);
            end
             m_kernel_data[1] = -1;
             m_kernel_data[3] = -1;
             m_kernel_data[4] = 5;
             m_kernel_data[5] = -1;
             m_kernel_data[7] = -1;
             `uvm_info(get_type_name(), $sformatf("Sharpen kernel is %p: .",m_kernel_data),UVM_LOW)

           end 
  BOX_BLUR: begin
             for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
               m_kernel_data.push_back(8'b1);
             end
             `uvm_info(get_type_name(), $sformatf("Box blur is %p: .", m_kernel_data), UVM_LOW)
            end

  GAUSSIAN_BLUR: begin
                  for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
                   m_kernel_data.push_back(8'b1);
                  end
                  m_kernel_data[1] = 2;
                  m_kernel_data[3] = 2;
                  m_kernel_data[5] = 2;
                  m_kernel_data[7] = 2;
                  m_kernel_data[4] = 4;
                   `uvm_info(get_type_name(), $sformatf("Gaussian blur is %p: .", m_kernel_data), UVM_LOW)
                 end
  default:  begin
             for(int i =0; i < (KERNEL_SIZE * KERNEL_SIZE); i++) begin
               m_kernel_data.push_back(8'b1);
             end
             `uvm_info(get_type_name(), $sformatf("Default kernel is %p: .", m_kernel_data), UVM_LOW)
            end

 endcase

endfunction: choose_kernel

`endif // BRAM_B_BASIC_SEQ_SV
