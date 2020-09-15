#pragma once
#include "GraphicsFormats.h"

namespace Graphics
{
    struct SAMPLE_DESC
    {
        unsigned Count;
        unsigned Quality;
    };

    enum class BIND_FLAG
    {
        BIND_VERTEX_BUFFER = 0x1L,
        BIND_INDEX_BUFFER = 0x2L,
        BIND_CONSTANT_BUFFER = 0x4L,
        BIND_SHADER_RESOURCE = 0x8L,
        BIND_STREAM_OUTPUT = 0x10L,
        BIND_RENDER_TARGET = 0x20L,
        BIND_DEPTH_STENCIL = 0x40L,
        BIND_UNORDERED_ACCESS = 0x80L,
        BIND_DECODER = 0x200L,
        BIND_VIDEO_ENCODER = 0x400L
    };

    enum class CPU_ACCESS_FLAG
    {
        CPU_ACCESS_WRITE = 0x10000L,
        CPU_ACCESS_READ = 0x20000L
    };

    struct TEXTURE2D_DESC
    {
        unsigned Width;
        unsigned Height;
        unsigned MipLevels;
        unsigned ArraySize;
        TEX_FORMAT Format;
        SAMPLE_DESC SampleDesc;
        BUFFER_USAGE Usage;
        BIND_FLAG BindFlags;
        CPU_ACCESS_FLAG CPUAccessFlags;
        unsigned MiscFlags;
    };
}