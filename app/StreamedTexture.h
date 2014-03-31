
#ifndef __STREAM_TEXTURE_TEST__
#define __STREAM_TEXTURE_TEST__

#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Utility.h>
#include <ArgonEngine/Sprite.h>
#include <iostream>
#include "StreamedTexture2.h"

struct StreamTextureTest : public Argon::Node {
    Argon::Sprite sprite;
    Argon::Input input;
    Argon::VirtualResource texture;
    Argon::VirtualResourceImage *image;
    int up_count;

    int xr,xg,xb;
    int resolution;
    StreamTextureTest(){
        name="StreamedTexture (Game of Life)";
        resolution=100;
        texture = "virtual://stream1/render_image1.png{w=h=100; f=RGBA8; mip=0; filter=0;}";
        image = texture.get_data<Argon::VirtualResourceImage*>();
        sprite.texture=texture;
        sprite.dimensions.setConstant(20);
        xr=xg=xb=64*4;
        for(int i=0;i<resolution*resolution*4;i+=4){
            image->set_pixel_color(i%resolution, i/resolution,
            Argon::Vector4f((rand()%100>75),(rand()%100>75),(rand()%100>75),1.));
        }
    }
    bool should_live(int y, int x, int c){
        int xc1=x;
        int xc2=x+1;
        int xc3=x-1;

        int yc1=y;
        int yc2=y+1;
        int yc3=y-1;
        int was_alive=image->get_pixel_color(xc1, yc1)[c]!=0;

        int alive=0;
        alive+=yc2<resolution&&image->get_pixel_color(xc1, yc2)[c]!=0;
        alive+=yc3>=0&&image->get_pixel_color(xc1, yc3)[c]!=0;
        if(xc2<resolution){
            alive+=image->get_pixel_color(xc2, yc1)[c]!=0;
            alive+=yc2<resolution&&image->get_pixel_color(xc2, yc2)[c]!=0;
            alive+=yc3>=0&&image->get_pixel_color(xc2, yc3)[c]!=0;
        }
        if(xc3>=0){
            alive+=image->get_pixel_color(xc3, yc1)[c]!=0;
            alive+=yc2<resolution&&image->get_pixel_color(xc3, yc2)[c]!=0;
            alive+=yc3>=0&&image->get_pixel_color(xc3, yc3)[c]!=0;
        }
        return (alive==2&&was_alive)||alive==3;

    }
    std::shared_ptr<Node> animate(float time){

        up_count++;

        Argon::Vector4f pos(input[Argon::kInputIDMouseX].value,
                     input[Argon::kInputIDMouseY].value,0.,1.);

        int half_r = resolution*0.5;

        int x = pos[0]*half_r-half_r;
        int y = pos[1]*-half_r+half_r;
        int num = (x+y*resolution);
        if(input[Argon::kInputIDMouseLeft].value==0)num=-1;
        if(!(up_count%4)&&!input['p'].value)
            for(int i=0;i<resolution*resolution;i++){
                image->set_pixel_color(i%resolution, i/resolution, Argon::Vector4f(
                should_live(i%resolution,i/resolution,2),
                should_live(i%resolution,i/resolution,1),
                should_live(i%resolution,i/resolution,0),1.));

            }
        if(num>0&&num<resolution*resolution)
                image->set_pixel_color(num%resolution, num/resolution,Argon::kWhiteColor);

        image->update_id_++;
        return input[' '].value?std::make_shared<StreamTextureTest2>():nullptr;
    }

};
#endif
