#include "memory.hpp"
#include "vp_addr.hpp"
#include <cstdint>

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;

memory::memory(sc_module_name name):
  sc_module(name),
  cpu_tsoc("cpu_tsoc"),
  conv_tsoc("conv_tsoc")
{
	cpu_tsoc.register_b_transport(this, &memory::b_transport);
	conv_tsoc.register_b_transport(this, &memory::b_transport);
 

}
vector<int> dram;
vector<int> convolution;
vector<double> kernel;


void memory::b_transport(pl_t& pl, sc_time& offset)
{
	tlm_command    cmd  = pl.get_command();
	uint64         addr = pl.get_address();
	unsigned char* data = pl.get_data_ptr();
	unsigned int len = pl.get_data_length();

	vector<int> buff;
	vector<double> buff_kernel;
	
		switch(cmd)
		{
		case TLM_WRITE_COMMAND:
		{	
			switch(addr){
			
				case VP_ADDR_MEMORY_A:
				//receiving  pixels of image from CPU 

					buff = *((vector<int>*)pl.get_data_ptr());
					
					for (int i = 0; i < len ; i++){

						dram.push_back(buff[i]);
						
					}
					
					
				break;

				case VP_ADDR_MEMORY_B:
				//receiving kernel from CPU
				
					buff_kernel = *((vector<double>*)pl.get_data_ptr());

					for (int i = 0; i < len ; i++){

						kernel.push_back(buff_kernel[i]);
						
					}
					cout<<"MEMORY: kernel received from CPU "<<endl;

				break;
	
				case VP_ADDR_MEMORY_C:
				//receiving the whole image after the convolution has finished from CONVOLUTION
					cout<<"MEMORY: image received after CONVOLUTION "<<endl;
					convolution = *((vector<int>*)pl.get_data_ptr());
					
				break;
				}


			break;
		}

		case TLM_READ_COMMAND:
		{	

			switch(addr){

				case VP_ADDR_MEMORY_A:
				//sending image to convolution to be processed
					cout<<"MEMORY: sending image to CONVOLUTION "<<endl;
					pl.set_data_ptr((unsigned char*)&dram);
					pl.set_data_length(dram.size());
					pl.set_response_status( TLM_OK_RESPONSE );

				break;
			
				case VP_ADDR_MEMORY_B:
				//sending kernel to convolution to be processed
					cout<<"MEMORY: sending kernel to CONVOLUTION "<<endl;
					pl.set_data_ptr((unsigned char*)&kernel);
					pl.set_data_length(kernel.size());
					pl.set_response_status( TLM_OK_RESPONSE );

				break;

				case VP_ADDR_MEMORY_C:
				//sending image to CPU after the CONVOLUTION has finished
					cout<<"MEMORY: image sent to CPU after CONVOLUTION "<<endl;
					pl.set_data_ptr((unsigned char*)&convolution);
					pl.set_data_length(convolution.size());
					pl.set_response_status( TLM_OK_RESPONSE );

				break;
				}
			break;
		}
		default:
			pl.set_response_status( TLM_COMMAND_ERROR_RESPONSE );
			SC_REPORT_ERROR("MEMORY", "TLM bad command");
		}
}

void memory::msg(const pl_t& pl)
{

}



