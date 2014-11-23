/**
 * @brief This file provides several groups of constants used in the RenderSystem.
 * @file RenderSystemConstants.h
 * @author Skyler Saleh
 **/
#ifndef RenderSystemConstants_h
#define RenderSystemConstants_h
#include "Utility.h"
#include <algorithm>
#include "Vector.h"
namespace Argon {
const extern StringIntern kColorAttribute;             //!< A string that identifies a Color Attribute in a Vertex Array.
const extern StringIntern kTextureAttribute;           //!< A string that identifies a Texture Attribute in a Vertex Array.
const extern StringIntern kNormalAttribute;            //!< A string that identifies a Normal Attribute in a Vertex Array.
const extern StringIntern kPositionAttribute;          //!< A string that identifies a Position Attribute in a Vertex Array.
enum RenderLayers{
    kRenderLayerDebug=1<<31,
    kRenderLayerMask=~kRenderLayerDebug
};
//! @brief This enum represents the various possible blend operations. Their functionality matches the OpenGL
//! blend modes of the same name.
enum BlendFactor
{
    kZero=0x0,
    kOne=0x1111,

    kDstColor=0x2222,
    kDstAlpha=0x3333,
    kOneMinusDstColor=0x4444,
    kOneMinusDstAlpha=0x5555,

    kSrcColor=0x6666,
    kSrcAlpha=0x7777,
    kOneMinusSrcColor=0x8888,
    kOneMinusSrcAlpha=0x9999,

    kConstantColor=0xAAAA,
    kOneMinusConstantColor=0xBBBB,
    kConstantAlpha=0xCCCC,
    kOneMinusConstantAlpha=0xDDDD,

    kSrcAlphaSaturate=0xEEEE,

    kBlendFactorMask =0xFFFF,

    kSrcColorMask =0xF,
    kSrcAlphaMask =0xF0,
    kSrcMask      =0xFF,

    kDstColorMask =0xF00,
    kDstAlphaMask =0xF000,
    kDstMask      =0xFF00,

    kSrcColorOffset=0,
    kSrcAlphaOffset=4,
    kDstColorOffset=8,
    kDstAlphaOffset=12,

    kBlendReplace = (kDstMask&kZero)|(kSrcMask&kOne),
    kBlendPremultipliedAlpha = (kDstMask&kOneMinusSrcAlpha)|(kSrcMask&kOne),
    kBlendAlpha = (kDstMask&kOneMinusSrcAlpha)|(kSrcMask&kSrcAlpha),
    kBlendMultiply = (kSrcMask&kDstColor)|(kDstMask&kOneMinusSrcAlpha),
    kBlendScreen   = (kSrcMask&kOne)|(kDstMask&kOneMinusSrcColor),
    kBlendAdditive = (kSrcMask&kSrcAlpha)|(kDstMask&kOne),
    kLinearDodge   = kOne,

};
//! @brief This represents the draw style to use for a VertexArray.
enum DrawType
{
    //! The vertexes are used to create list of triangles, where the last two vertices of one triangle are the first two vertices of the next triangle. This is the fastest on old hardware.
    kDrawTriangleStrip,
    kDrawPoints,                    //!< Each vertex is rendered as an individual point.
    kDrawLines,                     //!< Two vertexes form a line. Each line has its own pair of vertices. This is slow on most hardware.
    kDrawLineLoop,                  //!< A loop of lines are formed by using the previous vertex and the next vertex to form a line. This is slow on most hardware.
    kDrawTriangles,                 //!< Every 3 vertices define a new triangle. This is the fastest mode on modern hardware.
    kDrawTriangleFan                //!< The first vertex is the first vertex on all triangles. The second vertex is the previous vertex. This mode is relatively obscure.
};
//! @brief The variable type of the data in a Vertex Array.
enum RenderType
{
    kRenderTypeFloat=0,
    kRenderTypeShort=1,
    kRenderTypeByte =2,
    kRenderTypeUByte=3
};
//! @brief The Cull Mode to use when rendering the Renderable.
enum CullFaceStyle
{
    kCullNotSet,                //!< Unknown cull state
    kCullNone,                  //!< Don't cull any faces.
    kCullFrontFaceClockWise,             //!< Cull faces that face the viewer, assuming a clockwise winding.
    kCullBackFaceClockWise,               //!< Cull faces that face away from the viewer, assuming a clockwise winding.
    kCullFrontFaceCounterClockWise,     //!< Cull faces that face the viewer, assuming a counter-clockwise winding.
    kCullBackFaceCounterClockWise,               //!< Cull faces that face away from the viewer, assuming a counter-clockwise winding.
};
enum TextureFormat{

    kTextureRGB565=0x0,
    kTextureRGB8  =0x1,
    kTextureRGBF16=0x2,
    kTextureRGBF32=0x3,
    kTextureRGBF64=0x4,

    kTextureRGBA5551=0x5,
    kTextureRGBA4=0x6,
    kTextureRGBA8=0x7,
    kTextureRGBAF16=0x8,
    kTextureRGBAF32=0x9,
    kTextureRGBAF64=0xA,

    kTextureDepth16 = 0xB,
    kTextureDepth24 = 0xC,
    kTextureDepth32 = 0xD,
    kTextureStencil8= 0xE,

