library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use work.utils_pkg.all;


entity dp_memory is
     generic (
     DIMENSION: integer :=5;
     MAX_SIZE: integer := 256;
     WIDTH: integer := 8);
     
     port (
     clk: in std_logic;
     reset: in std_logic;
     
         -- Port 1 interface
         p1_addr_i: in std_logic_vector(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
         p1_data_i: in std_logic_vector(WIDTH-1 downto 0);
         p1_data_o: out std_logic_vector(WIDTH-1 downto 0);
         p1_wr_i: in std_logic;
         -- Port 2 interface
         p2_addr_i: in std_logic_vector(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
         p2_data_i: in std_logic_vector(WIDTH-1 downto 0);
         p2_data_o: out std_logic_vector(WIDTH-1 downto 0);
         p2_wr_i: in std_logic );
    end entity;
    
    architecture beh of dp_memory is
         type mem_t is array (0 to 2**log2c(MAX_SIZE*MAX_SIZE)-1) of
         std_logic_vector(WIDTH-1 downto 0);
         signal mem_s: mem_t;
    begin
     process (clk)
     begin
         if (clk'event and clk = '1') then
             if (reset = '1') then
                mem_s <= (others => (others => '0'));
             else
             if (p1_wr_i = '1') then
                mem_s(conv_integer(p1_addr_i)) <= p1_data_i;
             end if;
             if (p2_wr_i = '1') then
                mem_s(conv_integer(p2_addr_i)) <= p2_data_i;
             end if;
                 p1_data_o <= mem_s(conv_integer(p1_addr_i));
                 p2_data_o <= mem_s(conv_integer(p2_addr_i));
             end if;
         end if;
     end process;
end architecture beh;