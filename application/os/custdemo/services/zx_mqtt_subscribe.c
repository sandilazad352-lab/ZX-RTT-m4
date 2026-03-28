#include "zx_mqtt_subscribe.h"

#include "zx_mqtt_client.h"
#include "zx_mqtt_config.h"

#include <rtthread.h>

#ifdef PKG_USING_UMQTT
#include "umqtt.h"
#include "umqtt_internal.h"
#endif

static zx_mqtt_subscribe_cb_t s_subscribe_cb = RT_NULL;

#ifdef PKG_USING_UMQTT
static void mqtt_topic_recv_callback(struct umqtt_client *client, void *msg_data)
{
    RT_ASSERT(client);

    if (msg_data == RT_NULL)
    {
        return;
    }

    struct umqtt_pkgs_publish *msg = (struct umqtt_pkgs_publish *)msg_data;

    rt_kprintf("[custdemo] mqtt recv topic=%.*s payload=%.*s\n",
               (int)msg->topic_name_len,
               msg->topic_name,
               (int)msg->payload_len,
               msg->payload);

    if (s_subscribe_cb != RT_NULL)
    {
        s_subscribe_cb(msg->topic_name,
                       msg->topic_name_len,
                       (const char *)msg->payload,
                       msg->payload_len);
    }
}
#endif

void zx_mqtt_set_subscribe_callback(zx_mqtt_subscribe_cb_t cb)
{
    s_subscribe_cb = cb;
}

int zx_mqtt_subscribe_topic(const char *topic)
{
    if (topic == RT_NULL || topic[0] == '\0')
    {
        rt_kprintf("[custdemo] invalid mqtt subscribe topic\n");
        return -1;
    }

#ifdef PKG_USING_UMQTT
    if (zx_mqtt_ensure_started() < 0)
    {
        rt_kprintf("[custdemo] mqtt not ready\n");
        return -1;
    }

    zx_umqtt_client_t client = zx_mqtt_get_client();
    if (client == RT_NULL)
    {
        rt_kprintf("[custdemo] mqtt client null\n");
        return -1;
    }

    int ret = umqtt_subscribe(client, topic, UMQTT_QOS1, mqtt_topic_recv_callback);
    if (ret < 0)
    {
        rt_kprintf("[custdemo] mqtt subscribe failed: %d, topic=%s\n", ret, topic);
        return -1;
    }

    rt_kprintf("[custdemo] mqtt subscribe ok topic=%s\n", topic);
    return 0;
#else
    rt_kprintf("[custdemo] PKG_USING_UMQTT not enabled\n");
    return -1;
#endif
}

int zx_mqtt_subscribe_default_topic(void)
{
    return zx_mqtt_subscribe_topic(CUSTDEMO_MQTT_TOPIC);
}
