#ifndef USER_H
#define USER_H

#include <string>
#include <memory>

class User
{
public:
    typedef std::shared_ptr<User> Ptr;

    static Ptr load(std::string id);

    std::string toJson() const;
    static Ptr fromJson(std::string jsonString);

private:
    User(std::string id, std::string name);

    std::string m_id;
    std::string m_name;
};

#endif // USER_H
