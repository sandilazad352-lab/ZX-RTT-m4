#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include "hal_adcim.h"
#include "hal_gpai.h"
#include "aic_core.h"
#include "aic_hal_clk.h"
#include "gpai.h"

// Global variable to track the number of GPAI channels
uint8_t g_gpai_nums = 0;
// Array to hold the GPAI channel structures
struct aic_gpai_ch aic_gpai_chs[GPAI_NUM_MAX];

// Structure to represent a node in the linked list of GPAI channels
typedef struct _gpai_ch_node {
    struct aic_gpai_ch* chan; // Pointer to the GPAI channel
    struct _gpai_ch_node *next; // Pointer to the next node in the list
} gpai_ch_node_t;

// The head node of the linked list
static gpai_ch_node_t g_gpai_node;

// Function to find a GPAI channel by its ID
static struct aic_gpai_ch* gpai_find_ch(uint32_t ch) {
    if (ch >= GPAI_NUM_MAX) {
        return NULL;
    }

    gpai_ch_node_t *node = &g_gpai_node;
    // Traverse the linked list to find the channel
    while (node->next) {
        node = node->next;
        if (node->chan->id == ch) {
            return node->chan;
        }
    }

    // If not found, create a new channel and add it to the list
    struct aic_gpai_ch* chan = &aic_gpai_chs[g_gpai_nums];
    if (chan == NULL) {
        return NULL;
    }
    // Initialize the new channel
    chan->id = ch;
    chan->complete = NULL;
    chan->obtain_data_mode = AIC_GPAI_OBTAIN_DATA_BY_CPU; // Currently, only CPU data retrieval is supported
    chan->smp_period = 100;
    chan->available = 0;

    // Set FIFO depth based on the driver version
#if AIC_GPAI_DRV_V11
    chan->fifo_depth = 8;
#else
    chan->fifo_depth = 32;
#endif

    // Create a new node for the linked list
    gpai_ch_node_t* new_node = malloc(sizeof(gpai_ch_node_t));
    if (new_node == NULL) {
        return NULL;
    }

    // Add the new node to the linked list
    new_node->chan = chan;
    new_node->next = NULL;
    node->next = new_node;

    // Increment the GPAI channel count and update it in the hardware
    g_gpai_nums += 1;
    hal_gpai_set_ch_num(g_gpai_nums);

    return chan;
}

// Function to enable or disable a GPAI channel
int gpai_enabled(uint32_t ch, uint8_t enabled, enum aic_gpai_mode mode, uint32_t period_ms) {
    struct aic_gpai_ch *chan = gpai_find_ch(ch);
    if (chan == NULL) {
        return -1;
    }

    // Ensure the clock for the GPAI channel is enabled
    hal_gpai_clk_get(chan);
    if (enabled) {
        // Configure the channel as enabled
        chan->available = 1;
        chan->mode = mode;
        chan->smp_period = period_ms;
        chan->irq_count = 0;
        if (mode == AIC_GPAI_MODE_SINGLE) {
            // In single mode, set the IRQ count to 1
            chan->irq_count = 1;
            // Create a semaphore for synchronization
            chan->complete = aicos_sem_create(0);
        }
        // Initialize the GPAI channel
        aich_gpai_ch_init(chan, chan->pclk_rate);
    } else {
        // Disable the channel
        chan->available = 0;
        aich_gpai_ch_enable(chan->id, 0);
        // Delete the semaphore if it exists
        if (chan->complete) {
            aicos_sem_delete(chan->complete);
            chan->complete = NULL;
        }
    }

    return 0;
}

// Function to read the voltage from a GPAI channel
int gpai_read_mv(uint32_t ch) {
    uint32_t value = 0;
    struct aic_gpai_ch *chan = gpai_find_ch(ch);
    if (!chan) {
        return 0;
    }
    // Read the value from the GPAI channel
    aich_gpai_read(chan, (u32 *)&value, AIC_GPAI_TIMEOUT);

    // Temporary hardcoded scale factor; not used in current implementation
    int scale = 10000;
    // Convert the read value to millivolts
    int voltage = hal_adcim_auto_calibration(value, 3, scale) / 10;

    return voltage;
}

// Function to return the resolution of the GPAI channels
int gpai_get_resolution() {
    return 12; // Hardcoded resolution of 12 bits
}

// Function to initialize the GPAI subsystem
int gpai_init(void) {
    static uint8_t is_init = 0;
    if (is_init) {
        return 0;
    }
    is_init = 1;

    // Probe and initialize the ADCIM
    hal_adcim_probe();
    if (hal_gpai_clk_init()) {
        return -1;
    }

    // Request an IRQ for the GPAI and enable it
    aicos_request_irq(GPAI_IRQn, aich_gpai_isr, 0, NULL, NULL);
    // Enable the GPAI hardware
    aich_gpai_enable(1);
    // Initially, set the number of GPAI channels to 0
    hal_gpai_set_ch_num(0);
    return 0;
}
