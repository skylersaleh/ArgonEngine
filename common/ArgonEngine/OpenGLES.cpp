//Generated by the Argon Build System

//
//  AROpenGLES.cpp
//  NeonRush
//
//  Created by Skyler Saleh on 5/2/12.
//  Copyright (c) 2012 Argon Software. All rights reserved.
//
#include "OpenGLES.h"
#include <typeinfo>
#ifdef USE_OPENGL
#include <iostream>
#ifdef __IPHONE__
#import <GLKit/GLKit.h>
#endif
#include <string>
#include "Types.h"
#include "VirtualResourceImage.h"
namespace Argon{

void OpenGLES::bind_attributes(shader_data&d, std::shared_ptr<VertexArray> v)
{
    auto at_it = d.attributes.begin();
    while(at_it!=d.attributes.end()){
        std::vector<VertexAttribPair>::const_iterator it =v->attributes.begin();
        while(it!=v->attributes.end()&&it->attribute!=at_it->first)++it;
        if(it==v->attributes.end())glDisableVertexAttribArray(at_it->second);
        else{
            glEnableVertexAttribArray(at_it->second);
            GLenum t=GL_FLOAT;
            switch (it->type) {
            case Argon::kRenderTypeUByte: t=GL_UNSIGNED_BYTE;break;
            case Argon::kRenderTypeShort: t=GL_SHORT;break;
            case Argon::kRenderTypeFloat: t=GL_FLOAT;break;
            case Argon::kRenderTypeByte: t=GL_BYTE;break;
            default:
                break;
            }
            glVertexAttribPointer(at_it->second, it->components, t, false, v->stride, (void*)it->offset);
        }

        ++at_it;
    }

}
void OpenGLES::draw_vertex_array(std::shared_ptr<VertexArray> array,int end_vert,int d)
{
    GL_CHECK("Before Draw Elements\n");

    if(!array)return;
    size_t index_size= array->index_data.size();
    vert_data& vert_d =vertex_arrays[array];

    if(current_vert_array!=array){
        if(!vert_d.vert_buffer)return;

        current_vert_array=array;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_d.index_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vert_d.vert_buffer);
        bind_attributes(*current_shader,array);
    }

    GLenum draw_type;
    switch (d) {
    case Argon::kDrawPoints:
        draw_type=GL_POINTS;
#ifndef USE_OPENGLES
        glPointSize(array->prim_size);
#endif
        break;
    case Argon::kDrawLineLoop:draw_type=GL_LINE_LOOP; glLineWidth(array->prim_size);break;
    case Argon::kDrawLines:draw_type=GL_LINES;   glLineWidth(array->prim_size); break;
    case Argon::kDrawTriangles:draw_type=GL_TRIANGLES;break;
    case Argon::kDrawTriangleStrip:draw_type=GL_TRIANGLE_STRIP;break;
    case Argon::kDrawTriangleFan:draw_type=GL_TRIANGLE_FAN;break;
    default:draw_type=GL_TRIANGLES;break;
    }
#if GRAPHICS_DEBUG_LEVEL >8
    force_print(std::cout,*array)<<"\n";
#endif
    if(end_vert==-1)end_vert=index_size;
#ifndef USE_OPENGLES
    if(end_vert)
        glDrawRangeElements(draw_type,0,array->vertex_count(),end_vert,GL_UNSIGNED_SHORT,NULL);
#endif
#ifdef USE_OPENGLES
    if(end_vert)
        glDrawElements(draw_type,array->vertex_count(),GL_UNSIGNED_SHORT,NULL);
#endif
    if(end_vert>10000||array->vertex_count()>10000)

        GL_CHECK("After Draw Elements\n");

}
void OpenGLES::init_texture_formats(){
    unsigned int texture_mask=kTextureFBO|kTextureClamp|kTextureDontFilterPixels|kTextureDontMipmap|kTextureDontFilterMipmap;
    if(extensions["GL_EXT_texture_filter_anisotropic"]){
        GLfloat v=0;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &v);
        if(v>=8.)texture_mask|=kTextureAnsiotropic8x;
        else if(v>=4.)texture_mask|=kTextureAnsiotropic4x;
        else if(v>=2.)texture_mask|=kTextureAnsiotropic2x;
    }

