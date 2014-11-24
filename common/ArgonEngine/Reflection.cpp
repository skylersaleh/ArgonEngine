/**
 * @brief This provides a node that renders some basic shapes
 * @file Reflection.cpp
 * @author Skyler Saleh, Ian Wiggins
 **/
#include "Reflection.h"
#include <fstream>
#include "Utility.h"
#include <sstream>
#include <cassert>
#include <sstream>
#include <json/json.h>
#include <set>
#include "HandlePicker.h"

enum type{
    kInvalid=0x0,
    kAHF    =0x1,
    kInt8   =0x2,
    kInt16  =0x3,
    kInt32  =0x4,
    kInt64  =0x5,
    kUInt8  =0x6,
    kUInt16 =0x7,
    kUInt32 =0x8,
    kUInt64 =0x9,
    kFloat  =0xA,
    kDouble =0xB,
    kString =0xC,

};
namespace Argon{
void ReflectionBase::read(VirtualResource res){
    VisitableResource *r = res.get_data<VisitableResource*>();
    if(!r){
        std::cout<<"Virtual Resource "<<res.get_path_string()<<" is not visitable\n";
        return;
    }

    auto read_vis = r->read_visitor();
    visit(*read_vis);
    r->read_visitor_finish(read_vis);
}

void ReflectionBase::write(VirtualResource res){
    VisitableResource *r = res.get_data<VisitableResource*>();
    if(!r){
        std::cout<<"Virtual Resource "<<res.get_path_string()<<" is not visitable\n";
        return;
    }
    auto write_vis = r->write_visitor();
    visit(*write_vis);
    r->write_visitor_finish(write_vis);
}


    bool PrintVisitor::set_key(const char*key){
        strings.push_back("*KEY*");
        strings.push_back(key);
        return true;
    }
    bool PrintVisitor::is_array(size_t start){
        int complete=0;
        int recurse=0;
        for(;start<strings.size();++start){
            if(!recurse){

                if(strings[start]!="*KEY*"){
                    complete++;
                }
                else return complete==0;;
            }
            if(strings[start]=="*MAPB*")recurse++;
            if(strings[start]=="*MAPE*")recurse--;
            if(recurse<0)return complete==0;
            if(complete==2)return true;
        }
        return complete==0;

    }
    bool PrintVisitor::handle_double(double& v){
        std::stringstream s;
        s<<v;strings.push_back(s.str());
        return true;
    }
    bool PrintVisitor::handle_string(std::string& key){
        std::stringstream s;
        if(key.size()<400||print_long_strings)
            s<<"\""<<key<<"\"";
        else s<<"Long string with: "<<key.size()<<" characters.";
        strings.push_back(s.str());
        return true;
    }
    void PrintVisitor::print_string(std::ostream &s, size_t x,size_t indent_level){
        bool contains_newlines=false;
        for(size_t i=0;i<strings[x].size();++i){
            contains_newlines|=strings[x][i]=='\n';
        }
        if(contains_newlines){
            s<<"\n";
            indent(s,indent_level+1);
        }
        for(size_t i=0;i<strings[x].size();++i){
            s.put(strings[x][i]);
            if(strings[x][i]=='\n')indent(s, indent_level+1);
        }

    }
    void PrintVisitor::print(std::ostream &s, size_t &x, size_t indent_level,  int recurse){
        recurse--;
        bool was_array=false;
        bool was_key =false;
        int key_size =0;
        int print_key_elements=0;
        bool array_newline = false;
        if(!recurse)
        {
            indent(s, indent_level);
            s<<"Max Recursion Reached.";
            while (x<strings.size()&&recurse<1) {
                if(strings[x]=="*MAPB*")recurse--;
                if(strings[x]=="*MAPE*")recurse++;
                ++x;
            }
            return;
        }


        print_key_elements=17;

        for(;x<strings.size();++x){
            if(strings[x]=="*KEY*"){
                if(was_array){
                    if(print_key_elements==0){
                        s<<"... "<<key_size-print_key_elements<<" More Elements";
                    }
                    s<<"]"; indent_level--;
                    was_array=false;
                }

                if(key_size)s<<";\n";
                ++x;
                indent(s, indent_level);
                s<<strings[x]<<" = ";
                key_size=0;
                was_key=true;
                if(is_array(x+1)){
                    s<<"[";
                    indent_level++;
                    was_array=true;
                }
                print_key_elements=17;

            }else if(strings[x]=="*MAPB*") {
                if(key_size){ s<<",\n";
                    indent(s,indent_level);
                }
                s<<strings[++x];
                int x_old = x;
                ++x;
                s<<"{\n";
                print(s, x,indent_level+1,recurse);
                s<<"\n";
                indent(s,indent_level);
                s<<"}";
                if(x-x_old>20){
                    s<<"//"<<strings[x_old];
                }

                ++key_size;
            }else if(strings[x]=="*MAPE*")break;
            else if(print_key_elements>0){

                if(key_size){s<<", ";}

                print_string(s, x, indent_level);
                ++key_size;

                --print_key_elements;
            }else{
                ++key_size;
            }
        }
        if(was_array){s<<"]"; indent_level--;}
        if(was_key)s<<";";

        if(indent_level!=0)indent_level--;

    }
    struct JsonWriter: public Visitor{
        struct StackFrame;

