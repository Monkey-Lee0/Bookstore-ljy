#ifndef TOOLS_H
#define TOOLS_H

#include<string>

template<int len>
char* my_c_str(const std::string &a)
{
    static char s[len];
    for(int i=0;i<len;i++)
    {
        if(a.size()>i)
            s[i]=a[i];
        else
            s[i]='\0';
    }
    return s;
}

#endif
