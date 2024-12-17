#include<bits/stdc++.h>
#include"database.h"
using namespace std;
int n;
string ins;
name key;
int value;
double t,t1;
struct PP
{
    int a[100];
};
int main()
{
    ios::sync_with_stdio(false);
    // t=clock();
    // freopen("test.in","r",stdin);
    // freopen("test.out","w",stdout);
    ifstream f("tree");
    B_plus_Tree<name,int> Tree("tree",!f.good());
    f.close();
    cin>>n;
    while(n--)
    {
        // std::cerr<<n<<std::endl;
        cin>>ins>>key;
        if(ins=="insert")
        {
            cin>>value;
            Tree.Insert(key,value);
        }
        else if(ins=="find")
        {
            t1-=clock();
            auto ans=Tree.Find(key);
            if(ans.empty())
                cout<<"null"<<endl;
            else
            {
                for(const auto t:ans)
                    cout<<t<<" ";
                cout<<endl;
            }
            t1+=clock();
        }
        else if(ins=="delete")
        {
            cin>>value;
            Tree.Delete(key,value);
        }
    }
    // t=clock()-t;
    // std::cerr<<t1/CLOCKS_PER_SEC<<" "<<t/CLOCKS_PER_SEC<<endl;

    return 0;
}