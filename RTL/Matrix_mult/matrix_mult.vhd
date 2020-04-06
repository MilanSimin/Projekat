library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.utils_pkg.all;

entity matrix_mult is
generic (
     WIDTH: integer := 9; --data width of pixels
     SIZE: integer := 3; --for kernel size
	 MAX_SIZE: integer := 256 --maximum size of image
	 
);
port (
--------------- Clocking and reset interface ---------------
    
    clk: in std_logic;
    reset: in std_logic;
------------------- Input data interface -------------------
-- Matrix A memory interface
-- image matrix columns*lines (MAX_SIZE*MAX_SIZE)
    a_addr_o: out std_logic_vector(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
    a_data_i: in std_logic_vector(WIDTH-1 downto 0);
    a_wr_o: out std_logic;
-- Matrix B memory interface
-- kernel matrix 3x3
    b_addr_o: out std_logic_vector(log2c(SIZE*SIZE)-1 downto 0);
    b_data_i: in std_logic_vector(WIDTH-1 downto 0);
    b_wr_o: out std_logic;
--Dimension of the image 
    columns: in std_logic_vector(log2c(MAX_SIZE)-1 downto 0);
    lines: in std_logic_vector(log2c(MAX_SIZE)-1 downto 0);
------------------- Output data interface ------------------
-- Matrix C memory interface after convolution
    c_addr_o: out std_logic_vector(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
    c_data_o: out std_logic_vector(2*WIDTH-1 downto 0);
    c_wr_o: out std_logic;
--------------------- Command interface --------------------
    start: in std_logic;
--------------------- Status interface ---------------------
    ready: out std_logic);
end entity;

architecture beh of matrix_mult is

    --FMS states from ASMD diagram
    type state_type is (idle, l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11);
    signal state_reg, state_next: state_type;
    
    --signals for image and kernel
    signal x_reg, x_next: unsigned(1 downto 0);
    signal y_reg, y_next: unsigned(1 downto 0);
    signal h_reg, h_next: unsigned(log2c(MAX_SIZE)-1 downto 0);
    signal v_reg, v_next: unsigned(log2c(MAX_SIZE)-1 downto 0);
    
    --signals for addreses
    signal a_address_reg, a_address_next: unsigned(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
    signal b_address_reg, b_address_next: unsigned(log2c(SIZE*SIZE)-1 downto 0);
    
    --temporary register after multiplication
    signal temp_reg, temp_next: signed(2*WIDTH-1 downto 0);
    --address for pixels after convolution
    signal c_addres_reg, c_addres_next: unsigned(log2c(MAX_SIZE*MAX_SIZE)-1 downto 0);
begin

    process(clk,reset)
    begin 
        --reseting everything
        if reset ='1' then
            state_reg <= idle;
            x_reg <= (others=>'0');
            y_reg <= (others=>'0');
            h_reg <= (others=>'0');
            v_reg <= (others=>'0');
            temp_reg <= (others=>'0');
            c_addres_reg <= (others => '0');
            a_address_reg <=(others =>'0');
            b_address_reg <=(others =>'0');
            
            
         elsif(clk'event and clk='1')then
            state_reg <= state_next;
            x_reg <= x_next;
            y_reg <= y_next;
            h_reg <= h_next;
            v_reg <= v_next;
            temp_reg <= temp_next;
            c_addres_reg <= c_addres_next;
            a_address_reg<= a_address_next;
            b_address_reg<= b_address_next;
            
         end if;
     end process;
      
            
    process(start, state_reg, a_data_i, b_data_i, x_reg, x_next, y_reg, y_next, h_reg,a_address_reg,a_address_next,b_address_reg, b_address_next,
     h_next, v_reg, v_next, temp_reg, temp_next,c_addres_reg,c_addres_next,columns,lines)   
    begin

    --default assignments
    x_next <= x_reg;
    y_next <= y_reg;
    v_next <= v_reg;
    h_next <= h_reg;
    
    a_address_next <= a_address_reg;
    b_address_next <= b_address_reg;
    c_addres_next <= c_addres_reg;
    temp_next <= temp_reg;
    a_addr_o<=(others=>'0');
    b_addr_o<=(others=>'0');
    c_addr_o<=(others=>'0');
    c_data_o<=(others=>'0');
    a_wr_o <= '0';
    b_wr_o <= '0';
    c_wr_o <= '0';
    ready <= '0';         

    case state_reg is
    
         when idle =>
                ready <= '1';
             if start = '1' then
                state_next <= L1;
                c_addres_next <= to_unsigned(0,log2c(MAX_SIZE*MAX_SIZE));
             else
                state_next <= idle;
             end if;
        
         when L1 =>
            v_next <= to_unsigned(0, log2c(MAX_SIZE));
            state_next <= L2;
        
         when L2 =>
             h_next <= to_unsigned(0, log2c(MAX_SIZE));
             state_next <= L3;
         
         when L3 =>
            temp_next <= to_signed(0, 2*WIDTH);
            x_next <= to_unsigned(0, 2);
            state_next <= L4;
         
         when L4 =>
             y_next <= to_unsigned(0,2);
             state_next <= L5;
            
         when L5 =>
            a_address_next <= ((x_reg + v_reg)*unsigned(columns) + h_reg + y_reg);     
            b_address_next <= (x_reg*SIZE + y_reg);
            state_next <= L6;
            
         when L6 =>
            a_addr_o <= std_logic_vector(a_address_reg);         
            b_addr_o <= std_logic_vector(b_address_reg);
            state_next <= L7;
       
         when L7 =>
            temp_next <= temp_reg + signed(a_data_i)*signed(b_data_i);
            y_next <= y_reg +1;
            
            if (y_next = SIZE-1) then
                a_address_next <= ((x_reg + v_reg)*unsigned(columns) + h_reg + y_next);  
                b_address_next <= (x_reg*SIZE + y_next);
                state_next <=L8;
            else 
                state_next <=L5;
            end if;
         
        when L8 =>
            a_addr_o <= std_logic_vector(a_address_reg);         
            b_addr_o <= std_logic_vector(b_address_reg);
        
            x_next <= x_reg +1;  
            state_next <= L9;
            
            
          when L9 =>
            temp_next <= temp_reg + signed(a_data_i)*signed(b_data_i);
            if (x_reg = SIZE ) then
               state_next <=L10;
            else 
               state_next <=L4;
            end if;
               
              
          when L10 =>            
              c_addr_o <= std_logic_vector(c_addres_next);

                if (temp_next < 0)then 
                        c_data_o <= std_logic_vector(to_unsigned(0,2*WIDTH));
                 else if (temp_next > 255) then
                        c_data_o <= std_logic_vector(to_unsigned(255,2*WIDTH));
                 else
                        c_data_o <= std_logic_vector(temp_next);
                end if;

              end if;

              c_wr_o <= '1';
              c_addres_next<=c_addres_reg + 1; 
              
              h_next <= h_reg +1;

              if (h_next <= unsigned(columns)-3) then
                  state_next <=L3;
              else 
                  state_next <=L11;
              end if;
            
          when L11 => 
  
                v_next <= v_reg +1;
                if (v_next <= unsigned(lines)-3) then
                    state_next <=L2;
                else 
                    state_next <=idle;
                end if;
            
         end case;
      end process;
 end beh;
