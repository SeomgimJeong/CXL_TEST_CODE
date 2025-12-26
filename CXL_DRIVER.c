#include <stdio.h>
#include "stdint.h"
#include "CXL_REG.h"
#include "xil_printf.h"
#include "sleep.h"

// CSRS address
static volatile CMSS_CACHE_CFG_t* cxl_csrs = NULL;

int cxl_init(uintptr_t baseaddr, uint64_t start_addr, uint64_t end_addr) {
    uint32_t version_val;

    cxl_csrs = (volatile CMSS_CACHE_CFG_t*)baseaddr;

    // 버전 레지스터 테스트 (major bit만 확인)
    version_val = cxl_csrs->VERSION;

    if (((version_val & CMSS_CACHE_CFG__VERSION__MAJOR_bm) >> CMSS_CACHE_CFG__VERSION__MAJOR_bp) != CMSS_CACHE_CFG__VERSION__MAJOR_reset) {
        xil_printf("error");
        return -1;
    }

    // 64비트 시작 주소
    cxl_csrs->START_ADDR_L = (uint32_t)(start_addr & 0xFFFFFFFF);
    cxl_csrs->START_ADDR_H = (uint32_t)(start_addr >> 32);

    // 64비트 종료 주소
    cxl_csrs->END_ADDR_L = (uint32_t)(end_addr & 0xFFFFFFFF);
    cxl_csrs->END_ADDR_H = (uint32_t)(end_addr >> 32);

    cxl_csrs->CMD       = (uint32_t)(1); // start = 1

    xil_printf("--- CXL 컨트롤러 초기화 완료 ---\r\n");
    return 0;
}

