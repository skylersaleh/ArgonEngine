//Generated by the Argon Build System

/**
 * @brief This file declares all the classes used in the Render System.
 * @file RenderSystem.h
 * @author Skyler Saleh
 **/
#ifndef RenderSystem_h
#define RenderSystem_h
#include "BoundingCube.h"
#include "Matrix.h"
#include "TypeInfo.h"
#include <map>
#include <ArgonEngine/Utility.h>
#include "RenderSystemConstants.h"
#include "VirtualResource.h"
#include "VertexIterator.h"
#include <stddef.h>
#include <memory>
#include <array>
#include "Node.h"

namespace Argon {
   /**
    * @brief This class is used to implement a camera in a scene.
    * @details
    * Each active camera can have multiple render targets.
    * And each render target can only belong to one camera.
    * Every camera that has a render target that is accessed when rendering the main camera is rendered to.
    *
    * Regular Render to Texture functionality can be implemented by setting a cameras render
    * target to a Texture(.tex)
    *
    * While rendering with a specific renderer can be done by setting the target to framebuffer(.fbo)
    *
    * To render to the screen you use the Render Target "{Virtual}/main_render_target.tex"
    **/
    struct Camera: public ReflectionBase{
        //! The map used to map individual render targets to cameras.
        static std::map<VirtualResource, Camera*>& all_cameras(){
            static std::map<VirtualResource,Camera*> c;
            return c;
        }
        //! The transformation matrix of the camera.
        Matrix4f matrix;
        Matrix4f projection_matrix;
        Vector4f background_color=Argon::kBlackColor;

        Camera(){}
        //! Makes this camera the main one, that renders to the screen.
        void become_main_camera(){
            all_cameras()["virtual://main_render_target.tex"]=this;
        }
        //! Makes this camera the main one, that renders to the screen.
        void become_main_camera2D(){
            all_cameras()["virtual://2d_camera.tex"]=this;
        }
        Matrix4f inverse();
        //! Returns a pointer to the camera that is currently rendering to the screen.
        static Camera* get_main_camera(){return all_cameras()["virtual://main_render_target.tex"];}
        static Camera* get_main_camera2D(){return all_cameras()["virtual://2d_camera.tex"];}

        ~Camera();
        MAKE_VISIT_HEAD(Camera);
    };




    typedef StringIntern StringType;
    const static size_t kRenderTypeSize[] = {4,2,1,1};


    //!@brief converts a regular type to its render type.
    //!@tparam T The render type to convert.
    template<typename T> struct ToRenderType;
    template<> struct ToRenderType<float>{enum {type= kRenderTypeFloat}; };
    template<> struct ToRenderType<uint8_t>{enum {type= kRenderTypeUByte}; };
    template<> struct ToRenderType<int16_t>{enum {type= kRenderTypeShort}; };
    template<> struct ToRenderType<int8_t>{enum {type= kRenderTypeByte}; };
    //! @brief This class is used to represent a renderable mesh in the RenderSystem.
    struct VertexArray : public ReflectionBase
    {

        //! A tag used for debug purposes.
        StringIntern tag;
        //! A hint to the RenderSystem to optimize for frequent updates.
        bool updates_frequently;
        //! The raw vertex data in the array.
        std::vector<uint8_t> data;

        //! @brief The index data in the array.
        //! @details This is used to map vertex data to vertices, so vertices can be shared.
        std::vector<uint16_t> index_data;
        //! A vector containing all the attribute information.
        std::vector<VertexAttribPair> attributes;
        //! @brief Identifies the current version of the data.
        //! @details Increment this when the data in the array has been changed.
        int update_id;
        //! The draw mode to use.
        int draw_type;
        //! The stride between two vertices.
        ptrdiff_t stride;
        //! Recalculates the stride based on the added attributes.
        void recalc_stride();
        float prim_size=5;

