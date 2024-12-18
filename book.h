#ifndef BOOK_H
#define BOOK_H

#include <iomanip>

#include "database.h"
#include "log.h"
#include "user.h"

class book
{
public:
    name20 ISBN{};
    name60 BookName{},Author{},Keyword{};
    double Price=0;
    int Storage=0;
    book(const std::string& isbn,const std::string& bookname,const std::string& author,const std::string& keyword
        ,const double price,const int storage):Price(price),Storage(storage)
    {
        strcpy(ISBN,isbn.c_str());
        strcpy(BookName,bookname.c_str());
        strcpy(Author,author.c_str());
        strcpy(Keyword,keyword.c_str());
    }
};

inline B_Plus_Tree<name20,int> ISBN_tree("ISBN_tree");
inline B_Plus_Tree<name60,int> Bookname_tree("bookname_tree");
inline B_Plus_Tree<name60,int> Author_tree("author_tree");
inline B_Plus_Tree<name60,int> Keyword_tree("keyword_tree");
inline B_Plus_Tree<name10,int> Allbook_tree("allbook_tree");
inline DataInteractor<2> Book_info("book_info");

inline std::vector<std::string> split_keyword(const std::string &str)
{
    std::vector<std::string> S;
    std::string las;
    for(const auto t:str)
    {
        if(t=='|')
            S.push_back(las),las.clear();
        else
            las+=t;
    }
    S.push_back(las);
    return S;
}

inline bool check_isbn(const std::string &str)
{
    if(str.size()>20)
        return false;
    for(const auto t:str)
        if(t<32||t>126)
            return false;
    return true;
}

inline bool check_bookname(const std::string &str)
{
    if(str.size()>60)
        return false;
    for(const auto t:str)
        if(t<32||t>126||t=='\"')
            return false;
    return true;
}

inline bool check_quantity(const std::string &str)
{
    if(str.size()>10)
        return false;
    for(const auto t:str)
        if(t<'0'||t>'9')
            return false;
    if(std::stol(str)>INT_MAX||std::stol(str)<=0)
        return false;
    return true;
}

inline bool check_price(const std::string &str)
{
    if(str.size()>13)
        return false;
    for(int i=0;i<str.size();i++)
        if(str[i]!='.'&&(str[i]<'0'||str[i]>'9'))
            return false;
    try
    {
        auto x=std::stod(str);
        if(x<0)
            return false;
    }
    catch(...)
    {
        return true;
    }
    return true;
}

inline bool check_keyword(const std::string &str)
{
    if(str.size()>60)
        return false;
    for(const auto t:str)
        if(t<32||t>126||t=='\"')
            return false;
    if(str.empty())
        return false;
    std::vector<std::string> S;
    std::string las;
    for(const auto t:str)
    {
        if(t=='|')
        {
            if(las.empty())
                return false;
            S.push_back(las),las.clear();
        }
        else
            las+=t;
    }
    if(las.empty())
        return false;
    S.push_back(las);
    std::ranges::sort(S);
    for(int i=1;i<S.size();i++)
        if(S[i-1]==S[i])
            return false;
    return true;
}

inline void Show(const std::string &type,const std::string &info)
{
    if(now_privilege()<1)
        throw std::runtime_error("");
    std::vector<int> res;
    if(type=="ISBN")
    {
        if(!check_isbn(info))
            throw std::runtime_error("");
        res=ISBN_tree.Find(my_c_str<21>(info));
    }
    else if(type=="name")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_bookname(info.substr(1,info.size()-2)))
            throw std::runtime_error("");
        res=Bookname_tree.Find(my_c_str<61>(info.substr(1,info.size()-2)));
    }
    else if(type=="author")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_bookname(info.substr(1,info.size()-2)))
            throw std::runtime_error("");
        res=Author_tree.Find(my_c_str<61>(info.substr(1,info.size()-2)));
    }
    else if(type=="keyword")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_keyword(info.substr(1,info.size()-2))||
            split_keyword(info.substr(1,info.size()-2)).size()!=1)
            throw std::runtime_error("");
        res=Keyword_tree.Find(my_c_str<61>(info.substr(1,info.size()-2)));
    }
    else if(type.empty())
        res=Allbook_tree.Find("all");
    else
        throw std::runtime_error("");
    std::vector<book> RES;
    for(const auto &t:res)
        RES.push_back(Book_info.read_T<book>(t));
    std::ranges::sort(RES,[&](const book& a,const book& b){return strcmp(a.ISBN,b.ISBN)<0;});
    for(const auto &t:RES)
    {
        std::cout<<t.ISBN<<"\t"<<t.BookName<<"\t"<<t.Author<<"\t"<<t.Keyword<<"\t"<<
            std::fixed<<std::setprecision(2)<<t.Price<<"\t"<<t.Storage<<std::endl;
    }
    if(res.empty())
        std::cout<<std::endl;
}

