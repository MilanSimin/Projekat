#include "CPU.hpp"
#include "vp_addr.hpp"
#include <string>
#include <fstream>
#include <tlm_utils/tlm_quantumkeeper.h>
#include <vector>

#define SIZE 256
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

ifstream tekst("../VP/test/image.txt");

SC_HAS_PROCESS(cpu);
cpu::cpu(sc_module_name name):
	sc_module(name)
{
	SC_THREAD(proces);
}


//***************************************************************************


int image[SIZE][SIZE];
int final_image[SIZE][SIZE]={{0}};
int image_new[SIZE + 2][SIZE + 2];
int image_BlackAndWhite[SIZE][SIZE];


//scaning the picture into project from txt file (image.txt)
void cpu::scan_infile(int* lines, int* columns)
{
    /*Loads image into project*/

    //add file error protection
    ifstream infile;
	
    FILE* fp;
       	*lines = 1;
        char i=0;
        *columns = 1;
        fp = fopen("../VP/test/image.txt", "r");

    while((i=fgetc(fp))!='\n')
    {
        if (i == ' ')
        {
            ++(*columns);
        }
    }
    //*columns=*columns/3;
    while((i=fgetc(fp))!=EOF)
    {
        if (i == '\n')
        {
            (*lines)++;
        }
    }
        fclose(fp);

        infile.open("../VP/test/image.txt");
        if(infile.is_open())
        //load image into 3D array
        while(!infile.eof())
        {
            for(int i = 0; i < *lines; i++)
                for(int j = 0; j < *columns; j++){
                    
                        infile >> image[i][j];
                    }
        }
	cout<<"scan_infile end"<<endl;
	
	
}

//**************************************************************************************

//converting the image from RGB to black and white 
void cpu::BlackAndWhite( ){
		
		
		/*for(int i = 0; i < lines; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				int temp=0;
				temp = image[i][j][0]*0.072 + image[i][j][1]*0.7152 + image[i][j][2]*0.02126;
				
				image_BlackAndWhite[i][j] = temp;
			}
		}*/
		

}


//*************************************************************************************

//expanding the image with zeros beacuse of border cases when the convolution starts 
void cpu::addingZerosForEdges()
{		
		
		for(int i = 0; i < lines; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				image_new[i+1][j+1] = image[i][j];
				
			}
		}
	
	
}
//****************************************************************************************
void cpu::writeInFile(){

	ofstream outfile;
	ofstream checkfile;
    	checkfile.open("../VP/test/final_image.txt");

	if(checkfile.is_open())
    	cout<<"Starting to add image into final_image.txt "<<endl;
	    for(int i=0; i < lines; i++)
	    {
		for(int j=0; j < columns; j++)
		{
		    
		        checkfile << final_image[i][j];
		        //dont add space at end of line
		        if(j != columns-1)
		        {
		            checkfile << " ";
		     	}
		}
		//write to next line after end of line
		checkfile << "\n";
   	 }	
	cout <<"IMAGE saved successfully into final_image.txt after convolution "<<endl;
}
//*******************************************************************************************