        std::shared_ptr<StackFrame> stack_back=nullptr;
        std::shared_ptr<StackFrame> stack_free=nullptr;

        JsonWriter();
        virtual bool read_only(){return true;}
        bool write(VirtualResource res);
        bool write_str(std::string & st);

        void flush_value();

        virtual bool set_key(const char*key);

        virtual bool handle_int(intmax_t& v);

        virtual bool handle_double(double& v);

        virtual bool handle_string(std::string& s);

        virtual bool begin_map();

        virtual bool finish_map();
    };
    struct JsonReader: public Visitor{
        struct StackFrame{
            Json::Value value;
            Json::Value map_value;
            std::set<std::string> read_members;
            size_t size = 0;
        };
        std::vector<StackFrame> stack;
        JsonReader(){stack.push_back(StackFrame());}
        bool read(VirtualResource res);
        bool read(std::string &data);

        Json::Value get_value();

        virtual bool set_key(const char*key);

        bool can_unpack_value(){
            auto &b =stack.back();
            return !b.value.isNull()&& b.value.isValidIndex(b.size);
        }
        bool has_unhandled_keys(){
            return stack.back().read_members.size();
        }
        std::string get_unhandled_key(){
            return *stack.back().read_members.begin();
        };

        virtual bool handle_int(intmax_t& v)override;
        virtual bool handle_double(double & v)override;
        bool handle_string(std::string & v);


        virtual bool read_only(){return false;}
        virtual bool begin_map();

        virtual bool finish_map();
    };
    bool JsonReader::read(std::string &data){
        Json::Reader r;

        bool success = r.parse(data,stack.back().map_value,false);
        if(!success){
            std::cout<< "Failed to parse JSON from string. "<<"\n";
            std::cout<<"Error:\n"<<r.getFormatedErrorMessages()<<"\n";
        }
        stack.back().value = stack.back().map_value;
        return success&&!stack.back().map_value.isNull();
    }

    bool JsonReader::read(VirtualResource res){
        Json::Reader r;
        bool success = r.parse(res.get_data_as_string(),stack.back().map_value,false);
        if(!success){
            std::cout<< "Failed to parse JSON from resource: "<<res<<"\n";
            std::cout<<"Error:\n"<<r.getFormatedErrorMessages()<<"\n";
        }
        return success&&!stack.back().map_value.isNull();
    }

    Json::Value JsonReader::get_value(){
        auto &b =stack.back();
        if(b.value.isArray()){
            return b.value.get(b.size++,Json::Value());
        }else return b.value;
    }

    bool JsonReader::set_key(const char *key){
        auto &b =stack.back();
        b.value = b.map_value.get(key,Json::Value());
        b.size=0;
        b.read_members.erase(key);
        return !b.value.isNull();
    }

    bool JsonReader::handle_int(intmax_t &v){
        auto val = get_value();
        if(val.isConvertibleTo(Json::realValue))v=val.asDouble();
        else if(val.isString()){
            v = Argon::string_to_number(val.asString());
        }
        else return false;
        return true;
    }

    bool JsonReader::handle_double(double &v){
        auto val = get_value();
        if(val.isConvertibleTo(Json::realValue))v=val.asDouble();
        else if(val.isConvertibleTo(Json::intValue))v=val.asInt();
        else if(val.isString()){
            v = Argon::string_to_number(val.asString());
        }
        else return false;
        return true;
    }

