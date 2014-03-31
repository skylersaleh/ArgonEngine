
#ifndef __FONT_TEST__
#define __FONT_TEST__

#include <ArgonEngine/VirtualResourceImage.h>
#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Text.h>
#include <iostream>
#include "NortonCodes.h"
#include <memory>
#include <sstream>
#include <ArgonEngine/Utility.h>
struct Stringer{
    std::stringstream str;
    operator std::string(){
        return str.str();
    }
};
template<typename T> Stringer& operator << (Stringer &s , const T& v){
    s.str<<v;
    return s;
}
struct FontTest : public Argon::Node {
    Argon::Input input;

    Argon::VirtualResource font;
    Argon::Label l_left;
    Argon::Label l_cent;
    Argon::Label l_right;
    Argon::Label battery_percent;
    Argon::Label battery_time;
    Argon::Label window_pos;

    float total_time =0;
    FontTest(){

        name="FontTest";
        font = "resource://times.ttf";
        window_pos.parent=battery_time.parent=battery_percent.parent=l_left.parent=l_cent.parent=l_right.parent=this;

        l_left.render_str("Left Aligned", font);
        l_left.position[1]=3;

        l_cent.render_str("Center Aligned", font,Argon::kLineCenter);
        l_cent.position[1]=0;

        l_right.render_str("Right Aligned", font,Argon::kLineRight);
        l_right.position[1]=2;
        window_pos.yPosition()=-1;
        l_left.parent=this;
        l_right.parent=this;
        l_cent.parent=this;
        window_pos.parent=this;



    }
    std::shared_ptr<Argon::Node> animate(float dt){
        std::stringstream str;
        Argon::Vector2f pos(input[Argon::kInputIDMouseX].value,input[Argon::kInputIDMouseY].value);
        if(l_cent.overlaps_screen_coord(pos)){
            l_cent.scale=Vector3f(2.,2,2);
        }else  l_cent.scale=Vector3f(1.,1,1);
        str<<"Battery has "<<input[Argon::kInputIDBatteryPercent].value<< " % remaining";

        battery_percent.render_str(str.str().c_str(), font,Argon::kLineCenter);
        battery_percent.yPosition()=5;

        str.str("");
        str<<"and "<<input[Argon::kInputIDMouseY]<< " secs remaining";
        battery_time.render_str(str.str().c_str(), font,Argon::kLineCenter);
        battery_time.yPosition()=4;

        str.str("");
        str<<"Window is at x:"<<Argon::Screen::position[0]<<" y:"<<Argon::Screen::position[1];
        window_pos.render_str(str.str().c_str(), font);

        total_time+=dt;

        scale.setConstant(input[Argon::kInputIDUp].integral-input[Argon::kInputIDDown].integral+1);
        return input[' '].value? std::make_shared<NortonCodes>():nullptr;
    }

};
#endif
