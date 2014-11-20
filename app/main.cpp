
#include "ArgonAppFlowControl.h"
#include <ArgonEngine/ArgonInit.h>
#include <SDL2/SDL.h>
ArgonAppFlowControl*controller;
void manual_redraw(){
    if(controller->draw())Argon::swap_buffers();
}
int main(int argc, char** argv){
    Argon::initialize_engine("ArgonSoftware","ArgonApp");
    bool& run =Argon::Run_Engine;
    Argon::set_manual_redraw(manual_redraw);
    controller = new ArgonAppFlowControl();
    while(run){
        run = Argon::poll_events();
        if(controller->draw())Argon::swap_buffers();
    }
    return 0;
}
