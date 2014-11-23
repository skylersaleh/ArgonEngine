/**
 * @brief Open GLES
 * @file OpenGLES.h
 * @author Skyler Saleh
 **/

#ifndef NeonRush_AROpenGLES_h
#define NeonRush_AROpenGLES_h 1

#include "config.h"


#ifdef USE_OPENGL
#if GRAPHICS_DEBUG_LEVEL > 5 && USE_OPENGL

#define GL_CHECK(A) if(int x = glGetError()){std::cout<<"GLERROR: "<< A <<" E: "<< x <<"\n";}
#else
#define GL_CHECK(A) ;
#endif

#include "Utility.h"
#include <vector>
#include <map>
#include <set>
#include "Types.h"
#include "RenderSystem.h"
#include "RenderAPI.h"
#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#ifdef USE_GLEW
#include "GL/glew.h"
#endif
#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#endif
#ifdef USE_SDL
#ifndef USE_OPENGLES
    #include "SDL2/SDL_opengl.h"
#endif
#ifdef USE_OPENGLES
#ifdef PLATFORM_ANDROID
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
    #include "SDL2/SDL_opengles2.h"
#endif
#endif
#ifdef PLATFORM_WINDOWS
#undef near
#undef far
#endif
namespace Argon {
    class OpenGLES :public RenderAPI
{
    struct vert_data {

        size_t buff_size;
        GLuint vert_buffer;
        size_t index_size;
        GLuint index_buffer;
        size_t update_id;
        size_t vertices;

        size_t last_frame;
        vert_data():vert_buffer(0),index_buffer(0),update_id(0),last_frame(0){}
    };
    struct RealTexFormat {
        int texture_format;
        GLenum internal_format;
        GLenum format;
        GLenum type;
    };
    struct Uniform {
        GLint uniform;
        GLenum type=0;
        size_t size;
        void *pointer;
        size_t frame;
        Uniform() : uniform(-1){}
    };
    struct TexturePrim {
        GLuint tex;
        RealTexFormat f;
        int width;
        int height;
        size_t update_id;
        size_t last_frame;
        TexturePrim() : width(-1),height(-1),last_frame(0){
            glGenTextures(1, &tex);
        }
        void alloc_texture(int w, int h, const RealTexFormat &form){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            f= form;
            width = w;
            height = h;
            if(w!=next_pow2(w)||h!=next_pow2(h)) {
                f.texture_format|=kTextureDontMipmap;
            }
            GLenum min_filter = GL_NEAREST;
            GLenum mag_filter = GL_NEAREST;

            if(!(f.texture_format&kTextureDontMipmap)) {
                if(!(f.texture_format&kTextureDontFilterMipmap))
                    min_filter=!(f.texture_format&kTextureDontFilterPixels) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
                else
                    min_filter=!(f.texture_format&kTextureDontFilterPixels) ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
            }else min_filter=!(f.texture_format&kTextureDontFilterPixels) ? GL_LINEAR : GL_NEAREST;

            mag_filter = !(f.texture_format&kTextureDontFilterPixels) ? GL_LINEAR : GL_NEAREST;
            if(f.texture_format&kTextureAnsiotropic2x)
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
            else if(f.texture_format&kTextureAnsiotropic4x)
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
            else if(f.texture_format&kTextureAnsiotropic8x)
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
            // the texture wraps over at the edges (repeat)
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     f.texture_format&kTextureClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     f.texture_format&kTextureClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT );
            glTexImage2D(GL_TEXTURE_2D, 0, f.internal_format, w, h, 0, f.format, f.type,NULL);
        }
        inline int next_pow2(int v)
        {
            v--;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            v++;
            return v;
        }
        void upload_texture_data(int w, int h, void* data){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            if(data)
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, f.format, f.type,data);

            if (!(f.texture_format&kTextureDontMipmap)) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        ~TexturePrim(){
            glDeleteTextures(1, &tex);
        }
    };
    struct shader_data {
        GLuint program;
        GLint active_attribs;
        GLint active_uniforms;
        size_t last_frame;

        std::map<StringIntern,Uniform> uniforms;
        std::map<StringIntern,GLint> attributes;
        shader_data():last_frame(0),program(0){}
    };
    typedef std::map<StringIntern,Uniform>::iterator u_iter;

    unsigned int current_blend;
    std::vector<GLuint> bound_textures;
    GLint main_frame_buffer;
    GLuint render_tex_frame_buffer;
    GLuint current_frame_buffer;
    GLuint active_texture;
    std::shared_ptr<VertexArray> current_vert_array;
    Vector4f current_clear_color;
    int current_cull_face;
    size_t current_frame;
    shader_data* current_shader;
    unsigned int current_render_flags;
    std::vector<RealTexFormat> tex_formats;
    std::map<std::string,bool> extensions;
    std::map<VirtualResource,shader_data> shaders;

    std::map<std::shared_ptr<VertexArray>,vert_data> vertex_arrays;
    std::map<VirtualResource, TexturePrim> textures;

    void init_extensions()
    {
        const unsigned char * str = glGetString(GL_EXTENSIONS);
        if(!str) return;
        while (*str!='\0') {
            std::string ext;
            while(*str!='\0' && *str!=' ') {
                ext+=*str;
                ++str;
            }
            if(*str!='\0') ++str;
            extensions[ext] = true;
        }
    };
    bool initialized;
    bool compile_shader(GLuint &shader, GLenum type,const std::string & source);
    bool link_shader(GLuint shader);
    void init_fbos(){
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&main_frame_buffer);
        glGenFramebuffers(1, &render_tex_frame_buffer);

    }
    void bind_attributes(shader_data& data,std::shared_ptr<VertexArray> v);
