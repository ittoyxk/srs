/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2020 Winlin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SRS_PROTOCOL_UTILITY_HPP
#define SRS_PROTOCOL_UTILITY_HPP

#include <srs_core.hpp>

// for srs-librtmp, @see https://github.com/ossrs/srs/issues/213
#ifndef _WIN32
#include <sys/uio.h>
#endif

#include <string>
#include <vector>
#include <map>

#include <srs_kernel_consts.hpp>

class SrsMessageHeader;
class SrsSharedPtrMessage;
class SrsCommonMessage;
class ISrsProtocolReadWriter;

/**
 * parse the tcUrl, output the schema, host, vhost, app and port.
 * @param tcUrl, the input tcUrl, for example,
 *       rtmp://192.168.1.10:19350/live?vhost=vhost.ossrs.net
 * @param schema, for example, rtmp
 * @param host, for example, 192.168.1.10
 * @param vhost, for example, vhost.ossrs.net.
 *       vhost default to host, when user not set vhost in query of app.
 * @param app, for example, live
 * @param port, for example, 19350
 *       default to 1935 if not specified.
 * param param, for example, vhost=vhost.ossrs.net
 * @remark The param stream is input and output param, that is:
 *       input: tcUrl+stream
 *       output: schema, host, vhost, app, stream, port, param
 */
extern void srs_discovery_tc_url(std::string tcUrl, std::string& schema, std::string& host, std::string& vhost, std::string& app,
    std::string& stream, int& port, std::string& param);

// parse query string to map(k,v).
// must format as key=value&...&keyN=valueN
extern void srs_parse_query_string(std::string q, std::map<std::string, std::string>& query);

/**
 * generate ramdom data for handshake.
 */
extern void srs_random_generate(char* bytes, int size);
// Generate random value, use srandom(now_us) to init seed if not initialized.
extern long srs_random();

/**
 * generate the tcUrl without param.
 * @remark Use host as tcUrl.vhost if vhost is default vhost.
 */
extern std::string srs_generate_tc_url(std::string host, std::string vhost, std::string app, int port);

/**
 * Generate the stream with param.
 * @remark Append vhost in query string if not default vhost.
 */
extern std::string srs_generate_stream_with_query(std::string host, std::string vhost, std::string stream, std::string param, bool with_vhost = true);

/**
 * create shared ptr message from bytes.
 * @param data the packet bytes. user should never free it.
 * @param ppmsg output the shared ptr message. user should free it.
 */
extern srs_error_t srs_rtmp_create_msg(char type, uint32_t timestamp, char* data, int size, int stream_id, SrsSharedPtrMessage** ppmsg);
extern srs_error_t srs_rtmp_create_msg(char type, uint32_t timestamp, char* data, int size, int stream_id, SrsCommonMessage** ppmsg);

// get the stream identify, vhost/app/stream.
extern std::string srs_generate_stream_url(std::string vhost, std::string app, std::string stream);

// parse the rtmp url to tcUrl/stream,
// for example, rtmp://v.ossrs.net/live/livestream to
//      tcUrl: rtmp://v.ossrs.net/live
//      stream: livestream
extern void srs_parse_rtmp_url(std::string url, std::string& tcUrl, std::string& stream);

// Genereate the rtmp url, for instance, rtmp://server:port/app/stream?param
// @remark We always put vhost in param, in the query of url.
extern std::string srs_generate_rtmp_url(std::string server, int port, std::string host, std::string vhost, std::string app, std::string stream, std::string param);

// write large numbers of iovs.
extern srs_error_t srs_write_large_iovs(ISrsProtocolReadWriter* skt, iovec* iovs, int size, ssize_t* pnwrite = NULL);

// join string in vector with indicated separator
extern std::string srs_join_vector_string(std::vector<std::string>& vs, std::string separator);

// Whether domain is an IPv4 address.
extern bool srs_is_ipv4(std::string domain);

#endif

