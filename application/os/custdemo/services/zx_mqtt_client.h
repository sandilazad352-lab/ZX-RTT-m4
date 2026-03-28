#ifndef __ZX_MQTT_CLIENT_H__
#define __ZX_MQTT_CLIENT_H__

#include <stddef.h>

struct umqtt_client;
typedef struct umqtt_client *zx_umqtt_client_t;

int zx_mqtt_ensure_started(void);
int zx_mqtt_publish_payload(const char *topic, const char *payload, size_t length);
zx_umqtt_client_t zx_mqtt_get_client(void);

#endif  // __ZX_MQTT_CLIENT_H__
