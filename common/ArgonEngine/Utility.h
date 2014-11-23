/**
 * @brief Provides some Utility classes for the Argon Engine such as OrderedVector, FixedVector, ObjectTracer/ObjectTracker, and StringIntern.
 * @file Utility.h
 * @author Skyler Saleh
 **/
#ifndef Utility_h
#define Utility_h
#include "Thread.h"
#include <map>
#include <string>
#include "TypeInfo.h"
#include <cmath>
#include <iostream>
//#define USE_STACKSTRING 1
#define USE_STRINGINTERN 1
namespace Argon {
    static inline bool is_whitespace(int c){return c=='\n'||c==' '||c=='\r'||c=='\t';}

    template <typename T>
    static double string_to_number(T begin, T end){
        double whole =0;
        double part=1.0;
        double exponent = 0.0;
        double number_sign=1.0;
        double exponent_sign=1.0;

        int state=1;
        T i = begin;
        while (i!=end&&state!=10000) {
            char l = *i;

            switch(state){
                    //Whitespace
                case 0:
                    if(l!=' '||l!='\n'||l!='\t'||l!='\r'){state=1;--i;}
                    break;
                    //Number Type
                case 1:
                    switch (l){
                        case '-':number_sign*=-1.;break;
                        case '+':number_sign*=1.;break;
                        case '0':state=2;break;
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':state=7; continue;
                        case 'o':case 'O':state = 11;break;
                        case 'Y':case 'y':state = 12;break;
                        case 'T':case 't':state = 14;break;
                    }
                    if(l=='0') state=2;
                    break;
                    //Octal vs. Hex
                case 2:
                    if(l=='x') state =3;
                    else {state=5;--i;}
                    break;
                    //Hex Whole
                case 3:
                    whole*=16;
                    switch(l){
                        case '0':whole+=0;break;
                        case '1':whole+=1;break;
                        case '2':whole+=2;break;
                        case '3':whole+=3;break;
                        case '4':whole+=4;break;
                        case '5':whole+=5;break;
                        case '6':whole+=6;break;
                        case '7':whole+=7;break;
                        case '8':whole+=8;break;
                        case '9':whole+=9;break;
                        case 'a':case 'A':whole+=10;break;
                        case 'b':case 'B':whole+=11;break;
                        case 'c':case 'C':whole+=12;break;
                        case 'd':case 'D':whole+=13;break;
                        case 'e':case 'E':whole+=14;break;
                        case 'f':case 'F':whole+=15;break;
                        case '.':state = 4;whole/=16.0;break;
                        default:state=10000;break;
                    }
                    break;
                    //Hex Fraction
                case 4:
                    part/=16.;

                    switch(l){
                        case '0':whole+=0*part;break;
                        case '1':whole+=1*part;break;
                        case '2':whole+=2*part;break;
                        case '3':whole+=3*part;break;
                        case '4':whole+=4*part;break;
                        case '5':whole+=5*part;break;
                        case '6':whole+=6*part;break;
                        case '7':whole+=7*part;break;
                        case '8':whole+=8*part;break;
                        case '9':whole+=9*part;break;
                        case 'a':case 'A':whole+=10*part;break;
                        case 'b':case 'B':whole+=11*part;break;
                        case 'c':case 'C':whole+=12*part;break;
                        case 'd':case 'D':whole+=13*part;break;
                        case 'e':case 'E':whole+=14*part;break;
                        case 'f':case 'F':whole+=15*part;break;
                        default:state=10000;break;
                    }
                    break;
                    //Octal Whole
                case 5:
                    whole*=8;
                    switch(l){
                        case '0':whole+=0;break;
                        case '1':whole+=1;break;
                        case '2':whole+=2;break;
                        case '3':whole+=3;break;
                        case '4':whole+=4;break;
                        case '5':whole+=5;break;
                        case '6':whole+=6;break;
                        case '7':whole+=7;break;
                        case '.':whole/=8.;state=6;break;
                        default:state=10000;
                    }
                    break;
                    //Octal Fraction
                case 6:
                    part/=8.;
                    switch(l){
                        case '0':whole+=0*part;break;
                        case '1':whole+=1*part;break;
                        case '2':whole+=2*part;break;
                        case '3':whole+=3*part;break;
                        case '4':whole+=4*part;break;
                        case '5':whole+=5*part;break;
                        case '6':whole+=6*part;break;
                        case '7':whole+=7*part;break;
                        default:state=10000;
                    }
                    break;

                    //Dec Whole
                case 7:
                    whole*=10;
                    switch(l){
                        case '0':whole+=0;break;
                        case '1':whole+=1;break;
                        case '2':whole+=2;break;
                        case '3':whole+=3;break;
                        case '4':whole+=4;break;
                        case '5':whole+=5;break;
                        case '6':whole+=6;break;
                        case '7':whole+=7;break;
                        case '8':whole+=8;break;
                        case '9':whole+=9;break;
                        case '.':state = 8;whole/=10.0;break;
                        case 'e':whole/=10.;exponent=0;state=9;break;
                        case ',':whole/=10.;break;
                        default:state=10000;break;
                    }
                    break;
                    //Dec Fraction
                case 8:
                    part/=10.;

                    switch(l){
                        case '0':whole+=0*part;break;
                        case '1':whole+=1*part;break;
                        case '2':whole+=2*part;break;
                        case '3':whole+=3*part;break;
                        case '4':whole+=4*part;break;
                        case '5':whole+=5*part;break;
                        case '6':whole+=6*part;break;
                        case '7':whole+=7*part;break;
                        case '8':whole+=8*part;break;
                        case '9':whole+=9*part;break;
                        case 'e':exponent=0;state=9;break;
                        default:state=10000;break;
                    }
                    break;
                    //Dec Engineering Sign
                case 9:
                    switch(l){
                        case '-':exponent_sign*=-1.;break;
                        case '+':exponent_sign*=1.;break;
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':state=10; --i; break;
                    }
                    break;
                    //Dec Engineering
                case 10:
                    exponent*=10;
                    switch(l){
                        case '0':exponent+=0;break;
                        case '1':exponent+=1;break;
                        case '2':exponent+=2;break;
                        case '3':exponent+=3;break;
                        case '4':exponent+=4;break;
                        case '5':exponent+=5;break;
                        case '6':exponent+=6;break;
                        case '7':exponent+=7;break;
                        case '8':exponent+=8;break;
                        case '9':exponent+=9;break;
                        default:
                            state=10000;
                            part/=10.;
                            break;
                    }
                    break;
                    //On/Off
                case 11:
                    switch(l){
                        case 'n':case 'N':whole=1.;state=10000;break;
                            //Don't need to check rest because function will always return 0 here
                        default:state=10000;break;
                    }
                    break;
                    //Y
                case 12:
                    if(l=='e'||l=='E')state=13;
                    else state =10000;
                    break;
                    //Ye
                case 13:
                    //Yes
                    if(l=='s'||l=='S')whole=1.0;
                    state =10000;
                    break;
                    //T
                case 14:
                    if(l=='r'||l=='R')state = 15;
                    else state =10000;
                    break;
                    //Tr
                case 15:
                    if(l=='u'||l=='U')state = 16;
                    else state =10000;
                    break;
                    //Tru
                case 16:
                    //True
                    if(l=='e'||l=='E')whole=1.0;
                    state =10000;
                    break;



            }
            ++i;
        }
        return whole*number_sign*std::pow(10.,exponent*exponent_sign);
    }
    static inline double string_to_number(const std::string &s){
        return string_to_number(s.begin(), s.end());
    }