    bool JsonReader::handle_string(std::string &v){
        auto val = get_value();
        if(val.isNull())return false;
        v=val.asString();
        return true;
    }

    bool JsonReader::begin_map(){
        auto v = get_value();
        if(!v.isObject())return false;
        stack.push_back(StackFrame());
        auto &b = stack.back();
        b.map_value =v;
        //b.value = v;
        const auto & mn = v.getMemberNames();
        for(auto m : mn){
            b.read_members.insert(m);
        }
        return true;
    }

    bool JsonReader::finish_map(){

        if(stack.size()>1){
            stack.pop_back();
        }
        return true;
    }
    struct JsonWriter::StackFrame{
        Json::Value value;
        Json::Value map_value;

        std::string key="";
        size_t size = 0;
        std::shared_ptr<StackFrame> next=nullptr;
        void add_value(Json::Value v){
            if(size==0){
                value = v;
            }else if (size==1){
                Json::Value prev = value;
                value = Json::Value(Json::arrayValue);
                value.append(prev);
                value.append(v);

            }else value.append(v);
            ++size;
        }
    };

    JsonWriter::JsonWriter(){stack_back = std::make_shared<StackFrame>();}

    bool JsonWriter::write(VirtualResource res){
        std::stringstream str;

        str<< stack_back->value;
        auto s =str.str();
        res.resize(s.size());

        res.write(&s[0],s.size(),0);
        return true;
    }

    bool JsonWriter::write_str(std::string &st){
        std::stringstream str;
        str<<stack_back->value;
        st =str.str();
        return true;
    }

    void JsonWriter::flush_value(){
        auto &b = *stack_back;
        if(b.size)
            b.map_value[b.key]=b.value;
        b.key="";
        b.size=0;

    }



    bool JsonWriter::set_key(const char *key){
        flush_value();
        stack_back->key=key;
        return true;
    }

    bool JsonWriter::handle_int(intmax_t &v){
        auto &b = *stack_back;
        Json::Value val = v>INT32_MAX? Json::Value(double(v)): Json::Value(int(v));
        b.add_value(val);
        return true;

    }

    bool JsonWriter::handle_double(double &v){
        stack_back->add_value(Json::Value(v));
        return true;
    }

    bool JsonWriter::handle_string(std::string &s){
        stack_back->add_value(Json::Value(s));
        return true;

    }

    bool JsonWriter::begin_map(){
        if(stack_free){
            auto t = stack_free;
            stack_free = t->next;
            t->next = stack_back;
            stack_back = t;
        }else{
            auto t = stack_back;
            stack_back = std::make_shared<StackFrame>();
            stack_back->next=t;
        }
        auto &b = *stack_back;
        b.map_value = Json::Value(Json::objectValue);
        return true;
    }

    bool JsonWriter::finish_map(){
        flush_value();
        if(stack_back){
            auto b = stack_back;
            stack_back = b->next;
            stack_back->add_value(b->map_value);
            b->next= stack_free;
            stack_free =b;
        }
        return true;
    }

    std::shared_ptr<Visitor> JsonResource::read_visitor(){
        std::shared_ptr<JsonReader> json_read= std::make_shared<JsonReader>();
        std::string str;
        str.resize(source->size());

        source->read(&str[0], str.size(),0);

        if(!json_read->read(str)){
            std::cout<<"Failed to parse json file"<<std::endl;

        }
        return json_read;
    }

    std::shared_ptr<Visitor> JsonResource::write_visitor(){
        std::shared_ptr<JsonWriter>  json_write = std::make_shared<JsonWriter>();
        return json_write;
    }

    void JsonResource::write_visitor_finish(std::shared_ptr<Visitor> &v){
        std::shared_ptr<JsonWriter>  json_write=std::static_pointer_cast<JsonWriter>(v);
        std::string str;
        json_write->write_str(str);
        source->resize(str.size());
        source->write(&str[0],str.size(),0);
    }
    struct AHFWriter : public Visitor {

