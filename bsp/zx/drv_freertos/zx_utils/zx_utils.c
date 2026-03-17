#include "zx_utils.h"

void freertos_heap_info_print() {
    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);   
    printf("------------- ಥ_ಥ FreeRtos Memory ಥ_ಥ -------------\r\n");
    printf("ಥ_ಥ  total_free_bytes -> %u\r\n", heapStats.xAvailableHeapSpaceInBytes);
    printf("ಥ_ಥ  largest_free_block -> %u\r\n", heapStats.xSizeOfLargestFreeBlockInBytes);
    printf("ಥ_ಥ  minimum_ever_free_bytes -> %u\r\n", heapStats.xMinimumEverFreeBytesRemaining);
    printf("ಥ_ಥ  number_of_successful_allocations -> %u\r\n", heapStats.xNumberOfSuccessfulAllocations);
    printf("ಥ_ಥ  number_of_successful_frees -> %u\r\n", heapStats.xNumberOfSuccessfulFrees);
    printf("------------------ Memory Info End -------------------\n");
}

void psram_heap_info_print() {
    uint32_t total, used, max_used, free, max_free;
    // 调用aic_tlsf_mem_info获取内存信息
    aic_tlsf_mem_info(MEM_PSRAM_CMA, (u32 *)&total, (u32 *)&used, (u32 *)&max_used, (u32 *)&free, (u32 *)&max_free);

    // 打印内存信息
    printf("------------- ಥ_ಥ PSRAM Memory ಥ_ಥ -------------\n");
    printf("ಥ_ಥ  total -> %ld\n", total);
    printf("ಥ_ಥ  used -> %ld\n", used);
    printf("ಥ_ಥ  max_used -> %ld\n", max_used);
    printf("ಥ_ಥ  free -> %ld\n", free);
    printf("ಥ_ಥ  max_free -> %ld\n", max_free);
    printf("----------------- Memory Info End ----------------\n");
}