    /**
     * @brief This class implements a sorted std::vector. It operates like an std::set, however iterating through all elements is fast, and insertion is expensive,
     * and changes the memory location of its contents. However all elements are sorted like in a set.
     * @tparam T the value to store in the vector.
     **/


    template<class T>
    class InstanceList{
        void _add(){
            mutex().lock();
            prev_instance = NULL;
            next_instance=get_root();
            get_root()=static_cast<T*>(this);
            if(next_instance)
                next_instance->prev_instance=static_cast<T*>(this);
            mutex().unlock();

        }
        void _remove(){
            mutex().lock();
            if(next_instance)next_instance->prev_instance=prev_instance;
            if(prev_instance)prev_instance->next_instance=next_instance;
            if(get_root()==static_cast<T*>(this))get_root()=next_instance;
            mutex().unlock();

        }
    public:
        T* next_instance;
        T* prev_instance;
        InstanceList(){_add();}
        InstanceList(const InstanceList& i){_add();}
        InstanceList& operator=(InstanceList& i){return *this;}
        ~InstanceList(){_remove();}
        static T*& get_root(){static T* r=NULL;return r;}
        static Mutex& mutex(){static Mutex m;return m;}
    };
    template <typename T>
    struct NoAutoAssignPtr{
        T* ptr;
        T* operator = (T*p){return ptr =p;}
        T& operator*(){return *ptr;}
        T* operator->(){return ptr;}
        operator bool(){return ptr;}
        bool operator == (T* o)const{return ptr==o;}
        bool operator != (T* o)const{return ptr!=o;}
        bool operator <= (T* o)const{return ptr<=o;}
        bool operator < (T* o)const{return ptr<o;}
        bool operator >= (T* o)const{return ptr>=o;}
        bool operator > (T* o)const{return ptr>o;}

    };
    /**
     * @brief This class implements String Interning. It allows for fast comparisons, and copies. However mutations of the string are expensive.
     * @details
     * In String Interning every unique string gets its own number. This makes string comparison and copies an O(1) operation, and very fast.
     * However, since a string must be compared with all other StringInterns when it is modified, its creation and mutation are expensive.
     * This class is useful, when you have mostly static strings that need to be looked up, or compared against many times. StringInterns also do not compare to each other
     * alphabetically.
     **/
#if USE_STACKSTRING
    struct StackString16{
        struct Data{
            uint64_t a;
            uint64_t b;
        };
        struct String{
            uint8_t str[15];
            uint8_t size;
        };
        union{
            Data data;
            String string;
        };
        StackString16(){string.size=0;}
        StackString16(const StackString16& s):data(s.data){}
        StackString16(const std::string& s){set(s.c_str());}
        StackString16(const char* s){set(s);}