        std::vector<WriterStackData>* stack;
        virtual bool set_key(const char*key)override;
        virtual bool handle_int(intmax_t& v)override;
        virtual bool handle_double(double& v)override;
        virtual bool handle_string(std::string& key)override;
        virtual bool read_only(){return true;}
        virtual bool begin_map();
        virtual bool finish_map();
        void write_to_file(VirtualResourceIMPL::Source *p);
        AHFWriter();
        virtual ~AHFWriter();
    };



    struct AHFReader : public Visitor {
        struct StackData {
            struct KeyData {
                uint64_t offset;
                uint64_t current_off;
                uint64_t end;
                uint16_t type;
                bool handled;
            };
            uint64_t total_size;
            std::map<std::string, KeyData> keys;
            inline bool read_buffer(VirtualResourceIMPL::Source*stream, char *c, int size){
                stream->read(c, size, key_data->current_off);
                key_data->current_off+=size;
                return true;
            }
            KeyData*key_data;
            bool unpack_map(VirtualResourceIMPL::Source* stream, size_t offset);
            bool set_key(const char* k);
            inline bool unpack_int(intmax_t&val, VirtualResourceIMPL::Source*stream){
                if(!key_data||key_data->current_off>=key_data->end){return false;}
                val=0;
                switch (key_data->type) {

                case kUInt8:return read_buffer(stream,(char*)&val, 1);
                case kUInt16:return read_buffer(stream,(char*)&val, 2);
                case kUInt32:return read_buffer(stream,(char*)&val, 4);
                case kInt64:
                case kUInt64:return read_buffer(stream,(char*)&val, 8);
                case kInt8:{
                    int8_t v;
                    bool b= read_buffer(stream,(char*)&val, 1);
                    val=v;
                    return b;
                }
                case kInt16:{
                    int16_t v;
                    bool b= read_buffer(stream,(char*)&v, 2);
                    val=v;
                    return b;
                }
                case kInt32:{
                    int32_t v;
                    bool b= read_buffer(stream,(char*)&v, 4);
                    val=v;
                    return b;
                }
                case kFloat:
                case kDouble:{
                    double v;
                    bool b=unpack_double(v, stream);
                    val=(intmax_t)v;
                    return b;
                }

                }
                return false;
            }
            bool unpack_double(double&error, VirtualResourceIMPL::Source*stream);
            bool unpack_string(std::string&error, VirtualResourceIMPL::Source*stream);
            bool can_unpack_value(){
                return key_data && key_data->current_off<key_data->end;
            }
            size_t get_min_remaining_items(){
                if(!key_data)return 0;
                size_t bytes=  key_data->end-key_data->current_off;
                switch(key_data->type){
                    case kInt8:
                    case kUInt8:
                        return bytes;
                case kInt16:
                case kUInt16:
                    return bytes/2;
                case kInt32:
                case kUInt32:
                    return bytes/4;
                case kInt64:
                case kUInt64:
                    return bytes/8;
                case kFloat:
                    return bytes/4;
                case kDouble:
                    return bytes/8;
                default:
                    return can_unpack_value();
                }
            }

            bool has_unhandled_keys();
            std::string get_unhandled_key();
            StackData() : key_data(NULL){}
        };
        VirtualResourceIMPL::Source *source;
        //VirtualResource resource;

        int64_t buffer_start;
        std::vector<StackData*> stack;
        bool set_key(const char*key)override{
            return stack.back()->set_key(key);
        }
        size_t get_min_remaining_items(){
            return stack.back()->get_min_remaining_items();

        }

        bool can_unpack_value()override{
            if(stack.empty()||error) return false; else return stack.back()->can_unpack_value();
        }
        bool has_unhandled_keys()override{
            return stack.back()->has_unhandled_keys();
        }
        std::string get_unhandled_key()override{
            return stack.back()->get_unhandled_key();
        };

        virtual bool handle_int(intmax_t& v)override{
            return stack.back()->unpack_int(v, source);
        }
        virtual bool handle_double(double & v)override{
            return stack.back()->unpack_double(v, source);
        }
        bool handle_string(std::string & v)override{
            return stack.back()->unpack_string(v,source);
        }
        bool read_only()override{return false;}
        bool begin_map()override;
        bool finish_map()override;
    };

