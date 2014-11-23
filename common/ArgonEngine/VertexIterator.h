/**
 * @brief This file provides an implementation of a generic iterator for
 * VertexArrays.
 * @author Skyler Saleh, Ian Wiggin
 * @file VertexIterator.h
 **/
#ifndef VertexIterator_h
#define VertexIterator_h
#include <ArgonEngine/RenderSystemConstants.h>
namespace Argon{
    struct VertexArray;
    //! @brief Contains the information needed to manipulate an attributes data.
    struct VertexAttribPair: public ReflectionBase
    {
        //Just some helper functions for the VertexArrayIterator
        template<typename T>
        static void _from_float_(uint8_t*data, size_t x, float value){((T*)data)[x] = value;}
        template<typename T>
        static float _to_float_(uint8_t*data, size_t x){return ((T*)data)[x];}

        //! The name of the attribute. This is how you will access the data in a shader.
        StringIntern attribute;
        size_t stride;
        //! The components that the attribute contains.
        int components;
        //! A function that can get component 'x' of the data stored in the attribute.
        float (*to_float)(uint8_t* data, size_t x);
        //! A function that set component 'x' of an attributes data.
        void (*from_float)(uint8_t* data, size_t x,float value);
        //! The type of the attribute data.
        int type;
        //! The byte offset of the start of the attributes data in each vertex.
        ptrdiff_t offset;
        MAKE_VISIT(VertexAttribPair, {
            ADD_VAR_LONG(components);
            ADD_VAR_T(attribute);
            ADD_VAR_LONG(type);
            ADD_VAR_LONG(offset);
            switch(t.type) {
                case kRenderTypeFloat:
                    t.to_float = &_to_float_<float>;
                    t.from_float = &_from_float_<float>;
                    break;
                case kRenderTypeShort:
                    t.to_float = &_to_float_<short>;
                    t.from_float = &_from_float_<short>;
                    break;
                case kRenderTypeUByte:
                    t.to_float = &_to_float_<unsigned char>;
                    t.from_float = &_from_float_<unsigned char>;
                    break;
                case kRenderTypeByte:
                    t.to_float = &_to_float_<char>;
                    t.from_float = &_from_float_<char>;
                    break;

            }
            if(t.attribute=="position") t.attribute = kPositionAttribute;
            else if(t.attribute=="normal") t.attribute = kNormalAttribute;
        });
    };
    //! @brief This class provides a means of iterating through the arbitrary vertex data stored in a VertexArray.
    //! @details A Vertex Iterator will only iterate through a single attribute of data at a time, and it handles the proper
    //! conversion to and from the different data types stored in the array. It is not valid if the array changes size or attributes.
    class VertexIterator {
        uint8_t*  data;             //!< A pointer to the raw vertex data in the array.
        VertexAttribPair*attr;
        //! A class that mimics raw access to the individual vertices and passes it on to the VertexIterator.
        struct IterHelper {
            VertexIterator& parent;
            IterHelper(VertexIterator & p) : parent(p){}
            inline IterHelper operator++ (){
                return parent.add(1.);
            }
            inline IterHelper operator-- (){
                return parent.add(-1.);
            }
            template<typename T> inline IterHelper operator += (const T &x){return parent.add(x);} //!< See VertexIterator::add
            template<typename T> inline IterHelper operator -= (const T &x) {return parent.add(-x);}
            template<typename T> inline IterHelper operator *= (const T &x){return parent.mult(x);}//!< See VertexIterator::mult
            template<typename T> inline IterHelper operator /= (const T &x) {return parent.div(x);}//!< See VertexIterator::div
            template<typename T> inline bool operator==(const T &x) const {return parent.equals(x);}//!< See VertexIterator::equals
            template<typename T> inline bool operator!=(const T &x) const {return !parent.equals(x);}
            template<typename T> inline bool operator< (const T &x) const {return parent.less(x);}//!< See VertexIterator::less
            template<typename T> inline bool operator>=(const T &x) const {return !parent.less(x);}
            template<typename T> inline bool operator> (const T &x) const { return !parent.less(x)&&!parent.equals(x);}
            template<typename T> inline IterHelper& operator = (const T &x){parent.set(x);return*this;}//!< See VertexIterator::set

        };
    public:
        VertexIterator(uint8_t * data,
                       VertexAttribPair* attr) :attr(attr), data(data){}
        VertexIterator() : data(NULL),attr(NULL){}

        //! Returns true if VertexIterator iter points to the same vertex.
        bool operator==(const VertexIterator &iter) const {return data==iter.data;}
        //! Returns true if VertexIterator iter does not point to the same vertex.
        bool operator!=(const VertexIterator &iter) const {return data!=iter.data;}
        //! Returns true if VertexIterator iter points to a lower vertex.
        bool operator<(const VertexIterator &iter) const{
            return data < iter.data;
        }
        //! Advance the iterator 'x' vertices.
        VertexIterator& operator += (size_t x){
            data += attr->stride * x; return *this;
        }

