/**
 * @brief This provides a basic Sprite class.
 * @file Sprite.h
 * @author Skyler Saleh
 **/
#ifndef Sprite_h
#define Sprite_h
#include "Node.h"
#include "RenderSystem.h"

namespace Argon
{
//! @brief A basic sprite class.
//! @details A sprite is simply a textured rectangle on the screen. They are commonly used for the user interface,
//! bill-boards, or 2D elements.
struct Sprite : public Renderable
{
    VirtualResource & texture = uniform.get_texture("texture");
    Sprite();
    Sprite(const Sprite &s):Sprite(){operator =(s);}
    Sprite& operator = (const Sprite & s){
        Node::operator=(s);
        Renderable::operator =(s);
        return *this;
    }
    void set_shader(VirtualResource res){
        material=std::make_shared<Material>();
        material->shader=res;
    }
    MAKE_VISIT_HEAD(Sprite)

};

struct InstancedSprites:public Node{
    enum{
        kSpritesPerBatch=16384,
        kSpritesMask= kSpritesPerBatch-1,
        kVerticesPerBatch=kSpritesPerBatch*4
    };

    std::shared_ptr<Material> material = std::make_shared<Material>();
    VirtualResource &texture = material->uniform.get_texture("texture");
    VirtualResource &shader = material->shader;
    std::vector<std::shared_ptr<Renderable>> renders;
    std::vector<std::shared_ptr<VertexArray>> arrays;


    InstancedSprites(){
        shader="shader://instanced_sprite_shader.shd";
        texture = "resource://test.png";
        material->render_flags&=~(kRenderDepthMask);
    }
    InstancedSprites(const InstancedSprites&s):InstancedSprites(){operator=(s);}
    InstancedSprites &operator=(const InstancedSprites&s){
        renders=s.renders;
        arrays = s.arrays;
        return *this;
    }

    void set_sprites(size_t total_sprites);
    void set_sprite(size_t index,const Vector4f& color,const Vector3f& v0,const Vector3f& v1,const Vector3f& v2,const Vector3f& v3 ,const Vector2f &min_tex,const Vector2f& max_tex);
    void set_sprite(size_t index, const Vector4f& color,const Vector3f scale,const Vector3f position,const Quaternionf &rotation,Vector2f min_tex=Vector2f(0.f,0.f),Vector2f max_tex=Vector2f(1.f,1.f));


    void set_transform(size_t index,Node & transform,Vector2f min_tex=Vector2f(0,0), Vector2f max_tex=Vector2f(1,1));

};
};
#endif // ifndef Sprite_h
