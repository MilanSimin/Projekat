library IEEE;
use IEEE.std_logic_1164.all;
--use IEEE.std_logic_unsigned.all;
use work.utils_pkg.all;

entity mem_sub is
    generic (
        WIDTH: integer := 9; --data width of pixels
         MAX_SIZE: integer := 256 --maximum size of image
    );
    port(
        clk : in std_logic;
        reset : in std_logic;
        
        -- Interface to the AXI controllers
        reg_data_i : in std_logic_vector(7 downto 0);
        columns_wr_i : in std_logic;
        cmd_wr_i : in std_logic;
        lines_wr_i : in std_logic;
        
        
        lines_axi_o : out std_logic_vector(7 downto 0);
        columns_axi_o : out std_logic_vector(7 downto 0);
        cmd_axi_o : out std_logic;
        status_axi_o : out std_logic;
        
        columns_o : out std_logic_vector(7 downto 0);
        lines_o : out std_logic_vector(7 downto 0);
        start_o : out std_logic;
        ready_i : in std_logic );
        
end mem_sub;

architecture Behavioral of mem_sub is

    signal columns_s, lines_s : std_logic_vector(7 downto 0);
    signal cmd_s, status_s: std_logic;
    
    signal mem_data_wr_re_o, mem_data_wr_im_o : std_logic;
    signal mem_data_rd_re_o, mem_data_rd_im_o : std_logic;

    signal addr_s : std_logic_vector(15 downto 0);
    
begin
    columns_o <= columns_s;
    start_o <= cmd_s;
    lines_o <= lines_s;
    start_o <= cmd_s;
    ---------------------- REGISTERS ----------------------
    columns_axi_o <= columns_s;
    lines_axi_o <= lines_s;
    cmd_axi_o <= cmd_s;
    status_axi_o <= status_s;
    
    -- columns registar
    process(clk)
    begin
        if clk'event and clk = '1' then
            if reset = '1' then
                columns_s <= (others => '0');
            elsif columns_wr_i = '1' then
                columns_s <= reg_data_i (7 downto 0);
            end if;
        end if;
    end process;
    

    -- lines registar
    process(clk)
    begin
        if clk'event and clk = '1' then
            if reset = '1' then
                lines_s <= (others => '0');
            elsif lines_wr_i = '1' then
                lines_s <= reg_data_i (7 downto 0);
            end if;
        end if;
    end process;
    
    -- CMD register
    process(clk)
    begin
        if clk'event and clk = '1' then
            if reset = '1' then
                cmd_s <= '0';
            elsif cmd_wr_i = '1' then
                cmd_s <= reg_data_i(0);
            end if;
        end if;
    end process;
    
    -- STATUS register
    process(clk)
    begin
        if clk'event and clk = '1' then
            if reset = '1' then
                status_s <= '0';
            else
                status_s <= ready_i;
            end if;
        end if;
    end process;

end Behavioral;