# Sample Programs

The WebClient package provides three HTTP Client sample programs to demonstrate GET and POST functionality for data upload and download, as well as a complete file chunked download feature.

**Sample Files**

| Sample Program Path                            | Description |
| ----                                           | ---- |
| samples/webclient_get_sample.c                 | GET request test program |
| samples/webclient_post_sample.c                | POST request test program |
| samples/webclient_shard_download_sample.c      | Chunked download test program |

## Preparation

### Getting the Package

- Use menuconfig to obtain the package and sample code

    Open the ENV tool provided by RT-Thread and use **menuconfig** to configure the package.

    Enable the WebClient package and configure to enable test programs (Enable webclient GET/POST/SHARD samples), as shown below:

```shell
RT-Thread online packages
    IoT - internet of things  --->
        [*] WebClient: A HTTP/HTTPS Client for RT-Thread
        [ ]   Enable debug log output
        [*]   Enable webclient GET/POST/SHARD samples # Enable WebClient test programs
        [ ]   Enable file download feature support
              Select TLS mode (Not support)  --->
              Version (latest)  --->            # Use latest package version
```

- Use `pkgs --update` command to download the package
- Compile and download

## Running Examples

The test website used in these examples is the official RT-Thread website. The GET request example retrieves and prints file content from the website; the POST request example uploads data to the test website, which responds with the same data.

> HTTP data transmission includes header and body parts. Below, header data is referred to as `header data` and body data as `body data`.

### GET Request Example

GET request example flow:

- Create a client session structure
- Client sends GET request header data (using default header)
- Server responds with header and body data
- Print server response body data
- GET request test completed/failed

There are two ways to use the GET request example:

- Execute the GET request example using the `web_get_test` command in MSH to retrieve and display file information from the default URL. Use `web_get_test -s` to execute using the simplified interface (webclient_request), suitable for short data transmission. The log output is shown below:

```c
msh />web_get_test
webclient get response data:
RT-Thread is an open source IoT operating system from China, which has strong scalability: from a tiny kernel running on a tiny core, for example ARM Cortex-M0, or Cortex-M3/4/7, to a rich feature system running on MIPS32, ARM Cortex-A8, ARM Cortex-A9 DualCore etc.

msh />web_get_test -s
webclient send get request by simplify request interface.
webclient get response data:
RT-Thread is an open source IoT operating system from China, which has strong scalability: from a tiny kernel running on a tiny core, for example ARM Cortex-M0, or Cortex-M3/4/7, to a rich feature system running on MIPS32, ARM Cortex-A8, ARM Cortex-A9 DualCore etc.

msh />
```

- Execute the GET request example using `web_get_test [URI]` or `web_get_test -s [URI]` in MSH, where URI is a custom URL supporting GET requests.

### POST Request Example

POST request example flow:

- Create a client session structure
- Construct header data for POST request
- Client sends constructed header and body data
- Server responds with header and body data
- Print server response body data
- POST request test completed/failed

There are two ways to use the POST request example:

- Execute the POST request example using `web_post_test` command in MSH to retrieve and print response data (the default POST address echoes back uploaded data). Use `web_post_test -s` for the simplified interface. Log output is shown below:

```c
msh />web_post_test
webclient post response data :
RT-Thread is an open source IoT operating system from China!
msh />
msh />web_post_test -s
webclient send post request by simplify request interface.
webclient post response data:
RT-Thread is an open source IoT operating system from China!
msh />
```

- Execute the POST request example using `web_post_test [URI]` or `web_post_test -s [URI]`, where URI is a custom URL supporting POST requests.

### Chunked Download Example

Chunked download example flow:

- Create a client session structure
- Client sends HEAD request header data
- Server responds with header data
- Client sends GET request header data with Range field
- Server responds with header and specified body data
- Loop sending requests until all data is received
- Chunked download test completed/failed

There are two ways to use the chunked download example:

- Execute the chunked download example using `web_get_test -l [size]` in MSH to specify the maximum body data length and retrieve file information:

```c
msh >web_shard_test -l 115
Receive, len[0115]:
0000 - 0059: RT-Thread is an open source IoT operating system from China,
0060 - 0114:  which has strong scalability: from a tiny kernel runni
Total: [0115]Bytes

Receive, len[0115]:
0000 - 0059: ng on a tiny core, for example ARM Cortex-M0, or Cortex-M3/4
0060 - 0114: /7, to a rich feature system running on MIPS32, ARM Cor
Total: [0115]Bytes

Receive, len[0037]:
0000 - 0036: tex-A8, ARM Cortex-A9 DualCore etc.

Total: [0037]Bytes
msh />
```

The data obtained from multiple chunked downloads is identical to that from standard GET requests, confirming the chunked download feature works correctly.

- Execute the chunked download example using `web_get_test -u [URI]`, where URI is a custom URL supporting GET requests.