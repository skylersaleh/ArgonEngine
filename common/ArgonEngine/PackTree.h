/**
 * @brief A packtree.
 * @file PackTree.h
 * @author Skyler Saleh
 **/

#ifndef ARPackTree_h
#define ARPackTree_h
namespace Argon
{
template <typename T>
struct PackTree
{
    T x;
    T y;
    T width;
    T height;
    PackTree* child;
    PackTree* parent;
    bool leaf;
    void set(T x2, T y2, T w, T h, PackTree*parent2){
        x = x2; y=y2; width = w; height = h; parent = parent2;
    }

    PackTree() : x(0),y(0),width(0),height(0),child(NULL),parent(NULL),leaf(false){}
    PackTree* smallest_fit(T w, T h)
    {
        if (w>width||h>height||leaf) return NULL;
        if (has_children()) {
            PackTree* t1 = child[0].smallest_fit(w,h);
            PackTree* t2 = child[1].smallest_fit(w,h);
            if(!t1) return t2;
            if(!t2) return t1;

            if ((t1->width-w)*(t1->height-h)<(t2->width-w)*(t2->height-h))
                return t1;
            else
                return t2;

        }
        return this;
    }

    bool has_children(){
        return child;
    }
    PackTree* pack_node(T w, T h){
        if (width<w || height<h ||leaf) return NULL;
        if (has_children()) {
            PackTree* t = smallest_fit(w, h);
            if(t) return t->pack_node(w,h);
            return NULL;
        }

        if (width-w>height-h) {
            if (width!= w) {
                child = new PackTree[2];
                child[0].set(x,y,w,height,this);
                child[1].set(x+w,y,width-w,height,this);
                return child[0].pack_node(w, h);
            }
            if (height!= h) {
                child = new PackTree[2];
                child[0].set(x,y,width,h,this);
                child[1].set(x,y+h,width,height-h,this);
                return child[0].pack_node(w, h);
            }
        }else{
            if (height!= h) {
                child = new PackTree[2];
                child[0].set(x,y,width,h,this);
                child[1].set(x,y+h,width,height-h,this);
                return child[0].pack_node(w, h);
            }
            if (width!= w) {
                child = new PackTree[2];
                child[0].set(x,y,w,height,this);
                child[1].set(x+w,y,width-w,height,this);
                return child[0].pack_node(w, h);
            }
        }
        leaf = true;
        return this;
    }
    bool empty(){
        return !has_children() && !leaf;
    }
    void update_children(){
        if(child[0].empty() && child[1].empty())
        {
            PackTree*t = child;
            child = NULL;

            delete []t;
            remove_self();
        }
    }
    int size()
    {
        if (has_children())
            return child[0].size()+child[1].size();

        return leaf;
    }
    void remove_self()
    {
        leaf = false;
        if (parent) parent->update_children();
    }

    ~PackTree(){
        delete []child;
    }
};
}

#endif
