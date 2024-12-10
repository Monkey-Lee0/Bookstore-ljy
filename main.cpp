#include<bits/stdc++.h>
#include"database.h"
using namespace std;
B_plus_Tree<name,int> Tree;
int n;
string ins;
name key;
int value;
int main()
{
    ifstream f("tree");
    Tree.initialize("tree","data",!f.good());
    f.close();
    cin>>n;
    while(n--)
    {
        cin>>ins>>key;
        if(ins=="insert")
        {
            cin>>value;
            Tree.Insert(key,value);
        }
        else if(ins=="find")
        {
            auto ans=Tree.Find(key);
            if(ans.empty())
                cout<<"null"<<endl;
            else
            {
                for(const auto t:ans)
                    cout<<t<<" ";
                cout<<endl;
            }
        }
        else if(ins=="delete")
        {
            cin>>value;
            Tree.Delete(key,value);
        }
    }

    return 0;
}