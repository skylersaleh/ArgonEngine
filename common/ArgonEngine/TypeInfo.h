//Generated by the Argon Build System

/**
 * @brief This file contains the classes necessary to define a classes type information for use with the Reflection System.
 * @file TypeInfo.h
 * @author Skyler Saleh
 **/

#ifndef TypeInfo_h
#define TypeInfo_h
#include <stdint.h>
#include <memory>
#include <type_traits>
#include <map>
#include "Thread.h"
/**
 * @brief This macro allows you to quickly define the TypeInfo of most structs.
 * @details
 * To use a struct with the Reflection System, its TypeInfo must first be specified. This macro allows you to do that
 * easily. However, you may have to define your own TypeInfo class for more advanced functionality.
 *
 * Here is an example of its use, to describe the following classes:
 *
 * \code{.cpp}
 * struct MyClass{
 *      int x;
 *      int y;
 *      int derived;
 *      std::string string_data;
 *
 *      void recalculate_derived(){derived=x-y;}
 * };
 * struct MyClass2: public MyClass{
 *      int data;
 * };
 * \endcode
 *
 * You must add the MAKE_TYPE_INFO macro after the classes declaration like so:
 * \code{.cpp}
 * struct MyClass{
 *     int x;
 *     int y;
 *     int derived;
 *     std::string string_data;
 *
 *     void recalculate_derived(){derived=x-y;}
 * };
 * MAKE_TYPE_INFO(MyClass,{
 *     ADD_VAR(x);
 *     ADD_VAR(y);
 *     ADD_VAR2(string_data,"string data name");
 *     if(!mode)t.recalculate_derived();//Derived must be recalculated after unpacking.
 * };
 * struct MyClass2: public MyClass{
 *      int data;
 * };
 * MAKE_TYPE_INFO(MyClass2,{ADD_VAR(data);ADD_BASE(MyClass);});
 * \endcode
 *
 * Now MyClass and MyClass2 can be used in the Reflection System.
 **/
#define MAKE_VISIT(A,B)\
    virtual void visit(Visitor&v){\
        v.type_name = #A;\
        if(v.begin_map()){\
            A&t = *this;\
            B\
            v.finish_map();\
        }\
    }\

#define MAKE_VISIT_HEAD(A)\
    virtual void visit(Visitor&v);
#define REGISTER_FACTORY_EXPLICIT(A,B)\
    std::shared_ptr<ReflectionBase> make_##A(){return std::make_shared<A>();}\
    volatile bool reg_this##A= Argon::ReflectionBase::add_factory(B,make_##A);

#define MAKE_VISIT_IMPL(A,B)\
    std::shared_ptr<ReflectionBase> make_##A(){return std::make_shared<A>();}\
    bool reg_this##A= Argon::ReflectionBase::add_factory(#A,make_##A);\
    void A::visit(Visitor&v){\
        v.type_name = #A;\
        if(v.begin_map()){\
            A&t = *this;\
            B\
            v.finish_map();\
        }\
    }\

#define MAKE_TYPE_INFO(A,B)namespace Argon {template<>struct TypeInfo<A>{ \
        enum {valid=1}; \
        static void handle(A &t, Visitor & v){v.type_name = #A;if(v.begin_map()){handle2(t,v);v.finish_map();}}; \
        static void handle2(A &t, Visitor & v){B}}; }; \

//! A duplicate of MAKE_TYPE_INFO(A,B) except it is for use in the Argon namespace.
#define MAKE_TYPE_INFO_Argon(A,B)template<>struct TypeInfo<A>{ \
        enum {valid=1}; \
        static void handle(A &t, Visitor & v){v.type_name = #A;if(v.begin_map()){handle2(t,v);v.finish_map();}}; \
        static void handle2(A &t, Visitor & v){B}};