#ifdef USE_OPENGLES
    if(extensions["GL_OES_rgb8_rgba8"])
    {
        add_format(kTextureRGBA8|texture_mask, GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE);
        add_format(kTextureRGB8|texture_mask, GL_RGB, GL_RGB,GL_UNSIGNED_BYTE);
    }
    if(extensions["GL_OES_depth_texture"])
        add_format(kTextureDepth16|texture_mask, GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT16, GL_UNSIGNED_SHORT);

    if(extensions["GL_OES_depth24"])
        add_format(kTextureDepth24|texture_mask, GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT24_OES, GL_UNSIGNED_INT);
    if(extensions["GL_OES_texture_float"])
    {
        add_format(kTextureRGBF32|texture_mask, GL_RGB,GL_RGB, GL_FLOAT);
        add_format(kTextureRGBAF32|texture_mask, GL_RGBA,GL_RGBA, GL_FLOAT);
    }
    if(extensions["GL_OES_texture_half_float"])
    {

        add_format(kTextureRGBF16|texture_mask, GL_RGB,GL_RGB, GL_HALF_FLOAT_OES);
        add_format(kTextureRGBAF16|texture_mask, GL_RGBA, GL_RGBA,GL_HALF_FLOAT_OES);
    }
#else
    add_format(kTextureRGBA8|texture_mask, GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE);
    add_format(kTextureRGB8|texture_mask, GL_RGB, GL_RGB,GL_UNSIGNED_BYTE);
    add_format(kTextureRGBF32|texture_mask, GL_RGB32F_ARB,GL_RGB, GL_FLOAT);
    add_format(kTextureRGBAF32|texture_mask, GL_RGBA32F_ARB,GL_RGBA, GL_FLOAT);
    add_format(kTextureRGBF16|texture_mask, GL_RGB16F_ARB,GL_RGB, GL_FLOAT);
    add_format(kTextureRGBAF16|texture_mask, GL_RGBA16F_ARB, GL_RGBA,GL_FLOAT);
    add_format(kTextureDepth24|texture_mask, GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT, GL_FLOAT);
    add_format(kTextureDepth16|texture_mask, GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT, GL_FLOAT);

#endif
    add_format(kTextureRGB565|texture_mask, GL_RGB, GL_RGB,GL_UNSIGNED_SHORT_5_6_5);
    add_format(kTextureRGBA4|texture_mask, GL_RGBA, GL_RGBA,GL_UNSIGNED_SHORT_4_4_4_4);
    add_format(kTextureRGBA5551|texture_mask, GL_RGBA, GL_RGBA,GL_UNSIGNED_SHORT_5_5_5_1);



}
void OpenGLES::add_format(unsigned int base_texture_format, GLenum internal_format, GLenum format, GLenum type)
{
    RealTexFormat tex;
    tex.texture_format=base_texture_format;

    tex.format=format;
    tex.internal_format=internal_format;
    tex.type=type;

    tex_formats.push_back(tex);

}
GLenum blend_converter(unsigned int b)
{
    switch (b&0xF) {
    case kZero&0xF:                 return GL_ZERO;
    case kOne&0xF:                  return GL_ONE;
    case kDstColor&0xF:             return GL_DST_COLOR;
    case kDstAlpha&0xF:             return GL_DST_ALPHA;
    case kOneMinusDstColor&0xF:     return GL_ONE_MINUS_DST_COLOR;
    case kOneMinusDstAlpha&0xF:     return GL_ONE_MINUS_DST_ALPHA;
    case kSrcColor&0xF:             return GL_SRC_COLOR;
    case kSrcAlpha&0xF:             return GL_SRC_ALPHA;
    case kOneMinusSrcColor&0xF:     return GL_ONE_MINUS_SRC_COLOR;
    case kOneMinusSrcAlpha&0xF:     return GL_ONE_MINUS_SRC_ALPHA;

    case kConstantColor&0xF:        return GL_CONSTANT_COLOR;
    case kOneMinusConstantColor&0xF:return GL_ONE_MINUS_CONSTANT_COLOR;
    case kConstantAlpha&0xF:        return GL_CONSTANT_ALPHA;
    case kOneMinusConstantAlpha&0xF:return GL_ONE_MINUS_CONSTANT_ALPHA;

    case kSrcAlphaSaturate&0xF:     return GL_SRC_ALPHA_SATURATE;
    default:                    return GL_ZERO;
    }
}
void OpenGLES::set_cull_face(int face){
    if (current_cull_face!=face) {
        current_cull_face=face;
        switch(face)
        {
        case Argon::kCullNone:glDisable(GL_CULL_FACE);break;
        case Argon::kCullBackFaceCounterClockWise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            break;
        case Argon::kCullBackFaceClockWise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);
            break;
        case Argon::kCullFrontFaceCounterClockWise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glFrontFace(GL_CCW);

            break;
        case Argon::kCullFrontFaceClockWise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glFrontFace(GL_CW);
            break;
        }
    }
}
void OpenGLES::set_blend(unsigned int blend){
    if (current_blend!=blend) {
        if(blend==kBlendReplace){
            glDisable(GL_BLEND);
            glBlendFuncSeparate(blend_converter(blend>>kSrcColorOffset),
                                blend_converter(blend>>kDstColorOffset),
                                blend_converter(blend>>kSrcAlphaOffset),
                                blend_converter(blend>>kDstAlphaOffset));
        }else{
            if(current_blend==kBlendReplace) glEnable(GL_BLEND);
            glBlendFuncSeparate(blend_converter(blend>>kSrcColorOffset),
                                blend_converter(blend>>kDstColorOffset),
                                blend_converter(blend>>kSrcAlphaOffset),
                                blend_converter(blend>>kDstAlphaOffset));
        }
        current_blend=blend;

    }
}

