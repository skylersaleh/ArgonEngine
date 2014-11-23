/**
 * @brief General serialization.
 * @file Reflection.h
 * @author Skyler Saleh
 **/

#ifndef __GenericSerialization__Reflection__
#define __GenericSerialization__Reflection__
#include <iostream>
#include "TypeInfo.h"
#include "VirtualResource.h"
namespace Argon {



struct WriterStackData;

struct VisitableResource : public VirtualResourceIMPL::Data{
    VirtualResourceIMPL::Source *source;
    size_t update_id_=0;
    virtual size_t update_id(){return update_id_;}
    virtual bool loaded(){return true;}
    virtual VirtualResourceIMPL::Data* clone_type(const std::string& arguments)const=0;
    virtual std::shared_ptr<Visitor> read_visitor()=0;
    virtual std::shared_ptr<Visitor> write_visitor()=0;
    virtual void read_visitor_finish(std::shared_ptr<Visitor>&v){}
    virtual void write_visitor_finish(std::shared_ptr<Visitor>&v){}
    virtual bool save(VirtualResourceIMPL::Source* s){source = s; return true;}
    virtual bool reload(VirtualResourceIMPL::Source* s){source=s; return true;}
};
struct JsonResource: public VisitableResource{
    virtual bool loaded(){return true;}
    virtual VirtualResourceIMPL::Data* clone_type(const std::string& arguments)const{
        return new JsonResource;
    }
    virtual std::shared_ptr<Visitor> read_visitor()override;

    virtual std::shared_ptr<Visitor> write_visitor();
    virtual void write_visitor_finish(std::shared_ptr<Visitor>&v);


};
struct AHFResource: public VisitableResource{
    virtual bool loaded(){return true;}
    virtual VirtualResourceIMPL::Data* clone_type(const std::string& arguments)const{
        return new AHFResource;
    }
    virtual std::shared_ptr<Visitor> read_visitor()override;

    virtual std::shared_ptr<Visitor> write_visitor();
    virtual void write_visitor_finish(std::shared_ptr<Visitor>&v);


};

struct PrintVisitor : public Visitor {
    std::vector<std::string> strings;
    bool print_long_strings;
    PrintVisitor(bool print_long_strings=false) : print_long_strings(print_long_strings){}
    void clear(){
        strings.clear();
    }
    bool read_only()override{return true;}
    bool set_key(const char*key)override;
    virtual bool handle_double(double& v)override;
    virtual bool handle_string(std::string& key);
    void print_string(std::ostream &s,size_t x,size_t indent_level);
    bool begin_map(){
        strings.push_back("*MAPB*"); strings.push_back(type_name); return true;
    }
    bool finish_map(){
        strings.push_back("*MAPE*"); return true;
    }

    void indent(std::ostream &s, size_t indent){
        for(size_t x=0; x<indent; ++x) s<<"  ";
    }
    bool is_array(size_t start);
    void print(std::ostream &s, size_t &x, size_t indent_level=0,  int recurse=9);
};


}

inline static std::ostream& operator<<(std::ostream& s, Argon::PrintVisitor&vis){
    size_t x =0;
    vis.print(s, x);
    return s;
}
/*template<typename T>
static inline auto operator<<(std::ostream &s, T& v)
->typename std::enable_if<Argon::TypeInfo<T>::valid, std::ostream&>::type{
    Argon::PrintVisitor vis;
    vis.handle(v);
    return s<<vis;
}*/



#endif