//! Used to add a variable in the MAKE_TYPE_INFO macro. It is stored under a key that matches the variables name.
#define ADD_VAR(A) v.handle(t.A,#A)
//! Used to add a variable in the MAKE_TYPE_INFO macro under a certain key.
//! @param A The variable to add.
//! @param B The key to add it as.
#define ADD_VAR2(A,B) v.handle(t.A,B)
//! Used to add a base class in the MAKE_TYPE_INFO macro.
#define ADD_BASE(A){ \
v.set_key(#A);\
    A::visit(v);\
}
#include <limits>
#include <string>
namespace Argon {
struct Visitor;
/**
 * @brief This class represents an objects complete typeinfo.
 * @details
 * A specialization of this class must provide an enum where valid is set to one and a unpack and pack static functions.
 * The Unpack and Pack Static Functions should each take a refrerence to the type the typeinfo is describing along with a reference
 * to an Argon::Visitor. It should call the respective pack and unpack functions on the visitor for every variable required to
 * serialize or deserialize a class.
 *
 * You should use this class over TypeInfoEasy when you must provide specialized functionality for unpack/pack such as generation of derived data.
 *
 * \code{.cpp}
 * struct MyClass{
 *      int x;
 *      int y;
 *      int derived;
 *      std::string string_data;
 *
 *      void recalculate_derived(){derived=x-y;}
 * };
 * template<> struct Argon::TypeInfo<MyClass>{
 *      enum{valid=1};
 *      static void pack(const MyClass& obj, Argon::Visitor &vis){
 *          if(vis.pack_map()){
 *              vis.pack(obj.x,"x");
 *              vis.pack(obj.y,"y");
 *              vis.pack(obj.string_data,"string_data");
 *              vis.finish_map();
 *          }
 *      }
 *      static void unpack(MyClass&obj,Argon::Visitor&vis){
 *          if(vis.unpack_map())
 *          {
 *              vis.unpack(obj.x,"x");
 *              vis.unpack(obj.y,"y");
 *              vis.unpack(obj.string_data,"string_data");
 *              obj.recalculate_derived();
 *              vis.finish_map();
 *          }
 *      }
 * };
 * \endcode
 *
 * @tparam T The type to describe the type info of.
 **/
template <typename T> struct TypeInfo {enum {valid=0}; };
struct VirtualResource;
struct ReflectionBase{
    static std::map<std::string,std::shared_ptr<ReflectionBase>(*)()>& factory_map(){
        static std::map<std::string,std::shared_ptr<ReflectionBase>(*)()> m;
        return m;
    }
    static Mutex& mutex(){
        static Mutex m;
        return m;
    }

    static bool add_factory(const std::string& str, std::shared_ptr<ReflectionBase>(*ptr)()){
        mutex().lock();
        factory_map()[str]=ptr;
        mutex().unlock();
        return true;
    }
    static std::shared_ptr<ReflectionBase> make_type(const std::string& str){
        mutex().lock();
        auto s = factory_map()[str]();
        mutex().unlock();
        return s;
    }
    static void print_registered_factories(){
        mutex().lock();
        std::cout<< "There are "<< factory_map().size()<<" registered factories, that the reflection system can spawn:\n";
        size_t max=0;
        for (auto &a : factory_map()){
            if(max<a.first.size()+1)max=a.first.size()+1;
        }
        size_t indent = (max/4+1)*4;
        size_t pos =0;
        std::cout<<"|| ";
        for (auto &a : factory_map()){
            if(pos>=80){
                std::cout<<"\n|| ";
                pos=0;
            }
            std::cout<<a.first;
            pos+=a.first.size();
            while((pos%indent)&&pos<80){
                std::cout<<" ";
                ++pos;
            }

        }
        std::cout<<std::endl;
        mutex().unlock();
    }

    virtual void visit(Visitor& v){};
    virtual void read(VirtualResource res);
    virtual void write(VirtualResource res);
    virtual ~ReflectionBase(){}
};
/**
 * @brief The Visitor class is the key external class of the Reflection System.
 * @details
 * The Visitor class gets called on all variables of a class that needs to be serialized by the user or TypeInfo specialized
 * classes.
 **/
struct Visitor {
    ///These structs are used to help identify the variable type to call the correct function definition.
    template <typename T, bool type_valid, bool numerical, bool integer,bool reflection_base> struct HandleIMPL {};

    template <typename T, bool numerical, bool integer> struct HandleIMPL<T,false,numerical,integer,true>{
        static void go(Visitor& v, const T&t){
            const_cast<T&>(t).visit(v);
        }
    };

    template <typename T, bool numerical, bool integer,bool reflection> struct HandleIMPL<T,true,numerical,integer,reflection>{
        static void go(Visitor& v, const T&t){

            TypeInfo<T>::handle(const_cast<T&>(t),v);
        }
    };

    template <typename T> struct HandleIMPL< T,false,true,false,false>{
        static void go(Visitor& v, T&t){
            double d =t;
            v.handle_double(d);
            t=d;
        }
    };


