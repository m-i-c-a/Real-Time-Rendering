#ifndef LOADER_HPP
#define LOADER_HPP

#include <vector>
#include <string>

#include <glad/glad.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#include "Defines.hpp"

struct Vertex
{
    float pos[3]    = { 0.0f, 0.0f, 0.0f };
    float norm[3] = { 0.0f, 0.0f, 0.0f };
    float uv[2]     = { 0.0f, 0.0f };
};

void load_model(const std::string filepath, GLuint* vertexarray_handle, GLuint* vertex_buffer_handle, GLsizei* vertex_count) {

    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, reader_config)) {
        std::cerr << "Failed to open .obj file (" + filepath + "): " + reader.Error();
        exit(EXIT_FAILURE);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::vector<Vertex> vertices;
    vertices.reserve(shapes[0].mesh.num_face_vertices.size() * 3);

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {

                Vertex vert { 0 };

                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vert.pos[0] = attrib.vertices[3*size_t(idx.vertex_index)+0];
                vert.pos[1] = attrib.vertices[3*size_t(idx.vertex_index)+1];
                vert.pos[2] = attrib.vertices[3*size_t(idx.vertex_index)+2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    vert.norm[0] = attrib.normals[3*size_t(idx.normal_index)+0];
                    vert.norm[1] = attrib.normals[3*size_t(idx.normal_index)+1];
                    vert.norm[2] = attrib.normals[3*size_t(idx.normal_index)+2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    vert.norm[0] = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    vert.norm[1] = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                }

                vertices.push_back(vert);
            }
            index_offset += fv;
        }
    }

    *vertex_count = vertices.size();

    glBindVertexArray(*vertexarray_handle);

    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                (void*)(vertices.data()), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0u);
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1u);
    glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, norm));
    glEnableVertexAttribArray(2u);
    glVertexAttribPointer(2u, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, uv));

    glBindVertexArray(0u);
}

#endif // LOADER_HPP