//main function thread
void cpu::proces()
{	
	int i, j, k;

	sc_time loct;
	tlm_generic_payload pl;
	tlm_utils::tlm_quantumkeeper qk;
	qk.reset();

	scan_infile(&lines, &columns);//function for reading image into project
	//BlackAndWhite();//converting the image into black and white 
	addingZerosForEdges();//adding zeros on edges beacuse of border cases

	//*****************************************************************************************
	//sending the image size to convolution
	int size_lines=lines;
	int size_columns=columns;

	pl.set_address(VP_ADDR_CONV_LINES);
	pl.set_command(TLM_WRITE_COMMAND);	
	pl.set_data_length(1);
	pl.set_data_ptr((unsigned char*)&size_lines);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

	cout<<"CPU: image size_lines sent to convolution "<<endl;
	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);


	pl.set_address(VP_ADDR_CONV_COLUMNS);
	pl.set_command(TLM_WRITE_COMMAND);	
	pl.set_data_length(1);
	pl.set_data_ptr((unsigned char*)&size_columns);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

	cout<<"CPU: image size_columns sent to convolution "<<endl;
	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);
	//******************************************************************************************

	
	//selecting the operation to be done, selecting the kernel to operate
	
	vector<double> array1 = {0,0,0,0,1,0,0,0,0};
    	vector<double> array2 = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    	vector<double> array3 = {0, -1, 0,-1, 5, -1, 0, -1, 0};
    	//vector<double> array4 = {0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111, 0.111};
    	//vector<double> array5 = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
	int selection;

	cout<<"Select the type of kernel:\n" << "1. Identity Operator \n2. Edge detection \n3. Sharpening \n -> "<<endl;
	cin >> selection;

	switch (selection){

	case 1:

			pl.set_address(VP_ADDR_MEMORY_B);
			pl.set_command(TLM_WRITE_COMMAND);	
			pl.set_data_length(array1.size());
			pl.set_data_ptr((unsigned char*)&array1);
			pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
			cout<<"CPU: kernel sent to MEMORY "<<endl;

			mem_isoc->b_transport(pl, loct);
			qk.set_and_sync(loct);
			loct += sc_time(5, SC_NS);

							
		break;
	case 2:

			pl.set_address(VP_ADDR_MEMORY_B);
			pl.set_command(TLM_WRITE_COMMAND);	
			pl.set_data_length(array2.size());
			pl.set_data_ptr((unsigned char*)&array2);
			pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
			cout<<"CPU: kernel sent to MEMORY "<<endl;

			mem_isoc->b_transport(pl, loct);
			qk.set_and_sync(loct);
			loct += sc_time(5, SC_NS);
		break;

	case 3:

			pl.set_address(VP_ADDR_MEMORY_B);
			pl.set_command(TLM_WRITE_COMMAND);	
			pl.set_data_length(array3.size());
			pl.set_data_ptr((unsigned char*)&array3);
			pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
			cout<<"CPU: kernel sent to MEMORY "<<endl;

			mem_isoc->b_transport(pl, loct);
			qk.set_and_sync(loct);
			loct += sc_time(5, SC_NS);
		break;
	/*case 4:

			pl.set_address(VP_ADDR_MEMORY_B);
			pl.set_command(TLM_WRITE_COMMAND);	
			pl.set_data_length(array4.size());
			pl.set_data_ptr((unsigned char*)&array4);
			pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
			cout<<"CPU: kernel sent to MEMORY "<<endl;

			mem_isoc->b_transport(pl, loct);
			qk.set_and_sync(loct);
			loct += sc_time(5, SC_NS);
		break;
	case 5:

			pl.set_address(VP_ADDR_MEMORY_B);
			pl.set_command(TLM_WRITE_COMMAND);	
			pl.set_data_length(array5.size());
			pl.set_data_ptr((unsigned char*)&array5);
			pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
			cout<<"CPU: kernel sent to MEMORY "<<endl;

			mem_isoc->b_transport(pl, loct);
			qk.set_and_sync(loct);
			loct += sc_time(5, SC_NS);
		break;*/
	default:
		cerr << "Invalid selection";
	break;
	
	}
	
	//*************************************************************
	pl.set_address(VP_ADDR_CONV_KERNEL);
	pl.set_command(TLM_READ_COMMAND);	
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
	
	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);
	
	//********************************************************************************************
	//sending image to memory, in an array of 8 elements untill the whole image is sent
	int p=0;
	p=(columns+2)/8; // calculating the number of packages that will be sent to memory
	
	cout<<"CPU: Sending image to MEMORY "<<endl;
	vector<int> image_array;	
	for (int i=0; i < lines+2; i++){

		for(int n=0;n<=p;n++){
		
			if((columns+2)%8==0){
				
				for(int m=0; m < 8; m++){
				
					image_array.push_back(image_new[i][n*8+m]);
				
				}
							
				pl.set_address(VP_ADDR_MEMORY_A);
				pl.set_command(TLM_WRITE_COMMAND);	
				pl.set_data_length(8);
				pl.set_data_ptr((unsigned char*)&image_array);
				pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
				
				mem_isoc->b_transport(pl, loct);
				qk.set_and_sync(loct);
				loct += sc_time(5, SC_NS);
				image_array.clear();
		


			}else{
			int remainder=(columns+2)%8; 
			if(n<=p-1){
				for(int m=0; m < 8; m++){
				
					image_array.push_back(image_new[i][n*8+m]);
				
				}
							
				pl.set_address(VP_ADDR_MEMORY_A);
				pl.set_command(TLM_WRITE_COMMAND);	
				pl.set_data_length(8);
				pl.set_data_ptr((unsigned char*)&image_array);
				pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
				
				mem_isoc->b_transport(pl, loct);
				qk.set_and_sync(loct);
				loct += sc_time(5, SC_NS);
				image_array.clear();
		
			} else {
			
				for( int m=0;m<remainder;m++){

					image_array.push_back(image_new[i][n*8+m]);
	
				}
				pl.set_address(VP_ADDR_MEMORY_A);
				pl.set_command(TLM_WRITE_COMMAND);	
				pl.set_data_length(remainder);
				pl.set_data_ptr((unsigned char*)&image_array);
				pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
				
				mem_isoc->b_transport(pl, loct);
				qk.set_and_sync(loct);
				loct += sc_time(5, SC_NS);
				image_array.clear();	

			}

			}
			
			
				
			
		}
		
			
	}
	
		
	//*************************************************************
	//after the image is sent to memory, we start the convolution from here
	
	pl.set_address(VP_ADDR_CONV_BEGIN);
	pl.set_command(TLM_READ_COMMAND);	
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);
	
	int_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);

	//**************************************************************
	//recieving the image from memory after the convolution has finished 
	vector<int> picture;	
	
	pl.set_address(VP_ADDR_MEMORY_C);
	pl.set_command(TLM_READ_COMMAND);
	pl.set_response_status (TLM_INCOMPLETE_RESPONSE);

	mem_isoc->b_transport(pl, loct);
	qk.set_and_sync(loct);
	loct += sc_time(5, SC_NS);

	picture = *((vector<int>*)pl.get_data_ptr());
		
	cout<<"CPU: image received from MEMORY after convolution "<<endl;

	//****************************************************************
	//forming a matrix image from an array recieved from memory after the convolution has finished
	int m=0;
	
	for(int i = 0; i < columns+2; i++){
    		for(int j = 0; j < lines+2; j++){

				final_image[i][j]=picture[m];
				m++;
		}
	}
	
	//*******************************************************************

	writeInFile();//scaning the image into final_image.txt after the matrix has been formed


}

