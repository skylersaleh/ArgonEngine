 /**
 * @brief
 * Provides an Extension Handler for image data, and the class used to auto-load common-types.
 * @file VirtualResourceImage.h
 * @author Skyler Saleh
 *
 * @details
 * This file defines the base class for all virtual image data.
 **/

#ifndef __ArgonEngineApp__VirtualImageResource__
#define __ArgonEngineApp__VirtualImageResource__
#include "RenderSystem.h"
#include "VirtualResource.h"
#include "Thread.h"

namespace Argon {
/**
 * @brief The base class for all Extension Handlers that represent virtual image data.
 * @details
 * By default this will decode common image types. Such as:
 * - JPEG
 * - PNG
 * - TGA
 * - PSD
 * - GIF
 * - HDR
 * - PIC
 **/
struct VirtualResourceImage : public VirtualResourceIMPL::Data {
protected:
    int width;         //!< The width of the image in pixels.
    int height;        //!< The height of the image in pixels.
    unsigned int format;     //!< The texture format to use when creating a texture.
    Thread load_thread;

    unsigned char* image_data;      //!< A pointer to the image data.
    static void *thread_help(void*p);
public:
    size_t update_id_;
    RecursiveMutex load_mutex;
    bool done_loading;
    bool preloading;         //!< True if the image is currently preloading.
    VirtualResourceImage(int w, int h, unsigned int format) : width(w),height(h),format(format),image_data(NULL),done_loading(false),preloading(false),update_id_(0){}
    virtual ~VirtualResourceImage(){delete []image_data;}
    //! Used by the Virtual Resource System
    int get_width(){return width;}
    int get_height(){return height;}
    int get_format(){return format;}
    unsigned int parse_format(std::map<std::string,std::string>&arg_map)const;

    virtual VirtualResourceIMPL::Data* clone_type(const std::string& arguments)const{

        std::map<std::string,std::string> arg_map;
        arg_map["w"]=arg_map["h"]="-1";
        arg_map["f"]="RGBA8";
        arg_map["mip"]=arg_map["filter"]=arg_map["filter_mip"]="1";
        arg_map["ansiotropic"]="0";
        arg_map["clamp"]=arg_map["fbo"]="0";
        get_argument_map(arguments, arg_map);

        int w = string_to_number(arg_map["w"]);
        int h = string_to_number(arg_map["h"]);
        w=std::max(w,-1);
        h=std::max(h,-1);
        unsigned int format = parse_format(arg_map);



        VirtualResourceImage * im =  new VirtualResourceImage(w,h,format);
        if(w!=-1&&h!=-1)im->create(w, h,format);
        return im;
    }
    const unsigned char* get_image_data(){return image_data;}
    Vector4f get_pixel_color(int x, int y){
        return tex_lookup_color(image_data, format, x, y, width
        , height);
    }
    Vector4f get_sample_color(float x, float y){
        return tex_lookup_color(image_data, format, x*width+0.5, y*height+0.5, width
        , height);
    }

    void set_sample_color(float x, float y,Vector4f c){
        tex_set_color(image_data, format,c, x*width+0.5, y*height+0.5, width
        , height);
    }
    void set_pixel_color(float x, float y,Vector4f c){
        tex_set_color(image_data, format,c, x, y, width
        , height);
    }
    virtual size_t update_id(){return update_id_;}
    virtual bool loaded(){return done_loading;}
    void create(int w, int h, unsigned int tex_format = kTextureRGBA8){
        load_mutex.lock();
        image_data=new unsigned char[get_tex_format_pixel_size(format)*w*h];
        width=w;
        height=h;
        format=tex_format;
        load_mutex.unlock();
    }
    virtual bool save(VirtualResourceIMPL::Source* s);
    virtual bool reload(VirtualResourceIMPL::Source* s);
};
    struct RandomTexture:public VirtualResourceImage{
    private:

        Thread load_thread;
        int always_update;
        static void *thread_help(void*p);
    public:

        RandomTexture(int w, int h, unsigned int format,int always_update) : VirtualResourceImage(w,h,format),always_update(always_update){}
        virtual ~RandomTexture(){}
        virtual VirtualResourceIMPL::Data* clone_type(const std::string& arguments)const{

            std::map<std::string,std::string> arg_map;
            arg_map["w"]=arg_map["h"]="-1";
            arg_map["f"]="RGBA8";
            arg_map["mip"]=arg_map["filter"]=arg_map["filter_mip"]="1";
            arg_map["ansiotropic"]="0";
            arg_map["clamp"]=arg_map["fbo"]="0";
            arg_map["always_update"]="0";

            get_argument_map(arguments, arg_map);

            int w = string_to_number(arg_map["w"]);
            int h = string_to_number(arg_map["h"]);
            w=std::max(w,-1);
            h=std::max(h,-1);

            unsigned int format = parse_format(arg_map);

            RandomTexture * im =  new RandomTexture(w,h,format,always_update);
            if(w!=-1&&h!=-1)im->create(w, h,format);
            return im;
        }

        virtual size_t update_id(){return update_id_+=always_update;}
        virtual bool loaded(){return done_loading;}
        void create(int w, int h, unsigned int tex_format = kTextureRGBA8){
            load_mutex.lock();
            image_data=new unsigned char[get_tex_format_pixel_size(format)*w*h];
            width=w;
            height=h;
            format=tex_format;
            load_mutex.unlock();
        }
        virtual bool save(VirtualResourceIMPL::Source* s){return true;}
        virtual bool reload(VirtualResourceIMPL::Source* s){return true;}
    };

}
#endif /* defined(__ArgonEngineApp__VirtualImageResource__) */