        inline void set(const char* s){
            data.a=data.b=0;
            while(*s &&string.size<15)string.str[string.size++]=*(s++);

        }
        inline std::string str()const {return std::string((const char*)string.str,size_t(string.size));}
        inline bool operator==(const StackString16&str) const {
            return data.a==str.data.a&&data.b==str.data.b;
        }
        inline bool operator!=(const StackString16&str) const {
            return data.a!=str.data.a||data.b!=str.data.b;
        }
        inline bool operator<(const StackString16&str) const {
            return data.a==str.data.a?data.b<str.data.b:data.a<str.data.a;
        }
        inline bool operator<=(const StackString16&str) const {
            return data.a==str.data.a?data.b<=str.data.b:data.a<str.data.a;
        }
        inline bool operator>(const StackString16&str) const {
            return data.a==str.data.a?data.b>str.data.b:data.a>str.data.a;
        }
        inline bool operator>=(const StackString16&str) const {
            return data.a==str.data.a?data.b>=str.data.b:data.a>=str.data.a;
        }
    };
    static inline std::ostream& operator<<(std::ostream & stream,const StackString16& s){
        return stream<<s.str();
    }
    typedef StackString16 StringIntern;
    template<>
    struct TypeInfo<StringIntern>{
        enum {valid=1};
        static void handle(StringIntern&i,Visitor &v){
            v.type_name="StringIntern";
            std::string str(i.str());
            v.handle_string(str);
            if(!v.read_only())i=str;
        }

    };
#else
#if USE_STRINGINTERN

