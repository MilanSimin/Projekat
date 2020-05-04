library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.utils_pkg.all;

entity conv_ip is
generic (
     --MAYBE CHANGE THE WIDTH BACK TO 8?
     WIDTH: integer := 9; --data width of pixels
     SIZE: integer := 3; --for kernel size
	 MAX_SIZE: integer := 256; --maximum size of image
	 ADDR_WIDTH : integer := 32 
	 
);
port (
--------------- Clocking and reset interface ---------------
    
    clk: in std_logic;
    reset: in std_logic;
------------------- Input data interface -------------------
-- Matrix A memory interface
-- image matrix columns*lines (MAX_SIZE*MAX_SIZE)
    a_addr_o: out std_logic_vector(ADDR_WIDTH-1 downto 0);
    a_data_i: in std_logic_vector(WIDTH-1 downto 0);
    a_en_o: out std_logic;
-- Matrix B memory interface
-- kernel matrix 3x3
    b_addr_o: out std_logic_vector(7 downto 0);
    b_data_i: in std_logic_vector(WIDTH-1 downto 0);
    b_en_o: out std_logic;
--Dimension of the image 
    columns: in std_logic_vector(7 downto 0);
    lines: in std_logic_vector(7 downto 0);
------------------- Output data interface ------------------
-- Matrix C memory interface after convolution
    c_addr_o: out std_logic_vector(ADDR_WIDTH-1 downto 0);
    c_data_o: out std_logic_vector(2*WIDTH-1 downto 0);
    c_en_o: out std_logic;
    c_wr_o: out std_logic;
--------------------- Command interface --------------------
    start: in std_logic;
--------------------- Status interface ---------------------
    ready: out std_logic);
end entity;

architecture beh of conv_ip is

    --FSM states from ASMD diagram
    type state_type is (idle, reset_V,reset_H, reset_temp_and_X, reset_Y, calculate_address_A, calculate_address_B, calculate_address_A1, calculate_address_B1,  write_address, convolution_1_add, convolution_1_mult, result_and_temp, move_Y,check_Y ,calculate_address_1,write_address_1, move_X, check_x ,convolution_add, convolution_mult, result_conv, move_C, move_V, move_H, check_H, check_V);
    
    signal state_reg, state_next: state_type;
    
    --signals for kernel
    signal x_reg, x_next: unsigned(1 downto 0);-- signals for kernel, and small image when convolution starts multipling
    signal y_reg, y_next: unsigned(1 downto 0);-- only 2 bits,because the x and y will go 0-2 it doesn't go higher than that

    --signals for image
    signal h_reg, h_next: unsigned(7 downto 0);-- signals for image, they will go from 0 to the end,horizontal and vertical
    signal v_reg, v_next: unsigned(7 downto 0);-- end(depends on the size of image)
    
    --signals for addreses
    signal a_address_reg, a_address_next: unsigned(ADDR_WIDTH-1 downto 0);-- image address register
    signal b_address_reg, b_address_next: unsigned(7 downto 0); -- kernel address register
    
    --temporary register after multiplication
    signal temp_reg3, temp_next3 : signed(2*WIDTH-1 downto 0);
    signal temp_reg2, temp_next2 : signed(2*WIDTH-1 downto 0);
    signal temp_reg1, temp_next1: signed(2*WIDTH-1 downto 0);
    signal pom_reg, pom_next       : signed(2*WIDTH-1 downto 0);

    --register for pixels after convolution
    signal c_addres_reg, c_addres_next: unsigned(ADDR_WIDTH-1 downto 0);
    
    
