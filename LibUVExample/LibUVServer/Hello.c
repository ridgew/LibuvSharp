#include "uv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
libev / libuv / python tornado / nginx  性能比较
https://blog.csdn.net/beginning1126/article/details/49452015


https://www.cnblogs.com/cnxkey/articles/10058702.html
vs2017下搭建libuv环境

添加依赖项和忽略LIBCMTD库：
确保添加了以下几个依赖项：
libuv.lib
Ws2_32.lib
iphlpapi.lib
Userenv.lib
Psapi.lib

注：如果是编译x64的要加几个依赖：
advapi32.lib
iphlpapi.lib
psapi.lib
shell32.lib
userenv.lib
ws2_32.lib
user32.lib

*/


typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

static uv_tcp_t tcpServer;
static uv_loop_t* loop;
static uv_handle_t* server;

static void echo_alloc(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) {
	printf("echo alloc called, size[%d]\n", suggested_size);
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}


static void after_write(uv_write_t* req, int status) {
	write_req_t* wr;

	/* Free the read/write buffer and the request */
	wr = (write_req_t*)req;
	// free(wr->buf.base);
	free(wr);
	printf("after_write is called\n");
	if (status == 0)
		return;

	fprintf(stderr,
		"uv_write error: %s - %s\n",
		uv_err_name(status),
		uv_strerror(status));
}
static void on_close(uv_handle_t* peer) {
	free(peer);
}
static void after_read(uv_stream_t* handle,
	ssize_t nread,
	const uv_buf_t* buf) {
	int i;
	write_req_t *wr;
	uv_shutdown_t* sreq;

	free(buf->base);

	printf("1111---after_read is called\n");

	if (nread == UV_EOF) {
		printf("22222---after_read is called\n");
		return;
	}
	printf("3333---after_read is called\n");
	char * resp = "HTTP/1.1 200 OK\r\n\
Server: nginx/1.9.3\r\n\
Date: Thu, 13 Aug 2015 03:04:20 GMT\r\n\
Content-Type: text/plain\r\n\
Content-Length: 17\r\n\
Connection: keep-alive\r\n\r\n\
hello_world, shen";

	wr = (write_req_t*)malloc(sizeof *wr);
	wr->buf = uv_buf_init(resp, strlen(resp));

	if (uv_write(&wr->req, handle, &wr->buf, 1, after_write)) {
		printf("uv_write failed\n");
	}
	printf("4444---after_read is called\n");
	uv_close((uv_handle_t*)handle, on_close);
}



static void on_connection(uv_stream_t* server, int status) {
	uv_stream_t* stream;
	int r;

	if (status != 0) {
		fprintf(stderr, "Connect error %s\n", uv_err_name(status));
	}
	stream = (uv_stream_t*)malloc(sizeof(uv_tcp_t));
	r = uv_tcp_init(loop, (uv_tcp_t*)stream);

	/* associate server with stream */
	stream->data = server;
	r = uv_accept(server, stream);
	r = uv_read_start(stream, echo_alloc, after_read);
}


int main2() {
	loop = uv_default_loop();

	struct sockaddr_in addr;
	int r;

	int port = 8080;
	uv_ip4_addr("0.0.0.0", port, &addr);
	server = (uv_handle_t*)&tcpServer;
	r = uv_tcp_init(loop, &tcpServer);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Socket creation error\n");
		return 1;
	}

	r = uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr, 0);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Bind error\n");
		return 1;
	}

	r = uv_listen((uv_stream_t*)&tcpServer, SOMAXCONN, on_connection);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Listen error %s\n", uv_err_name(r));
		return 1;
	}

	printf("start loop\n");
	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}