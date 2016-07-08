/* Copyright 2010 Sophos Limited. All rights reserved. Sophos is a registered
 * trademark of Sophos Limited.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __SXE_HTTPD_H__
#define __SXE_HTTPD_H__

#include "sxe.h"
#include "sxe-http.h"
#include "sxe-list.h"

#define HTTPD_CONTENT_LENGTH          "Content-Length"
#define HTTPD_CONNECTION_CLOSE_HEADER "Connection"
#define HTTPD_CONNECTION_CLOSE_VALUE  "close"

/* NOTE: Order of states is important! */
typedef enum {
    SXE_HTTPD_CONN_FREE = 0,           /* not connected */
    SXE_HTTPD_CONN_BAD,                /* invalid http, sink mode until close */
    SXE_HTTPD_CONN_IDLE,               /* client connected */
    SXE_HTTPD_CONN_REQ_LINE,           /* VERB <SP> URL [<SP> VERSION] */
    SXE_HTTPD_CONN_REQ_HEADERS,        /* reading headers */
    SXE_HTTPD_CONN_REQ_BODY,           /* at BODY we run the handler for each chunk. */
    SXE_HTTPD_CONN_REQ_RESPONSE,       /* generating the response (read disabled) */
    SXE_HTTPD_CONN_NUMBER_OF_STATES
} SXE_HTTPD_CONN_STATE;

typedef enum {
    SXE_HTTP_METHOD_INVALID=0,
    SXE_HTTP_METHOD_GET,
    SXE_HTTP_METHOD_HEAD,
    SXE_HTTP_METHOD_PUT,
    SXE_HTTP_METHOD_POST,
    SXE_HTTP_METHOD_DELETE
} SXE_HTTP_METHOD;

typedef enum {
    SXE_HTTP_VERSION_UNKNOWN=0,
    SXE_HTTP_VERSION_1_0,
    SXE_HTTP_VERSION_1_1
} SXE_HTTP_VERSION;

#define WANT_BODY(request) (request->method == SXE_HTTP_METHOD_PUT || request->method == SXE_HTTP_METHOD_POST)

struct SXE_HTTPD;
struct SXE_HTTPD_REQUEST;

typedef void       (*sxe_httpd_connect_handler)(struct SXE_HTTPD_REQUEST *);
typedef SXE_RETURN (*sxe_httpd_request_handler)(struct SXE_HTTPD_REQUEST *, const char *method, unsigned mlen,
                                                const char *url, unsigned ulen, const char *version, unsigned vlen);
typedef void       (*sxe_httpd_header_handler)(struct SXE_HTTPD_REQUEST *, const char *key, unsigned klen,
                                             const char *val, unsigned vlen);
typedef void       (*sxe_httpd_eoh_handler)(struct SXE_HTTPD_REQUEST *);
typedef void       (*sxe_httpd_body_handler)(struct SXE_HTTPD_REQUEST *, const char *chunk, unsigned chunklen);
typedef void       (*sxe_httpd_respond_handler)(struct SXE_HTTPD_REQUEST *);
typedef void       (*sxe_httpd_close_handler)(struct SXE_HTTPD_REQUEST *);

/* Invoked from sxe_http_header_parse(buffer, length, callback, user_data) for
 * each HTTP header found in buffer.
 *
 * NOTE: to stop the callback from being invoked again, return false. The
 * entire header block will still be checked, and MUST contain everything from
 * the first header to the last byte before the terminating \r\n\r\n.
 *
 * NOTE: the callback is invoked as valid headers are found. Later, the header
 * may be found to be invalid -- it is the caller's responsibility to undo any
 * damage if sxe_http_header_parse() returns false.
 */
typedef bool (*sxe_http_header_handler)(const char *key, unsigned key_length,
                                        const char *val, unsigned val_length,
                                        void *user_data);

/* This handler is invoked when sxe_httpd_response_sendfile() has finished
 * sending the requested amount of data.
 */
typedef void (*sxe_httpd_sendfile_handler)(struct SXE_HTTPD_REQUEST *, SXE_RETURN, void *);

#define SXE_HTTPD_REQUEST_USER_DATA(request)                         (request)->user_data
#define SXE_HTTPD_REQUEST_USER_DATA_AS_UNSIGNED(request) ((unsigned)((request)->user_data))
#define SXE_HTTPD_REQUEST_SERVER_USER_DATA(request)                  (request)->server->user_data

/* Pointer substraction is signed but index is unsigned */
#define SXE_HTTPD_REQUEST_INDEX(base_ptr, object_ptr) (((uintptr_t)(object_ptr) - (uintptr_t)(base_ptr))/sizeof(SXE_HTTPD_REQUEST))

typedef struct SXE_HTTPD_REQUEST {
    struct SXE_HTTPD         * server;
    SXE                      * sxe;                  /* NOT FOR APPLICATION USE! */
    void                     * user_data;            /* not used by sxe_httpd -- to be used by applications */

    /* Parsed info from the request line and headers */
    SXE_HTTP_METHOD            method;
    SXE_HTTP_VERSION           version;
    SXE_HTTP_MESSAGE           message;
    unsigned                   in_content_length;    /* value from Content-Length header */
    unsigned                   in_content_seen;      /* number of body bytes read */

    /* Output: have we finished headers? */
    bool                       out_eoh;

    /* Handle sendfile */
    sxe_httpd_sendfile_handler sendfile_handler;
    void *                     sendfile_userdata;
} SXE_HTTPD_REQUEST;

static inline SXE * sxe_httpd_request_get_sxe(SXE_HTTPD_REQUEST * request) { return request->sxe; };    /* For diagnostics */

typedef struct SXE_HTTPD {
    SXE_HTTPD_REQUEST       * requests;
    void                    * user_data;
    sxe_httpd_connect_handler on_connect;
    sxe_httpd_request_handler on_request;
    sxe_httpd_header_handler  on_header;
    sxe_httpd_eoh_handler     on_eoh;
    sxe_httpd_body_handler    on_body;
    sxe_httpd_respond_handler on_respond;
    sxe_httpd_close_handler   on_close;
} SXE_HTTPD;

#define SXE_HTTPD_SET_HANDLER(httpd, handler, function) sxe_httpd_set_ ## handler ## _handler((httpd), function)

#include "sxe-httpd-proto.h"

#endif

/* vim: set ft=c sw=4 sts=4 ts=8 listchars=tab\:^.,trail\:@ expandtab list: */
