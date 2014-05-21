#include <memory>
#include <vector>

#include <microhttpd.h>

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

class HttpServer
{
public:
    typedef std::shared_ptr<HttpServer> Ptr;
    ~HttpServer();

    static Ptr create(std::function< std::string(std::string, std::vector<std::string>) > callback);
    bool start();

private:
    HttpServer(std::function< std::string(std::string, std::vector<std::string>) > callback);

    static int answer_to_connection (void *cls, MHD_Connection *connection,
                                        const char *url,
                                        const char *method, const char *version,
                                        const char *upload_data,
                                        size_t *upload_data_size, void **con_cls);

    MHD_Daemon *m_daemon;
    std::function<std::string(std::string, std::vector<std::string>)> m_response;
};

#endif // HTTPSERVER_H
