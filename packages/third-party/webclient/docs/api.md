# API Documentation

## Create Session

```c
struct webclient_session *webclient_session_create(size_t header_sz);
```

Create a client session structure.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|header_sz          | Maximum supported header length     |
| **Return**        | **Description**                     |
|!= NULL            | Pointer to webclient session structure |
|= NULL             | Creation failed                     |

## Close Session Connection

```c
int webclient_close(struct webclient_session *session);
```

Close the client session connection and release memory.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
| **Return**        | **Description**                     |
|=0                 | Success                             |

## Send GET Request

```c
int webclient_get(struct webclient_session *session, const char *URI);
```

Send an HTTP GET request.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|URI                | HTTP server address to connect      |
| **Return**        | **Description**                     |
|`>0`               | HTTP response status code           |
|<0                 | Failed to send request              |

## Send HEAD/GET Request with Specified Data Size

```c
int webclient_shard_position_function(struct webclient_session *session, const char *URI, int size);
```

Send an HTTP GET/HEAD request with Range header for resumable/chunked downloads.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|URI                | HTTP server address to connect      |
|size               | Receive buffer size                 |
| **Return**        | **Description**                     |
|`>0`               | HTTP response status code           |
|<0                 | Failed to send request              |

## Send POST Request

```c
int webclient_post(struct webclient_session *session, const char *URI, const void *post_data, size_t data_len);
```

Send an HTTP POST request to upload data to the HTTP server.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|URI                | HTTP server address to connect      |
|post_data          | Address of data to upload           |
|data_len           | Length of data to upload            |
| **Return**        | **Description**                     |
|`>0`               | HTTP response status code           |
|<0                 | Failed to send request              |

## Send Data

```c
int webclient_write(struct webclient_session *session, const void *buffer, size_t size);
```

Send data to the connected server.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|buffer             | Address of data to send             |
|size               | Length of data to send              |
| **Return**        | **Description**                     |
|`>0`               | Length of data successfully sent    |
|=0                 | Connection closed                   |
|<0                 | Failed to send data                 |

## Receive Data

```c
int webclient_read(struct webclient_session *session, void *buffer, size_t size);
```

Receive data from the connected server.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|buffer             | Address to store received data      |
|size               | Maximum length of data to receive   |
| **Return**        | **Description**                     |
|`>0`               | Length of data successfully received |
|=0                 | Connection closed                   |
|<0                 | Failed to receive data              |

## Set Receive and Send Timeout

```c
int webclient_set_timeout(struct webclient_session *session, int millisecond);
```

Set the receive and send timeout for the connection.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|millisecond        | Timeout duration in milliseconds    |
| **Return**        | **Description**                     |
|=0                 | Timeout set successfully            |

## Add Header Fields

```c
int webclient_header_fields_add(struct webclient_session *session, const char *fmt, ...);
```

Add header fields after creating the session and before sending GET/POST requests.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|fmt                | Format string for field data        |
|...                | Field data as variable arguments   |
| **Return**        | **Description**                     |
|`>0`               | Length of field data successfully added |
| <=0               | Failed to add or header exceeds limit |

## Get Header Field Value

```c
const char *webclient_header_fields_get(struct webclient_session *session, const char *fields);
```

Retrieve header field value by field name after sending GET/POST requests.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|fields             | HTTP field name                     |
| **Return**        | **Description**                     |
|= NULL             | Failed to retrieve data             |
|!= NULL            | Successfully retrieved field data   |

## Receive Response Data to Specified Address

```c
int webclient_response(struct webclient_session *session, void **response, size_t *resp_len);
```

Receive response data to a specified address after sending GET/POST requests.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
|response           | String address to store received data |
|resp_len           | Pointer to received data length     |
| **Return**        | **Description**                     |
| `>0`              | Length of data successfully received |
| <=0               | Failed to receive data              |

## Send GET/POST Request and Receive Response

```c
int webclient_request(const char *URI, const char *header, const void *post_data, size_t data_len, void **response, size_t *resp_len);
```

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|URI                | HTTP server address to connect      |
|header             | Header data to send                 |
|                   | = NULL: send default headers, supports GET/POST |
|                   | != NULL: send specified headers, supports GET/POST |
|post_data          | Data to send to server              |
|                   | = NULL: GET request                 |
|                   | != NULL: POST request               |
| data_len          | Length of data to send              |
|response           | String address to store received data |
|resp_len           | Pointer to received data length     |
| **Return**        | **Description**                     |
| `>0`              | Length of data successfully received |
| <=0               | Failed to receive data              |

## Construct Request Header Data

```c
int webclient_request_header_add(char **request_header, const char *fmt, ...);
```

Construct and add header data before sending requests with webclient_request.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|request_header     | Address of request header buffer    |
|fmt                | Format string for field data        |
|...                | Field data as variable arguments   |
| **Return**        | **Description**                     |
| `>0`              | Length of field data successfully added |
| <=0               | Failed to add header or insufficient memory |

## Get HTTP Response Status Code

```c
int webclient_resp_status_get(struct webclient_session *session);
```

Get the HTTP response status code after sending GET/POST requests.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
| **Return**        | **Description**                     |
| `>0`              | HTTP response status code           |

## Get Content-Length Field

```c
int webclient_content_length_get(struct webclient_session *session);
```

Get the Content-Length field value after sending GET/POST requests.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|session            | Pointer to current connection session structure |
| **Return**        | **Description**                     |
| `>0`              | Content-Length field value          |
| <0                | Failed to retrieve                  |

## Download File to Local

```c
int webclient_get_file(const char *URI, const char *filename);
```

Download a file from the HTTP server and save it locally.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|URI                | HTTP server address to connect      |
|filename           | Local file path and name            |
| **Return**        | **Description**                     |
|=0                 | Download successful                 |
|<0                 | Download failed                     |

## Upload File to Server

```c
int webclient_post_file(const char *URI, const char *filename, const char *form_data);
```

Upload a file to the HTTP server.

| Parameter         | Description                         |
|:------------------|:-----------------------------------|
|URI                | HTTP server address to connect      |
|filename           | Local file path and name to upload  |
|form_data          | Additional options                  |
| **Return**        | **Description**                     |
|=0                 | Upload successful                   |
|<0                 | Upload failed                       |
