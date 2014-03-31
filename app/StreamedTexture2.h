

#ifndef __STREAM_TEXTURE_TEST2__
#define __STREAM_TEXTURE_TEST2__

#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Utility.h>
#include <ArgonEngine/Sprite.h>
#include <iostream>
#include "StreamedVertexArray.h"
#include <ArgonEngine/VirtualResourceImage.h>
struct StreamTextureTest2 : public Argon::Node {
    Argon::Sprite sprite;
    Argon::Input input;
    Argon::VirtualResource texture;
    Argon::VirtualResourceImage *image;
    float * array;
    float * array2;

    int up_count;
    float positionr;
    float velocityr;;
    float positiong;
    float velocityg;;
    float positionb;
    float velocityb;;
    int xr,xg,xb;
    int resolution;
    StreamTextureTest2(){
        name="StreamedTexture(Lighting Imposter)";
        resolution=100;
        texture="virtual://stream2/render_image1.tex{w=h=100; f=RGB565; mip=0; filter=0;}";
        image = texture.get_data<Argon::VirtualResourceImage*>();

        sprite.texture=texture;

        sprite.dimensions=Vector3f(20,20.,20.);
        positionr=positiong=positionb =0;
        velocityr=velocityg=velocityb=0;
        xr=xg=xb=64*4;
        array = new float[resolution*resolution*4];
        array2 = new float[resolution*resolution*4];

        for(int i=0;i<resolution*resolution*4;i++)
            array2[i]=array[i]=0.0;

    }

    void set(float * array, int x, int y, float v){
        float t =array[(x+y*resolution)*4];
        array[(x+y*resolution)*4]=array[(x+y*resolution)*4+1]*0.5;
        array[(x+y*resolution)*4+1]=t*0.5;

        t =array[(x+y*resolution)*4+3];
        array[(x+y*resolution)*4+3]=array[(x+y*resolution)*4+2]*0.5;
        array[(x+y*resolution)*4+2]=t*0.5;

    }
    std::shared_ptr<Node> animate(float dt){
        up_count++;

        Argon::Vector3f pos(input[Argon::kInputIDMouseX].value,
                            input[Argon::kInputIDMouseY].value,1.);


        int half_r = resolution*0.5;

        int x = pos[0]*half_r-half_r;
        int y = pos[1]*-half_r+half_r;
        int num = (x+y*resolution);
        for(int i=resolution+1;i<resolution*resolution-resolution-1;i++){
            float v = std::max(std::min(0.1f*(array2[i*4]+array2[i*4+1]+array2[i*4+2]+array2[i*4+3]),1.f),0.f);
            image->set_pixel_color(i%resolution, i/resolution, Argon::Vector4f(v,0,0,1.));

        }
        float factor = 0.98;
        float factor5 = 0.01;

        for(int x=1;x<resolution-1;++x){
            for(int y=1;y<resolution-1;++y){

                array[(x+y*resolution)*4]= std::max(array2[(x+1+y*resolution)*4],array2[(x+y*resolution)*4])*factor-factor5;
                array[(x+y*resolution)*4+1]= std::max(array2[(x-1+y*resolution)*4+1],array2[(x+y*resolution)*4+1])*factor-factor5;
                array[(x+y*resolution)*4+2]= std::max(array2[(x+y*resolution+resolution)*4+2],array2[(x+y*resolution)*4+2])*factor-factor5;
                array[(x+y*resolution)*4+3]= std::max(array2[(x+y*resolution-resolution)*4+3],array2[(x+y*resolution)*4+3])*factor-factor5;


            }
        }
        float factor2 = 0.1;
        float factor3 = 0.5;
        float factor3b = 1.-factor3;

        for(int x=1;x<resolution-1;++x){
            for(int y=1;y<resolution-1;++y){
                int s1 = x+y*resolution;


                float d1 = array[s1*4]*factor2;
                float d2 = array[s1*4+1]*factor2;
                float d3 = array[s1*4+2]*factor2;
                float d4 = array[s1*4+3]*factor2;

                array[s1*4]-=d1*2.;
                array[s1*4+1]-=d2*2.;
                array[s1*4+2]-=d3*2.;
                array[s1*4+3]-=d4*2.;
                //Right
                array[(x+y*resolution+resolution)*4]+=d1*factor3b;
                array[(x+y*resolution-resolution)*4]+=d1*factor3b;

                array[(x+y*resolution+resolution)*4+2]+=d1*factor3;
                array[(x+y*resolution-resolution)*4+3]+=d1*factor3;
                //left
                array[(x+y*resolution+resolution)*4+1]+=d2*factor3b;
                array[(x+y*resolution-resolution)*4+1]+=d2*factor3b;

                array[(x+y*resolution+resolution)*4+2]+=d2*factor3;
                array[(x+y*resolution-resolution)*4+3]+=d2*factor3;

                //Top
                array[(x+y*resolution+1)*4+2]+=d3*factor3b;
                array[(x+y*resolution-1)*4+2]+=d3*factor3b;

                array[(x+y*resolution+1)*4+0]+=d3*factor3;
                array[(x+y*resolution-1)*4+1]+=d3*factor3;

                //Bottom
                array[(x+y*resolution+1)*4+3]+=d4*factor3b;
                array[(x+y*resolution-1)*4+3]+=d4*factor3b;

                array[(x+y*resolution+1)*4+0]+=d4*factor3;
                array[(x+y*resolution-1)*4+1]+=d4*factor3;

            }
        }
        set(array,51,50,0);
        set(array,51,51,0);
        set(array,51,52,0);
        set(array,51,53,0);
        set(array,51,54,0);
        set(array,51,55,0);
        set(array,51,56,0);
        set(array,51,57,0);

        set(array,50,50,0);
        set(array,50,51,0);
        set(array,50,52,0);
        set(array,50,53,0);
        set(array,50,54,0);
        set(array,50,55,0);
        set(array,50,56,0);
        set(array,50,57,0);

        set(array,51,57,0);
        set(array,52,57,0);
        set(array,53,57,0);
        set(array,54,57,0);
        set(array,55,57,0);
        set(array,56,57,0);

        set(array,49,50,0);
        set(array,48,50,0);
        set(array,47,50,0);
        set(array,46,50,0);
        set(array,45,50,0);
        set(array,44,50,0);

        if(num>0&&num<resolution*resolution)array[num*4]=array[num*4+1]=array[num*4+2]=array[num*4+3]=20.;;
        for(int i=0;i<resolution*resolution*4;i++){
            array2[i]=std::max(array[i],0.f);
            array[i]=0.;
        }
        image->update_id_++;
        return input[' '].value?std::make_shared<StreamVertexArrayTest>():nullptr;
    }
};



#endif