    struct WriterStackData{
        std::stringstream table;
        std::stringstream data;
        uint64_t offset;
        std::string key;
        int type;
        uint64_t get_key_size();
        WriterStackData();
        void finalize_key();
        bool pack_int(intmax_t v);
        bool pack_double(double v);
        bool pack_string(const std::string& v);
        bool pack_map(WriterStackData& d);
        WriterStackData(const WriterStackData& s){
            table.clear();
            table<<s.table.str();
            data.clear();
            data<<s.data.str();
            offset=s.offset;
            key=s.key;
            type=s.type;
        }
        WriterStackData& operator=(const WriterStackData& s){
            if(&s==this)return *this;
            table.clear();
            table<<s.table.str();
            data.clear();
            data<<s.data.str();
            offset=s.offset;
            key=s.key;
            type=s.type;
            return *this;
        }
    };
    bool Visitor::handle_double(double &v){std::stringstream s;s<<v; std::string str=s.str();return handle_string(str);}
    AHFWriter::AHFWriter(){
        stack = new std::vector<WriterStackData>();
        stack->push_back(WriterStackData());
    }
    bool AHFWriter::handle_int(intmax_t& v){return stack->back().pack_int(v);}
    bool AHFWriter::handle_double(double& v){return stack->back().pack_double(v);}
    bool AHFWriter::handle_string(std::string& key){return stack->back().pack_string(key);}
    bool AHFWriter::begin_map(){stack->push_back(WriterStackData());return true;}
    AHFWriter::~AHFWriter(){delete stack;}
    uint64_t WriterStackData::get_key_size(){
        uint64_t x= offset;
        offset=0;
        return x;
    }
     WriterStackData::WriterStackData():offset(0),type(0),table(""),data(""){
        table.clear();
        table.write("AHF",3);
    }
    void WriterStackData::finalize_key(){
        uint64_t o=get_key_size();
        uint16_t ks= key.size();
        if(type){
            table.write((const char*)&type, 2);
            table.write((const char*)&o, 8);
            table.write((const char*)&ks, 2);

            table.write((const char*)key.data(),ks);
            type=0;
        }
    }
    bool  WriterStackData::pack_int(intmax_t v){
        if(type==0)type= kInt64;
        data.write((const char*)&v, 8);
        offset+=8;
        return type==kInt64;
    }
    bool  WriterStackData::pack_double(double v){
        if(type==0)type= kDouble;
        data.write((const char*)&v, 8);
        offset+=8;
        return type==kDouble;
    }
    bool  WriterStackData::pack_string(const std::string& v){
        if(type==0)type= kString;
        uint16_t s =v.size();
        data.write((const char*)&s, 2);
        data.write((const char*)v.data(),s);
        offset+=s+2;
        return type==kString;
    }
    bool  WriterStackData::pack_map(WriterStackData& d){
        if(type==0)type= kAHF;
        int x=0;
        uint64_t s =d.table.str().size()+d.data.str().size();
        data.write(d.table.str().data(), d.table.str().size());
        data.write((char*)&x, 2);
        data.write(d.data.str().data(), d.data.str().size());
        offset+=s+2;
        return type==kAHF;
    }
    bool AHFWriter::set_key(const char*key){
        stack->back().finalize_key();
        stack->back().key=key;
        return true;
    }
    void AHFWriter::write_to_file(VirtualResourceIMPL::Source*p){
        if(!p)return;
        p->write((char*)stack->back().data.str().c_str(), stack->back().data.str().size(), 0);


    }

    bool AHFWriter::finish_map(){
        if(stack->size()>1){
            stack->back().finalize_key();
            bool b=(stack->end()-2)->pack_map(stack->back());
            stack->pop_back();
            return b;
        }
        return true;
    }



