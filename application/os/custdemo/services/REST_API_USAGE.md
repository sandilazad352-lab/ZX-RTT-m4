# Backend REST API Consumer Documentation

## Overview
RT-Thread embedded device consuming Node.js REST API backend. All data operations are performed on remote server via HTTP requests.

## Architecture
- **Pure Backend**: All CRUD operations use HTTP calls to Node.js server
- **No Local Storage**: No in-memory database on embedded device
- **Stateless Client**: Device acts as HTTP client only

---

## Setup & Configuration

### 1. Include Header
```c
#include "zx_rest_api.h"
```

### 2. Initialize
```c
/* Initialize REST API */
zx_rest_api_init();

/* Configure backend server */
zx_rest_api_set_server("192.168.1.10", 3000, "/api/items");
```

**Configuration Parameters:**
- `host`: Backend server IP/hostname
- `port`: HTTP server port
- `base_path`: API base path

**Default Configuration:** `http://192.168.1.100:3000/api/items`

---

## Backend Setup (Node.js Example)

### Express.js Backend Implementation
```javascript
const express = require('express');
const app = express();

app.use(express.json());

const items = [];
let nextId = 1;

// CREATE - POST /api/items
app.post('/api/items', (req, res) => {
    const item = {
        id: nextId++,
        name: req.body.name,
        description: req.body.description,
        value: req.body.value,
        timestamp: new Date().toISOString()
    };
    items.push(item);
    res.json(item);
});

// READ - GET /api/items/:id
app.get('/api/items/:id', (req, res) => {
    const item = items.find(i => i.id === parseInt(req.params.id));
    if (!item) return res.status(404).json({ error: 'Not found' });
    res.json(item);
});

// LIST - GET /api/items
app.get('/api/items', (req, res) => {
    res.json(items);
});

// UPDATE - PUT /api/items/:id
app.put('/api/items/:id', (req, res) => {
    const item = items.find(i => i.id === parseInt(req.params.id));
    if (!item) return res.status(404).json({ error: 'Not found' });
    
    item.name = req.body.name || item.name;
    item.description = req.body.description || item.description;
    item.value = req.body.value || item.value;
    res.json(item);
});

// DELETE - DELETE /api/items/:id
app.delete('/api/items/:id', (req, res) => {
    const idx = items.findIndex(i => i.id === parseInt(req.params.id));
    if (idx === -1) return res.status(404).json({ error: 'Not found' });
    
    items.splice(idx, 1);
    res.json({ success: true });
});

app.listen(3000, () => console.log('API listening on port 3000'));
```

---

## CRUD Operations

### 1. CREATE - Add New Item (HTTP POST)
```c
rest_item_t item = {0};
rt_strncpy(item.name, "Device1", sizeof(item.name) - 1);
rt_strncpy(item.description, "Temperature sensor", sizeof(item.description) - 1);
item.value = 25;  // Temperature value

int result = zx_rest_backend_create(&item);

if (result == 0) {
    rt_kprintf("✓ Created successfully\n");
    rt_kprintf("  ID: %d\n", item.id);
    rt_kprintf("  Name: %s\n", item.name);
    rt_kprintf("  Value: %d\n", item.value);
} else {
    rt_kprintf("✗ Create failed (error: %d)\n", result);
}
```

**HTTP Request:**
```
POST /api/items HTTP/1.1
Host: 192.168.1.10:3000
Content-Type: application/json

{"id":0,"name":"Device1","description":"Temperature sensor","value":25}
```

**Expected Response:**
```json
{
    "id": 1,
    "name": "Device1",
    "description": "Temperature sensor",
    "value": 25,
    "timestamp": "2026-03-29T10:30:00.000Z"
}
```

---

### 2. READ - Fetch Single Item (HTTP GET)
```c
rest_item_t item = {0};
int result = zx_rest_backend_read(1, &item);  // Get item with ID=1

if (result == 0) {
    rt_kprintf("✓ Read successfully\n");
    rt_kprintf("  ID: %d\n", item.id);
    rt_kprintf("  Name: %s\n", item.name);
    rt_kprintf("  Description: %s\n", item.description);
    rt_kprintf("  Value: %d\n", item.value);
} else {
    rt_kprintf("✗ Read failed (error: %d)\n", result);
}
```

