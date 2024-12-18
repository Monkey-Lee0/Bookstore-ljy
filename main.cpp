#include"cmd.h"
#include<bits/stdc++.h>
using namespace std;

int main()
{
    // freopen("my.ans","w",stdout);
    if(User_info.read_info(0)==2*sizeof(int))
    {
        user new_user("sjtu","root",7);
        const int index=User_info.write_T<user>(new_user);
        User_tree.Insert(my_c_str<31>("root"),index);
    }
    string str;
    while(getline(cin,str))
        cmd(str);

    return 0;
}