        //! Decrements the iterator 'x' vertices.
        VertexIterator& operator -= (size_t x){
            data += attr->stride * x; return *this;
        }
        //! Returns an iterator that is 'x' vertices ahead.
        VertexIterator operator + (size_t x) const {
            VertexIterator v = *this; return v += x;
        }
        //! Returns an iterator that is 'x' vertices behind.
        VertexIterator operator - (size_t x) const {
            VertexIterator v = *this; return v -= x;
        }
        //! Advances the iterator one vertex.
        VertexIterator& operator++ (){
            data+=attr->stride;
            return *this;
        }
        //! Decrements the iterator once.
        VertexIterator& operator-- (){
            data-=attr->stride;
            return *this;
        }
        //! Advances the iterator one vertex.
        VertexIterator operator++ (int){
            VertexIterator i = *this;
            operator++();
            return i;
        }
        //! Decrements the iterator once.
        VertexIterator operator-- (int){
            VertexIterator i = *this;
            operator--();
            return i;
        }

        //! Adds a scalar to the vertex values.
        IterHelper add(float value){
            for(size_t x = 0; x<attr->components; ++x) attr->from_float(data,x, attr->to_float(data,x) + value);
            return IterHelper(*this);
        }

        //! Multiplies the vertex values against a scalar.
        IterHelper mult(float value){
            for(size_t x = 0; x<attr->components; ++x) attr->from_float(data,x, attr->to_float(data,x) * value);
            return IterHelper(*this);
        }

        //! Divides the vertex values by a scalar.
        IterHelper div(float value){
            for(size_t x = 0; x<attr->components; ++x) attr->from_float(data,x, attr->to_float(data,x) / value);
            return IterHelper(*this);
        }

        //! Sets the vertex values to a scalar.
        IterHelper set(float value){
            for(size_t x = 0; x<attr->components; ++x) attr->from_float(data,x, value);
            return IterHelper(*this);
        }
        //! Returns the current vertices data.
        IterHelper operator*(){
            return IterHelper(*this);
        }
        //! Returns true if the current vertex is less than a value.
        bool less(float value){
            bool b = false;
            for(size_t x = 0; x<attr->components; ++x) b |= attr->to_float(data,x)<value;
            return b;
        }
        //! Returns true if the current vertex equals a value.
        bool equals(float value){
            bool b = true;
            for(size_t x = 0; x<attr->components; ++x) b &= attr->to_float(data,x)==value;
            return b;
        }
        //! Adds a vector the current vertex's value.
        template <typename T,size_t comps>  IterHelper add(const VectorBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps; ++x) attr->from_float(data,x, attr->to_float(data,x) + v[x]);
            return IterHelper(*this);
        }
        //! Multiplies the current vertex's value by a vector.
        template <typename T,size_t comps>  IterHelper mult(const VectorBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps; ++x) attr->from_float(data,x, attr->to_float(data,x) * v[x]);
            return IterHelper(*this);
        }
        //! Divides the current vertex's value by a vector.
        template <typename T,size_t comps>  IterHelper div(const VectorBase<T,comps>&v){
            size_t min = attr->components<comps? attr->components:comps;
            for(size_t x = 0; x<min; ++x) attr->from_float(data,x, attr->to_float(data,x) / v[x]);
            return IterHelper(*this);
        }
        //! Sets the current vertex's value to a vector.
        template <typename T,size_t comps>  inline void set(const VectorBase<T,comps>&v){
            size_t min = attr->components<comps? attr->components:comps;
            float*d=(float*)data;
            if(attr->type== kRenderTypeFloat)while(min--) d[min]=v[min];
            else for(size_t x = 0; x<min; ++x) attr->from_float(data,x,v[x]);
        }
        //! Returns true if the current vertex's value is less the vector 'v'.
        template <typename T,size_t comps> bool less(const VectorBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps; ++x) {
                float f = attr->to_float(data,x);
                if(f!=v[x]) return f<v[x];
            }
            return false;
        }
        //! Returns true if the current vertex's value is equal to the vector 'v'.
        template <typename T,size_t comps> bool equals(const VectorBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps; ++x) {if(attr->to_float(data,x)!=v[x]) return true; }
            return true;
        }
        //! Adds a matrix to the vertex's values.
        template <typename T,size_t comps>  IterHelper add(const MatrixBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps * comps; ++x) attr->from_float(data,x, attr->to_float(data,x) + v[x]);
            return IterHelper(*this);
        }
        //! Sets the current vertex's values to the matrix 'v'.
        template <typename T,size_t comps> inline void set(const MatrixBase<T,comps>&v){

            for(size_t x = 0; x<attr->components&&x<comps * comps; ++x) attr->from_float(data,x,v[x]);
        }
        //! Returns true if the current vertex's values are less than matrix 'v'.
        template <typename T,size_t comps> bool less(const MatrixBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps * comps; ++x) {
                float f = attr->to_float(data,x);
                if(f!=v[x]) return f<v[x];
            }
            return false;
        }
        //! Returns true if the current vertex's values are equal to the matrix 'v'.
        template <typename T,size_t comps> bool equals(const MatrixBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps * comps; ++x) {if(attr->to_float(data,x)!=v[x]) return true; }
            return true;
        }
        //! Gets the values of the current vertex in matrix 'v'.
        template <typename T,size_t comps> void get(MatrixBase<T,comps>&v){
            for(size_t x = 0; x<attr->components&&x<comps * comps; ++x) v[x] = attr->to_float(data,x);
        }
        //! Gets the values of the current vertex in vector 'v'
        template <typename T,size_t comps> void get(VectorBase<T,comps>&v){
            size_t x = attr->components<comps ? attr->components : comps;
            while(x--) v[x] = attr->to_float(data,x);
        }
    };
}

#endif