        VertexArray();
        //! Returns true if attribute 's' is present in the vertex array.
        bool has_attribute(const StringIntern& s);
        //! An iterator to the start of the data of attribute 's'
        VertexIterator begin(const StringIntern& s);
        //! An iterator to the end of the data of attribute 's'
        VertexIterator end(const StringIntern& s){return begin(s) + vertex_count();}
        //! Returns a bounding cube that holds the bound of all vertices' data for attribute 's'.
        BoundingCube get_attribute_bounds(const StringIntern & s);
        BoundingCube get_attribute_bounds(const StringIntern & s) const {return const_cast<VertexArray*>(this)->get_attribute_bounds(s);}
        //! Returns the total number of vertices in the array.
        int vertex_count() const {if(stride) return data.size() / stride; else return 0;}
        //! Adds an attribute to the array. The array must be resized after this operation.
        template<typename T> void add_attribute(int components, const StringIntern& attrib){
            VertexAttribPair pair;
            pair.attribute =attrib;
            pair.components=components;
            pair.to_float=&VertexAttribPair::_to_float_<T>;
            pair.from_float=&VertexAttribPair::_from_float_<T>;
            pair.type=ToRenderType<T>::type;
            pair.offset=stride;
            attributes.push_back(pair);
            recalc_stride();

        }
        //! Resize the vertex array to contain 'size' vertices.
        void set_size(size_t size){data.resize(size * stride);}

        //! Returns a pointer to the start of the vertices data.
        const uint8_t * data_start() const {return &data[0];}

        //! Returns a pointer to the start of the vertices data.
        uint8_t * data_start(){return &data[0];}

        //! Returns a pointer to the start of the data of vertex 'x'.
        const uint8_t * vertex(size_t x) const {return &data[x * stride];}
        struct Buffer{
            const uint8_t *d;
            size_t size;
            bool operator< (const Buffer&b)const{
                for(size_t i=0;i<size;++i)if(d[i]!=b.d[i])return d[i]<b.d[i];
                return d[size-1]<b.d[size-1];
            }
        };
        void optimize_vertices();
        //! Returns a pointer to the start of the data of vertex 'x'.
        uint8_t * data_start(size_t x){return &data[x * stride];}
        //! Auto-generates an index array by numbering the vertices sequentially.
        void generate_indices();
        void generate_indices_quad();

        //! Append the data 'd' of size 'bytes_size' to the end of the array.
        void append_data(const uint8_t* d, int bytes_size);
        //! Sets the vertex array data by resizing the data to 'bytes_size' bytes, and copying the data from 'd'.
        void set_data(const uint8_t* d, int bytes_size);
        //! Returns the attrib_pair for attribute 's'.
        VertexAttribPair get_attribute(const StringIntern&s);
        MAKE_VISIT_HEAD(VertexArray);
    };

    //!@brief Represents a light in the render system.
    struct Light : public InstanceList<Light>,public Node{

        //! The lights falloff distance in meters. Larger values make the lights bounds larger.
        float distance=10;

        //! The strength of specular lighting from the light.
        float specular_power=1;
        //! The strength of diffuse lighting from the light.
        float diffuse_power=1;
        MAKE_VISIT_HEAD(Light)

    };


    struct Uniforms: ReflectionBase{
        std::map<StringIntern,std::vector<VirtualResource>> textures;
        std::map<StringIntern, std::vector<Matrix4f> > mat4;
        std::map<StringIntern, std::vector<Matrix3f> > mat3;
        std::map<StringIntern, std::vector<Matrix2f> > mat2;
        std::map<StringIntern, std::vector<Vector4f> > f4;
        std::map<StringIntern, std::vector<Vector3f> > f3;
        std::map<StringIntern, std::vector<Vector2f> > f2;
        std::map<StringIntern, std::vector<float> > f;

        VirtualResource & get_texture(const StringIntern &s){
            auto &it = textures[s];
            if(it.empty())it.push_back(VirtualResource());
            return it[0];
        }
        float & get_scalar(const StringIntern &s){
            auto &it = f[s];
            if(it.empty())it.push_back(0);
            return it[0];
        }

        Matrix4f & get_mat4(const StringIntern &s){
            auto &it = mat4[s];
            if(it.empty())it.push_back(Argon::Matrix4f());
            return it[0];
        }
        Matrix3f & get_mat3(const StringIntern &s){
            auto &it = mat3[s];
            if(it.empty())it.push_back(Matrix3f());
            return it[0];
        }
        Matrix2f & get_mat2(const StringIntern &s){
            auto &it = mat2[s];
            if(it.empty())it.push_back(Matrix2f());
            return it[0];
        }

        Vector4f & get_fvec4(const StringIntern &s){
            auto &it = f4[s];
            if(it.empty())it.push_back(Vector4f(0,0,0,0));
            return it[0];
        }
        Vector3f & get_fvec3(const StringIntern &s){
            auto &it = f3[s];
            if(it.empty())it.push_back(Vector3f(0,0,0));
            return it[0];
        }
        Vector2f & get_fvec2(const StringIntern &s){
            auto &it = f2[s];
            if(it.empty())it.push_back(Vector2f(0,0));
            return it[0];
        }
        MAKE_VISIT_HEAD(Uniforms)
    };

