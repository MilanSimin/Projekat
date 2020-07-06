//Scoreboard za Convolusion IP : Svo cekiranje se vrsi u write funkcijama, nema run phase

`ifndef CONV_IP_SCOREBOARD_SV
`define CONV_IP_SCOREBOARD_SV

`uvm_analysis_imp_decl(_axi_lite)
`uvm_analysis_imp_decl(_bram_a)
`uvm_analysis_imp_decl(_bram_b)
`uvm_analysis_imp_decl(_bram_c)

class conv_ip_scoreboard extends uvm_scoreboard;
	`uvm_component_utils(conv_ip_scoreboard)

   conv_ip_top_cfg m_cfg;
  //clone items
  axi_lite_item axi_clone_item;
  bram_a_item bram_a_clone;
  bram_b_item bram_b_clone;
  bram_c_item bram_c_clone;
  int signed bram_b_que[$];
  int unsigned bram_a_que[$];
  int num_of_data_a; //brojac za bram A
  int order_of_data_b; //brojac za bram B
  int count_of_data_c; // brojac za C
  int columns, lines; 
  int start_count = 0; //broj konvolucija
  bit start_happend = 0, ready;
  string s;
  
	uvm_analysis_imp_axi_lite#(axi_lite_item, conv_ip_scoreboard) m_axi_lite;
	uvm_analysis_imp_bram_a#(bram_a_item, conv_ip_scoreboard) m_bram_a;
  uvm_analysis_imp_bram_b#(bram_b_item, conv_ip_scoreboard) m_bram_b;
  uvm_analysis_imp_bram_c#(bram_c_item, conv_ip_scoreboard) m_bram_c;
	
	function new(string name = "conv_ip_scoreboard", uvm_component parent);
		super.new(name,parent);
	endfunction

	
	 extern virtual function void build_phase(uvm_phase phase);	
 	 extern virtual function void write_axi_lite(axi_lite_item m_axi_item);
 	 extern virtual function void write_bram_a(bram_a_item m_bram_a_item);
 	 extern virtual function void write_bram_b(bram_b_item m_bram_b_item);
 	 extern virtual function void write_bram_c(bram_c_item m_bram_c_item);
   extern virtual function void check_phase(uvm_phase phase);

endclass;


function void conv_ip_scoreboard::build_phase(uvm_phase phase);
	super.build_phase(phase);
	m_axi_lite = new("m_axi_lite",this);
	m_bram_a = new("m_bram_a",this);
  m_bram_b = new("m_bram_b",this);
	m_bram_c = new("m_bram_c",this);

   // get configuration
  if(!uvm_config_db #(conv_ip_top_cfg)::get(this, "", "m_cfg", m_cfg)) begin
    `uvm_fatal(get_type_name(), "Failed to get configuration object from config DB!")
  end

endfunction: build_phase

