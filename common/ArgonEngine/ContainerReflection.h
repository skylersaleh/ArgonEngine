/**
 * @brief Adds STL containers to the reflection system.
 * @file ContainerReflection.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef GenericSerialization_ContainerReflection_h
#define GenericSerialization_ContainerReflection_h
#include "TypeInfo.h"
#include <vector>
#include <set>
#include <list>
#include <deque>
#include <map>
#include <array>
#include <memory>
#include "Utility.h"
#include "HandlePicker.h"

namespace Argon {
template <typename T,typename T2>
struct TypeInfo<std::vector<T,T2> >{
    enum {valid=1};

    static void handle(std::vector<T,T2> &t, Visitor & v){
        v.type_name="std::vector";
        if(v.read_only()){
            for(size_t x=0; x<t.size(); ++x) HANDLE_WITH_PICKER(v,t[x]);
        }else{
            t.clear();
            size_t items;
            while((items=v.get_min_remaining_items())){
                size_t off = t.size();
                t.resize(t.size()+items);
                while(items--){
                    HANDLE_WITH_PICKER(v,t[off++]);
                }
            }
        }
    }
};
template <typename T,size_t size>
struct TypeInfo<std::array<T,size> >{
    enum {valid=1};
    static void handle(std::array<T,size> &t, Visitor & v){
        v.type_name="std::array";
        for(size_t x=0; x<t.size(); ++x) HANDLE_WITH_PICKER(v,t[x]);
    }

};
template <typename T,typename T2,typename T3>
struct TypeInfo<std::set<T,T2,T3> >{
    enum {valid=1};
    static void handle(std::set<T,T2,T3> &t, Visitor & v){
        v.type_name="std::set";
        auto it = t.begin();
        if(v.read_only()){
            while(it!=t.end()) {HANDLE_WITH_PICKER(v,*it); ++it; }
        }else{
            t.clear();
            T val;
            while(v.can_unpack_value()) {
                HANDLE_WITH_PICKER(v,val);
                t.insert(val);
            }
        }
    }

};
template <typename T,typename T2>
struct TypeInfo<std::list<T,T2> >{
    enum {valid=1};
    static void handle(std::list<T,T2> &t, Visitor & v){
        v.type_name="std::list";
        auto it = t.begin();
        if(v.read_only()){
        while(it!=t.end()) {HANDLE_WITH_PICKER(v,*it); ++it; }
        }else{
            t.clear();
            T val;
            while(v.can_unpack_value()) {
                HANDLE_WITH_PICKER(v,val);
                t.push_back(val);
            }
        }
    }
};
template <typename T,typename T2>
struct TypeInfo<std::deque<T,T2> >{
    enum {valid=1};

    static void handle(std::deque<T,T2> &t, Visitor & v){
        v.type_name="std::deque";
        if(v.read_only()){
            for(size_t x=0; x<t.size(); ++x) HANDLE_WITH_PICKER(v,t[x]);
        }else{
            t.clear();
            t.reserve(v.get_min_remaining_items());

            while(v.can_unpack_value()){
                t.push_back(T());
                HANDLE_WITH_PICKER(v,t.back());
            }
        }
    }
};
template <typename T,typename T2>
struct TypeInfo<std::pair<T,T2> >{
    enum {valid=1};

    static void handle(std::pair<T,T2> &t, Visitor & v){
        v.type_name="std::pair";
        if(v.begin_map()){
            HANDLE_KEY_WITH_PICKER(v,t.first,"a");
            HANDLE_KEY_WITH_PICKER(v,t.second,"b");
            v.finish_map();
        }
    }
};
template <typename T,typename T2,typename T3, typename T4>
struct TypeInfo<std::map<T,T2,T3,T4> >{
    enum {valid=1};


    static void unpack(std::map<T,T2,T3,T4> &t, Visitor & v){
        v.type_name="std::map";
        auto it = t.begin();
        if(v.read_only()){
            while(it!=t.end()) {HANDLE_WITH_PICKER(v,*it); ++it; }
        }else{
            t.clear();
            typename std::pair<T,T2> val;
            while(v.can_unpack_value()) {
                HANDLE_WITH_PICKER(v,val);
                t.insert(val);
            }
        }
    }
};
template <typename T2,typename T3, typename T4>
struct TypeInfo<std::map<std::string,T2,T3,T4> >{
    enum {valid=1};

    static void handle(std::map<std::string,T2,T3,T4> &t, Visitor & v){
        v.type_name="std::map";
        if(v.begin_map()) {
            if(v.read_only()){
                auto it = t.begin();
                while(it!=t.end()) {HANDLE_KEY_WITH_PICKER(v,it->second,it->first.c_str()); ++it; }
            }else{
                std::string s;
                while(v.has_unhandled_keys()) {
                    s= v.get_unhandled_key();
                    HANDLE_KEY_WITH_PICKER(v,t[s],s.c_str());
                }
            }
            v.finish_map();
        }
    }

};
#if USE_STRINGINTERN
template <typename T2,typename T3, typename T4>
struct TypeInfo<std::map<Argon::StringIntern,T2,T3,T4> >{
    enum {valid=1};


    static void handle(std::map<StringIntern,T2,T3,T4> &t, Visitor & v){
        v.type_name="std::map";
        if(v.begin_map()) {
            if(v.read_only()){
                auto it = t.begin();
                while(it!=t.end()) {HANDLE_KEY_WITH_PICKER(v,it->second,it->first.str().c_str()); ++it; }
            }else{
                std::string s;
                while(v.has_unhandled_keys()) {
                    s= v.get_unhandled_key();
                    HANDLE_KEY_WITH_PICKER(v,t[s],s.c_str());
                }
            }
            v.finish_map();
        }
    }

};
#endif
template<typename T>
struct TypeInfo<std::shared_ptr<T> >{
    enum {valid=1};
    static void handle(std::shared_ptr<T> &t, Visitor & v){
        v.type_name = "shared_ptr";
        if(t)HANDLE_WITH_PICKER(v,*t);
        else if(!v.read_only()){
            auto va=std::make_shared<T>();
            HANDLE_WITH_PICKER(v,*va);
            t=va;
        }
    }

};
}

#endif