begin

    process(clk,reset)
    begin 
        --reseting everything
	--reset for BRAM is on HIGH, on AXI is LOW!!!
        if reset ='1' then
            state_reg <= idle;
            x_reg <= (others=>'0');
            y_reg <= (others=>'0');
            h_reg <= (others=>'0');
            v_reg <= (others=>'0');
            temp_reg1 <= (others=>'0');
            temp_reg2 <=(others => '0');
            temp_reg3 <=(others => '0');
            c_addres_reg <= (others => '0');
            a_address_reg <=(others =>'0');
            b_address_reg <=(others =>'0');
            pom_reg <= (others => '0');
            
            
         elsif(clk'event and clk='1')then
            state_reg <= state_next;
            x_reg <= x_next;
            y_reg <= y_next;
            h_reg <= h_next;
            v_reg <= v_next;
            temp_reg1 <= temp_next1;
            temp_reg2 <= temp_next2;
            temp_reg3 <= temp_next3;
            pom_reg <= pom_next;
            c_addres_reg <= c_addres_next;
            a_address_reg<= a_address_next;
            b_address_reg<= b_address_next;
            
         end if;
     end process;
      
            
    process(start, state_reg, a_data_i, b_data_i, x_reg, x_next, y_reg, y_next, h_reg, h_next,v_reg, v_next, a_address_reg, a_address_next, b_address_reg, b_address_next, temp_reg1, temp_next1,temp_reg2,temp_next2,temp_reg3, temp_next3, pom_next, pom_reg, c_addres_reg,c_addres_next,columns,lines)  
 
    begin
    --default assignments
    x_next <= x_reg;
    y_next <= y_reg;
    v_next <= v_reg;
    h_next <= h_reg;
    
    a_address_next <= a_address_reg;
    b_address_next <= b_address_reg;
    c_addres_next <= c_addres_reg;
    temp_next1 <= temp_reg1;
    temp_next2 <= temp_reg2;
    temp_next3 <= temp_reg3;
    pom_next <= pom_reg;
    
    a_en_o <= '0';
    a_addr_o <=(others => '0');
    b_en_o <= '0';
    b_addr_o <=(others => '0');
    c_wr_o <= '0';
    c_addr_o <=(others => '0');
    c_data_o <=(others => '0');	
    c_en_o <= '0';
    ready <= '0';         

    case state_reg is
    
         when idle =>
                ready <= '1';
             if start = '1' then -- start the IP core
                state_next <= reset_V;
             else
                state_next <= idle;
             end if;
        when reset_V =>
                      
             v_next <= to_unsigned(0, 8); --set value to 0
             c_addres_next <= to_unsigned(0,ADDR_WIDTH);
    
             state_next <= reset_H;
            
         when reset_H =>
         
             h_next <= to_unsigned(0, 8); --set value to 0
             state_next <= reset_temp_and_X;
         
         when reset_temp_and_X =>
         
            c_wr_o <= '0';
            temp_next1      <=      to_signed(0, 2*WIDTH);      -- set value to 0
            temp_next2      <=      to_signed(0, 2*WIDTH);      -- set value to 0
            temp_next3      <=      to_signed(0, 2*WIDTH);      -- set value to 0
            x_next          <=      to_unsigned(0, 2);          -- set value to 0
            pom_next        <=      to_signed(0, 2*WIDTH);      -- set value to 0
            state_next <= reset_Y;
         
         when reset_Y =>
             pom_next <= to_signed(0, 2*WIDTH);                 -- set value to 0
             y_next <= to_unsigned(0,2);                        -- set value to 0
             state_next <= calculate_address_A;
            
         when calculate_address_A =>
         
            a_address_next <= ((x_reg + v_reg)*unsigned(columns) + h_reg + y_reg)*4;  
            state_next <= calculate_address_B;
            
         when calculate_address_B =>
         
            b_address_next <= (x_reg*SIZE + y_reg)*4;
            state_next <= write_address;
            
         when write_address =>
         
            a_en_o <= '1';
            b_en_o <= '1';
            a_addr_o <= std_logic_vector(a_address_reg);         
            b_addr_o <= std_logic_vector(b_address_reg);
            
            state_next <= convolution_1_mult;
         
         when convolution_1_mult =>
            
            a_en_o <= '0';
            b_en_o <= '0';
         
            pom_next <= signed(a_data_i) * signed(b_data_i);
            state_next <= convolution_1_add;
       
         when convolution_1_add =>
         
            temp_next1 <= temp_reg1 + pom_reg;         
            
            state_next <= move_Y;
            
         when move_Y => 
         
             pom_next <= to_signed(0, 2*WIDTH); 
             y_next <= y_reg +1;
            
            state_next <= check_Y;
            
               
         when check_Y =>
            
            if (y_reg = SIZE-1) then 
                
                state_next <= calculate_address_A1;
                
            else 
                state_next <=calculate_address_A;
            end if;
            
          when calculate_address_A1 =>
          
                a_address_next <= ((x_reg + v_reg)*unsigned(columns) + h_reg + y_reg)*4; 
                state_next <= calculate_address_B1;
                           
          when calculate_address_B1 =>                    
          
                b_address_next <= (x_reg*SIZE + y_reg)*4;                       
                state_next <= write_address_1;
                
           when write_address_1 =>
 
                a_en_o <= '1';
                b_en_o <= '1';
                
                a_addr_o <= std_logic_vector(a_address_reg);         
                b_addr_o <= std_logic_vector(b_address_reg);
                
            
                state_next <= convolution_mult;
                         
         when convolution_mult =>
         
            a_en_o <= '0';
            b_en_o <= '0';
            pom_next <= signed(a_data_i) * signed(b_data_i);
            state_next <= convolution_add;
       
         when convolution_add =>
         
            temp_next2 <= temp_reg2 + pom_reg;
            state_next <= move_X;
            
          when move_X =>
          
            x_next <= x_reg +1;
            state_next <= check_X;
            
          when check_x =>
          
            if (x_reg = SIZE ) then
               state_next <= result_and_temp;
            else 
               state_next <=reset_Y;
            end if;
         
          when result_and_temp =>
          
                temp_next3 <= temp_reg3 + temp_reg1 + temp_reg2;                
                state_next <= result_conv;
              
          when result_conv =>   
          
                c_en_o <= '1';
                c_wr_o <= '1';         
                c_addr_o <= std_logic_vector(c_addres_reg);
                if (temp_reg3 < 0)then 
                    c_data_o <= std_logic_vector(to_unsigned(0,2*WIDTH));
                 else if (temp_reg3 > 255) then
                    c_data_o <= std_logic_vector(to_unsigned(255,2*WIDTH));
                 else
                    c_data_o <= std_logic_vector(temp_reg3);
                 end if;
                end if;
                state_next <= move_C;
              
           when move_C =>
           
              c_wr_o <= '0';
              c_en_o <= '0';
              c_addres_next <= c_addres_reg + 4; 
              state_next    <= move_H;
                               
           when move_H =>             
             
              h_next <= h_reg +1;
              state_next <= check_H;
              
           when check_H =>  
              
              if (h_reg <= unsigned(columns)-3) then
                  state_next <=reset_temp_and_X;
              else 
                  state_next <=move_V;
              end if;
              
          when move_V =>   
          
                v_next <= v_reg +1;
                state_next <= check_V;
           
          when check_V =>    
           
                if (v_reg <= unsigned(lines)-3) then
                    state_next <=reset_H;
                else 
                    state_next <=idle;
                end if;
                
          when others =>
            state_next <= idle;
          
            
         end case;
      end process;
 end beh;