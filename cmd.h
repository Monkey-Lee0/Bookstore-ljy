#ifndef CMD_H
#define CMD_H

#include "user.h"
#include "book.h"

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
    try
    {
        const auto res=decompose(str);
        if(res.empty())
            return ;
        if(res[0]=="quit"||res[0]=="exit")
        {
            if(res.size()!=1)
                throw std::runtime_error("");
            exit(0);
        }
        if(res.size()>=2&&res[0]=="show"&&res[1]=="finance")
        {
            if(res.size()==2)
                Show_finance("");
            else if(res.size()==3)
                Show_finance(res[2]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="su")
        {
            if(res.size()==2)
                Su(res[1],"");
            else if(res.size()==3)
                Su(res[1],res[2]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="logout")
        {
            if(res.size()==1)
                Logout();
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="register")
        {
            if(res.size()==4)
                Register(res[1],res[2],res[3]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="passwd")
        {
            if(res.size()==4)
                Passwd(res[1],res[2],res[3]);
            else if(res.size()==3)
                Passwd(res[1],"",res[2]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="useradd")
        {
            if(res.size()==5)
                Useradd(res[1],res[2],res[3],res[4]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="delete")
        {
            if(res.size()==2)
                Delete(res[1]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="show")
        {
            if(res.size()==1)
                Show("","");
            else if(res.size()==2)
            {
                if(res[1].front()!='-')
                    throw std::runtime_error("");
                int pos=-1;
                for(int i=0;i<res[1].size();i++)
                    if(res[1][i]=='=')
                    {
                        pos=i;
                        break;
                    }
                if(pos==-1||pos==1)
                    throw std::runtime_error("");
                Show(res[1].substr(1,pos-1),res[1].substr(pos+1));
            }
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="buy")
        {
            if(res.size()==3)
                Buy(res[1],res[2]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="select")
        {
            if(res.size()==2)
                Select(res[1]);
            else
                throw std::runtime_error("");
        }
        else if(res[0]=="modify")
        {
            if(res.size()==1||res.size()>6)
                throw std::runtime_error("");
            int pos[7],chk=1;
            std::vector<std::string> S;
            for(int i=1;i<res.size();i++)
            {
                if(res[i].front()!='-')
                    chk=0;
                else
                {
                    pos[i]=-1;
                    for(int j=0;j<res[1].size();j++)
                        if(res[i][j]=='=')
                        {
                            pos[i]=j;
                            break;
                        }
                    if(pos[i]==-1)
                        chk=0;
                    S.push_back(res[i].substr(1,pos[i]-1));
                    if(S.back()!="ISBN"&&S.back()!="name"&&S.back()!="author"&&S.back()!="keyword"&&S.back()!="price")
                        chk=0;
                }
            }
            std::ranges::sort(S);
            for(int i=1;i<S.size();i++)
                if(S[i]==S[i-1])
                    chk=0;
            if(!chk)
                throw std::runtime_error("");
            for(int i=1;i<res.size();i++)
                Modify_check_only(res[i].substr(1,pos[i]-1),res[i].substr(pos[i]+1));
            for(int i=1;i<res.size();i++)
                Modify(res[i].substr(1,pos[i]-1),res[i].substr(pos[i]+1));
        }
        else if(res[0]=="import")
        {
            if(res.size()!=3)
                throw std::runtime_error("");
            Import(res[1],res[2]);
        }
        else
            throw std::runtime_error("");
    }
    catch(...)
    {
        std::cout<<"Invalid"<<std::endl;
    }
}

#endif