inline void Buy(const std::string &ISBN,const std::string &Quantity)
{
    if(!check_isbn(ISBN)||!check_quantity(Quantity)||now_privilege()<1)
        throw std::runtime_error("");
    const auto res=ISBN_tree.Find(my_c_str<21>(ISBN));
    if(res.empty())
        throw std::runtime_error("");
    auto now_book=Book_info.read_T<book>(res.back());
    const int cnt=std::stoi(Quantity);
    if(cnt>now_book.Storage)
        throw std::runtime_error("");
    double ans=now_book.Price*cnt;
    std::cout<<std::fixed<<std::setprecision(2)<<ans<<std::endl;
    Finance_log.write_T<double>(ans);
    now_book.Storage-=cnt;
    Book_info.update_T<book>(now_book,res.back());
}

inline void Select(const std::string &ISBN)
{
    if(!check_isbn(ISBN)||now_privilege()<3)
        throw std::runtime_error("");
    if(const auto res=ISBN_tree.Find(my_c_str<21>(ISBN)); res.empty())
    {
        book new_book(ISBN,"","","",0,0);
        const int index=Book_info.write_T<book>(new_book);
        ISBN_tree.Insert(my_c_str<21>(ISBN),index);
        login_stack.back().second=index;
        ISBN_tree.Insert(my_c_str<21>(ISBN),index);
        Allbook_tree.Insert("all",index);
    }
    else
        login_stack.back().second=res.back();
}

inline void Modify(const std::string &type,const std::string &info)
{
    if(now_privilege()<3||login_stack.back().second==-1)
            throw std::runtime_error("");
    const auto index=login_stack.back().second;
    auto now_book=Book_info.read_T<book>(index);
    if(type=="ISBN")
    {
        if(!check_isbn(info)||!ISBN_tree.Find(my_c_str<21>(info)).empty())
            throw std::runtime_error("");
        ISBN_tree.Delete(now_book.ISBN,index);
        memcpy(now_book.ISBN,my_c_str<21>(info),sizeof(now_book.ISBN));
        ISBN_tree.Insert(my_c_str<21>(info),index);
    }
    else if(type=="name")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_bookname(info.substr(1,info.size()-2)))
            throw std::runtime_error("");
        Bookname_tree.Delete(now_book.BookName,index);
        memcpy(now_book.BookName,my_c_str<61>(info.substr(1,info.size()-2)),sizeof(now_book.BookName));
        Bookname_tree.Insert(my_c_str<61>(info.substr(1,info.size()-2)),index);
    }
    else if(type=="author")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_bookname(info.substr(1,info.size()-2)))
            throw std::runtime_error("");
        Author_tree.Delete(now_book.Author,index);
        memcpy(now_book.Author,my_c_str<61>(info.substr(1,info.size()-2)),sizeof(now_book.Author));
        Author_tree.Insert(my_c_str<61>(info.substr(1,info.size()-2)),index);
    }
    else if(type=="keyword")
    {
        if(info.size()<2||info.front()!='\"'||info.back()!='\"')
            throw std::runtime_error("");
        if(!check_keyword(info.substr(1,info.size()-2)))
            throw std::runtime_error("");
        auto tmp=split_keyword(std::string(now_book.Keyword));
        for(const auto& t:tmp)
            Keyword_tree.Delete(my_c_str<61>(t),index);
        memcpy(now_book.Keyword,my_c_str<61>(info.substr(1,info.size()-2)),sizeof(now_book.Keyword));
        tmp=split_keyword(info.substr(1,info.size()-2));
        for(const auto& t:tmp)
            Keyword_tree.Insert(my_c_str<61>(t),index);
    }
    else if(type=="price")
    {
        if(!check_price(info))
            throw std::runtime_error("");
        now_book.Price=std::stod(info);
    }
    else
        throw std::runtime_error("");
    Book_info.update_T(now_book,index);
}

inline void Import(const std::string &Quantity,const std::string &TotalCost)
{
    if(!check_quantity(Quantity)||!check_price(TotalCost)||now_privilege()<3||login_stack.back().second==-1)
        throw std::runtime_error("");
    const auto index=login_stack.back().second;
    auto now_book=Book_info.read_T<book>(index);
    now_book.Storage+=std::stoi(Quantity);
    Book_info.update_T(now_book,index);
    double tmp=-std::stod(TotalCost);
    Finance_log.write_T<double>(tmp);
}

#endif