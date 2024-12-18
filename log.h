#ifndef LOG_H
#define LOG_H

#include "database.h"
#include "user.h"

inline DataInteractor<2> Finance_log("finance_log");

inline void Show_finance(const std::string &Count)
{
    if(now_privilege()!=7)
        throw std::runtime_error("");
    const int mx=Finance_log.read_info(0);
    const int mx_siz=(mx-2*static_cast<int>(sizeof(int)))/static_cast<int>(sizeof(double));
    int x=-1;
    if(Count.empty())
        x=mx_siz;
    else
    {
        try
        {
            x=std::stoi(Count);
        }
        catch(...)
        {
            return std::cout<<"Invalid",void();
        }
    }

    if(x>mx_siz)
        throw std::runtime_error("");
    double plus=0,minus=0;
    for(int i=mx-x*static_cast<int>(sizeof(double));i<mx;i+=sizeof(double))
    {
        const auto tmp=Finance_log.read_T<double>(i);
        if(tmp>0)
            plus+=tmp;
        else
            minus-=tmp;
    }
    std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<plus<<" - "<<minus<<std::endl;
}

#endif
