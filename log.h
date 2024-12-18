#ifndef LOG_H
#define LOG_H

#include "database.h"
#include "user.h"

inline DataInteractor<2> Finance_log("finance_log");

inline bool check_count(const std::string &str)
{
    if(str.empty())
        return true;
    if(str.size()>10)
        return false;
    for(const auto t:str)
        if(t<'0'||t>'9')
            return false;
    if(std::stol(str)>INT_MAX||std::stol(str)<0)
        return false;
    return true;
}

inline void Show_finance(const std::string &Count)
{
    if(now_privilege()!=7||!check_count(Count))
        throw std::runtime_error("");
    const int mx=Finance_log.read_info(0);
    const int mx_siz=(mx-2*static_cast<int>(sizeof(int)))/static_cast<int>(sizeof(double));
    int x=-1;
    if(!Count.empty())
    {
        x=std::stoi(Count);
        if(x>mx_siz)
            throw std::runtime_error("");
    }
    if(!x)
    {
        std::cout<<std::endl;
        return ;
    }
    if(x==-1)
        x=mx_siz;
    double plus=0,minus=0;
    for(int i=mx-x*static_cast<int>(sizeof(double));i<mx;i+=sizeof(double))
    {
        if(const auto tmp=Finance_log.read_T<double>(i); tmp>0)
            plus+=tmp;
        else
            minus-=tmp;
    }
    std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<plus<<" - "<<minus<<std::endl;
}

#endif
