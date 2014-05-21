#include "httpserver.h"
#include "user.h"

#include <iostream>

std::string response(std::string method, std::vector<std::string> id)
{
    if (method == "GET") {
        return User::load(id.back())->toJson();
    }
}

int main ()
{
    HttpServer::Ptr server = HttpServer::create(response);
    server->start();

    //haha, this is cute :P
    int a;
    std::cin >> a;
}

