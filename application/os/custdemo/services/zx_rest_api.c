#include "zx_rest_api.h"

#include <rtthread.h>

#ifdef LPKG_USING_WEBCLIENT
#include "webclient.h"
#endif

#define CUSTDEMO_REST_API_URL "http://192.168.1.7:11434/api/chat"

static void rest_api_worker(void *parameter)
{
    RT_UNUSED(parameter);

#ifdef LPKG_USING_WEBCLIENT
    char *response = RT_NULL;
    size_t resp_len = 0;

    rt_kprintf("[custdemo] rest GET: %s\n", CUSTDEMO_REST_API_URL);

    if (webclient_request(CUSTDEMO_REST_API_URL, RT_NULL, RT_NULL, 0, (void **)&response, &resp_len) < 0)
    {
        rt_kprintf("[custdemo] rest request failed\n");
        return;
    }

    if (response != RT_NULL)
    {
        rt_kprintf("[custdemo] rest json (%d bytes):\n%s\n", (int)resp_len, response);
        web_free(response);
    }
    else
    {
        rt_kprintf("[custdemo] rest response empty\n");
    }
#else
    rt_kprintf("[custdemo] LPKG_USING_WEBCLIENT not enabled\n");
#endif
}

int zx_rest_api_fetch_json_async(void)
{
    rt_thread_t tid = rt_thread_create("restapi",
                                       rest_api_worker,
                                       RT_NULL,
                                       4096,
                                       15,
                                       10);
    if (tid == RT_NULL)
    {
        rt_kprintf("[custdemo] rest worker create failed\n");
        return -1;
    }

    rt_thread_startup(tid);
    return 0;
}
