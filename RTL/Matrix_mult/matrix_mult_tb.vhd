library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.utils_pkg.all;

entity matrix_mult_tb is
end entity;
architecture beh of matrix_mult_tb is
     constant MAX_SIZE_c: integer :=256;
     constant  DIMENSION: integer := 5;
     constant DATA_WIDTH_c: integer := 8;
     constant columns_c: integer := 4;
     constant lines_c: integer := 5;
     constant SIZE_c: integer := 3;
     type mem_t is array (0 to lines_c*columns_c-1) of
     std_logic_vector(DATA_WIDTH_c-1 downto 0);
     
     type mem_kernel is array (0 to SIZE_c*SIZE_c-1) of
          std_logic_vector(DATA_WIDTH_c-1 downto 0);

 constant MEM_A_CONTENT_c: mem_t :=
         (conv_std_logic_vector(1, DATA_WIDTH_c),
         conv_std_logic_vector(2, DATA_WIDTH_c),
         conv_std_logic_vector(3, DATA_WIDTH_c),
         conv_std_logic_vector(4, DATA_WIDTH_c),
         conv_std_logic_vector(5, DATA_WIDTH_c),
         conv_std_logic_vector(6, DATA_WIDTH_c),
         conv_std_logic_vector(7, DATA_WIDTH_c),
         conv_std_logic_vector(8, DATA_WIDTH_c),
         conv_std_logic_vector(9, DATA_WIDTH_c),
         conv_std_logic_vector(10, DATA_WIDTH_c),
         conv_std_logic_vector(11, DATA_WIDTH_c),
         conv_std_logic_vector(12, DATA_WIDTH_c),
         conv_std_logic_vector(13, DATA_WIDTH_c),
         conv_std_logic_vector(14, DATA_WIDTH_c),
         conv_std_logic_vector(15, DATA_WIDTH_c),
         conv_std_logic_vector(16, DATA_WIDTH_c),
         conv_std_logic_vector(17, DATA_WIDTH_c),
         conv_std_logic_vector(18, DATA_WIDTH_c),
         conv_std_logic_vector(19, DATA_WIDTH_c),
         conv_std_logic_vector(20, DATA_WIDTH_c));
 
 
 constant MEM_B_CONTENT_c: mem_kernel := 
        ( conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c),
          conv_std_logic_vector(1, DATA_WIDTH_c));
  
  
          signal clk_s: std_logic;
          signal reset_s: std_logic;
          
          signal columns_s: std_logic_vector(log2c(MAX_SIZE_c)-1 downto 0);
          signal lines_s: std_logic_vector(log2c(MAX_SIZE_c)-1 downto 0);

          -- Matrix A memory interface
          signal mem_a_addr_s: std_logic_vector(log2c(MAX_SIZE_c*MAX_SIZE_c)-1 downto 0);
          signal mem_a_data_in_s: std_logic_vector(DATA_WIDTH_c-1 downto 0);
          signal mem_a_wr_s: std_logic;
          signal a_addr_s: std_logic_vector(log2c(MAX_SIZE_c*MAX_SIZE_c)-1 downto 0);
          signal a_data_in_s: std_logic_vector(DATA_WIDTH_c-1 downto 0);
          signal a_wr_s: std_logic;
          
          -- Matrix B memory interface
          signal mem_b_addr_s: std_logic_vector(log2c(SIZE_c*SIZE_c)-1 downto 0);
          signal mem_b_data_in_s: std_logic_vector(DATA_WIDTH_c-1 downto 0);
          signal mem_b_wr_s: std_logic;
          signal b_addr_s: std_logic_vector(log2c(SIZE_c*SIZE_c)-1 downto 0);
          signal b_data_in_s: std_logic_vector(DATA_WIDTH_c-1 downto 0);
          signal b_wr_s: std_logic;
          
          -- Matrix C memory interface
          signal c_addr_s: std_logic_vector(log2c(MAX_SIZE_c*MAX_SIZE_c)-1 downto 0);
          signal c_data_out_s: std_logic_vector(2*DATA_WIDTH_c + MAX_SIZE_c downto 0);
          signal c_wr_s: std_logic;
          signal start_s: std_logic := '0';
          signal ready_s: std_logic;
  
  
  
 begin
          clk_gen: process
          begin
          clk_s <= '0', '1' after 100 ns;
          wait for 200 ns;
          end process;
          
      stim_gen: process
      begin
      -- Apply system level reset
      reset_s <= '1';
      wait for 500 ns;
      reset_s <= '0';
      wait until falling_edge(clk_s);
      
      -- Load the data into the matrix A memory
      mem_a_wr_s <= '1';
      for i in 0 to columns_c-1 loop
      for j in 0 to lines_c-1 loop
      mem_a_addr_s <= conv_std_logic_vector(i*lines_c+j, mem_a_addr_s'length);
      mem_a_data_in_s <= MEM_A_CONTENT_c(i*lines_c+j);
      
      wait until falling_edge(clk_s);
       end loop;
       end loop;
       mem_a_wr_s <= '0';
       
       
       -- Load the data into the matrix B memory
       mem_b_wr_s <= '1';
       for i in 0 to SIZE_c-1 loop
       for j in 0 to SIZE_c-1 loop
       mem_b_addr_s <= conv_std_logic_vector(i*SIZE_c+j, mem_b_addr_s'length);
       mem_b_data_in_s <= MEM_B_CONTENT_c(i*SIZE_c+j);
       wait until falling_edge(clk_s);
       end loop;
       end loop;
       mem_b_wr_s <= '0';
      
       -- Start the multiplication process
       start_s <= '1';
       wait until falling_edge(clk_s);
       start_s <= '0';
      
       -- Wait until matrix multiplication module signals operation has been complted
       wait until ready_s = '1';
      
       -- End stimulus generation
       wait;
       end process;
       
       
       -- Matrix A memory
       matrix_a_mem: entity work.dp_memory(beh)
       generic map (
       MAX_SIZE => MAX_SIZE_c,
       DIMENSION => DIMENSION,
       WIDTH => DATA_WIDTH_c)
       port map (
       clk => clk_s,
       reset => reset_s,
       p1_addr_i => mem_a_addr_s,
       p1_data_i => mem_a_data_in_s,
       p1_data_o => open,
       p1_wr_i => mem_a_wr_s,
       p2_addr_i => a_addr_s,
       p2_data_i => (others => '0'),
       p2_data_o => a_data_in_s,
       p2_wr_i => a_wr_s);
       
       
       -- Matrix B memory
       matrix_b_mem: entity work.dp_memory_small(beh)
       generic map (
       WIDTH => DATA_WIDTH_c,
       SIZE => SIZE_c)
       port map (
       clk => clk_s,
       reset => reset_s,
       p1_addr_i => mem_b_addr_s,
       p1_data_i => mem_b_data_in_s,
       p1_data_o => open,
       p1_wr_i => mem_b_wr_s,
       p2_addr_i => b_addr_s,
       p2_data_i => (others => '0'),
       p2_data_o => b_data_in_s,
       p2_wr_i => b_wr_s);
             
             
       -- Matrix C memory
       matrix_c_mem: entity work.dp_memory(beh)
       generic map
      (
       MAX_SIZE => MAX_SIZE_c,
       WIDTH => 2*DATA_WIDTH_c+MAX_SIZE_c+1,
       DIMENSION => DIMENSION
       )
       port map
      (
       clk => clk_s,
       reset => reset_s,
       p1_addr_i => (others => '0'),
       p1_data_i => (others => '0'),
       p1_data_o => open,
       p1_wr_i => '0',
       p2_addr_i => c_addr_s,
       p2_data_i => c_data_out_s,--ovde je problem , ocekuje 21 dobija 25
       p2_data_o => open,
       p2_wr_i => c_wr_s);
       
   
       -- DUT
        columns_s <= conv_std_logic_vector(columns_c, log2c(MAX_SIZE_c));
        lines_s <= conv_std_logic_vector(lines_c, log2c(MAX_SIZE_c));
      
       matrix_multiplication_core: entity work.matrix_mult(two_seg_arch)
       
       generic map
      (
       MAX_SIZE => MAX_SIZE_c,
       WIDTH => DATA_WIDTH_c,
       DIMENSION => DIMENSION,
       SIZE => SIZE_c)
       port map
      (
       --------------- Clocking and reset interface ---------------
       clk => clk_s,
       reset => reset_s,
       ------------------- Input data interface -------------------
       -- Matrix A memory interface
       a_addr_o => a_addr_s,
       a_data_i => a_data_in_s,
       a_wr_o => a_wr_s,
       -- Matrix B memory interface
       b_addr_o => b_addr_s,
       b_data_i => b_data_in_s,
       b_wr_o => b_wr_s,
       -- Matrix dimensions definition interface
       columns => columns_s,
       lines => lines_s,
       
       ------------------- Output data interface ------------------
       -- Matrix C memory interface
       c_addr_o => c_addr_s,
       c_data_o => c_data_out_s,
       c_wr_o => c_wr_s,
       --------------------- Command interface --------------------
       start => start_s,
       --------------------- Status interface ---------------------
       ready => ready_s);
      end architecture beh;