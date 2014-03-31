#ifndef __NORTON_CODES__
#define __NORTON_CODES__

#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Utility.h>
#include <ArgonEngine/Sprite.h>
#include <iostream>
#include "ArgonAppBase.h"
#include "RayTriangleTest.h"
#include <algorithm>

    struct NortonCodes : public Argon::Node {
        Argon::Sprite sprite;
        Argon::Input input;
        Argon::VirtualResourceImage *image;
        const int resolution=256;

        NortonCodes(){
            name="NortonCodes";
            sprite.texture="virtual://stream2/render_image3.tex{w=h=256; f=RGBA8; mip=0; filter=0;}";
            image = sprite.texture.get_data<Argon::VirtualResourceImage*>();
            sprite.dimensions=Vector3f(20,20.,0.);
        }

        std::shared_ptr<Node> animate(float time){
            std::vector<int> codes;
            codes.reserve(resolution*resolution);

            for(int y=0;y<resolution;++y)
            for(int x=0;x<resolution;++x){
                int v =0;
                int xt = x;
                int yt = y;

                for (int i=0; i<8; ++i) {
                    v<<=1;
                    v|=(xt>>(7-i))&1;
                    v<<=1;
                    v|=(yt>>(7-i))&1;
                }
                codes.push_back(v);
            }
            std::sort(codes.begin(),codes.end());
            for(size_t i=0;i<codes.size();++i){
                int v =codes[i];
                int xt = 0;
                int yt = 0;

                for (int i2=0; i2<8; ++i2) {
                    int mask = 1<<i2;
                    xt|= (v>>i2)&mask;
                    yt|= (v>>(i2+1))&mask;

                }
                Argon::Vector4f c(i/float(codes.size()),(int(i/float(codes.size())*256*256+0.5)&0xFF)/255., 0,  1.);
                image->set_pixel_color(xt, yt, c);
            }
            image->update_id_++;
            return input[' '].value?std::make_shared<RayTriangleTest>():nullptr;
        }
    };



#endif