**HTTP Request:**
```
GET /api/items/1 HTTP/1.1
Host: 192.168.1.10:3000
```

---

### 3. LIST - Get All Items (HTTP GET)
```c
rest_item_t *items = RT_NULL;
int count = 0;

int result = zx_rest_backend_list(&items, &count);

if (result == 0) {
    rt_kprintf("✓ Listed %d items\n", count);
    for (int i = 0; i < count; i++) {
        rt_kprintf("  [%d] %s (value=%d)\n", 
                   items[i].id, items[i].name, items[i].value);
    }
    if (items) rt_free(items);  // Free allocated memory
} else {
    rt_kprintf("✗ List failed (error: %d)\n", result);
}
```

**HTTP Request:**
```
GET /api/items HTTP/1.1
Host: 192.168.1.10:3000
```

---

### 4. UPDATE - Modify Existing Item (HTTP PUT)
```c
rest_item_t item = {0};
item.id = 1;  // Must set ID of item to update
rt_strncpy(item.name, "Device1_Updated", sizeof(item.name) - 1);
rt_strncpy(item.description, "Updated description", sizeof(item.description) - 1);
item.value = 30;  // New value

int result = zx_rest_backend_update(&item);

if (result == 0) {
    rt_kprintf("✓ Updated successfully\n");
    rt_kprintf("  ID: %d\n", item.id);
    rt_kprintf("  New Value: %d\n", item.value);
} else {
    rt_kprintf("✗ Update failed (error: %d)\n", result);
}
```

**HTTP Request:**
```
PUT /api/items/1 HTTP/1.1
Host: 192.168.1.10:3000
Content-Type: application/json

{"id":1,"name":"Device1_Updated","description":"Updated description","value":30}
```

---

### 5. DELETE - Remove Item (HTTP DELETE)
```c
int result = zx_rest_backend_delete(1);  // Delete item with ID=1

if (result == 0) {
    rt_kprintf("✓ Deleted successfully\n");
} else {
    rt_kprintf("✗ Delete failed (error: %d)\n", result);
}
```

**HTTP Request:**
```
DELETE /api/items/1 HTTP/1.1
Host: 192.168.1.10:3000
```

---

## Complete Example Program

```c
#include "zx_rest_api.h"
#include <rtthread.h>

static void backend_crud_demo(void *parameter)
{
    RT_UNUSED(parameter);
    rt_thread_mdelay(2000);  // Wait for network ready

    /* Initialize */
    zx_rest_api_init();
    zx_rest_api_set_server("192.168.1.10", 3000, "/api/items");

    rt_kprintf("\n=== Backend CRUD Demo ===\n\n");

    /* 1. CREATE */
    rt_kprintf("[1] Creating item...\n");
    rest_item_t item = {0};
    rt_strncpy(item.name, "Sensor1", sizeof(item.name) - 1);
    rt_strncpy(item.description, "Room temperature", sizeof(item.description) - 1);
    item.value = 22;

    int result = zx_rest_backend_create(&item);
    if (result == 0) {
        rt_kprintf("    ✓ Created with ID: %d\n\n", item.id);
    } else {
        rt_kprintf("    ✗ Failed with error: %d\n\n", result);
        return;
    }

    /* 2. READ */
    rt_kprintf("[2] Reading item ID=%d...\n", item.id);
    rest_item_t read_item = {0};
    result = zx_rest_backend_read(item.id, &read_item);
    if (result == 0) {
        rt_kprintf("    ✓ Read: %s = %d\n\n", read_item.name, read_item.value);
    } else {
        rt_kprintf("    ✗ Failed with error: %d\n\n", result);
    }

    /* 3. UPDATE */
    rt_kprintf("[3] Updating item ID=%d...\n", item.id);
    item.value = 25;  // New temperature
    result = zx_rest_backend_update(&item);
    if (result == 0) {
        rt_kprintf("    ✓ Updated value to %d\n\n", item.value);
    } else {
        rt_kprintf("    ✗ Failed with error: %d\n\n", result);
    }

    /* 4. LIST */
    rt_kprintf("[4] Listing all items...\n");
    rest_item_t *items = RT_NULL;
    int count = 0;
    result = zx_rest_backend_list(&items, &count);
    if (result == 0) {
        rt_kprintf("    ✓ Found %d item(s)\n", count);
        if (items) rt_free(items);
    } else {
        rt_kprintf("    ✗ Failed with error: %d\n", result);
    }
    rt_kprintf("\n");

    /* 5. DELETE */
    rt_kprintf("[5] Deleting item ID=%d...\n", item.id);
    result = zx_rest_backend_delete(item.id);
    if (result == 0) {
        rt_kprintf("    ✓ Deleted successfully\n\n");
    } else {
        rt_kprintf("    ✗ Failed with error: %d\n\n", result);
    }

    rt_kprintf("=== Demo Complete ===\n");
}

int backend_crud_demo_start(void)
{
    rt_thread_t tid = rt_thread_create("backend_demo",
                                       backend_crud_demo,
                                       RT_NULL,
                                       3072,  // Stack size
                                       10,    // Priority
                                       10);   // Timeslice
    if (tid == RT_NULL) {
        rt_kprintf("Failed to create demo thread\n");
        return -1;
    }
    rt_thread_startup(tid);
    return 0;
}
```