void OpenGLES::cache_texture(VirtualResource tex){
    VirtualResourceImage* image = tex.get_data<VirtualResourceImage*>();
    if(image){
        TexturePrim & p =textures[tex];
        p.last_frame=0;
        bool realloc = p.width!=image->get_width()||p.height!=image->get_height();
        if(realloc){
            RealTexFormat format =get_closest_format(image->get_format());
            p.alloc_texture(image->get_width(), image->get_height(), format);
            if(bound_textures.size())
                glBindTexture(GL_TEXTURE_2D, bound_textures[0]);
        }

        if(p.update_id!=image->update_id()||realloc){

            p.upload_texture_data(image->get_width(), image->get_height(), (void*)image->get_image_data());
            p.update_id=image->update_id();
            if(bound_textures.size())
                glBindTexture(GL_TEXTURE_2D, bound_textures[0]);
        }
    }else{
        std::cout<<tex.get_path_string()<<" is not a valid texture.\n";
    }
}

void OpenGLES::cache_material(Material& state, const VirtualResource&  shader){
    GL_CHECK("BEFORE CACHE MATERIAL");

    GL_CHECK("After CACHE MATERIAL");

}
#define SET_UNIFORM(A,B,C){\
    bool cont=true;\
    for(int x=0;x<size;++x){\
    auto it2 = all_uniforms[x]->A.find(it->first);\
    if(it2!= all_uniforms[x]->A.end()){\
    cont=false;\
    B (it->second.uniform,(GLsizei)std::min(it2->second.size(),it->second.size),(C*)&(it2->second[0]));\
    break;\
}\
}\
    if(cont)\
    std::cout<<"No value was set for uniform: "<<it->first<<" of type: "<<#A<<" and size "<<it->second.size<<"\n";\
}
#define SET_UNIFORM_MAT(A,B,C){\
    bool cont =true;\
    for(int x=0;x<size;++x){\
    auto it2 = all_uniforms[x]->A.find(it->first);\
    if(it2!= all_uniforms[x]->A.end()){\
    cont=false;\
    B (it->second.uniform,(GLsizei)std::min(it2->second.size(),it->second.size),false,(C*)&(it2->second[0]));\
    break;\
}\
}\
    if(cont)std::cout<<"No value was set for uniform: "<<it->first<<" of type: "<<#A<<" and size "<<it->second.size<<"\n";\
}

