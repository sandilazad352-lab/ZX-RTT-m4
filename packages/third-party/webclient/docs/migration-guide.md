# Migration Guide

## 1. V1.0.0 -> V2.0.0 Changes

This section describes the changes to the WebClient package after upgrading from `V1.0.0` to the latest version `V2.0.0` and how to adapt to the new version.

### Function Changes

1. Added `webclient_session_create()` function for creating client session structures

    In V1.0.0, the client session structure did not need to be manually created. In V2.0.0, manual creation is required, allowing users to customize the header buffer length and reduce resource usage.

2. `webclient_open` -> `webclient_get`

    The V1.0.0 function `webclient_open` for sending GET requests has been renamed to `webclient_get`. The difference is that `webclient_open` only sends GET requests with default headers, while `webclient_get` supports both default and custom request headers.

3. `webclient_open_position` -> `webclient_get_position`

    Similar changes to the `webclient_open` function mentioned above.

4. Added `webclient_post` function for sending POST requests

    In V1.0.0, sending POST requests required users to complete the entire process including session creation, connection, and header transmission. V2.0.0 provides a `webclient_post` function that encapsulates this entire process. See [API Reference](api.md) and [User Guide](user-guide.md) for usage details.

5. Enhanced header field handling

    V1.0.0 lacked methods for processing header data; users could only concatenate strings. V2.0.0 introduces improvements: `webclient_header_fields_add` for adding request header fields and `webclient_header_fields_get` for retrieving response header field data.

6. Additional utility functions

    - `webclient_resp_status_get()`: Get current response status code
    - `webclient_content_length_get()`: Get current response Content-Length

### Process Changes

#### GET Request Process

Example: Sending a GET request with custom headers.

V1.0.0 GET request flow:

- Concatenate headers (string concatenation)
- Create client session structure
- Establish connection with server
- Send GET request headers
- Receive and parse server response headers
- Receive server response body
- Close connection

```c
const char header = "xxx";
struct webclient_session *session = RT_NULL;

session = web_malloc(sizeof(struct webclient_session))

webclient_connect(session, URI);

webclient_send_header(session, WEBCLIENT_GET, header, strlen(header));

webclient_handle_response(session);

while(1)
{
     webclient_read(session, post_data, strlen(post_data));
     ...
}

webclient_close(session);
```

V2.0.0 GET request flow:

- Create session structure
- Concatenate headers (function-based)
- Send GET request and receive response
- Receive server response data
- Close connection

```c
struct webclient_session *session = RT_NULL;

session = webclient_session_create(1024)

webclient_header_fields_add(session, "HOST: %s", URI);

webclient_get(session, URI);

while(1)
{
     webclient_read(session, buffer, buf_sz);
     ...
}

webclient_close(session);
```

#### POST Request Process

Example: Sending a POST request with custom headers.

V1.0.0 POST request flow:

- Concatenate headers (string concatenation)
- Create client session structure
- Establish connection with server
- Send POST request headers
- Receive and parse server response headers
- Upload data to server
- Close connection

```c
const char header = "xxx";
struct webclient_session *session = RT_NULL;

session = web_malloc(sizeof(struct webclient_session))

webclient_connect(session, URI);

webclient_send_header(session, WEBCLIENT_POST, header, strlen(header));

webclient_handle_response(session);

while(1)
{
     webclient_write(session, post_data, strlen(post_data));
     ...
}

webclient_close(session);
```

V2.0.0 POST request flow:

- Create session structure
- Concatenate headers (function-based)
- Send headers and body data to server, receive server response
- Close connection

```c
struct webclient_session *session = RT_NULL;

session = webclient_session_create(1024)

webclient_header_fields_add(session, "Content-Length: %s", post_data_sz);

webclient_post(session, URI, post_data, strlen(post_data));

webclient_close(session);
```

For more details on GET/POST workflows, see the package [User Guide](user-guide.md).

## 2. V2.1.0 -> V2.2.0 Changes

V2.2.0 primarily modifies function parameter definitions to support non-string data formats. Other function workflows remain unchanged.

### Function Changes

- `webclient_post` parameters updated

  Changed `const char *post_data` to `const void *post_data` and added `size_t data_len` parameter to specify data length.

- `webclient_read` parameters updated

  Changed `unsigned char *buffer` to `void *buffer` to support non-string data reception.

- `webclient_write` parameters updated

  Changed `const unsigned char *buffer` to `const void *buffer` to support non-string data transmission.

- `webclient_response` parameters updated

  Changed `unsigned char **response` to `void **response` and added `size_t *resp_len` parameter to retrieve response data length for non-string data reception.

- `webclient_request` parameters updated

  Changed `const char *post_data` to `const void *post_data`, `unsigned char **response` to `void **response`, and added `size_t data_len` and `size_t *resp_len` parameters to support non-string data transmission and reception.

