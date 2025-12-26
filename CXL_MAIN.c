#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "CXL_REG.h"
#include "stdint.h"
#include "sleep.h"


#define NUMBER_OF_TEST  100

void test_HBM()
{
    //HBM verfication
    volatile uint64_t *hbm_ptr  = (volatile uint64_t *)HBM_BASE_ADDR;
    uint64_t hbm_read_data[NUMBER_OF_TEST];

    //write
    for(uint64_t i = 0; i<NUMBER_OF_TEST; i++) {

        hbm_ptr[i]                  = i;
        usleep(10);
    }

    //read
    for(uint64_t j = 0; j<NUMBER_OF_TEST; j++){
        hbm_read_data[j]            = hbm_ptr[j];
        usleep(10);

        xil_printf("//HBM// \n read_data: %lx, expected_data: %lx \n",hbm_read_data[j], j);
    }
}

void test_DDR()
{
    //DDR verification
    volatile uint64_t *ddr_ptr      = (volatile uint64_t *)DDR_BASE_ADDR;
    uint64_t ddr_read_data[10];

    //write
    for(uint64_t i = 0; i<NUMBER_OF_TEST; i++) {

        ddr_ptr[i]                  = i;
        usleep(10); // dummy loop
    }

    //read
    for(uint64_t j = 0; j<NUMBER_OF_TEST; j++){
        ddr_read_data[j]            = ddr_ptr[j];
        usleep(10);

        xil_printf("//DDR// \n read_data: %lx, expected_data: %lx \n",ddr_read_data[j], j);
    }

}


int main()
{
    usleep(100);

    init_platform();

    if (cxl_init(XPAR_CMSS_CACHE_CFG_0_BASEADDR, START_ADDRESS, END_ADDRESS) != 0) {
        xil_printf("init failure \r\n");
        cleanup_platform();
        return -1;
    }
    usleep(100);

    test_DDR();

    test_HBM();

    cleanup_platform();

    return 0;

}


