#pragma once
#include "Neurex/renderer/VertexArray.h"

namespace Neurex
{

    class OpenGLVertexArray :
        public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void bind() override;
        void unbind() override;

        void add_vertex_buffer(std::shared_ptr<VertexBuffer>& vb) override;
        void set_index_buffer(std::shared_ptr<IndexBuffer>& vb) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const override;
        const std::shared_ptr<IndexBuffer>& get_index_buffer() const override;

    private:
        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;
        std::shared_ptr<IndexBuffer> index_buffer;
        unsigned int renderer_id = 0;
    };

}