library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity image_convolution_new_v1_0 is
	generic (
		-- Users to add parameters here
        WIDTH : integer := 9;
        MAX_SIZE : integer := 256;
        SIZE : integer := 3;
        ADDR_WIDTH : integer := 32;
		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 4
	);
	port (
		-- Users to add ports here
        -- Interface to the BRAM A matrix module
		clka      : out std_logic;
        reseta    : out std_logic;
        ena       : out STD_LOGIC; 
        addra     : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0); 
        dina      : out STD_LOGIC_VECTOR (WIDTH-1 downto 0);
        douta     : in STD_LOGIC_VECTOR (WIDTH-1 downto 0);
        wea       : out STD_LOGIC;
        
        -- Interface to the BRAM B matrix module
		clkb      : out std_logic;
        resetb    : out std_logic;
        enb       : out STD_LOGIC;
        addrb     : out STD_LOGIC_VECTOR (7 downto 0); 
        dinb      : out STD_LOGIC_VECTOR (WIDTH-1 downto 0);
        doutb     : in STD_LOGIC_VECTOR (WIDTH-1 downto 0);
        web       : out STD_LOGIC;
        
        -- Interface to the BRAM C matrix module
		clkc      : out std_logic;
        resetc    : out std_logic;
        enc       : out STD_LOGIC;
        addrc     : out STD_LOGIC_VECTOR (ADDR_WIDTH-1 downto 0); 
        dinc      : out STD_LOGIC_VECTOR (2*WIDTH-1 downto 0);
        doutc     : in STD_LOGIC_VECTOR (2*WIDTH-1 downto 0);
        wec       : out STD_LOGIC;
        
        
		-- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
end image_convolution_new_v1_0;

architecture arch_imp of image_convolution_new_v1_0 is
        signal reset_s : std_logic;
        
        ------------------ AXI Lite Interface ---------------------
        signal s_reg_data_o : std_logic_vector(7 downto 0);
        signal s_columns_wr_o : std_logic;
        signal s_lines_wr_o : std_logic;
        signal s_cmd_wr_o : std_logic;
                
        signal s_columns_axi_i : std_logic_vector(7 downto 0);
        signal s_lines_axi_i : std_logic_vector(7 downto 0);
        signal s_cmd_axi_i : std_logic;
        signal s_status_axi_i : std_logic;
        
        ------------------------- Interface to matrix_mult Module ----------------------------
        signal s_a_addr_o:  std_logic_vector(ADDR_WIDTH-1 downto 0);
        signal s_a_data_i:  std_logic_vector(WIDTH-1 downto 0);
        signal s_a_wr_o:  std_logic;
        
        signal s_b_addr_o:  std_logic_vector(7 downto 0);
        signal s_b_data_i:  std_logic_vector(WIDTH-1 downto 0);
        signal s_b_wr_o:  std_logic;
        
        signal s_c_addr_o:  std_logic_vector(ADDR_WIDTH-1 downto 0);
        signal s_c_data_o:  std_logic_vector(2*WIDTH-1 downto 0);
        signal s_c_wr_o:  std_logic;
        
        signal s_columns : STD_LOGIC_VECTOR (7 downto 0);
        signal s_lines : STD_LOGIC_VECTOR (7 downto 0);
        
        signal s_start : STD_LOGIC;
        signal s_ready : STD_LOGIC;
		----------------------signals to BRAM interface------------------
		signal ena_s: STD_LOGIC;	
		signal enb_s: STD_LOGIC;
		signal enc_s: STD_LOGIC;
		
		signal doa_s: STD_LOGIC_VECTOR(WIDTH-1 downto 0);
		signal dob_s: STD_LOGIC_VECTOR(WIDTH-1 downto 0);
		signal doc_s: STD_LOGIC_VECTOR(2*WIDTH-1 downto 0);
begin

 reset_s <= not s00_axi_aresetn;
-- Instantiation of Axi Bus Interface S00_AXI
AXI_LITE_inst : entity work.image_conv_new_v1_0_S00_AXI(arch_imp)
	generic map (
	    WIDTH => WIDTH,
	    MAX_SIZE => MAX_SIZE,  
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
	    reg_data_o => s_reg_data_o,
        columns_wr_o => s_columns_wr_o,
        lines_wr_o => s_lines_wr_o,
        cmd_wr_o => s_cmd_wr_o,
        
        columns_axi_i => s_columns_axi_i,
        lines_axi_i => s_lines_axi_i,	   
	    cmd_axi_i => s_cmd_axi_i,
	    status_axi_i => s_status_axi_i,
	
	
		S_AXI_ACLK	=> s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	=> s00_axi_wdata,
		S_AXI_WSTRB	=> s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	=> s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	=> s00_axi_rdata,
		S_AXI_RRESP	=> s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready
	);

	-- Add user logic here
    mem_subsystem_inst: entity work.mem_sub(Behavioral)
        generic map(
            WIDTH => WIDTH,
            MAX_SIZE => MAX_SIZE
        )
        port map(
            clk   => s00_axi_aclk,
            reset => reset_s,
            
            -- Interface to the AXI controllers
            reg_data_i  => s_reg_data_o,
            columns_wr_i  => s_columns_wr_o,
            lines_wr_i => s_lines_wr_o,
            cmd_wr_i    => s_cmd_wr_o,
            
            
            columns_axi_o  => s_columns_axi_i,
            lines_axi_o => s_lines_axi_i,
            cmd_axi_o    => s_cmd_axi_i,
            status_axi_o => s_status_axi_i,
            
            -- Interface to the matrix multiply module
            columns_o => s_columns,
            lines_o => s_lines,
            start_o  => s_start,
            ready_i  => s_ready
         );
         
         
             
         matrix_mult_inst: entity work.conv_ip(beh)
             generic map ( 
                    WIDTH        => WIDTH,
                    MAX_SIZE     => MAX_SIZE,
                    SIZE         => SIZE,
                    ADDR_WIDTH   => ADDR_WIDTH
             )
             port map ( 
                    clk => s00_axi_aclk,
                    reset => reset_s,
                    
                    a_data_i => douta,
                    a_addr_o => addra,
                    a_en_o => ena,
                    
                    b_data_i => doutb,
                    b_addr_o => addrb,
                    b_en_o => enb,
                    
                    c_data_o => dinc,
                    c_addr_o => addrc,
                    c_wr_o => wec,
                    c_en_o => enc,
                    
                    columns => s_columns,
                    lines => s_lines,
                    
                    start => s_start,
                    ready => s_ready);
             
	    clka   <= s00_axi_aclk;
		clkb   <= s00_axi_aclk;
		clkc   <= s00_axi_aclk;
		reseta <= reset_s;
		resetb <= reset_s;
		resetc <= reset_s;
		wea    <= '0';
		web    <= '0';
		--addra  <= s_a_addr_o;
		--addrb  <= s_b_addr_o;
		--addrc  <= s_c_addr_o;
		dina   <= (others=>'0');
		dinb   <= (others=>'0');
	
					
end arch_imp;