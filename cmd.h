#ifndef CMD_H
#define CMD_H

#include"user.h"

inline std::vector<std::string> decompose(const std::string &str)
{
    std::vector<std::string> S;
    std::string tmp;
    for(const char t:str)
        if(t==' ')
        {
            if(!tmp.empty())
                S.push_back(tmp);
            tmp.clear();
        }
        else
            tmp+=t;
    if(!tmp.empty())
        S.push_back(tmp);
    return S;
}

inline void cmd(const std::string &str)
{
    const auto res=decompose(str);
    if(res.empty())
        return ;
    if(res[0]=="quit"||res[0]=="exit")
    {
        if(res.size()!=1)
            std::cout<<"Invalid"<<std::endl;
        exit(0);
    }
    if(res[0]=="su")
    {
        if(res.size()==2)
            Su(res[1],"");
        else if(res.size()==3)
            Su(res[1],res[2]);
        else
            std::cout<<"Invalid"<<std::endl;
    }
    else if(res[0]=="logout")
    {
        if(res.size()==1)
            Logout();
        else
            std::cout<<"Invalid"<<std::endl;
    }
    else if(res[0]=="register")
    {
        if(res.size()==4)
            Register(res[1],res[2],res[3]);
        else
            std::cout<<"Invalid"<<std::endl;
    }
    else if(res[0]=="passwd")
    {
        if(res.size()==4)
            Passwd(res[1],res[2],res[3]);
        else if(res.size()==3)
            Passwd(res[1],"",res[2]);
        else
            std::cout<<"Invalid"<<std::endl;
    }
    else if(res[0]=="useradd")
    {
        if(res.size()==5)
            Useradd(res[1],res[2],res[3],res[4]);
        else
            std::cout<<"Invalid"<<std::endl;
    }
    else if(res[0]=="delete")
    {
        if(res.size()==2)
            Delete(res[1]);
        else
            std::cout<<"Invalid"<<std::endl;
    }
}

#endif