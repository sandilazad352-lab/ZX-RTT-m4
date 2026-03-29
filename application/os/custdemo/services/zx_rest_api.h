#ifndef __ZX_REST_API_H__
#define __ZX_REST_API_H__

#include <rtthread.h>

/* Data structure for CRUD operations */
typedef struct {
    int id;
    char name[64];
    char description[256];
    int value;
    char timestamp[32];
} rest_item_t;

/* Backend server configuration */
typedef struct {
    char host[128];
    int port;
    char base_path[128];  /* e.g., "/api/items" */
} rest_server_config_t;

/* Function declarations */
/* Initialization */
int zx_rest_api_init(void);
int zx_rest_api_set_server(const char *host, int port, const char *base_path);

/* Backend CRUD operations (HTTP calls to Node.js) */
int zx_rest_backend_create(rest_item_t *item);       /* POST /api/items */
int zx_rest_backend_read(int id, rest_item_t *result);    /* GET /api/items/:id */
int zx_rest_backend_list(rest_item_t **items, int *count); /* GET /api/items */
int zx_rest_backend_update(rest_item_t *item);       /* PUT /api/items/:id */
int zx_rest_backend_delete(int id);                  /* DELETE /api/items/:id */

#endif  // __ZX_REST_API_H__
