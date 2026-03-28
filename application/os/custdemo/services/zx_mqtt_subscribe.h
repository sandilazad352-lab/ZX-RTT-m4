#ifndef __ZX_MQTT_SUBSCRIBE_H__
#define __ZX_MQTT_SUBSCRIBE_H__

#include <stddef.h>

typedef void (*zx_mqtt_subscribe_cb_t)(const char *topic,
                                       size_t topic_len,
                                       const char *payload,
                                       size_t payload_len);

void zx_mqtt_set_subscribe_callback(zx_mqtt_subscribe_cb_t cb);
int zx_mqtt_subscribe_topic(const char *topic);
int zx_mqtt_subscribe_default_topic(void);

#endif  // __ZX_MQTT_SUBSCRIBE_H__
