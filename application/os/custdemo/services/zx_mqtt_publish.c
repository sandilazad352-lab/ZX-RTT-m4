#include "zx_mqtt_publish.h"
#include "zx_mqtt_json.h"

int zx_mqtt_publish_text(const char *text)
{
    return zx_mqtt_publish_json_text(text);
}
