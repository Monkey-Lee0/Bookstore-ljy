#ifndef USER_H
#define USER_H

#include <cassert>

#include "database.h"
#include "tools.h"

class user
{
public:
    name30 Password{},Username{};
    int Privilege=0;
    user()=default;
    user(const std::string& password,const std::string& username,const int privilege):Privilege(privilege)
    {
        strcpy(Password,password.c_str());
        strcpy(Username,username.c_str());
    }
};

inline B_Plus_Tree<name30,int> User_tree("user_tree");

inline DataInteractor<2> User_info("user_info");

inline std::vector<std::pair<user,int>> login_stack;

inline std::vector<int> index_stack;
inline std::unordered_map<int,int> in_stack;

inline int now_privilege()
{
    if(login_stack.empty())
        return 0;
    return login_stack.back().first.Privilege;
}

inline bool check_userid(const std::string &str)
{
    if(str.size()>30)
        return false;
    for(const auto t:str)
        if((t>'9'||t<'0')&&(t>'z'||t<'a')&&(t>'Z'||t<'A')&&t!='_')
            return false;
    return true;
}

inline bool check_username(const std::string &str)
{
    if(str.size()>30)
        return false;
    for(const auto t:str)
        if(t<32||t>126)
            return false;
    return true;
}

inline bool check_privilege(const std::string &str)
{
    if(str.size()!=1)
        return false;
    if(str[0]<'0'||str[0]>'7')
        return false;
    if(str[0]!='1'&&str[0]!='3'&&str[0]!=7)
        return false;
    return true;
}

inline void Su(const std::string& UserID,const std::string &Password)
{
    if(!check_userid(UserID)||!check_userid(Password))
        throw std::runtime_error("");
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        throw std::runtime_error("");
    const int index=res.back();
    if(const auto User=User_info.read_T<user>(index);
        strcmp(User.Password,my_c_str<31>(Password))==0||now_privilege()>User.Privilege)
        login_stack.emplace_back(User,-1),in_stack[index]++,index_stack.push_back(index);
    else
        throw std::runtime_error("");
}

inline void Logout()
{
    if(login_stack.empty())
        throw std::runtime_error("");
    in_stack[index_stack.back()]--;
    index_stack.pop_back();
    login_stack.pop_back();
}

inline void Register(const std::string &UserID,const std::string &Password,const std::string &Username)
{
    if(!check_userid(UserID)||!check_userid(Password)||!check_username(Username))
        throw std::runtime_error("");
    if(const auto res=User_tree.Find(my_c_str<31>(UserID)); !res.empty())
        throw std::runtime_error("");
    user new_user(Password,Username,1);
    const int index=User_info.write_T<user>(new_user);
    User_tree.Insert(my_c_str<31>(UserID),index);
}

inline void Passwd(const std::string &UserID,const std::string &CurrentPassword,const std::string &NewPassword)
{
    if(!check_userid(UserID)||!check_userid(CurrentPassword)||!check_userid(NewPassword))
        throw std::runtime_error("");
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        throw std::runtime_error("");
    const int index=res.back();
    if(auto User=User_info.read_T<user>(index); now_privilege()==7||!strcmp(User.Password,my_c_str<31>(CurrentPassword)))
    {
        strcpy(User.Password,my_c_str<31>(NewPassword));
        User_info.update_T<user>(User,index);
    }
    else
        throw std::runtime_error("");
}

inline void Useradd(const std::string &UserID,const std::string &Password,
    const std::string &Privilege,const std::string &Username)
{
    if(!check_userid(UserID)||!check_userid(Password)||!check_privilege(Privilege)||!check_username(Username))
        throw std::runtime_error("");
    if(const auto res=User_tree.Find(my_c_str<31>(UserID)); !res.empty())
        throw std::runtime_error("");
    if(now_privilege()<=Privilege[0]-'0')
        throw std::runtime_error("");
    user new_user(Password,Username,Privilege[0]-'0');
    const int index=User_info.write_T<user>(new_user);
    User_tree.Insert(my_c_str<31>(UserID),index);
}

inline void Delete(const std::string &UserID)
{
    if(now_privilege()!=7||!check_userid(UserID))
        throw std::runtime_error("");
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        throw std::runtime_error("");
    if(in_stack[res.back()])
        throw std::runtime_error("");
    User_tree.Delete(my_c_str<31>(UserID),res.back());
}

#endif