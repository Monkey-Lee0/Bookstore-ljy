#ifndef USER_H
#define USER_H

#include "database.h"
#include "tools.h"

class user
{
public:
    name30 Password{},Username{};
    int Privilege=0;
    user()=default;
    user(name30 password,name30 username,const int privilege):Privilege(privilege)
    {
        memcpy(Password,password,sizeof(password));
        memcpy(Username,username,sizeof(username));
    }
};

inline B_Plus_Tree<name30,int> User_tree("user_tree");

inline DataInteractor<2> User_info("user_info");

inline std::vector<std::tuple<int,user,int>> login_stack;

inline int now_privilege()
{
    if(login_stack.empty())
        return 0;
    return std::get<1>(login_stack.back()).Privilege;
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
        return std::cout<<"Invalid"<<std::endl,void();
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    const int index=res.back();
    if(const auto User=User_info.read_T<user>(index);
        strcmp(User.Password,my_c_str<31>(Password))==0||now_privilege()>User.Privilege)
        login_stack.emplace_back(index,User,-1);
    else
        std::cout<<"Invalid"<<std::endl;
}

inline void Logout()
{
    if(login_stack.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    login_stack.pop_back();
}

inline void Register(const std::string &UserID,const std::string &Password,const std::string &Username)
{
    if(!check_userid(UserID)||!check_userid(Password)||!check_username(Username))
        return std::cout<<"Invalid"<<std::endl,void();
    if(const auto res=User_tree.Find(my_c_str<31>(UserID)); res.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    user new_user(my_c_str<31>(Password),my_c_str<31>(Username),1);
    const int index=User_info.write_T<user>(new_user);
    User_tree.Insert(my_c_str<31>(UserID),index);
}

inline void Passwd(const std::string &UserID,const std::string &CurrentPassword,const std::string &NewPassword)
{
    if(!check_userid(UserID)||!check_userid(CurrentPassword)||!check_userid(NewPassword))
        return std::cout<<"Invalid"<<std::endl,void();
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    const int index=res.back();
    if(auto User=User_info.read_T<user>(index); now_privilege()==7||!strcmp(User.Password,my_c_str<31>(CurrentPassword)))
    {
        strcpy(User.Password,my_c_str<31>(NewPassword));
        User_info.update_T<user>(User,index);
    }
}

inline void Useradd(const std::string &UserID,const std::string &Password,
    const std::string &Privilege,const std::string &Username)
{
    if(!check_userid(UserID)||!check_userid(Password)||!check_privilege(Privilege)||!check_username(Username))
        return std::cout<<"Invalid"<<std::endl,void();
    if(const auto res=User_tree.Find(my_c_str<31>(UserID)); !res.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    if(now_privilege()<=Privilege[0]-'0')
        return std::cout<<"Invalid"<<std::endl,void();
    user new_user(my_c_str<31>(Password),my_c_str<31>(Username),Privilege[0]-'0');
    const int index=User_info.write_T<user>(new_user);
    User_tree.Insert(my_c_str<31>(UserID),index);
}

inline void Delete(const std::string &UserID)
{
    if(now_privilege()!=7||!check_userid(UserID))
        return std::cout<<"Invalid"<<std::endl,void();
    const auto res=User_tree.Find(my_c_str<31>(UserID));
    if(res.empty())
        return std::cout<<"Invalid"<<std::endl,void();
    User_tree.Delete(my_c_str<31>(UserID),res.back());
}

#endif