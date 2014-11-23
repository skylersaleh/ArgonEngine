/**
 * @brief Utilities for rendering text.
 * @file Text.h
 * @author Skyler Saleh
 **/

#ifndef __ArgonTest__Text__
#define __ArgonTest__Text__
#include <ArgonEngine/Utility.h>
#include "ArgonEngine/VirtualResourceImage.h"
#include "ArgonEngine/Node.h"
#include <iostream>

namespace Argon{
    enum LineStyle{
        kLineLeft,
        kLineCenter,
        kLineRight,
    };


    const int kGlyphTextureSize = 1024;

    const int kGlyphWidth = 64;
    const int kGlyphHeight =64;
    const int kGlyphRenderRes = 220;
    const float kGlyphRFactor = kGlyphRenderRes/float(kGlyphHeight);

    const int kGlyphCacheSize = (kGlyphTextureSize/kGlyphWidth)*(kGlyphTextureSize/kGlyphHeight)*4;

    const int kGlyphCacheBorder = 16;



    struct Layout{
        float line_height;
        float line_width;
        float height;
        LineStyle style;
    };
    struct GlyphStyle{
        float skew=0.;
        float weight=1.;
        float scale=1.;
        float subpixel=1.;
        float outline_width=0.;
        float v_align=0.;
        Vector4f fill_color=kWhiteColor;
        Vector4f outline_color=kClearColor;
    };


    struct Glyph{
        int glyph;
        float tex_x;
        float tex_y;

        float width_ratio;
        float height_ratio;

        float xmin;
        float xmax;
        float ymin;
        float ymax;


        Argon::Vector4f map_color;
        Argon::VirtualResource font;
    };


    struct GlyphCache{
        Argon::VirtualResource texture;
        Argon::VirtualResourceImage *image;
        static GlyphCache & get_cache(){
            static GlyphCache cache;
            return cache;
        }
        struct GlyphKey{
            Argon::VirtualResource resource;
            int glyph;
            bool operator !=(const GlyphKey& k)const{return resource!=k.resource||glyph!=k.glyph;}
            bool operator >(const GlyphKey& k)const{return resource!=k.resource?resource>k.resource:glyph>k.glyph;}
            bool operator <(const GlyphKey& k)const{return resource!=k.resource?resource<k.resource:glyph<k.glyph;}
            bool operator >=(const GlyphKey& k)const{return resource!=k.resource?resource>k.resource:glyph>=k.glyph;}
            bool operator <=(const GlyphKey& k)const{return resource!=k.resource?resource<k.resource:glyph<=k.glyph;}
            bool operator ==(const GlyphKey& k)const{return resource==k.resource&&glyph==k.glyph;}
        };
        Argon::PriorityCache<Glyph, GlyphKey, kGlyphCacheSize> glyphs;

        GlyphCache();
        inline void set_val(const int x,int y, unsigned char* array);
        unsigned char interpolate(float x, float y, unsigned char *bu);
        void build_distance_field(uint8_t * bu,int pos);
        Glyph* get_glyph(Argon::VirtualResource font, int glyph);
    };
    struct Font:public Argon::VirtualResourceIMPL::Data{
        Font(){}
        virtual void get_font_size_ratio(float &w_r, float & h_r)=0;
        virtual void get_glyph_size_ratio(int glyph, float &w_r, float & h_r)=0;

        virtual void get_glyph_size(int glyph, Glyph &g)=0;

        virtual float get_advance(int last_glyph,int glyph)=0;
        virtual void get_glyph_bitmap(int glyph, uint8_t* d, int w, int h,int border=0)=0;

        virtual bool reload(Argon::VirtualResourceIMPL::Source* s)=0;

        virtual Data* clone_type(const std::string& arguments)const=0;
        virtual bool loaded()=0;
        virtual bool save(Argon::VirtualResourceIMPL::Source* s){return false;}
        virtual size_t update_id(){return 0;}

    };

    struct Label: public Renderable{
        Argon::Vector2f glpyh_scale;
        Label();
        void render_str(const char* string,Argon::VirtualResource font,LineStyle s = kLineLeft);
        MAKE_VISIT_HEAD(Label)
    };


};

#endif /* defined(__ArgonTest__Text__) */
