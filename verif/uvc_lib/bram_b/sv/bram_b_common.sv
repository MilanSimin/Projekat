`ifndef BRAM_B_COMMON_SV
`define BRAM_B_COMMON_SV

parameter int KERNEL_SIZE = 3;
typedef enum {IDENTITY, EDGE_DETECTION, SHARPEN, BOX_BLUR, GAUSSIAN_BLUR} kernel_t;

`endif // BRAM_B_COMMON_SV
