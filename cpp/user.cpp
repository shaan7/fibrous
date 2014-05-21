#include "user.h"

#include <jsoncpp/json/json.h>

#include <redis3m/connection.h>
#include <redis3m/simple_pool.h>

namespace {
    redis3m::simple_pool::ptr_t pool = redis3m::simple_pool::create("localhost", 6379);
}

User::User(std::string id, std::string name) : m_id(id), m_name(name)
{

}

User::Ptr User::load(std::string id)
{
    redis3m::connection::ptr_t conn;

    conn = pool->get();
    redis3m::reply r = conn->run(redis3m::command("GET")(id));
    pool->put(conn);

    return fromJson(r.str());
}

std::string User::toJson() const
{
    Json::Value root;
    root["id"] = m_id;
    root["name"] = m_name;

    Json::StyledWriter writer;
    return writer.write(root);
}

User::Ptr User::fromJson(std::string jsonString)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(jsonString, root);

    return Ptr(
        new User(root.get("id", 0).asString(), root.get("name", "").asString()));
}
