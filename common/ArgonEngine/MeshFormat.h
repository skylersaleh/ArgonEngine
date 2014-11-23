/**
 * @brief Argon Mesh Format.
 * @file MeshFormat.h
 * @author Skyler Saleh
 **/

#ifndef Neon_Rush_ARMeshFormat_h
#define Neon_Rush_ARMeshFormat_h

#include <vector>
#include "Node.h"
#include <map>
#include "Utility.h"
#include "RenderSystem.h"
namespace Argon {
struct MeshFormatFile;
struct MeshFormatObjectData :public ReflectionBase {
    StringIntern mesh;
    Matrix4f matrix;
    Vector3f position=Vector3f(0,0,0);
    Vector3f scale=Vector3f(1,1,1);
    Quaternion<float> quaternion;
    StringIntern parent;
    std::vector<StringIntern> materials;
    MAKE_VISIT_HEAD(MeshFormatObjectData);
};

struct MeshFormatLightData : public ReflectionBase{
    Vector4f color=Vector4f(1,1,1,1);
    Matrix4f matrix;
    StringIntern parent;
    Argon::StringIntern type;
    float intensity;
    float distance;
    float angle;
    float softness;
    MAKE_VISIT_HEAD(MeshFormatLightData);

};



struct MeshFormatFile : public ReflectionBase
{
    Matrix4f global_matrix;
    std::map<StringIntern,std::shared_ptr<Material> > materials;
    std::map<StringIntern,MeshFormatLightData> lights;
    std::map<StringIntern,std::shared_ptr<VertexArray> > meshes;
    std::map<StringIntern,MeshFormatObjectData> objects;
    std::shared_ptr<Material> get_material(const StringIntern& s);
    MAKE_VISIT_HEAD(MeshFormatFile);
};

struct MeshFormatObject :  public Renderable {
    MeshFormatObjectData* data;
    MeshFormatFile* file;

    std::vector<std::shared_ptr<MeshFormatObject> > objects;
    std::vector<std::shared_ptr<MeshFormatObject> > lights;
    MeshFormatObject() : data(NULL){}
    MeshFormatObject(MeshFormatObjectData* d, MeshFormatFile*f){
        set_mesh_data(d,f);
    }
    MeshFormatObject(const MeshFormatObject& d){
        set_mesh_data(d.data,d.file);
    }
    MeshFormatObject& operator=(const MeshFormatObject& d){
        set_mesh_data(d.data,d.file); return *this;
    }

    void set_mesh_data(MeshFormatObjectData* d,MeshFormatFile* file);

};
struct MeshFormatLight : public Light {

    MeshFormatLightData* data;
    MeshFormatLight() : data(NULL){}
    MeshFormatLight(MeshFormatLightData* d){
        set_mesh_data(d);
    }
    MeshFormatLight(const MeshFormatLight& d){
        set_mesh_data(d.data);
    }
    MeshFormatLight& operator=(const MeshFormatLight& d){
        set_mesh_data(d.data); return *this;
    }

    void set_mesh_data(MeshFormatLightData* d);
    MAKE_VISIT_HEAD(MeshFormatLight)

};
class MeshFormat :public Node
{
private:
    VirtualResource path;
    MeshFormatFile file;
public:

    typedef std::map<StringIntern,MeshFormatObjectData>::iterator object_data_iterator;
    typedef std::map<StringIntern,MeshFormatLightData>::iterator light_data_iterator;
    std::map<StringIntern, std::shared_ptr<Node> > spawned_nodes;
    void load_file(VirtualResource p);
    void create_objects();
    std::shared_ptr<MeshFormatObject> spawn_object(const StringIntern& s);
    std::shared_ptr<Material> get_material(const StringIntern & s){
        return file.get_material(s);
    }
    const std::shared_ptr<VertexArray> vbo(const std::string& s){
        return file.meshes[file.objects[s].mesh];
    }
    object_data_iterator begin_object_data(){
        return file.objects.begin();
    }
    object_data_iterator end_object_data(){
        return file.objects.end();
    }
    light_data_iterator begin_light_data(){
        return file.lights.begin();
    }
    light_data_iterator end_light_data(){
        return file.lights.end();
    }
    MeshFormatObjectData& get_object_data(const StringIntern& object){
        return file.objects[object];
    }
    Matrix4f &get_global_matrix(){
        return file.global_matrix;
    }
    std::shared_ptr<Node> get_object(const StringIntern& s){
        return spawned_nodes[s];
    }

    MeshFormatFile& get_file(){
        return file;
    }

};
};

#endif
