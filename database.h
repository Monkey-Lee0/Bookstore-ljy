#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <utility>

template<class T,int info_len=4>
class MemoryRiver
{
private:
    std::fstream file;
    std::string file_name;
    int sizeofT=sizeof(T);
public:
    MemoryRiver()=default;
    explicit MemoryRiver(std::string file_name):file_name(std::move(file_name)){}
    void initialize(const std::string& FN="")
    {
        if(!FN.empty())
            file_name=FN;
        file.open(file_name,std::ios::out);
        int tmp=0;
        for(int i=0;i<info_len;++i)
            file.write(reinterpret_cast<char *>(&tmp),sizeof(int));
        file.close();
    }
    void get_info(int &tmp,const int n)
    {
        if(n>info_len)
            return;
        file.open(file_name);
        file.seekg((n-1)*static_cast<int>(sizeof(int)));
        file.read(reinterpret_cast<char*>(&tmp),sizeof(int));
        file.close();
    }
    int get_info(const int n){int tmp;get_info(tmp,n);return tmp;}
    void write_info(int tmp,const int n)
    {
        if (n>info_len)
            return;
        file.open(file_name);
        file.seekp((n-1)*static_cast<int>(sizeof(int)));
        file.write(reinterpret_cast<char*>(&tmp),sizeof(int));
        file.close();
    }
    int write(T &t)
    {
        int index,p;
        get_info(p,2);
        if(!p)
        {
            get_info(index,1);
            index++;
            write_info(index,1);
        }
        else
        {
            index=p;
            file.open(file_name);
            file.seekg(info_len*sizeof(int)+(index-1)*sizeofT);
            file.read(reinterpret_cast<char*>(&p),sizeof(int));
            file.close();
            write_info(p,2);
        }
        file.open(file_name);
        file.seekp(info_len*sizeof(int)+(index-1)*sizeofT);
        file.write(reinterpret_cast<char*>(&t),sizeofT);
        file.close();
        return index;
    }
    void update(T &t,const int index)
    {
        file.open(file_name);
        file.seekp(info_len*sizeof(int)+(index-1)*sizeofT);
        file.write(reinterpret_cast<char*>(&t),sizeofT);
        file.close();
    }
    void read(T &t,const int index)
    {
        file.open(file_name);
        file.seekg(info_len*sizeof(int)+(index-1)*sizeofT);
        file.read(reinterpret_cast<char*>(&t),sizeofT);
        file.close();
    }
    T read(const int index){T tmp;read(tmp,index);return tmp;}
    void Delete(const int index)
    {
        int p;
        get_info(p,2);
        file.open(file_name);
        file.seekp(info_len*sizeof(int)+(index-1)*sizeofT);
        file.write(reinterpret_cast<char*>(&p),sizeof(int));
        file.close();
        write_info(index,2);
    }
};

constexpr int MAXN=8;
using name=char[65];

template<class T>
class Node
{
public:
    T key[MAXN]{};
    int son[MAXN]{-1};
    int fa=-1;
    Node(){memset(son,-1,sizeof(son));}
};

template<class T>
class list
{
public:
    T val;
    int nxt=-2;
    list()=default;
    list(const T &a,const int &b):val(a),nxt(b){}
};

template<class T>
int COUNT_OF(const Node<T> &a,bool op=false)
{
    for(int i=0;i<MAXN;i++)
        if(a.son[i]==-1)
            return i;
    return MAXN;
}

