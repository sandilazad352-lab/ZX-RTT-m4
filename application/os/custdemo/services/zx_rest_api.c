#include "zx_rest_api.h"

#include <rtthread.h>
#include <string.h>
#include <stdio.h>

#ifdef LPKG_USING_WEBCLIENT
#include "webclient.h"
#endif

/* Backend server configuration */
static rest_server_config_t server_config = {
    .host = "192.168.15.35",
    .port = 3000,
    .base_path = "/api/items"
};

/* Helper function: Build URL for backend API */
static char* build_url(const char *endpoint)
{
    static char url[512];
    rt_snprintf(url, sizeof(url), "http://%s:%d%s%s",
                server_config.host, server_config.port, server_config.base_path, endpoint);
    return url;
}

/* Initialize REST API */
int zx_rest_api_init(void)
{
    rt_kprintf("[custdemo] rest_api initialized\n");
    return 0;
}

/* Set backend server configuration */
int zx_rest_api_set_server(const char *host, int port, const char *base_path)
{
    if (host == RT_NULL || base_path == RT_NULL)
        return -1;
    
    rt_strncpy(server_config.host, host, sizeof(server_config.host) - 1);
    server_config.port = port;
    rt_strncpy(server_config.base_path, base_path, sizeof(server_config.base_path) - 1);
    
    rt_kprintf("[custdemo] rest_api server set to http://%s:%d%s\n", 
               server_config.host, server_config.port, server_config.base_path);
    return 0;
}

/* Helper: Create JSON request body for item */
static char* item_to_json(rest_item_t *item)
{
    static char json[512];
    
    /* Validate required field: name must not be empty */
    if (!item || !item->name || item->name[0] == '\0')
    {
        rt_kprintf("[custdemo] ERROR: item name is required and cannot be empty\n");
        rt_snprintf(json, sizeof(json), "{\"id\":%d,\"name\":\"Unknown\",\"description\":\"\",\"value\":0}", item ? item->id : 0);
    }
    else
    {
        rt_snprintf(json, sizeof(json),
                    "{\"id\":%d,\"name\":\"%s\",\"description\":\"%s\",\"value\":%d}",
                    item->id, item->name, item->description, item->value);
    }
    
    return json;
}

/* Helper: Parse JSON response to item */
static int json_to_item(const char *json, rest_item_t *item)
{
    if (json == RT_NULL || item == RT_NULL)
        return -1;
    
    /* Parse JSON response from backend with all fields */
    /* Format: {"id":1,"name":"test","description":"desc","value":42,"timestamp":"2026-03-29T..."} */
    
    /* Initialize to safe defaults */
    rt_memset(item, 0, sizeof(rest_item_t));
    
    /* Parse all fields including timestamp */
    int matched = sscanf(json, 
        "{\"id\":%d,\"name\":\"%63[^\"]\",\"description\":\"%255[^\"]\",\"value\":%d,\"timestamp\":\"%31[^\"]\"}",
        &item->id, item->name, item->description, &item->value, item->timestamp);
    
    /* If full parse failed, try without timestamp field (backward compatibility) */
    if (matched < 4)
    {
        matched = sscanf(json,
            "{\"id\":%d,\"name\":\"%63[^\"]\",\"description\":\"%255[^\"]\",\"value\":%d}",
            &item->id, item->name, item->description, &item->value);
    }
    
    return (matched >= 4) ? 0 : -1;
}

/* BACKEND CREATE - POST /api/items 
 * HTTP Method: POST
 * Sends JSON body in request to create new item
 * Expected Response: {"id":N,"name":"...","description":"...","value":0,"timestamp":"..."} 
 */
int zx_rest_backend_create(rest_item_t *item)
{
#ifdef LPKG_USING_WEBCLIENT
    if (item == RT_NULL)
        return -1;

    char *url = build_url("");
    char *post_data = item_to_json(item);
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] backend POST: %s\n", url);
    rt_kprintf("[custdemo] payload: %s\n", post_data);

    /* POST request: (url, headers_ptr, post_data_ptr, post_data_len, response_ptr, response_len_ptr) */
    int result = webclient_request(url, (void *)post_data, rt_strlen(post_data), 0,
                                   (void **)&response, &resp_len);
    
    if (result < 0)
    {
        rt_kprintf("[custdemo] backend create request failed (result=%d)\n", result);
        return -2;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] backend response: %s\n", response);
        json_to_item(response, item);
        web_free(response);
        return 0;
    }

    return -3;
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
    return -1;