**Run it:**
```c
backend_crud_demo_start();
```

---

## Return Codes

| Code | Meaning | Action |
|------|---------|--------|
| **0** | Success | Operation completed |
| **-1** | Invalid parameters | Check function arguments |
| **-2** | Item not found | ID doesn't exist on server |
| **-3** | HTTP error | Network/server error |

---

## Data Structure

```c
typedef struct {
    int id;                      // Item ID (assigned by server)
    char name[64];              // Item name
    char description[256];      // Item description
    int value;                  // Item value/data
    char timestamp[32];         // Server timestamp
} rest_item_t;
```

---

## JSON Request/Response Format

### Request (POST/PUT)
```json
{
    "id": 0,                    // 0 for new items, ID for updates
    "name": "Device Name",
    "description": "Description",
    "value": 100
}
```

### Response (Success)
```json
{
    "id": 1,
    "name": "Device Name",
    "description": "Description",
    "value": 100,
    "timestamp": "2026-03-29T10:30:45.123Z"
}
```

### Response (Error)
```json
{
    "error": "Item not found"
}
```

---

## Configuration & Build

### Required Configuration
Enable in Kconfig:
- `LPKG_USING_WEBCLIENT` - HTTP client library

### Modify Default Server
In `zx_rest_api.c`:
```c
static rest_server_config_t server_config = {
    .host = "192.168.1.100",     // Change IP
    .port = 3000,                // Change port
    .base_path = "/api/items"    // Change API path
};
```

Or use function:
```c
zx_rest_api_set_server("new.ip.address", 8080, "/api/v2/items");
```

---

## Troubleshooting

### Connection Failed
- ✓ Verify server IP and port
- ✓ Check network connectivity
- ✓ Ensure server is running

### Item Not Found
- ✓ Verify item ID exists
- ✓ Check item was created successfully
- ✓ Query LIST to see available items

### JSON Parse Error
- ✓ Verify server response format matches expected JSON
- ✓ Check character encoding (UTF-8)
- ✓ Ensure escaped quotes in strings

### Memory Issues
- ✓ Increase thread stack size (minimum 3KB)
- ✓ Free allocated memory from `LIST` operation
- ✓ Avoid large descriptions (max 256 chars)

---

## Best Practices

1. **Always initialize before use:**
   ```c
   zx_rest_api_init();
   zx_rest_api_set_server(...);
   ```

2. **Check return codes:**
   ```c
   if (zx_rest_backend_create(&item) != 0) {
       rt_kprintf("Error creating item\n");
   }
   ```

3. **Free allocated memory:**
   ```c
   rest_item_t *list = RT_NULL;
   int count = 0;
   zx_rest_backend_list(&list, &count);
   if (list) rt_free(list);
   ```

4. **Use proper string copying:**
   ```c
   rt_strncpy(item.name, "Value", sizeof(item.name) - 1);
   ```

5. **Add delay before making requests:**
   ```c
   rt_thread_mdelay(2000);  // Wait for network
   ```

---

## Network Requirements

- TCP/IP stack configured
- Network interface initialized
- Backend server reachable and running
- HTTP port open and accessible
- Recommended: 8KB+ available heap memory