template<class T,class T0>
class B_plus_Tree
{
    MemoryRiver<Node<T>> node_file;
    MemoryRiver<list<T0>> data_file;
    void split(const int &index,Node<T> left)
    {
        Node<T> right;
        for(int i=MAXN/2+1;i<MAXN;i++)
        {
            strcpy(right.key[i-MAXN/2],left.key[i]);
            right.son[i-MAXN/2]=left.son[i],left.son[i]=-1;
        }
        right.son[0]=left.son[MAXN/2];left.son[MAXN/2]=-1;
        right.fa=left.fa;
        node_file.update(left,index);
        const int index_r=node_file.write(right);
        if(left.fa==-1)
        {
            Node<T> new_root;
            strcpy(new_root.key[1],left.key[MAXN/2]);
            new_root.son[0]=index,new_root.son[1]=index_r;
            left.fa=right.fa=node_file.write(new_root);
            node_file.update(left,index);
            node_file.update(right,index_r);
            const int depth=node_file.get_info(4)+1;
            node_file.write_info(left.fa,3);
            node_file.write_info(depth,4);
        }
        else
        {
            Node<T> fa=node_file.read(left.fa);
            const int cnt=COUNT_OF(fa);
            int pos=0;
            for(int i=0;i<cnt;i++)
                if(fa.son[i]==index)
                {
                    pos=i+1;
                    break;
                }
            for(int i=cnt-1;i>=pos;--i)
                strcpy(fa.key[i+1],fa.key[i]),fa.son[i+1]=fa.son[i];
            strcpy(fa.key[pos],left.key[MAXN/2]),fa.son[pos]=index_r;
            if(cnt+1>=MAXN)
                split(left.fa,fa);
            else
                node_file.update(fa,left.fa);
        }
    }
public:
    void initialize(const std::string& s1,const std::string &s2,int mode=true)
    {
        if(mode)
            node_file.initialize(s1),data_file.initialize(s2);
        else
            node_file=MemoryRiver<Node<T>>(s1),data_file=MemoryRiver<list<T0>>(s2);
    }
    void Insert(const T key,const T0 val)
    {
        int now=node_file.get_info(3),depth=node_file.get_info(4);
        if(!depth)
        {
            Node<T> a;list<T0> b(val,-2);
            strcpy(a.key[0],key);
            a.son[0]=data_file.write(b);
            node_file.write_info(node_file.write(a),3);
            ++depth;
            node_file.write_info(depth,4);
            return ;
        }
        while(depth!=1)
        {
            --depth;
            const auto tmp=node_file.read(now);
            for(int i=1;i<=MAXN;i++)
            {
                if(i==MAXN||tmp.son[i]==-1)
                {
                    now=tmp.son[i-1];
                    break;
                }
                if(strcmp(tmp.key[i],key)>0)
                {
                    now=tmp.son[i-1];
                    break;
                }
            }
        }
        auto tmp=node_file.read(now);
        const auto cnt=COUNT_OF(tmp,true);
        int pos=cnt;
        for(int i=0;i<cnt;i++)
        {
            if(tmp.son[i]==-1)
                break;
            const int ok=strcmp(tmp.key[i],key);
            if(ok==0)
            {
                list<T0> x2(val,tmp.son[i]);
                tmp.son[i]=data_file.write(x2);
                node_file.update(tmp,now);
                return ;
            }
            if(ok>0)
            {
                pos=i;
                break;
            }
        }
        for(int i=cnt-1;i>=pos;--i)
            strcpy(tmp.key[i+1],tmp.key[i]),tmp.son[i+1]=tmp.son[i];
        list<T0> x(val,-2);
        strcpy(tmp.key[pos],key),tmp.son[pos]=data_file.write(x);
        if(cnt+1>=MAXN)
        {
            Node<T> right;
            for(int i=MAXN/2;i<MAXN;i++)
            {
                strcpy(right.key[i-MAXN/2],tmp.key[i]);
                right.son[i-MAXN/2]=tmp.son[i],tmp.son[i]=-1;
            }
            right.fa=tmp.fa;
            node_file.update(tmp,now);
            const int index_r=node_file.write(right);
            if(tmp.fa==-1)
            {
                Node<T> new_root;
                strcpy(new_root.key[1],right.key[0]);
                new_root.son[0]=now,new_root.son[1]=index_r;
                tmp.fa=right.fa=node_file.write(new_root);
                node_file.update(tmp,now);
                node_file.update(right,index_r);
                ++depth;
                node_file.write_info(tmp.fa,3);
                node_file.write_info(depth,4);
            }
            else
            {
                Node<T> fa=node_file.read(tmp.fa);
                const int cnt1=COUNT_OF(fa);
                int pos1=0;
                for(int i=0;i<cnt1;i++)
                    if(fa.son[i]==now)
                    {
                        pos1=i+1;
                        break;
                    }
                for(int i=cnt1-1;i>=pos1;--i)
                    strcpy(fa.key[i+1],fa.key[i]),fa.son[i+1]=fa.son[i];
                strcpy(fa.key[pos1],right.key[0]),fa.son[pos1]=index_r;
                if(cnt1+1>=MAXN)
                    split(tmp.fa,fa);
                else
                    node_file.update(fa,tmp.fa);
            }
        }
        else
            node_file.update(tmp,now);
    }
    std::vector<int> Find(const T key)
    {
        int now=node_file.get_info(3),depth=node_file.get_info(4);
        if(!depth)
            return {};
        while(depth!=1)
        {
            --depth;
            const auto tmp=node_file.read(now);
            for(int i=1;i<=MAXN;i++)
            {
                if(i==MAXN||tmp.son[i]==-1)
                {
                    now=tmp.son[i-1];
                    break;
                }
                if(strcmp(tmp.key[i],key)>0)
                {
                    now=tmp.son[i-1];
                    break;
                }
            }
        }
        auto tmp=node_file.read(now);
        const auto cnt=COUNT_OF(tmp);
        std::vector<T0> val;
        for(int i=0;i<cnt;i++)
        {
            const int ok=strcmp(tmp.key[i],key);
            if(ok==0)
            {
                int ptr=tmp.son[i];
                while(ptr>=0)
                {
                    list<T0> p;
                    data_file.read(p,ptr);
                    val.push_back(p.val);
                    ptr=p.nxt;
                }
                sort(val.begin(),val.end());
                return val;
            }
            if(ok>0)
                return{};
        }
        return {};
    }
    void Delete(const T key,const T0 val)
    {
        int now=node_file.get_info(3),depth=node_file.get_info(4);
        if(!depth)
            return ;
        while(depth!=1)
        {
            --depth;
            const auto tmp=node_file.read(now);
            for(int i=1;i<=MAXN;i++)
            {
                if(i==MAXN||tmp.son[i]==-1)
                {
                    now=tmp.son[i-1];
                    break;
                }
                if(strcmp(tmp.key[i],key)>0)
                {
                    now=tmp.son[i-1];
                    break;
                }
            }
        }
        auto tmp=node_file.read(now);
        const auto cnt=COUNT_OF(tmp);
        for(int i=0;i<cnt;i++)
        {
            const int ok=strcmp(tmp.key[i],key);
            if(ok==0)
            {
                int ptr=tmp.son[i],las_ptr=-1;
                list<T0> las,p;
                while(ptr>=0)
                {
                    las=p;
                    data_file.read(p,ptr);
                    if(p.val==val)
                    {
                        data_file.Delete(ptr);
                        if(las_ptr==-1)
                        {
                            tmp.son[i]=p.nxt;
                            node_file.update(tmp,now);
                        }
                        else
                        {
                            las.nxt=p.nxt;
                            data_file.update(las,las_ptr);
                        }
                        return ;
                    }
                    las_ptr=ptr;
                    ptr=p.nxt;
                }
                return ;
            }
            if(ok>0)
                return ;
        }
    }
};

#endif