function void conv_ip_scoreboard::write_axi_lite(axi_lite_item m_axi_item);

	$cast(axi_clone_item,m_axi_item.clone());	
  //cuvamo vrednost kolona u promenljivu sa istim imenom
  if(axi_clone_item.addr == COLUMNS_REG_ADDR) begin
   columns = axi_clone_item.data;
   `uvm_info(get_type_name(), $sformatf("COLUMNS : %d", columns), UVM_LOW)

 //cekiranje reset vrednosti -> postavljeno je polje u konfiguraciji samo za test u kome se citaju reset vrednosti i ovde se proverava da li su u redu
   if(m_cfg.reset_happend == 1 && axi_clone_item.read == 1) begin
    if(columns !== 0) begin
     `uvm_error(get_type_name(), $sformatf("Reset value of COLUMNS should be 0, but it is %d.",columns))
    end
    else begin 
     `uvm_info(get_type_name(), "Reset value for COLUMNS is 0.", UVM_LOW)
    end
   end
  end

  //cuvamo rednost linija u itoimenu promenljivu
  else if (axi_clone_item.addr == LINES_REG_ADDR) begin
   lines = axi_clone_item.data;
   `uvm_info(get_type_name(), $sformatf("LINES : %d", lines), UVM_LOW)

   //cekiranje reset vrednosti
    if(m_cfg.reset_happend == 1 && axi_clone_item.read == 1) begin
     if(lines !== 0) begin
      `uvm_error(get_type_name(), $sformatf("Reset value of COLUMNS should be 0, but it is %d.",lines))
     end
     else begin 
      `uvm_info(get_type_name(), "Reset value for LINES is 0.", UVM_LOW)
     end
    end
   end

  else if(axi_clone_item.addr == START_REG_ADDR) begin
   //cekiranje reset vrednosti
   if(m_cfg.reset_happend == 1 && axi_clone_item.read == 1) begin
    if(axi_clone_item.data !== 0) begin
     `uvm_error(get_type_name(), $sformatf("Reset value of START should be 0, but it is %d.",axi_clone_item.data))
    end
    else begin 
     `uvm_info(get_type_name(), "Reset value for START is 0.", UVM_LOW)
    end
   end
   //KONVOLUCIJA JE STARTOVANA
   if(axi_clone_item.data == 1) begin
     `uvm_info(get_type_name(), $sformatf("Convolution started"), UVM_LOW)
    start_happend = 1;
    start_count ++;
   end
  end

  else if(axi_clone_item.addr == READY_REG_ADDR) begin
   //cekiranje reset vrednosti
   if(m_cfg.reset_happend == 1 && axi_clone_item.read == 1) begin
     if(axi_clone_item.data !== 1) begin
      `uvm_error(get_type_name(), $sformatf("Reset value of READY should be 1, but it is %d.",axi_clone_item.data))
    end
    else begin 
     `uvm_info(get_type_name(), "Reset value for READY is 1.", UVM_LOW)
    end
   end
   if(axi_clone_item.data == 1) begin
    ready = 1;
   end
  end

//Ako pokusamo da pristupimo registru koji ne postoji
  if(axi_clone_item.addr == READY_REG_ADDR || axi_clone_item.addr == COLUMNS_REG_ADDR || axi_clone_item.addr == LINES_REG_ADDR || axi_clone_item.addr == START_REG_ADDR) begin
    `uvm_info(get_type_name(), $sformatf("AXI DATA SCOREBOARD: \n%s", axi_clone_item.sprint()), UVM_DEBUG)
  end
  else begin
   `uvm_error(get_type_name(), $sformatf("Register with the address of %d doesn't exist.",axi_clone_item.addr))
  end

//ispisivanje item-a za debug
`uvm_info(get_type_name(), $sformatf("AXI DATA SCOREBOARD: \n%s", axi_clone_item.sprint()), UVM_DEBUG)
endfunction:write_axi_lite

function void conv_ip_scoreboard::write_bram_a(bram_a_item m_bram_a_item);
	 $cast(bram_a_clone,m_bram_a_item.clone());
   bram_a_que.push_back(bram_a_clone.m_data_a_in[num_of_data_a]); //punim queue A sa podacima
   num_of_data_a++; //da bih mogla da ubacim po jedan podatak??

   //provera validnosti podatka (da li je enable na 1)
   if(bram_a_clone.en_a !== 1) begin 
    `uvm_error(get_type_name(), "Bram A data is not valid because en is at 0.")
   end
   else begin
    `uvm_info(get_type_name(), "Bram A data is valid.",UVM_LOW)
   end

//info za debug
  `uvm_info(get_type_name(), $sformatf("BRAM A SCOREBOARD: \n%s num: %d", bram_a_clone.sprint(), num_of_data_a), UVM_DEBUG)
  `uvm_info(get_type_name(), $sformatf("BRAM_A QUE: \n%p", bram_a_que), UVM_LOW)
	
endfunction: write_bram_a

function void conv_ip_scoreboard::write_bram_b(bram_b_item m_bram_b_item);
  $cast(bram_b_clone,m_bram_b_item.clone());
	bram_b_que.push_back(bram_b_clone.m_data_b_in);

//provera validnosti podatka (da li je enable na 1)
   if(bram_b_clone.en_b !== 1) begin 
    `uvm_error(get_type_name(), "Bram B data is not valid because en is at 0.")
   end
   else begin
    `uvm_info(get_type_name(), "Bram B data is valid.",UVM_LOW)
   end

