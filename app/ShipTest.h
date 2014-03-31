//Generated by the Argon Build System


#ifndef __SHIP_TEST__
#define __SHIP_TEST__

#include <ArgonEngine/MeshFormat.h>
#include <ArgonEngine/Node.h>
#include <ArgonEngine/Hardware.h>
#include <ArgonEngine/Sprite.h>
#include <iostream>
#include "SpriteTest.h"

struct ShipTest : public Argon::Node {
    Argon::MeshFormat ship;
    Argon::Input input;
    ShipTest(){
        name="ShipTest";
        ship.load_file("resource://ship.ahf");
        ship.create_objects();
    }
    std::shared_ptr<Node> animate(float time){
        return input[' '].value?std::make_shared<SpriteTest>():nullptr;
    }

};
#endif