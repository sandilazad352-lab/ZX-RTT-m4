#include "zx_mqtt_client.h"
#include "zx_mqtt_config.h"

#include <rtthread.h>

#ifdef PKG_USING_UMQTT
#include "umqtt.h"
#endif

#ifdef PKG_USING_UMQTT
static umqtt_client_t s_mqtt_client;
static rt_bool_t s_mqtt_started = RT_FALSE;

zx_umqtt_client_t zx_mqtt_get_client(void)
{
    return s_mqtt_client;
}

int zx_mqtt_ensure_started(void)
{
    if (s_mqtt_started && s_mqtt_client != RT_NULL)
    {
        return 0;
    }

    struct umqtt_info info = {0};
    info.uri = CUSTDEMO_MQTT_URI;
    info.client_id = CUSTDEMO_MQTT_CLIENT_ID;

    s_mqtt_client = umqtt_create(&info);
    if (s_mqtt_client == RT_NULL)
    {
        rt_kprintf("[custdemo] mqtt create failed\n");
        return -1;
    }

    if (umqtt_start(s_mqtt_client) < 0)
    {
        rt_kprintf("[custdemo] mqtt start failed\n");
        umqtt_delete(s_mqtt_client);
        s_mqtt_client = RT_NULL;
        return -1;
    }

    s_mqtt_started = RT_TRUE;
    rt_kprintf("[custdemo] mqtt connected: %s\n", CUSTDEMO_MQTT_URI);
    return 0;
}
#else
int zx_mqtt_ensure_started(void)
{
    rt_kprintf("[custdemo] PKG_USING_UMQTT not enabled\n");
    return -1;
}

zx_umqtt_client_t zx_mqtt_get_client(void)
{
    return RT_NULL;
}
#endif

int zx_mqtt_publish_payload(const char *topic, const char *payload, size_t length)
{
    if (topic == RT_NULL || topic[0] == '\0' || payload == RT_NULL || length == 0)
    {
        rt_kprintf("[custdemo] invalid mqtt publish params\n");
        return -1;
    }

#ifdef PKG_USING_UMQTT
    if (zx_mqtt_ensure_started() < 0)
    {
        rt_kprintf("[custdemo] mqtt not ready\n");
        return -1;
    }

    int ret = umqtt_publish(s_mqtt_client,
                            UMQTT_QOS1,
                            topic,
                            (void *)payload,
                            length,
                            1000);
    if (ret < 0)
    {
        rt_kprintf("[custdemo] mqtt publish failed: %d\n", ret);
        return -1;
    }

    return 0;
#else
    rt_kprintf("[custdemo] PKG_USING_UMQTT not enabled\n");
    return -1;
#endif
}