    struct StringIntern{
        struct TrieNode{
            char letter;
            TrieNode* less;
            TrieNode* equal;
            TrieNode* greater;
            TrieNode* parent;
            TrieNode(char l,TrieNode* parent):parent(parent),letter(l),less(NULL),equal(NULL),greater(NULL){}
        };
        static Mutex &create_m(){static Mutex m; return m;}
        static TrieNode* &root(){static TrieNode* r=NULL; return r;}
        TrieNode* data;
        StringIntern(){set("");}
        StringIntern(const StringIntern& s):data(s.data){}
        StringIntern(const std::string &s){set(s);}
        StringIntern(const char* s){set(s);}

        void set(const char* s, size_t size);
        void set(const std::string &s){
            set(s.c_str(),s.size());
        }

        std::string str()const;
        void print(TrieNode* s,const std::string &partial,int indent);
        void print_all(){
            print(root(), "", 0);
        }
        const TrieNode* hash() const {
            return data;
        }
        bool operator==(const StringIntern&str) const {
            return data==str.data;
        }
        bool operator!=(const StringIntern&str) const {
            return data!=str.data;
        }
        bool operator<(const StringIntern&str) const {
            return data<str.data;
        }
        bool operator<=(const StringIntern&str) const {
            return data<=str.data;
        }
        bool operator>(const StringIntern&str) const {
            return data>str.data;
        }
        bool operator>=(const StringIntern&str) const {
            return data>=str.data;
        }
    };
    static inline std::ostream& operator<<(std::ostream & stream,const StringIntern& s){
        return stream<<s.str();
    }
    template<>
    struct TypeInfo<StringIntern>{
        enum {valid=1};
        static void handle(StringIntern&i,Visitor &v){
            v.type_name="StringIntern";
            std::string str(i.str());
            v.handle_string(str);
            if(!v.read_only())i=str;
        }

    };
#endif
#endif
    template <typename TData, typename TKey, size_t size>
    struct PriorityCache{
        struct CacheNode{
            TData data;
            TKey key;
            CacheNode * more_fresh;
            CacheNode * more_stale;
        };
        CacheNode * most_stale;
        CacheNode * most_fresh;
        CacheNode cache[size];
        std::map<TKey, CacheNode*> mapping;
        size_t get_cache_size(){return size;}
        CacheNode * get_cache_entry(TKey &k){
            CacheNode *& n = mapping[k];
            if(!n){
                n=most_stale;
                mapping.erase(n->key);
                most_stale = n->more_fresh;
                most_stale->more_stale=NULL;
                n->data = TData();

            }
            if(n==most_fresh)most_fresh=n->more_stale;
            if(n->more_stale){
                n->more_stale->more_fresh=n->more_fresh;


            }
            if(n->more_fresh)
                n->more_fresh->more_stale=n->more_stale;
            if(n==most_stale)most_stale= n->more_fresh;

            most_fresh->more_fresh= n;
            n->more_stale=most_fresh;
            most_fresh=n;
            n->more_fresh=NULL;
            return n;
        }
        size_t get_cache_position(CacheNode* item){return item-cache;}
        size_t get_cache_position(TKey item){return get_cache_entry(item)-cache;}

        PriorityCache(){
            most_fresh = cache+size-1;
            most_stale = cache;
            for(int x=0;x<size;++x){
                cache[x].more_fresh = cache+x+1;
                cache[x].more_stale = cache+x-1;
                if(x==size-1)cache[x].more_fresh=NULL;
                if(x==0)cache[x].more_stale=NULL;

            }
        };
        bool in_cache(TKey k){
            return mapping[k];
        }
        TData& operator[](TKey& k){
            return get_cache_entry(k)->data;
        }

    };

#if !USE_STRINGINTERN
    typedef std::string StringIntern;
#endif
};

#endif
