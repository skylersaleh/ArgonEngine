/**
 * @brief The Virtual Resource system.
 * @file VirtualResource.h
 * @author Skyler Saleh
 **/
#ifndef ARVIRTUAL_RESOURCE_DEF
#define ARVIRTUAL_RESOURCE_DEF
#include <stdint.h>
#include <vector>
#include "Thread.h"
#include <string>
#include "TypeInfo.h"
#include "Utility.h"
#include <map>
namespace Argon{
    namespace VirtualResourceIMPL{
        size_t search_string(const std::string &s, char character,size_t begin=0,size_t end=0);
        size_t advance_whitespace(const std::string &s,size_t begin=0,size_t end=0);
        size_t trim_whitespace(const std::string &s,size_t begin=0,size_t end=0);
        size_t parse_arguments(const std::string & s, std::string &args, size_t begin, size_t end);
        size_t search_string_reverse(const std::string &s, char character,size_t begin=0,size_t end=0);
        struct Source{
            Source(){}
            virtual bool reload(){return true;}
            virtual bool save(){return true;}
            virtual size_t size()const=0;
            virtual void resize(size_t byte_size)=0;
            virtual size_t read(char* buffer, size_t buffer_size, size_t offset)=0;
            virtual size_t write(const char* buffer, size_t buffer_size, size_t offset)=0;
            virtual Source* create_source(const std::string& path,const std::string &arguments)=0;
            virtual uint8_t* get_pointer(){return NULL;}
            virtual size_t update_id(){return 0;}
            virtual ~Source(){}
        };
        struct Data{
            virtual bool loaded()=0;
            virtual bool reload(Source* s)=0;
            virtual bool save(Source* s)=0;

            virtual Data* clone_type(const std::string& arguments)const=0;

            virtual size_t update_id()=0;
            virtual ~Data(){}
        };
        struct Pointer{
            size_t users;
            size_t read_id;
            Data* data;
            Source* source;
            Pointer():users(0),data(NULL),source(NULL){}
            void update_data(){
                if(data&&source)data->reload(source);
            }
            void remove_data();
            void remove_source();
            void set_source(Source* s,bool reload);
            size_t update_id();
            void set_data(Data *d, bool reload);
            bool can_delete(){return !users;}
            ~Pointer(){remove_data();remove_source();}
        };

    };
    void get_argument_map(const std::string & args, std::map<std::string,std::string>& arg_map);
    struct VirtualResource{
        VirtualResourceIMPL::Pointer *p_node;
        size_t last_update_id=-1;
        static std::map<std::string,VirtualResourceIMPL::Pointer> &all_nodes();
        static std::map<std::string,VirtualResourceIMPL::Source*> &all_sources();
        static std::map<std::string,VirtualResourceIMPL::Data*> &all_data();
        static Mutex& m();
        static void initialize();

        void remove();
        void set(VirtualResourceIMPL::Pointer *p);
        void set_source(VirtualResourceIMPL::Source *s,bool reload=true){
            p_node->set_source(s, reload);
        }
        void set_source(const std::string &s,bool reload=true);
        void set_data(VirtualResourceIMPL::Data *d,bool reload=true){
            p_node->set_data(d, reload);
        }
        void set_data(const std::string &ext,bool reload=true);
        void set(const std::string & path);
        size_t update_id();
        bool has_updated(){
            if(last_update_id!=update_id()){last_update_id=update_id();return true;}
            return false;
        }

        size_t size()const{return p_node&&p_node->source?p_node->source->size():0;}
        void resize(size_t byte_size){if(p_node&&p_node->source) p_node->source->resize(byte_size);}
        size_t read(char* buffer, size_t buffer_size, size_t offset){
            if(p_node){
                return p_node->source ? p_node->source->read(buffer,buffer_size,offset):0;
            }
            return 0;
        }
        void reload(){if(p_node&&p_node->source)p_node->source->reload();}
        size_t write(const char* buffer, size_t buffer_size, size_t offset){
            return p_node&&p_node->source ? p_node->source->write(buffer,buffer_size,offset):0;
        }
        std::string get_data_as_string();
        std::map<std::string,VirtualResourceIMPL::Pointer>::iterator
        get_pointer_iter();
        std::map<std::string,VirtualResourceIMPL::Pointer>::const_iterator
        get_pointer_iter()const;
        std::string get_path_string()const{
            auto it =get_pointer_iter();
            return it==all_nodes().end()? "Unknown Path": it->first;
        }
        template <typename T=VirtualResourceIMPL::Source*> T get_source(){return p_node? dynamic_cast<T>(p_node->source):NULL;}

        template <typename T> T get_data(){return p_node? dynamic_cast<T>(p_node->data):NULL;}
        bool operator<(const VirtualResource& r)const{return p_node<r.p_node;}
        bool operator>(const VirtualResource& r)const{return p_node>r.p_node;}
        bool operator<=(const VirtualResource& r)const{return p_node<=r.p_node;}
        bool operator>=(const VirtualResource& r)const{return p_node>=r.p_node;}
        bool operator!=(const VirtualResource& r)const{return p_node!=r.p_node;}
        bool operator==(const VirtualResource& r)const{return p_node==r.p_node;}
        VirtualResource():p_node(NULL){}

        VirtualResource(const std::string&path):p_node(NULL){set(path);}
        VirtualResource(const char* path):p_node(NULL){set(path);}

        VirtualResource(const VirtualResource& r):p_node(NULL){set(r.p_node);}



        VirtualResource& operator=(const VirtualResource&r ){if(&r!=this)set(r.p_node);return *this;}
        operator bool()const {return p_node;}
    };


    struct VirtualResourceIO:public VirtualResourceIMPL::Source{
        VirtualResourceIO(const std::string &str,bool allow_write=false):path(str),allow_write(allow_write),data(""){}
        Mutex loading;
        size_t update_id_;
        std::string path;
        std::string data;
        bool allow_write;

        bool reload();
        bool save();
        virtual size_t update_id();
        size_t size()const{return data.size();}
        size_t read(char* buffer, size_t buffer_size, size_t offset);
        virtual uint8_t* get_pointer(){return(uint8_t*)&data[0];}
        void resize(size_t byte_size){data.resize(byte_size);}
        size_t write(const char* buffer, size_t buffer_size, size_t offset);
        VirtualResourceIMPL::Source *create_source(const std::string& pa,const std::string& args);
    };



    template<>
    struct TypeInfo<VirtualResource>{
        enum {valid=1};

        static void handle(VirtualResource&i,Visitor &v){
            v.type_name="VirtualResource";
            std::string str= i.get_path_string();
            v.handle_string(str);
            if(!v.read_only())i=str;
        }

    };
};
#endif