    bool AHFReader::StackData::unpack_map(VirtualResourceIMPL::Source* stream, size_t offset){
        char b[3];
        if(!stream)return false;
        stream->read(b,3, offset);

        if(b[0]!='A')return false;
        if(b[1]!='H')return false;
        if(b[2]!='F')return false;
        uint64_t table_size=5;
        uint64_t data_size=0;
        uint16_t type=0;

        size_t off = offset+3;

        stream->read((char*)&type,2,off);
        off+=2;

        bool eof=false;
        while (type&&!eof) {
            uint64_t size=0;
            std::string key;

            eof|=!stream->read((char*)&size,8,off);
            off+=8;

            uint16_t c=0;
            eof|=!stream->read((char*)&c, 2,off);
            off+=2;

            key.resize(c);
            eof|=!stream->read(&key[0], c,off);
            off+=c;

            keys[key].current_off=keys[key].offset=data_size;
            data_size+=size;
            keys[key].handled=false;
            keys[key].type=type;
            keys[key].end=data_size;

            eof|=!stream->read((char*)&type,2,off);
            off+=2;
            table_size+=12+key.size();
        }
        std::map<std::string, KeyData>::iterator it=keys.begin();
        while(it!=keys.end())
        {
            it->second.offset+=table_size+offset;
            it->second.end+=table_size+offset;
            it->second.current_off=it->second.offset;
            ++it;
        }
        total_size=table_size+data_size;
        set_key("");
        return true;
    }
    bool AHFReader::StackData::set_key(const char* k){
        if(!keys.count(k)){
            key_data=NULL;
            return false;
        }
        key_data =&keys[k];
        key_data->handled=true;
        return true;
    }

    bool AHFReader::StackData::unpack_double(double &val, VirtualResourceIMPL::Source*stream){
        if(!key_data||key_data->current_off>=key_data->end){ return false;}
        switch (key_data->type) {
            case kInt8:
            case kInt16:
            case kInt32:
            case kInt64:
            case kUInt8:
            case kUInt16:
            case kUInt32:
            case kUInt64:{
                intmax_t v;
                bool b=unpack_int(v, stream);
                if(b)
                val=(double)v;
                return b;
            }
            case kDouble:return read_buffer(stream,(char*)&val, 8);
            case kFloat:{
                float v;
                bool b=read_buffer(stream,(char*)&v, 4);
                val=v;
                return b;
            }

            default:
                break;
        }
        return false;
    }
    bool AHFReader::StackData::unpack_string(std::string &v, VirtualResourceIMPL::Source *stream){
        if(!key_data||key_data->type!=kString||key_data->current_off>=key_data->end){return false;}
        uint16_t size=0;
        read_buffer(stream,(char*)&size,2);

        v.resize(size);
        read_buffer(stream,(char*)&v[0],size);

        return true;
    }
    bool AHFReader::StackData::has_unhandled_keys(){

        std::map<std::string, KeyData>::iterator it = keys.begin();
        while(it!=keys.end()){
            if(it->second.handled==false)return true;
            ++it;
        }

        return false;
    }
    std::string AHFReader::StackData::get_unhandled_key(){
        std::map<std::string, KeyData>::iterator it = keys.begin();
        while(it!=keys.end()){
            if(it->second.handled==false)return it->first;
            ++it;
        }
        return "";
    }
    bool AHFReader::begin_map(){
        uint64_t off =0;
        if(stack.size()){
            if(!stack.back()->key_data){return false;};
            off = stack.back()->key_data->current_off;
            if(stack.back()->key_data->type!=kAHF||stack.back()->key_data->current_off>=stack.back()->key_data->end){return false;}
        }

        stack.push_back(new StackData());
        stack.back()->key_data=NULL;
        if(stack.back()->unpack_map(source,off))return true;


        stack.pop_back();
        return false;
    }
    bool AHFReader::finish_map(){

        if(stack.size()>1){



            (*(stack.end()-2))->key_data->current_off+=stack.back()->total_size;
            delete stack.back();
            stack.pop_back();
        }
        return true;
    }
    std::shared_ptr<Visitor> AHFResource::read_visitor(){
        std::shared_ptr<AHFReader> read= std::make_shared<AHFReader>();
        std::string str;
        read->source=source;
        return read;
    }

    std::shared_ptr<Visitor> AHFResource::write_visitor(){
        std::shared_ptr<AHFWriter>  write = std::make_shared<AHFWriter>();
        return write;
    }

    void AHFResource::write_visitor_finish(std::shared_ptr<Visitor> &v){
        std::shared_ptr<AHFWriter>  write=std::static_pointer_cast<AHFWriter>(v);
        write->write_to_file(source);
    }


}

using Argon::Pick_Handle;

std::ostream& operator<<(std::ostream &s, Argon::ReflectionBase& v){
    Argon::PrintVisitor vis;
    HANDLE_WITH_PICKER(vis,v);
    size_t x =0;

    vis.print(s,x);
    return s;
}
