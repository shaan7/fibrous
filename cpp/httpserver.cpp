#include "httpserver.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vector>
#include <iostream>

#include <cstring>

namespace {
    static const uint16_t PORT = 8888;
    static const int MAXANSWERSIZE = 512;
    static const int MAXNAMESIZE = 20;
}

using namespace std;

HttpServer::HttpServer(function< string(string, vector<string>) > callback)
{
    m_response = callback;
}

bool HttpServer::start()
{
    m_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                             &HttpServer::answer_to_connection, this, MHD_OPTION_END);

    return m_daemon != NULL;
}

HttpServer::~HttpServer()
{
    MHD_stop_daemon (m_daemon);
}

HttpServer::Ptr HttpServer::create(function< string(string, vector<string>) > callback)
{
    return HttpServer::Ptr(new HttpServer(callback));
}

int HttpServer::answer_to_connection(void* cls, MHD_Connection* connection,
                                     const char* url, const char* method,
                                     const char* version,
                                     const char* upload_data,
                                     size_t* upload_data_size, void** con_cls)
{
    vector<string> parts;
    boost::split(parts, url, boost::is_any_of("/"));

    HttpServer *This = static_cast<HttpServer*>(cls);

    const char *page = This->m_response(string(method), parts).c_str();

    MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer (strlen (page),
                                            (void*) page, MHD_RESPMEM_MUST_COPY);

    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}
