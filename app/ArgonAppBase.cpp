
#include "ArgonAppBase.h"
#include "BasicShapeTest.h"

ArgonAppBase::ArgonAppBase(){
    total_time=0;
    model.load_file("resource://snake.ahf");
    model.create_objects();
    name="ArgonAppBase";
    //model.enable_bounds_renderering_recursive(Argon::kBlueColor);
    light=model.spawned_nodes["Lamp.001"];
    light->parent=this;

    //model.get_file().write("user://snake.ahf");


    //light->visit(read);

    //light->write("user://light.json");
    //light->read("user://light.ahf");


}


std::shared_ptr<Argon::Node> ArgonAppBase::animate(float time){

    //Variable to store elapsed time, it will suffer some precision issues, but its fine for its purpose here.'


    total_time+=time;

    Argon::Vector3f pos(input[Argon::kInputIDMouseX].value,input[Argon::kInputIDMouseY].value,(input[Argon::kInputIDUp].integral-
            input[Argon::kInputIDDown].integral)*0.1);

    light->position=screen_to_local(pos);
    if(input[' '].value)return std::make_shared<BasicShapeTest>();
    return nullptr;

} // animate