#endif
}

/* BACKEND READ - GET /api/items/:id
 * HTTP Method: GET
 * No request body, retrieves single item
 * Expected Response: {"id":1,"name":"...","description":"...","value":0,"timestamp":"..."}
 */
int zx_rest_backend_read(int id, rest_item_t *result)
{
#ifdef LPKG_USING_WEBCLIENT
    if (result == RT_NULL || id <= 0)
        return -1;

    char endpoint[64];
    rt_snprintf(endpoint, sizeof(endpoint), "/%d", id);
    char *url = build_url(endpoint);
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] backend GET: %s\n", url);

    int result_code = webclient_request(url, RT_NULL, RT_NULL, 0, (void **)&response, &resp_len);
    if (result_code < 0)
    {
        rt_kprintf("[custdemo] backend read request failed (result=%d)\n", result_code);
        return -2;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] backend response: %s\n", response);
        json_to_item(response, result);
        web_free(response);
        return 0;
    }

    return -3;
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
    return -1;
#endif
}

/* BACKEND LIST - GET /api/items
 * HTTP Method: GET
 * No request body, retrieves all items
 * Expected Response: [{"id":1,...},{"id":2,...}]
 */
int zx_rest_backend_list(rest_item_t **items, int *count)
{
#ifdef LPKG_USING_WEBCLIENT
    if (items == RT_NULL || count == RT_NULL)
        return -1;

    char *url = build_url("");
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] backend GET LIST: %s\n", url);

    int result_code = webclient_request(url, RT_NULL, RT_NULL, 0, (void **)&response, &resp_len);
    if (result_code < 0)
    {
        rt_kprintf("[custdemo] backend list request failed (result=%d)\n", result_code);
        return -2;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] backend response (%d bytes): %s\n", (int)resp_len, response);
        /* TODO: Parse JSON array response and populate items */
        web_free(response);
        return 0;
    }

    return -3;
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
    return -1;
#endif
}

/* BACKEND UPDATE - PUT /api/items/:id
 * HTTP Method: PUT
 * Sends JSON body with updated fields
 * Expected Response: {"id":N,"name":"...","description":"...","value":0,"timestamp":"..."}
 */
int zx_rest_backend_update(rest_item_t *item)
{
#ifdef LPKG_USING_WEBCLIENT
    if (item == RT_NULL || item->id <= 0)
        return -1;

    char endpoint[64];
    rt_snprintf(endpoint, sizeof(endpoint), "/%d", item->id);
    char *url = build_url(endpoint);
    char *post_data = item_to_json(item);
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] backend PUT: %s\n", url);
    rt_kprintf("[custdemo] payload: %s\n", post_data);

    int result = webclient_request(url, (void *)post_data, rt_strlen(post_data), 0,
                                   (void **)&response, &resp_len);
    
    if (result < 0)
    {
        rt_kprintf("[custdemo] backend update request failed (result=%d)\n", result);
        return -2;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] backend response: %s\n", response);
        web_free(response);
        return 0;
    }

    return -3;
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
    return -1;
#endif
}

/* BACKEND DELETE - DELETE /api/items/:id
 * HTTP Method: DELETE  
 * No request body, deletes item by ID
 * Expected Response: {"success":true}
 */
int zx_rest_backend_delete(int id)
{
#ifdef LPKG_USING_WEBCLIENT
    if (id <= 0)
        return -1;

    char endpoint[64];
    rt_snprintf(endpoint, sizeof(endpoint), "/%d", id);
    char *url = build_url(endpoint);
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] backend DELETE: %s\n", url);

    int result_code = webclient_request(url, RT_NULL, RT_NULL, 0, (void **)&response, &resp_len);
    if (result_code < 0)
    {
        rt_kprintf("[custdemo] backend delete request failed (result=%d)\n", result_code);
        return -2;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] backend response: %s\n", response);
        web_free(response);
        return 0;
    }

    return -3;
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
    return -1;
#endif
}