#define SET_UNIFORM_COPY(A,B,C,D){\
    bool cont =true;\
    for(int x=0;x<size;++x){\
    auto it2 = all_uniforms[x]->A.find(it->first);\
    if(it2!= all_uniforms[x]->A.end()){\
    cont=false;\
    std::vector<B> temp;\
    temp.reserve(it2->second.size());\
    for(auto &i :it2->second)\
    temp.push_back(i.cast<D>());\
    C (it->second.uniform,(GLsizei)std::min(temp.size(),it->second.size),(D*)&(temp[0]));\
    break;\
}\
}\
    if(cont)std::cout<<"No value was set for uniform: "<<it->first<<" of type: "<<#A<<" and size "<<it->second.size<<"\n";\
}
#define SET_UNIFORM_COPY2(A,B,C,D){\
    bool cont =true;\
    for(int x=0;x<size;++x){\
    auto it2 = all_uniforms[x]->A.find(it->first);\
    if(it2!= all_uniforms[x]->A.end()){\
    cont=false;\
    std::vector<B> temp(it2->second.begin(),it2->second.end());\
    temp.reserve(it2->second.size());\
    C (it->second.uniform,(GLsizei)std::min(temp.size(),it->second.size),(D*)&(temp[0]));\
    break;\
}\
}\
if(cont)std::cout<<"No value was set for uniform: "<<it->first<<" of type: "<<#A<<" and size "<<it->second.size<<"\n";\
}
void OpenGLES::set_uniforms(Uniforms ** all_uniforms, int size){
    typedef VectorBase<GLint, 2> gliv2;
    typedef VectorBase<GLint, 3> gliv3;
    typedef VectorBase<GLint, 4> gliv4;

    typedef VectorBase<GLuint, 2> gluiv2;
    typedef VectorBase<GLuint, 3> gluiv3;
    typedef VectorBase<GLuint, 4> gluiv4;

    typedef VectorBase<GLboolean, 2> glbv2;
    typedef VectorBase<GLboolean, 3> glbv3;

    typedef VectorBase<GLboolean, 4> glbv4;
    auto it = current_shader->uniforms.begin();
    int tex_index=0;
    while (it!=current_shader->uniforms.end()) {
        switch (it->second.type) {
        case 0: ++it; continue; break;
        case GL_SAMPLER_2D:
        {
            bool cont =true;
            for(int x=0;x<size;++x){
                auto it2 = all_uniforms[x]->textures.find(it->first);
                if(it2!= all_uniforms[x]->textures.end()){
                    cont=false;
                    for(int s=0;s<std::min(it->second.size,it2->second.size());++s){
                        if(bound_textures.size()<=tex_index){
                            bound_textures.resize(tex_index+1);
                        }
                        cache_texture(it2->second[s]);
                        if(bound_textures[tex_index]!=textures[it2->second[s]].tex){
                            bound_textures[tex_index]=textures[it2->second[s]].tex;

                            //if(active_texture!=GL_TEXTURE0+tex_index){
                            glActiveTexture(GL_TEXTURE0+tex_index);
                            active_texture=GL_TEXTURE0+tex_index;
                            //}
                            glBindTexture(GL_TEXTURE_2D, textures[it2->second[s]].tex);

                            glUniform1i(it->second.uniform+s, tex_index);
                            ++tex_index;
                        }
                    }

                }
            }
            if(cont)std::cout<<"No value was set for uniform: "<<it->first<<" of type: texture and size "<<it->second.size<<"\n";\

        }

            break;
        case GL_INT: SET_UNIFORM_COPY2(f, GLint, glUniform1iv,GLint) break;
        case GL_INT_VEC2: SET_UNIFORM_COPY(f2, gliv2, glUniform2iv, GLint) break;
        case GL_INT_VEC3: SET_UNIFORM_COPY(f3, gliv3, glUniform1iv,GLint) break;
        case GL_INT_VEC4: SET_UNIFORM_COPY(f4, gliv4, glUniform1iv,GLint) break;
#ifndef USE_OPENGLES
        case GL_BOOL:
        case GL_UNSIGNED_INT: SET_UNIFORM_COPY2(f, GLuint, glUniform1uiv,GLuint) break;
        case GL_BOOL_VEC2:
        case GL_UNSIGNED_INT_VEC2: SET_UNIFORM_COPY(f2, gluiv2, glUniform2uiv,GLuint) break;
        case GL_BOOL_VEC3:
        case GL_UNSIGNED_INT_VEC3: SET_UNIFORM_COPY(f3, gluiv3, glUniform3uiv,GLuint) break;
        case GL_BOOL_VEC4:
        case GL_UNSIGNED_INT_VEC4: SET_UNIFORM_COPY(f4, gluiv4, glUniform4uiv,GLuint) break;
#endif
        case GL_FLOAT:SET_UNIFORM(f,glUniform1fv,GLfloat);break;
        case GL_FLOAT_VEC2:SET_UNIFORM(f2,glUniform2fv,GLfloat);break;
        case GL_FLOAT_VEC3:SET_UNIFORM(f3,glUniform3fv,GLfloat);break;
        case GL_FLOAT_VEC4:SET_UNIFORM(f4,glUniform4fv,GLfloat);break;
        case GL_FLOAT_MAT2:SET_UNIFORM_MAT(mat2,glUniformMatrix2fv,GLfloat);break;
        case GL_FLOAT_MAT3:SET_UNIFORM_MAT(mat3,glUniformMatrix3fv,GLfloat);break;
        case GL_FLOAT_MAT4:SET_UNIFORM_MAT(mat4,glUniformMatrix4fv,GLfloat);break;
        default: std::cout<<std::hex<<"Shader uses unknown uniform type: "<<it->second.type<<"\n"<<std::dec;break;


        }

        GL_CHECK("AFTER SET MAT uniform");
        ++it;
    }

}
void OpenGLES::set_shader(Argon::Renderable*state, VirtualResource& shader,Uniforms** uniforms, int size)
{

    if(!state->material)return;
    shader_data &s = shaders[shader];
    s.last_frame=0;
    if(!s.program)s.program=make_shader(shader);
    if(current_shader!=&s){glUseProgram(s.program);        current_vert_array=nullptr;}
    current_shader=&s;

    GL_CHECK("AFTER GL USE PROGRAM");

    set_uniforms(uniforms,size);



}
bool OpenGLES::link_shader(GLuint shader)
{
    GLint status;
    glLinkProgram(shader);

    GLint logLength;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(shader, logLength, &logLength, log);
        printf("Program link log:\n%s", log);

        free(log);
    }

    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    if (status == 0) printf("Shader compilation failed\n");

    return status;
}
bool OpenGLES::compile_shader(GLuint &shader, GLenum type, const std::string& source){

    const GLchar* s = (const GLchar*)source.c_str();
    glShaderSource(shader, 1, &s, NULL);
    glCompileShader(shader);
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);

        printf("Shader compile log:\n%s", log);
        free(log);
    }
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) glDeleteShader(shader);

    return status;
}
void OpenGLES::cache_array(std::shared_ptr<VertexArray> array){
    vert_data &d=vertex_arrays[array];
    d.last_frame=0;
    if(d.vert_buffer==0){
        glGenBuffers(1, &d.vert_buffer);
        glGenBuffers(1, &d.index_buffer);
        d.buff_size=0;
        d.update_id = array->update_id-1;
    }
    if(d.update_id!=array->update_id){
        if(array->vertex_count()&&array->index_data.size()){
            GL_CHECK("Before Vertex Buffer Update\n");

            GLenum g = array->updates_frequently?GL_STREAM_DRAW:GL_STATIC_DRAW;
            d.buff_size=array->vertex_count()*array->stride;
            d.vertices = array->vertex_count()-1;
            d.index_size=array->index_data.size()*2;
            d.update_id=array->update_id;

            glBindBuffer(GL_ARRAY_BUFFER, d.vert_buffer);
            glBufferData(GL_ARRAY_BUFFER, d.buff_size, array->data_start(), g);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.index_buffer);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.index_size, &array->index_data[0], g);

        }
    }

}



