library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.utils_pkg.all;

entity IP_testbench is
end entity;

architecture beh of IP_testbench is
    
    constant DATA_WIDTH_c : integer := 9;
	constant ADDR_WIDTH_c : integer := 32;
    constant MAX_SIZE_c : integer := 256;
    constant SIZE_c : integer :=3 ;
    constant WADDR : integer := 32;
    constant WADDR_B : integer := 8;

    
    
    
    ---------------------- TEST 1-----------------------
    --RESULTS: 54, 63, 90, 99
    --constant columns_c : integer := 4;
    --constant lines_c : integer := 4;
    --type mem_t is array (0 to columns_c*lines_c-1) of integer;
    --type mem_kernel is array (0 to SIZE_c*SIZE_c-1) of integer;
    --constant MEM_A_CONTENT_c: mem_t :=
      --  (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    
    
    --constant MEM_B_CONTENT_c: mem_kernel := 
      --  ( 1,1,1,1,1,1,1,1,1);
        
        
    ---------------------- TEST 2-----------------------
    --RESULTS: 81, 90, 99, 108, 117, 144, 153, 162, 171, 180
    constant columns_c : integer := 7;
    constant lines_c : integer := 4;
    type mem_t is array (0 to columns_c*lines_c-1) of integer;
    type mem_kernel is array (0 to SIZE_c*SIZE_c-1) of integer;  
    
    constant MEM_A_CONTENT_c: mem_t :=
    (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28);
    
    
    constant MEM_B_CONTENT_c: mem_kernel := 
    ( 1,1,1,1,1,1,1,1,1);        
        
    ---------------------TEST 3------------------------
    --RESULTS: 255, 255, 255, 0, 0, 0, 10, 82, 164, 0, 0, 0
    --constant columns_c : integer := 5;
    --constant lines_c : integer := 6;
    --type mem_t is array (0 to columns_c*lines_c-1) of integer;
    --type mem_kernel is array (0 to SIZE_c*SIZE_c-1) of integer;  
    
    --constant MEM_A_CONTENT_c: mem_t :=
    --(11,2,3,24,5,6,77,86,97,100,51,12,43,24,75,63,37,18,19,220,71,22,6,155,215,132,27,96,89,130);
    
    
    --constant MEM_B_CONTENT_c: mem_kernel := 
    --(-1,-1,-1,-1,8, -1,-1,-1,-1);  
        
        
------------------------------------------------------------------

    signal clk_s: std_logic;
    signal reset_s: std_logic;
    
    
    ---------------------- CONV_IP core's address map-------------------------
    constant COLUMNS_REG_ADDR_C : integer := 0;
    constant LINES_REG_ADDR_C : integer := 4;
    constant CMD_REG_ADDR_C : integer := 8;
    constant STATUS_REG_ADDR_C : integer := 12;
	
    
    ---------------------- Ports for BRAM Initialization ----------------------  
    signal tb_a_en_i      : std_logic;
    signal tb_a_addr_i    : std_logic_vector(ADDR_WIDTH_c-1 downto 0);
    signal tb_a_data_i    : std_logic_vector(DATA_WIDTH_c-1 downto 0);
    signal tb_a_we_i      : std_logic;
	
	signal tb_b_en_i      : std_logic;
    signal tb_b_addr_i    : std_logic_vector(7 downto 0);
    signal tb_b_data_i    : std_logic_vector(DATA_WIDTH_c-1 downto 0);
    signal tb_b_we_i      : std_logic; 
	
	signal tb_c_en_i      : std_logic;
    signal tb_c_addr_i    : std_logic_vector(ADDR_WIDTH_c-1 downto 0);
	signal tb_c_data_o    : std_logic_vector(2*DATA_WIDTH_c-1 downto 0);
    signal tb_c_we_i      : std_logic; 
    
    ------------------------- Ports to CONV_IP ---------------------------------
    signal ip_a_en     : std_logic;
    signal ip_a_we     : std_logic;
    signal ip_a_addr    : std_logic_vector(ADDR_WIDTH_c-1 downto 0);
    signal ip_a_data    : std_logic_vector(DATA_WIDTH_c-1 downto 0);
	
	signal ip_b_en      : std_logic;
    signal ip_b_addr    : std_logic_vector(7 downto 0);
    signal ip_b_data    : std_logic_vector(DATA_WIDTH_c-1 downto 0);
    signal ip_b_we      : std_logic; 
	
	signal ip_c_en      : std_logic;
    signal ip_c_addr    : std_logic_vector(ADDR_WIDTH_c-1 downto 0);
	signal ip_c_data    : std_logic_vector(2*DATA_WIDTH_c-1 downto 0);
    signal ip_c_we      : std_logic; 
    
    
    
    ------------------- AXI Interfaces signals -------------------
    -- Parameters of Axi-Lite Slave Bus Interface S00_AXI
    constant C_S00_AXI_DATA_WIDTH_c : integer := 32;
    constant C_S00_AXI_ADDR_WIDTH_c : integer := 4;
    
    -- Ports of Axi-Lite Slave Bus Interface S00_AXI
    signal s00_axi_aclk_s : std_logic := '0';
    signal s00_axi_aresetn_s : std_logic := '1';
    signal s00_axi_awaddr_s : std_logic_vector(C_S00_AXI_ADDR_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_awprot_s : std_logic_vector(2 downto 0) := (others => '0');
    signal s00_axi_awvalid_s : std_logic := '0';
    signal s00_axi_awready_s : std_logic := '0';
    signal s00_axi_wdata_s : std_logic_vector(C_S00_AXI_DATA_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_wstrb_s : std_logic_vector((C_S00_AXI_DATA_WIDTH_c/8)-1 downto 0) := (others => '0');
    signal s00_axi_wvalid_s : std_logic := '0';
    signal s00_axi_wready_s : std_logic := '0';
    signal s00_axi_bresp_s : std_logic_vector(1 downto 0) := (others => '0');
    signal s00_axi_bvalid_s : std_logic := '0';
    signal s00_axi_bready_s : std_logic := '0';
    signal s00_axi_araddr_s : std_logic_vector(C_S00_AXI_ADDR_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_arprot_s : std_logic_vector(2 downto 0) := (others => '0');
    signal s00_axi_arvalid_s : std_logic := '0';
    signal s00_axi_arready_s : std_logic := '0';
    signal s00_axi_rdata_s : std_logic_vector(C_S00_AXI_DATA_WIDTH_c-1 downto 0) := (others => '0');
    signal s00_axi_rresp_s : std_logic_vector(1 downto 0) := (others => '0');
    signal s00_axi_rvalid_s : std_logic := '0';
    signal s00_axi_rready_s : std_logic := '0';

       
begin
 reset_s <= not s00_axi_aresetn_s; --reset for BRAM

clk_gen: process is
    begin
        clk_s <= '0', '1' after 100 ns;
        wait for 200 ns;
    end process;

stimulus_generator: process
    variable axi_read_data_v : std_logic_vector(31 downto 0);
    variable transfer_size_v : integer;
begin
    report "Start !";

    -- reset AXI-lite interface. Reset will be 10 clock cycles wide
    s00_axi_aresetn_s <= '0';
    -- wait for 5 falling edges of AXI-lite clock signal
    for i in 1 to 5 loop
        wait until falling_edge(clk_s);
    end loop;
    -- release reset
    s00_axi_aresetn_s <= '1';
    wait until falling_edge(clk_s);
    
    report "Loading the matrix dimensions information into the CONV_IP core!";
    
    ----------------------------------------------------------------------
    -- Initialize the Matrix Multiplier core --
    ----------------------------------------------------------------------
    report "Loading the matrix dimensions information into the Matrix Multiplier core!";
 
    -- Set the value for COLUMNS
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(COLUMNS_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '1';
    s00_axi_wdata_s <= conv_std_logic_vector(columns_c, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '1';
    s00_axi_wstrb_s <= "1111";
    s00_axi_bready_s <= '1';
    wait until s00_axi_awready_s = '1';
    wait until s00_axi_awready_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(0, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '0';
    s00_axi_wdata_s <= conv_std_logic_vector(0, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '0';
    s00_axi_wstrb_s <= "0000";
    wait until s00_axi_bvalid_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_bready_s <= '0';
    wait until falling_edge(clk_s); 
    
    -- wait for 5 falling edges of AXI-lite clock signal
    for i in 1 to 5 loop
        wait until falling_edge(clk_s);
    end loop;
   
    
    -- Set the value for LINES
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(LINES_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '1';
    s00_axi_wdata_s <= conv_std_logic_vector(lines_c, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '1';
    s00_axi_wstrb_s <= "1111";
    s00_axi_bready_s <= '1';
    wait until s00_axi_awready_s = '1';
    wait until s00_axi_awready_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(0, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '0';
    s00_axi_wdata_s <= conv_std_logic_vector(0, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '0';
    s00_axi_wstrb_s <= "0000";
    wait until s00_axi_bvalid_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_bready_s <= '0';
    wait until falling_edge(clk_s);
    
    -- wait for 5 falling edges of AXI-lite clock signal
    for i in 1 to 5 loop
        wait until falling_edge(clk_s);
    end loop;


    -------------------------------------------------------------------------------------------
    -- Load the element values into the A Matrix  --
    -------------------------------------------------------------------------------------------
	transfer_size_v := columns_c*lines_c;
    report "Loading matrix A elements values into the core!";
    wait until falling_edge(clk_s);
    
    for i in 0 to (columns_c)*(lines_c)-1 loop
    
        wait until falling_edge(clk_s);
        tb_a_en_i <= '1';
        tb_a_addr_i <= conv_std_logic_vector( i*4 , ADDR_WIDTH_c); --STAVLJAMO *4 ZBOG BRAMA
        tb_a_data_i <= conv_std_logic_vector(MEM_A_CONTENT_c(i), DATA_WIDTH_c);
        tb_a_we_i <= '1';
        
        for i in 1 to 3 loop
            wait until falling_edge(clk_s);
        end loop;
        tb_a_en_i <= '0';
        tb_a_we_i <= '0';
    end loop;
    tb_a_en_i <= '0';
    tb_a_we_i <= '0';
    
    -------------------------------------------------------------------------------------------
    -- Load the element values into the B Matrix  --
    -------------------------------------------------------------------------------------------
    report "Loading matrix B elements values into the core!";
    
	  for j in 0 to (SIZE_c)*(SIZE_c)-1 loop
	  
        wait until falling_edge(clk_s);
        tb_b_en_i <= '1';
        tb_b_we_i <= '1';
        tb_b_addr_i <= conv_std_logic_vector( j*4 , WADDR_B ); --ISTO *4 ZBOG BRAMA
		tb_b_data_i <= conv_std_logic_vector(MEM_B_CONTENT_c(j), DATA_WIDTH_c);
        
        for i in 1 to 3 loop
            wait until falling_edge(clk_s);
        end loop;
        tb_b_en_i <= '0';
        tb_b_we_i <= '0';
    end loop;
    tb_b_en_i <= '0';
    tb_b_we_i <= '0';
    
    -------------------------------------------------------------------------------------------
    -- Start the Matrix Multiplier core --
    -------------------------------------------------------------------------------------------
    report "Starting the convolution process!";
    -- Set the start bit (bit 0 in the CMD register) to 1
    
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(CMD_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '1';
    s00_axi_wdata_s <= conv_std_logic_vector(1, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '1';
    s00_axi_wstrb_s <= "1111";
    s00_axi_bready_s <= '1';
    wait until s00_axi_awready_s = '1';
    wait until s00_axi_awready_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(0, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '0';
    s00_axi_wdata_s <= conv_std_logic_vector(0, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '0';
    s00_axi_wstrb_s <= "0000";
    wait until s00_axi_bvalid_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_bready_s <= '0';
    wait until falling_edge(clk_s);
    
    
    -- wait for 5 falling edges of AXI-lite clock signal
    for i in 1 to 5 loop
        wait until falling_edge(clk_s);
    end loop;


    report "Clearing the start bit!";
    -- Set the start bit (bit 0 in the CMD register) to 0
    
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(CMD_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '1';
    s00_axi_wdata_s <= conv_std_logic_vector(0, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '1';
    s00_axi_wstrb_s <= "1111";
    s00_axi_bready_s <= '1';
    wait until s00_axi_awready_s = '1';
    wait until s00_axi_awready_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_awaddr_s <= conv_std_logic_vector(0, C_S00_AXI_ADDR_WIDTH_c);
    s00_axi_awvalid_s <= '0';
    s00_axi_wdata_s <= conv_std_logic_vector(0, C_S00_AXI_DATA_WIDTH_c);
    s00_axi_wvalid_s <= '0';
    s00_axi_wstrb_s <= "0000";
    wait until s00_axi_bvalid_s = '0';
    wait until falling_edge(clk_s);
    s00_axi_bready_s <= '0';
    wait until falling_edge(clk_s);
    
    -------------------------------------------------------------------------------------------
    -- Wait until CONV_IP core finishes convolution --
    -------------------------------------------------------------------------------------------
    report "Waiting for the convolution process to complete!";
    loop
        -- Read the content of the Status register
        wait until falling_edge(clk_s);
        s00_axi_araddr_s <= conv_std_logic_vector(STATUS_REG_ADDR_C, C_S00_AXI_ADDR_WIDTH_c);
        s00_axi_arvalid_s <= '1';
        s00_axi_rready_s <= '1';
        wait until s00_axi_arready_s = '1';
        wait until s00_axi_arready_s = '0';
        wait until falling_edge(clk_s);
        s00_axi_araddr_s <= conv_std_logic_vector(0, C_S00_AXI_ADDR_WIDTH_c);
        s00_axi_arvalid_s <= '0';
        s00_axi_rready_s <= '0';
        
        -- Check is the 1st bit of the Status register set to one
        if (s00_axi_rdata_s(0) = '1') then
            -- convolution process completed
            exit;
        else
            wait for 1000 ns;
        end if;
    end loop;

    -------------------------------------------------------------------------------------------
    -- Read the elements of matrix C--
    -------------------------------------------------------------------------------------------
    report "Reading the results of the C matrix!";

	for k in 0 to (columns_c-2)*(lines_c-2)-1 loop
        wait until falling_edge(clk_s);
        tb_c_en_i <= '1';
		tb_c_we_i<= '0';
        tb_c_addr_i <= conv_std_logic_vector ( k*4 , ADDR_WIDTH_c);
        
        for i in 1 to 3 loop
            wait until falling_edge(clk_s);
        end loop;
    end loop;
    
    tb_c_en_i <= '0';
    report "Finished!";
    wait;
end process;

---------------------------------------------------------------------------
---- DUT --
---------------------------------------------------------------------------
uut: entity work.image_convolution_new_v1_0(arch_imp)
    generic map(
        WIDTH => DATA_WIDTH_c,
        MAX_SIZE => MAX_SIZE_c,
        SIZE => SIZE_c,
        ADDR_WIDTH => ADDR_WIDTH_c
        )
    port map (
        -- Interface to the BRAM A module
        ena     => ip_a_en,
        wea     => ip_a_we,
        addra   => ip_a_addr,
        dina    => open,
        douta   => ip_a_data,
        reseta  => open,
        clka    => open,
        
        -- Interface to the BRAM B module
        
        enb     => ip_b_en,
        web     => ip_b_we,
        addrb   => ip_b_addr,
        dinb    => open,
        doutb   => ip_b_data,
        resetb  => open,
        clkb    => open,
        
        --Interface to the BRAM C module
        enc     => ip_c_en,
        wec     => ip_c_we,
        addrc   => ip_c_addr,
        dinc    => ip_c_data,
        doutc   =>(others=>'0'),
        resetc  => open,
        clkc    => open,

        -- Ports of Axi Slave Bus Interface S00_AXI
        s00_axi_aclk    => clk_s,
        s00_axi_aresetn => s00_axi_aresetn_s,
        s00_axi_awaddr  => s00_axi_awaddr_s,
        s00_axi_awprot  => s00_axi_awprot_s, 
        s00_axi_awvalid => s00_axi_awvalid_s,
        s00_axi_awready => s00_axi_awready_s,
        s00_axi_wdata   => s00_axi_wdata_s,
        s00_axi_wstrb   => s00_axi_wstrb_s,
        s00_axi_wvalid  => s00_axi_wvalid_s,
        s00_axi_wready  => s00_axi_wready_s,
        s00_axi_bresp   => s00_axi_bresp_s,
        s00_axi_bvalid  => s00_axi_bvalid_s,
        s00_axi_bready  => s00_axi_bready_s,
        s00_axi_araddr  => s00_axi_araddr_s,
        s00_axi_arprot  => s00_axi_arprot_s,
        s00_axi_arvalid => s00_axi_arvalid_s,
        s00_axi_arready => s00_axi_arready_s,
        s00_axi_rdata   => s00_axi_rdata_s,
        s00_axi_rresp   => s00_axi_rresp_s,
        s00_axi_rvalid  => s00_axi_rvalid_s,
        s00_axi_rready  => s00_axi_rready_s);
        
Bram_A: entity work.bram(beh)
    generic map(
                WIDTH       =>  DATA_WIDTH_c,
                MAX_SIZE    =>  MAX_SIZE_c,
                WADDR       =>  WADDR
                )
    port map(
               clka => clk_s,
               clkb => clk_s,
	           reseta=> reset_s,
	           ena=>tb_a_en_i,
	           wea=> tb_a_we_i,
	           addra=> tb_a_addr_i,
	           dia=> tb_a_data_i,
	           doa=> open,
	
	           resetb=>reset_s,
	           enb=>ip_a_en,
	           web=>ip_a_we,
	           addrb=>ip_a_addr,
	           dib=>(others=>'0'),
	           dob=> ip_a_data
	        );
	        
Bram_B: entity work.bram(beh)
    generic map(
                WIDTH       =>  DATA_WIDTH_c,
                MAX_SIZE    =>  MAX_SIZE_c,
                WADDR       =>  WADDR_B
                )
    port map(
               clka => clk_s,
               clkb => clk_s,
               reseta=> reset_s,
	           ena=>tb_b_en_i,
	           wea=> tb_b_we_i,
	           addra=> tb_b_addr_i,
	           dia=> tb_b_data_i,
	           doa=> open,
	
	           resetb=>reset_s,
	           enb=>ip_b_en,
	           web=>ip_b_we,
	           addrb=>ip_b_addr,
	           dib=>(others=>'0'),
	           dob=> ip_b_data
	        );    
	        
Bram_C: entity work.bram(beh)
    generic map(
                WIDTH       =>  2*DATA_WIDTH_c,
                MAX_SIZE    =>  MAX_SIZE_c,
                WADDR       =>  WADDR
                )
    port map(
               clka => clk_s,
               clkb => clk_s,
	           reseta=> reset_s,
	           ena=>tb_c_en_i,
	           wea=> tb_c_we_i,
	           addra=> tb_c_addr_i,
	           dia=> (others=>'0'),
	           doa=> open,
	
	           resetb=>reset_s,
	           enb=>ip_c_en,
	           web=>ip_c_we,
	           addrb=>ip_c_addr,
	           dib=>ip_c_data,
	           dob=>open
	        );    
end architecture beh;