    template <typename T> struct HandleIMPL<T,false,true,true,false>{
        static void go(Visitor& v,T&t){
            intmax_t i = t;
            v.handle_int(i);
            t=i;
        }
    };
    virtual bool read_only()=0;
    bool error;     //!< True if an error has occurred false otherwise.
    //! A function pointer that returns the name of a type currently being described.
    const char*type_name;
    Visitor() : error(false){}
    //! Sets the key to identify a piece of data by when packing or unpacking. Returns true if successful.
    virtual bool set_key(const char*key)=0;

    //! Handles an integer. Call this function multiple times to pack an array. Returns true if successful.
    virtual bool handle_int(intmax_t& v){
        double d = v;
        bool b=handle_double(d);
        v=d;
        return b;
    }

    //! Handles a double. Call this function multiple times to pack an array. Returns true if successful.
    virtual bool handle_double(double& v);

    //! Handles a string. Call this function multiple times to pack an array. Returns true if successful.
    virtual bool handle_string(std::string& key)=0;
    //! Marks the beginning of a new map of data to pack. You must call this in each typeinfo that
    //! identifies data by a key. You also must check that the return type is true before
    //! packing data or calling finish map on the visitor. Returns true if successful.
    virtual bool begin_map()=0;


    //! Marks the end of a map of data. There must be the same amount of finish map calls as successful
    //! pack_map or unpack_map calls. Returns true on success.
    virtual bool finish_map()=0;
    //! Returns true if their is another value that can be unpacked for a certain key. You use
    //! this to unpack arrays.
    virtual bool can_unpack_value(){
        return false;
    }
    virtual size_t get_min_remaining_items(){
        return can_unpack_value();
    }

    //! Returns true if the current map has keys that have not been accessed using set_key.
    virtual bool has_unhandled_keys(){
        return false;
    }
    //! Returns a std::string that contains one key that has not been handled.
    virtual std::string get_unhandled_key(){
        return "";
    };



    virtual ~Visitor(){}

    /**
     * Packs or unpacks a given variable under a certain key by identifying its type.
     * @tparam mode Whether to pack or unpack the variable. True to pack, False to unpack.
     * @tparam T The type of the variable to pack or unpack.
     * @param t The variable to pack or unpack.
     * @param str The key to pack or unpack the variable under.
     **/
    template<typename T>
    void handle(T& t, const char* str){
        set_key(str);
        HandleIMPL<T,
                   TypeInfo<T>::valid,
                   std::numeric_limits<T>::is_specialized,
                   std::numeric_limits<T>::is_integer,
                   std::is_base_of<ReflectionBase,T>::value
                >::go(*this,t);
    }
    /**
     * Packs or unpacks a given variable under a certain key by identifying its type.
     * @tparam mode Whether to pack or unpack the variable. True to pack, False to unpack.
     * @tparam T The type of the variable to pack or unpack.
     * @param t The variable to pack or unpack.
     * @param str The key to pack or unpack the variable under.
     **/
    template<typename T>
    void handle(T& t){
        HandleIMPL<T,
                   TypeInfo<T>::valid,
                   std::numeric_limits<T>::is_specialized,
                   std::numeric_limits<T>::is_integer,
                   std::is_base_of<ReflectionBase,T>::value
                >::go(*this,t);
    }





};
/**
 * @brief This class provides an implementation of an TypeInfo class that allows you to combine your
 * pack and unpack functions into one function called handle. It allows you to have less duplication in
 * your type info declarations.
 *
 * @details The class should be subclassed when defining a TypeInfo. It is used like this:
 *
 * \code{.cpp}
 * struct MyClass{
 *      int x;
 *      int y;
 *      int derived;
 *      std::string string_data;
 *
 *      void recalculate_derived(){derived=x-y;}
 * };
 * template<> struct Argon::TypeInfo<MyClass>: public Argon::TypeInfoEasy<Argon::TypeInfo<MyClass>,MyClass>{
 *      enum{valid=1};
 *      template <bool mode>
 *      static void handle(MyClass&obj,Argon::Visitor&vis){
 *          vis.handle<mode>(obj.x,"x");
 *          vis.handle<mode>(obj.y,"y");
 *          vis.handle<mode>(obj.string_data,"string_data");
 *          if(!mode)obj.recalculate_derived(); //Derived must be recalculated after unpacking.
 *      }
 * };
 * \endcode
 *
 * @tparam T The TypeInfo class that is subclassing the TypeInfoEasy struct.
 * @tparam TType The Type to describe the TypeInfo for.
 **/


template<>
struct TypeInfo<std::string>{
    enum{ valid=1};
    static void handle(std::string &t, Visitor & v){
        v.handle_string(t);
    }

};
};
std::ostream& operator<<(std::ostream &s, Argon::ReflectionBase& v);

#endif