int OpenGLES::make_shader(VirtualResource& shader){

    shader_data &s =shaders[shader];
    if(s.program)return s.program;
    current_shader=NULL;


    s.program= glCreateProgram();

    std::string shader_string;
    std::string prefix_string="#version 120\n";
#ifndef USE_OPENGLES
    prefix_string+= "#define lowp  \n#define mediump \n #define highp \n";
#endif
    prefix_string+= "#line 0\n";
    shader_string = shader.get_data_as_string();

    //std::cout<<"----BEGIN---\n"<<shad_da<<"\n";
    std::string vert_sh = prefix_string+"#define VERTEX_SHADER 1\n"+shader_string;
    std::string frag_sh = prefix_string+"#define FRAGMENT_SHADER 1\n"+shader_string;
    //std::cout<<"----END---\n";

    GLuint v=glCreateShader(GL_VERTEX_SHADER);
    GLuint f=glCreateShader(GL_FRAGMENT_SHADER);

    bool success=compile_shader(v, GL_VERTEX_SHADER, vert_sh) && compile_shader(f, GL_FRAGMENT_SHADER, frag_sh);

    if(!success)
        std::cout<<"Failed to compile program:"<<shader.get_path_string()<<"\n";

    glAttachShader(s.program, v);
    glAttachShader(s.program, f);

    success = link_shader(s.program);
    if (!success)
        std::cout<< "Failed to link program: "<<s.program<<"\n";

    glGetProgramiv(s.program,GL_ACTIVE_ATTRIBUTES,&s.active_attribs);
    for(GLuint i=0;i<s.active_attribs;++i){
        GLchar buffer[100];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(s.program,i,100,&length,&size,&type,buffer);
        s.attributes[buffer]=glGetAttribLocation(s.program,buffer);
    }
    glGetProgramiv(s.program,GL_ACTIVE_UNIFORMS,&s.active_uniforms);
    for(GLuint i=0;i<s.active_uniforms;++i){
        GLchar buffer[100];
        GLsizei length;
        GLint size;
        GLenum type=0;
        glGetActiveUniform(s.program,i,100,&length,&size,&type,buffer);
        for(int x=0;x<100;++x){
            if(buffer[x]=='[')buffer[x]='\0';
        }
        Uniform & u = s.uniforms[buffer];
        u.uniform=glGetUniformLocation(s.program, buffer);
        u.size=size;
        u.type=type;
        u.pointer=NULL;
        u.frame=current_frame-1;
    }
    if(v)glDetachShader(s.program, v);
    if(f)glDetachShader(s.program, f);
    if(f)glDeleteShader(f);
    if(v)glDeleteShader(v);
    if(!success)glDeleteProgram(s.program);
    return success?s.program:0;
}

