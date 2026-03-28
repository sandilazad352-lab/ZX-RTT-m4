#include "zx_mqtt_json.h"
#include "zx_mqtt_client.h"
#include "zx_mqtt_config.h"

#include <rtthread.h>
#include <string.h>

static size_t json_escaped_len(const char *text)
{
    size_t len = 0;

    while (*text)
    {
        unsigned char c = (unsigned char)*text++;
        if (c == '"' || c == '\\' || c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t')
        {
            len += 2;
        }
        else if (c < 0x20)
        {
            len += 6;
        }
        else
        {
            len += 1;
        }
    }

    return len;
}

static char *json_escape_copy(char *dst, const char *src)
{
    static const char hex[] = "0123456789ABCDEF";

    while (*src)
    {
        unsigned char c = (unsigned char)*src++;

        switch (c)
        {
        case '"':
            *dst++ = '\\';
            *dst++ = '"';
            break;
        case '\\':
            *dst++ = '\\';
            *dst++ = '\\';
            break;
        case '\b':
            *dst++ = '\\';
            *dst++ = 'b';
            break;
        case '\f':
            *dst++ = '\\';
            *dst++ = 'f';
            break;
        case '\n':
            *dst++ = '\\';
            *dst++ = 'n';
            break;
        case '\r':
            *dst++ = '\\';
            *dst++ = 'r';
            break;
        case '\t':
            *dst++ = '\\';
            *dst++ = 't';
            break;
        default:
            if (c < 0x20)
            {
                *dst++ = '\\';
                *dst++ = 'u';
                *dst++ = '0';
                *dst++ = '0';
                *dst++ = hex[(c >> 4) & 0x0F];
                *dst++ = hex[c & 0x0F];
            }
            else
            {
                *dst++ = (char)c;
            }
            break;
        }
    }

    *dst = '\0';
    return dst;
}

int zx_mqtt_publish_json_text(const char *text)
{
    if (text == RT_NULL || text[0] == '\0')
    {
        rt_kprintf("[custdemo] empty text, skip publish\n");
        return -1;
    }

    const char *prefix = "{\"message\":\"";
    const char *suffix = "\",\"source\":\"custdemo_ui\"}";
    size_t text_len = json_escaped_len(text);
    size_t payload_len = strlen(prefix) + text_len + strlen(suffix);

    char *payload = (char *)rt_malloc(payload_len + 1);
    if (payload == RT_NULL)
    {
        rt_kprintf("[custdemo] mqtt json malloc failed\n");
        return -1;
    }

    char *p = payload;
    memcpy(p, prefix, strlen(prefix));
    p += strlen(prefix);
    p = json_escape_copy(p, text);
    memcpy(p, suffix, strlen(suffix));
    p += strlen(suffix);
    *p = '\0';

    int ret = zx_mqtt_publish_payload(CUSTDEMO_MQTT_TOPIC, payload, strlen(payload));
    if (ret < 0)
    {
        rt_free(payload);
        return -1;
    }

    rt_kprintf("[custdemo] mqtt publish ok topic=%s payload=%s\n", CUSTDEMO_MQTT_TOPIC, payload);
    rt_free(payload);
    return 0;
}