//debug
	`uvm_info(get_type_name(), $sformatf("BRAM B SCOREBOARD: \n%s", bram_b_clone.sprint()), UVM_DEBUG)

//cekiranje adrese
  if(4*order_of_data_b == bram_b_clone.m_addr_b_out) begin
    `uvm_info(get_type_name(),"Address of B is okay !", UVM_LOW)
  end
  else begin
    `uvm_error(get_type_name(),$sformatf("Address of B is %d, it should be %d", bram_b_clone.m_addr_b_out, 4*order_of_data_b))
  end
  order_of_data_b++;
	
endfunction: write_bram_b

function void conv_ip_scoreboard::write_bram_c(bram_c_item m_bram_c_item);

 int unsigned data_a;
 int signed kernel_data;
 bit signed [2*WIDTH-1:0] data_c;
  $cast(bram_c_clone,m_bram_c_item.clone());

  //ako nisam primila start, a dobila sam rezultat -> error
 if(start_happend == 0) begin
   `uvm_error(get_type_name, "Convolution executed but wasn't started")
  end
  if(ready !== 1) begin
   `uvm_error(get_type_name(), "Ready wasn't at 1. Previous convolution was still in progress.")
  end
  //provera validnosti podatka (da li je enable na 1)
   if(bram_c_clone.en_c !== 1) begin 
    `uvm_error(get_type_name(), "Bram C data is not valid because en is at 0.")
   end
   else begin
    `uvm_info(get_type_name(), "Bram C data is valid.",UVM_LOW)
   end
//CEKIRANJE ADRESE
  if(4*count_of_data_c == bram_c_clone.m_addr_c) begin
    `uvm_info(get_type_name(),"Address of C is okay !", UVM_LOW)
  end
  else begin
    `uvm_error(get_type_name(),$sformatf("Address of C is %d, it should be %d", bram_c_clone.m_addr_c, 4*count_of_data_c))
  end
  count_of_data_c++;

// CEKIRANJE PODATAKA 
  repeat(KERNEL_SIZE*KERNEL_SIZE) begin
   data_a = bram_a_que.pop_front(); //izbaci iz queue po jedan clan pocevsi od prvog
   kernel_data = bram_b_que.pop_front();
   data_c = data_c + data_a * kernel_data; //pomnozimo A i B i dodamo na predhodni rezultat
   `uvm_info(get_type_name(), $sformatf("DATA A IS: %d", data_a), UVM_DEBUG)
   `uvm_info(get_type_name(), $sformatf("DATA B IS: %d", kernel_data), UVM_DEBUG)
   `uvm_info(get_type_name(), $sformatf("REZULTAT: %d", data_c), UVM_DEBUG)
  end	
  if(data_c > 255) begin //ako je veci od 255, stavi na 255
   data_c = 255;
  end
  else if( data_c < 0) begin //ako je manji od nule, stavi na 0
   data_c = 0;
  end
  if(data_c == bram_c_clone.m_data_c_out) begin
   `uvm_info(get_type_name(), "Expected and got data MATCH !", UVM_LOW)
  end
  else begin
   `uvm_error(get_type_name(),$sformatf("MISMATCH , expected : %d, got : %d!",data_c,bram_c_clone.m_data_c_out))
  end
  //sada su i A i B redovi prazni jer smo izbacili svih 9 clanova, ponovo ih punimo za sledeci rezultat C
  order_of_data_b = 0; //stalno se cita iz istih 9 adresa, pa resetujemo brojac
  if(count_of_data_c == ((columns-2)*(lines-2))) begin 
   count_of_data_c = 0;
   start_happend = 0;
  end
endfunction: write_bram_c

function void conv_ip_scoreboard::check_phase(uvm_phase phase);
	
	// check for bram a 
 if(start_count !== 0) begin
	assert_epmty_queue_bram_a_after_pop_front: assert (bram_a_que.size() == 0)
	  begin
	    s = "\n Queue bram_a is empty! \n";
	    // print message
	    `uvm_info(get_type_name(), $sformatf("%s", s), UVM_HIGH)
	  end else begin
	    s = $sformatf("\n Queue koji sadrzi podatke iz bram_a nije prazan, sadrzi %0d: \n" ,bram_a_que.size());
	    
	    // print message
      `uvm_error(get_type_name(), $sformatf("%s", s))
	    
	    foreach (bram_a_que[i]) begin
	      `uvm_info(get_type_name(), $sformatf("Transakcija [%d] :", i), UVM_LOW)
	      //bram_a_que.print();
	    end
	  end
	  
  // check for bram b 
	assert_epmty_queue_bram_b_after_pop_front: assert (bram_b_que.size() == 0)
	  begin
	    s = "\n Queue bram_b is empty! \n";
	    // print message
	    `uvm_info(get_type_name(), $sformatf("%s", s), UVM_HIGH)
	  end else begin
	    s = $sformatf("\n Queue koji sadrzi podatke iz bram_b nije prazan, sadrzi %0d: \n" ,bram_b_que.size());
	    
	    // print message
      `uvm_error(get_type_name(), $sformatf("%s", s))
	    
	    foreach (bram_b_que[i]) begin
	      `uvm_info(get_type_name(), $sformatf("Transakcija [%d] :", i), UVM_LOW)
	      //bram_b_que.print();
	    end
	  end
   `uvm_info( get_type_name(), $sformatf("Number of convolution is %d. ", start_count), UVM_LOW)
   start_count = 0; // vrati start_conut na 0 za sledecu konvoluciju
	 end
   else begin //ako nema starta
    `uvm_info(get_type_name(), "Convolution wasn't started.", UVM_LOW)
   end
endfunction: check_phase

`endif // CONV_IP_SCOREBOARD_SV