    //! @brief This class represents the material information when rendering.
    struct Material:public ReflectionBase{

        Uniforms uniform;
        //! @brief A map of the shaders used in for rendering.
        //! @details
        //! Different Renderers require different shaders to work. So check
        //! the documentation of your chosen renderer to determine what shaders
        //! you can provide.
        VirtualResource shader= "shader://fallback.shd";

        //! @brief The blending style to use. This is a bitmask of Argon::BlendFactor
        unsigned int blend= kBlendReplace;

        //! @brief a map of the uniforms used in the material.
        //! @details
        //! This allows your application to provide information to your shaders as
        //! uniform variables. These are used to control the appearance of objects.
        //! Read the shaders source to see what values can be modified.
        Vector4f &color=uniform.get_fvec4("color");

        //! A tag used for debugging
        StringIntern name="Unnamed Material";

        //! @brief The render flags to use when rendering. This is a bitfield of Argon::RenderFlags.
        //! @details
        //! This is bitwise AND'd with the Renderables render_flags, and controls things like masks.
        int render_flags=kRenderDefault;
        Material()=default;
        Material(const Material& m):Material(){operator=(m);}
        Material& operator = (const Material &m);
        bool is_transparent() const {return blend!=kBlendReplace;}
        MAKE_VISIT_HEAD(Material)
    };

    static std::shared_ptr<Material> DefaultMaterial(new Material);
    static std::shared_ptr<Material> DebugMaterial(){
        static std::shared_ptr<Material> m(new Material);
        m->shader="resource://debug.shd";
        return m;
    }
    struct Frustrum:public ReflectionBase{
        Frustrum(){}
        void extract_from_matrix(const Matrix4f &m);
        Frustrum(const Matrix4f & m){extract_from_matrix(m);}
        std::array<Vector3f, 6> planes;
        std::array<float, 6> distances;

        enum plane_mapping{
            kLeft,
            kRight,
            kTop,
            kBottom,
            kFar,
            kNear
        };
        enum collision_mapping{
            kInside,
            kCollides,
            kOutside
        };


        bool should_cull(const BoundingCube &c)const{ return false;if(c.size[0]==-1.)return false;
            return test_cube(c)==kOutside;}
        collision_mapping test_cube(const BoundingCube &c)const;
        MAKE_VISIT_HEAD(Frustrum)


    };

     /**
     * @brief This class is used to create an object that is to be rendered.
     * @details
     * The Renderable class is used to describe the rendering attributes of an object
     * in the ArgonEngine.
     *
     * Each Renderable serves a node to a Doubly Linked List, which is used to iterate through
     * all Renderables in a scene. This means that the act of having a Renderable present in memory
     * causes it render(as long as their is a renderer present).
     **/
    struct Renderable : public InstanceList<Renderable>, public Node
    {
        Uniforms uniform;
        Matrix4f& last_world_matrix=uniform.get_mat4("matrix");
        Matrix4f& last_normal_matrix=uniform.get_mat4("normal_matrix");
        Vector4f& last_color = uniform.get_fvec4("color");

        //! The vertex array(mesh) to use when rendering the object.
        std::shared_ptr<VertexArray> vertex_array=nullptr;
        //! @brief Flags that control the rendering of an object such as the associated masks.
        //! @details
        //! This is a bitmask of RenderFlags, and is bitwised AND'd with the materials
        //! variable of the same name to generate an objects render flags.
        int render_flags=kRenderDefault;
        StringIntern tag="Unnamed Renderable";

        //! Set to the number of the last vertex to render from the vertex array, or -1 to render all vertices.
        int end_vertex=-1;
        //! What cull style to use when rendering the object.
        int cull_face=kCullBackFaceCounterClockWise;

        //! The Material to use when rendering.
        std::shared_ptr<Material> material=DefaultMaterial;

        //! Returns true if the Renderable needs alpha blending.
        bool is_transparent() const {return material->is_transparent();}
        void update_transform();
        Renderable()=default;
        Renderable(const Renderable& r):Renderable(){operator =(r);}
        Renderable& operator = (const Renderable &r);
        MAKE_VISIT_HEAD(Renderable)


    };



};

#endif // ifndef NeonRush_ARRenderSystem_h