public:
    OpenGLES() : initialized(false){
        init_fbos();
    }
    void cache_array(std::shared_ptr<VertexArray> array);
    void cache_material(Material& state, const VirtualResource&  shader);
    void clear(bool color, bool depth, bool stencil){
        GLbitfield clear=0;
        if(color) clear|=GL_COLOR_BUFFER_BIT;
        if(depth) clear|=GL_DEPTH_BUFFER_BIT;
        if(stencil) clear|=GL_STENCIL_BUFFER_BIT;

        glClear(clear);
    }
    void discard_buffers(bool color, bool depth, bool stencil){
        if(extensions["GL_EXT_discard_framebuffer"]) {
            std::vector<GLenum> v;
            if(color) v.push_back(GL_COLOR_ATTACHMENT0);
            if(depth) v.push_back(GL_DEPTH_ATTACHMENT);
            if(stencil) v.push_back(GL_STENCIL_ATTACHMENT);
#ifndef PLATFORM_ANDROID
#ifdef GL_EXT_discard_framebuffer
            glDiscardFramebufferEXT(GL_FRAMEBUFFER, v.size(), &v[0]);
#endif
#endif
        }
    }
    void bind_depth_texture(VirtualResource t){
        GL_CHECK("BeforeDepthTexBind");

        if(!textures[t].tex) return;
        //if(current_frame_buffer!=render_tex_frame_buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, render_tex_frame_buffer);
#if USE_OPENGLES
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,textures[t].tex,0);
#else
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[t].tex, 0);
#endif
        current_frame_buffer=render_tex_frame_buffer;
        GLenum g=glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if(g!=GL_FRAMEBUFFER_COMPLETE) std::cout<<"FBO is not complete error:"<<g<<"\n";
        current_frame_buffer=render_tex_frame_buffer;
        GL_CHECK("AfterDepthTexBind");

    }
    void bind_color_texture(VirtualResource t,int level, int buffer){
        GL_CHECK("BeforeColorTexBind");
        //if(current_frame_buffer!=render_tex_frame_buffer)
        if(!textures[t].tex) return;
        glBindFramebuffer(GL_FRAMEBUFFER, render_tex_frame_buffer);
#if USE_OPENGLES
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+buffer, GL_TEXTURE_2D,textures[t].tex, level);
#else
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+buffer, textures[t].tex, level);
#endif
        GLenum g=glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if(g!=GL_FRAMEBUFFER_COMPLETE) std::cout<<"FBO is not complete error:"<<std::hex<<g<<"\n";
        current_frame_buffer=render_tex_frame_buffer;
        GL_CHECK("AfterColorTexBind");

    }
    void set_uniforms(Uniforms ** uniforms, int size);

    void bind_default_framebuffer(){
        GL_CHECK("BeforeDefault Bind");

        //if(current_frame_buffer!=main_frame_buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, main_frame_buffer);

        current_frame_buffer=main_frame_buffer;
        GL_CHECK("After Default bind");

    }

    void bind_render_framebuffer(){

        glBindFramebuffer(GL_FRAMEBUFFER, render_tex_frame_buffer);
    }
    void set_viewport(int x,int y, int w, int h)
    {
        glScissor(x, y, w, h);
        glViewport(x, y, w, h);
    }
    void set_clear_color(Argon::Vector4f v){
        if(v!=current_clear_color)
            glClearColor(v[0], v[1], v[2], v[3]);
        current_clear_color=v;
    }
    int make_shader(VirtualResource& shader);
    void set_shader(Renderable*,VirtualResource& x,Uniforms** uniforms, int size);
    void add_format(unsigned int base_format,GLenum internal_format, GLenum format, GLenum type);
    void cache_texture(VirtualResource tex);
    void init_texture_formats();
    RealTexFormat get_closest_format(int format){
        RealTexFormat ret = tex_formats.back();
        for (size_t x=0; x<tex_formats.size(); ++x) {
            if (texture_format_fitness(format, ret.texture_format)<texture_format_fitness(format, tex_formats[x].texture_format)) {
                ret=tex_formats[x];
            }
        }
        if((ret.texture_format&kTextureAnsiotropicMask)<(format&kTextureAnsiotropicMask))
            format=(format&~kTextureAnsiotropicMask)|(ret.texture_format&kTextureAnsiotropicMask);
        ret.texture_format= (ret.texture_format&~(kTextureFlagMask|kTextureAnsiotropicMask))|(format&(kTextureFlagMask|kTextureAnsiotropicMask));

        return ret;
    }
    void set_render_flags(unsigned int render_flags){
        unsigned int delta = render_flags^current_render_flags;
        if(delta&kRenderDepthTest) {
            if(render_flags&kRenderDepthTest)
                glEnable(GL_DEPTH_TEST);
            else glDisable(GL_DEPTH_TEST);
        }

        if(delta&kRenderDepthMask)
            glDepthMask(bool(render_flags&kRenderDepthMask));

        if(delta&kRenderColorMask)
            glColorMask(render_flags&kRenderRedMask,
                        render_flags&kRenderGreenMask,
                        render_flags&kRenderBlueMask,
                        render_flags&kRenderAlphaMask);

        current_render_flags=render_flags;

    }
    void set_blend(unsigned int blend);
    void set_cull_face(int face);
    void set_depth_range(float near, float far){
#ifdef USE_OPENGLES
        glDepthRangef(near,far);
#else
        glClearDepth(far);
        glDepthRange(near, far);
#endif
    }
    void draw_vertex_array(std::shared_ptr<VertexArray> array,int end_vert, int d);
    void update_resources();
};
};

#endif //USE_OPENGL
#endif