    kTextureDepthStencil = 0xF,
    kTextureFormatMask = 0xFF,
    kTextureAnsiotropicMask= 0xF00,
    kTextureAnsiotropic  = 0x100,
    kTextureAnsiotropic2x= 0x100,
    kTextureAnsiotropic4x= 0x200,
    kTextureAnsiotropic8x= 0x300,

    kTextureFlagMask = 0xFFFFF000,

    kTextureDontMipmap = 0x1000,
    kTextureDontFilterMipmap = 0x2000,
    kTextureDontFilterPixels = 0x4000,
    kTextureFBO = 0x8000,
    kTextureClamp=0x10000


};
inline static unsigned int get_tex_format_pixel_size(unsigned int format){
    const static int lookup[16]={
    2,3,12,12,24, //RGB
    2,2,4,16,16,32,//RGBA
    4,4,4,4,0     //D/S
    };
    return lookup[format&0xF];
}
void tex_set_color(unsigned char* data, unsigned int format,Vector4f color, int x, int y, int w, int h);
Vector4f tex_lookup_color(unsigned char* data, unsigned int format, int x, int y, int w, int h);


 double texture_format_fitness(unsigned int tex_form1, unsigned int tex_form2);
//! @brief The variable type of a Uniform.
enum UniformType {
    kUniformScalar = 0x1,
    kUniformVec2   = 0x2,
    kUniformVec3   = 0x3,
    kUniformVec4   = 0x4,

    kUniformMatrix2x2 = 0x5,
    kUniformMatrix2x3 = 0x6,
    kUniformMatrix2x4 = 0x7,

    kUniformMatrix3x2 = 0x8,
    kUniformMatrix3x3 = 0x9,
    kUniformMatrix3x4 = 0xA,

    kUniformMatrix4x2 = 0xB,
    kUniformMatrix4x3 = 0xC,
    kUniformMatrix4x4 = 0xD,


    kUniformComponentMask = 0xF,

    kUniformFloatData     = 0x10,
    kUniformIntData       = 0x20,

    kUniformTypeMask  = 0xFF,

    kUniformSizeMask = 0xFF00,
    kUniformSizeOffset = 8,

    kUniformSizeIncrement = 0x100,



    kUniformFloat=kUniformScalar|kUniformFloatData,
    kUniformFVec2=kUniformVec2|kUniformFloatData,
    kUniformFVec3=kUniformVec3|kUniformFloatData,
    kUniformFVec4=kUniformVec4|kUniformFloatData,
    kUniformFMat2x2=kUniformMatrix2x2|kUniformFloatData,
    kUniformFMat2x3=kUniformMatrix2x3|kUniformFloatData,
    kUniformFMat2x4=kUniformMatrix2x4|kUniformFloatData,

    kUniformFMat3x2=kUniformMatrix3x2|kUniformFloatData,
    kUniformFMat3x3=kUniformMatrix3x3|kUniformFloatData,
    kUniformFMat3x4=kUniformMatrix3x4|kUniformFloatData,

    kUniformFMat4x2=kUniformMatrix4x2|kUniformFloatData,
    kUniformFMat4x3=kUniformMatrix4x3|kUniformFloatData,
    kUniformFMat4x4=kUniformMatrix4x4|kUniformFloatData,

    kUniformInt=kUniformScalar|kUniformIntData,
    kUniformIVec2=kUniformVec2|kUniformIntData,
    kUniformIVec3=kUniformVec3|kUniformIntData,
    kUniformIVec4=kUniformVec4|kUniformIntData,
    kUniformIMat2x2=kUniformMatrix2x2|kUniformIntData,
    kUniformIMat3x3=kUniformMatrix3x3|kUniformIntData,
    kUniformIMat4x4=kUniformMatrix4x4|kUniformIntData,


};

//! @brief Flags for rendering
enum RenderFlags {
    kRenderShadow    =0x1,     //!< The object should cast a shadow.
    kRenderDepthMask =0x2,     //!< The object should write into the depth buffer.
    kRenderDepthTest =0x4,     //!< The object should use depth testing.
    kRenderOpaqueFlags= kRenderDepthMask|kRenderDepthTest,
    kRenderRedMask   =0x8,     //!< The object should render into the red channel.
    kRenderGreenMask =0x10,     //!< The object should render into the green channel.
    kRenderBlueMask  =0x20,     //!< The object should render into the blue channel.
    kRenderAlphaMask =0x40,     //!< The object should render into the alpha channel.
    kRenderRGBMask= kRenderRedMask|kRenderGreenMask|kRenderBlueMask,
    kRenderColorMask= kRenderRedMask|kRenderGreenMask|kRenderBlueMask|kRenderAlphaMask,

    kRenderShouldRender =0x80,     //!< The object should render at all.
    kRenderDefault = 0xFF,      //!< The default render flags.

};
//! @brief Flags for framebuffer update.
enum FramebufferFlags {
    kShouldClearDepth=0x1,
    kShouldClearColor=0x2,
    kShouldClearStencil=0x4,

    kShouldRenderOpaque=0x8,
    kShouldRenderTransparent=0x10,

    kSortTransparent=0x20,
    kSortOpaque=0x40,

    kShouldRenderAll=kShouldRenderOpaque|kShouldRenderTransparent,
    kShouldClearAll=kShouldClearDepth|kShouldClearColor|kShouldClearStencil,
    kSortAll=kSortTransparent|kSortOpaque
};
};

#endif
