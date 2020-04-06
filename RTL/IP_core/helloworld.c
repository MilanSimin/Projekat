#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_io.h"
u32 image[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
u32 kernel[9] = {1,1,1,1,1,1,1,1,1};
u32 real_result[16];
int  i = 0;
int main()
{


    init_platform();

    Xil_DCacheDisable();
    Xil_ICacheDisable();

    print("Starting program\n\r");

    printf("start is: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+8));
    printf("ready is: %d\n\r",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+12));

    Xil_Out32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR, 4);
    printf("lines: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR));

    Xil_Out32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR + 4, 4);
    printf("columns: %d\n\r",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+4));

    for (i = 0; i < 16; i++)
    {

    		Xil_Out32(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + i*4, image[i]);
    		printf("image[%d\]: %d\n", i,Xil_In32(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR+i*4));

    }
    for (i = 0; i < 9; i++)
    {
    	Xil_Out32(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + i*4, kernel[i]);
    	printf("kernel[%d\]: %d\n", i,Xil_In32(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR+i*4));
    }

    printf("start is: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+8));
    printf("ready is: %d\n\r",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+12));


    Xil_Out32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR + 8, 1);


    printf("start is: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+8));
    printf("ready is: %d\n\r",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+12));

    Xil_Out32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR + 8, 0);
    printf("Data written in\n\r");
    while (!Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR + 12));
    printf("Convolution finished\n\r");

    for (i = 1; i < 5;i++)
    {
    	real_result [i]=Xil_In32(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + i*4);
    }

    for (i = 1; i < 5; i++){
    	printf("result\[%d\]: %d\n", i, Xil_In32(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + i*4));
    }

    printf("start is: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+8));
    printf("ready is: %d\n",Xil_In32(XPAR_IMAGE_CONV_NEW_0_S00_AXI_BASEADDR+12));

    cleanup_platform();
    return 0;
}

