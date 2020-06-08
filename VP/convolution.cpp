#include "convolution.hpp"
#include "vp_addr.hpp"
#include <tlm_utils/tlm_quantumkeeper.h>
#include <vector>



SC_HAS_PROCESS(conv);
conv::conv(sc_module_name name):
	sc_module(name),
	int_tsoc("int_tsoc"),
	conv_isoc("conv_isoc")
{
	int_tsoc.register_b_transport(this, &conv::b_transport);
	
}

vector <int> final_img;

sc_time loct;
tlm_generic_payload pl;
tlm_utils::tlm_quantumkeeper qk;


void conv::b_transport(pl_t& pl, sc_time& offset)
{
		
	
	tlm_command    cmd  = pl.get_command();
	uint64         addr = pl.get_address();
	unsigned char *data = pl.get_data_ptr();

	vector<double> array;
	vector<int> image;
	int m=0,n=0;

		switch(cmd)
		{
		case TLM_WRITE_COMMAND:
		{
				
			switch(addr){
				case VP_ADDR_CONV:
					
					pl.set_response_status( TLM_OK_RESPONSE );
					msg(pl);
	
					break;	
	
				case CONV_LINES:	
					//receiving image size from cpu
					lines =*((int*)pl.get_data_ptr());
					
					cout<<"CONVOLUTION: image size_lines received "<<endl;	

					pl.set_response_status( TLM_OK_RESPONSE );
					break;

				case CONV_COLUMNS:
					columns =*((int*)pl.get_data_ptr());

					cout<<"CONVOLUTION: image size_columns received "<<endl;

					pl.set_response_status( TLM_OK_RESPONSE );
					//msg(pl);	
					break;


				default:

					pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
					break;
			}
		}

		break;
		case TLM_READ_COMMAND:
		{
			switch(addr){
				case CONV_KERNEL:
					
					//recieving the kernel in an array form and transforming into a matrix 3x3
					
					pl.set_address(VP_ADDR_MEMORY_B);
					pl.set_command(TLM_READ_COMMAND);
					pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

					conv_isoc->b_transport(pl, loct);
					qk.set_and_sync(loct);
					loct += sc_time(5, SC_NS);

					array = *((vector<double>*)pl.get_data_ptr());

					for(int i = 0; i < 3; i++){

				    		for(int j = 0; j < 3; j++){

							kernel[i][j]=array[n++];

						}

					}
					
					cout<<"CONVOLUTION: kernel received from MEMORY "<<endl;
					pl.set_response_status(TLM_OK_RESPONSE);
					break;

				
				case CONV_BEGIN:
					//receiving image from memory and placing into a matrix lines+2 x columns+2 
					
					pl.set_address(VP_ADDR_MEMORY_A);
					pl.set_command(TLM_READ_COMMAND);
					pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

					conv_isoc->b_transport(pl, loct);
					qk.set_and_sync(loct);
					loct += sc_time(5, SC_NS);

					image = *((vector<int>*)pl.get_data_ptr());
					
					cout<<"CONVOLUTION: image received from MEMORY "<<endl;

					m=0;
					for(int i = 0; i < columns+2; i++){
				    		for(int j = 0; j < lines+2; j++){
							matrix[i][j]=image[m++];
						}
					}
				
					convolution();
					break;

			}
			break;
		}
		default:
			pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
			SC_REPORT_ERROR("CORE", "TLM bad command");
		}
	
}
void conv::convolution(){
	//the convolution of matrix and kernel
	cout<<"CONVOLUTION: Starting the convolution "<<endl;

	for (int x = 1; x < columns+3; x++)
	{
		for (int y = 1; y < lines+3 ; y++)
		{
			int value=0;
			for (int u =- 1; u <= 1; u++)
			{
				for (int v =- 1; v <= 1; v++)
				{

					value=value + ( matrix[x+u][y+v] * kernel[u + 1][v + 1]);
				
				}

			}
			//this is used so that the values won't go higher than 255 or lower than 0, 
			// because pixels are represented from 0 to 255 
			if(value> 255)	
				value=255;
			if(value< 0)
				value=0;
			//taking the values of every pixel and putting them into an array
			final_img.push_back(value);
										
		}
	}	
	cout<<"CONVOLUTION: convolution has finished, sending image back to MEMORY "<<endl;
	//sending the image back to memory after convolution
	pl.set_address(VP_ADDR_MEMORY_C);
	pl.set_command(TLM_WRITE_COMMAND);	
	pl.set_data_length(final_img.size());
	pl.set_data_ptr((unsigned char*)&final_img);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
	
	conv_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);
			
	

}


void conv::msg(const pl_t& pl)
{
	
}