void OpenGLES::update_resources(){
    if(!initialized){

        init_fbos();
        init_extensions();
        glDisable(GL_SCISSOR_TEST);
        init_texture_formats();
#ifndef USE_OPENGLES
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &main_frame_buffer);
#endif
#ifdef USE_OPENGLES
        glGetIntegerv(GL_FRAMEBUFFER_BINDING,&main_frame_buffer);
#endif
    }
    std::map<VirtualResource, TexturePrim>::iterator it = textures.begin();
    while(it!=textures.end()){
        it->second.last_frame++;
        if(it->second.last_frame>300){
            it= textures.erase(it);
        }else ++it;
    }


    std::map<std::shared_ptr<VertexArray>,vert_data>::iterator it2 = vertex_arrays.begin();
    while(it2!=vertex_arrays.end()){
        it2->second.last_frame++;
        if(it2->second.last_frame>30){
            glDeleteBuffers(1,&it2->second.index_buffer);
            glDeleteBuffers(1,&it2->second.vert_buffer);
            std::cout<<"DELETE\n";
            it2= vertex_arrays.erase(it2);
        }else ++it2;
    }
    std::map<VirtualResource, shader_data>::iterator it3 = shaders.begin();
    while(it3!=shaders.end()){
        it3->second.last_frame++;
        if(it3->second.last_frame>300){
            glDeleteProgram(it3->second.program);
            it3= shaders.erase(it3);
        }else ++it3;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);

    bound_textures.clear();
    initialized=true;

    current_frame++;
    current_render_flags=0;
    current_cull_face=kCullNotSet;
    current_vert_array=nullptr;
    current_shader=NULL;
    current_clear_color=Vector4f(0, 0, 0, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glDisableVertexAttribArray(7);

}
};
#endif //USE_OPENGL
