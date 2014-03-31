//Generated by the Argon Build System

//
//  NeonRushFlowControl.cpp
//  Neon Rush
//
//  Created by Skyler Saleh on 6/23/12.
//  Copyright (c) 2012 Argon Software. All rights reserved.
//

#include "ArgonAppFlowControl.h"
#include "ArgonEngine/Matrix.h"
#include "ArgonEngine/Quaternion.h"
void ArgonAppFlowControl::animate(){
    //Get the delta timestep from the OS and do a sanity check on the result
    float d_time=t.delta_time();
    static double last_t=0;
    if(d_time>0.3)d_time=0.2;
    static int fps_count=5;
    static int max_count=0;
    static float max_v=0;
    if(++fps_count>5){
        std::stringstream str;
        float time =float(t.time()-last_t);
        if(max_v<time){max_count=0;max_v=time;}
        float fps_full=fps_count/time;
        last_t=t.time();
        str<< "FPS: "<<int(fps_full*100+0.5f)/100.f<<" Max: "<<max_v;
        fps.render_str(str.str().c_str(), font);
        fps_count=0;
        str.str("");
        str<<"Test: "<<current_node->name<<"\n";
        test_name.render_str(str.str().c_str(), font,Argon::kLineRight);
        str.str("");
        str<<"Test: "<<current_node->name<<" | Argon Engine Test Scenes";
        Argon::Screen::title=str.str();
        if(++max_count>10)max_v=0;

    }

    Argon::Screen::full_screen = input['f'].value;
    fps.position[1]=-10;
    fps.position[0]=-10/Argon::Screen::ratio();
    fps.position[1]=-10+(peak1->peak);

    test_name.position[1]=peak2->peak;
    test_name.position[0]=10/Argon::Screen::ratio();
    left_source.position[0]=right_source.position[0]=input[Argon::kInputIDMouseX].value*3.0;
    left_source.position[1]=right_source.position[1]=input[Argon::kInputIDMouseY].value*3.0;
    left_source.position[2]=right_source.position[1]=(input[Argon::kInputIDUp].integral-input[Argon::kInputIDDown].integral)*3.0;
    right_source.position[0]+=2.;
    //Smooth the time step to avoid jerkiness in animations when the frame rate is erradic.

    smoothed_time=smoothed_time*0.9+d_time*0.1;

    //Create an animated quaternion to rotate the camera and look snazzy.
    static float x_old=0;
    static float y_old=0;
    Argon::Quaternionf q=Quaternionf::Identity();
    Vector3f view_delta = Vector3f(input[Argon::kInputIDMouseY].value*4.-y_old,
            input[Argon::kInputIDMouseX].value*-4.-x_old,
            0);

    Vector3f view_delta2 = Vector3f(input[Argon::kInputIDJoyY].integral*4.,
            input[Argon::kInputIDJoyX].integral*4.,
            (input[Argon::kInputIDJoyButton10].integral-input[Argon::kInputIDJoyButton11].integral)*3.);
    view_delta+=view_delta2;
    if(view_delta.norm()>0.01)
    q= AngleAxisf(view_delta.norm(),view_delta.normalized());
    Argon::Vector3f jp(input['a'].integral*15-input['t'].integral*15,
            input['1'].integral*15-
            input['2'].integral*15,
            input['w'].integral*15.-input['r'].integral*15);\
    Argon::Vector3f jp2(input[Argon::kInputIDJoyX2].integral*15,
            0.,
            -input[Argon::kInputIDJoyY2].integral*15);\
    jp+=jp2;
    x_old=input[Argon::kInputIDMouseX].value*-4.;
    y_old= input[Argon::kInputIDMouseY].value*4;
    input['w'].integral=
    input['a'].integral=
    input['r'].integral=
    input['t'].integral=
    input['1'].integral=
    input['2'].integral=
    input[Argon::kInputIDUp].integral=
    input[Argon::kInputIDDown].integral=
    input[Argon::kInputIDLeft].integral=
    input[Argon::kInputIDRight].integral=
    input[Argon::kInputIDJoyY2].integral=
    input[Argon::kInputIDJoyX2].integral=
    input[Argon::kInputIDJoyButton10].integral=
    input[Argon::kInputIDJoyButton11].integral=
    input[Argon::kInputIDJoyX].integral=
    input[Argon::kInputIDJoyY].integral=
    input[Argon::kInputIDJoyButton8].integral=
    input[Argon::kInputIDJoyButton9].integral=0;
    //Argon::Vector3f translate;
    translate+=q*jp;
    accum_rot*=q;
    transform_matrix=Translation3f(jp)*q*transform_matrix;

    //Create the transformation matrix for the camera
    camera.projection_matrix =Argon::PerspectiveMatrix<float>(20, 1./Argon::Screen::ratio(), 1, 100);

    camera.matrix =(transform_matrix).matrix();

    //camera.matrix = ident.matrix();
    //camera.projection_matrix = ident.matrix();
    //Make the camera active
    camera.become_main_camera();
    ui_camera.become_main_camera2D();
    ui_camera.projection_matrix =Argon::OrthoMatrix<float>(10./Argon::Screen::ratio(), -10./Argon::Screen::ratio(), 10, -10, -10, 10);
    ui_camera.matrix =Affine3f::Identity().matrix();

    //Handle flow control
    if(current_node)set_node(current_node->animate(smoothed_time));
}
