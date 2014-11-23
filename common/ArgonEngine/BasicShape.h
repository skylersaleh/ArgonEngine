/**
 * @brief This provides a node that renders some basic shapes
 * @file BasicShapes.h
 * @author Skyler Saleh, Ian Wiggins
 **/
#ifndef BasicShapes_h
#define BasicShapes_h
#include "Node.h"
#include "RenderSystem.h"
#include "MeshFormat.h"

namespace Argon
{
    //! @brief A basic shape class.
    struct BasicShape : public Node
    {
        std::shared_ptr<MeshFormat> model;
        std::shared_ptr<Node> primitive;
        BasicShape();
        void set_box();
        void set_sphere();
        void set_torus();
        void set_cylinder();
        void set_plane();
        void set_ring();
        ~BasicShape();
        MAKE_VISIT_HEAD(BasicShape);
    };
    struct BoundsRenderer:public InstanceList<BoundsRenderer>, public ReflectionBase{
        Node* parent=NULL;
        Vector4f color;
        MAKE_VISIT_HEAD(BoundsRenderer)
    };

    struct BoundingCubeNode: public Renderable{

        void set(const BoundingCube&c);
        BoundingCubeNode(){initialize();}
        void initialize();
        MAKE_VISIT_HEAD(BoundingCubeNode)
    };

};

#endif
