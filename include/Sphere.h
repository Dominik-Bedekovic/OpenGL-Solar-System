#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Sphere
{
private:
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    GLuint VBO, VAO, EBO;
    float radius = 1.0f;
    int sectorCount = 36;
    int stackCount = 18;

public:
    Sphere(float r, int sectors, int stacks)
    {
        radius = r;
        sectorCount = sectors;
        stackCount = stacks;

        // Generate Vertices
        float x, y, z, xy;
        float s, t;

        float sectorStep = (float) (2 * 3.14159265358979323846 / sectorCount);
        float stackStep = (float) (3.14159265358979323846 / stackCount);
        float sectorAngle{}, stackAngle{};

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = (float) ((3.14159265358979323846 / 2) - (i * stackStep));
            xy = radius * cosf(stackAngle);//r * cos(stackAngle)
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);

                sphereVertices.push_back(x);
                sphereVertices.push_back(y);
                sphereVertices.push_back(z);

                s = (float) j / sectorCount;
                t = (float) i / stackCount;
                sphereVertices.push_back(s);
                sphereVertices.push_back(t);
            }
        }
        // Generate Vertices

        //  Generate Indices
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    sphereIndices.push_back(k1);
                    sphereIndices.push_back(k2);
                    sphereIndices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1))
                {
                    sphereIndices.push_back(k1 + 1);
                    sphereIndices.push_back(k2);
                    sphereIndices.push_back(k2 + 1);
                }
            }
        }
        //  Generate Indices

        //  Generate Buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        //  Generate Buffers

        //  Bind Buffers
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (unsigned int) sphereVertices.size() * sizeof(float),
                     sphereVertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (unsigned int) sphereIndices.size() * sizeof(unsigned int),
                     sphereIndices.data(), GL_DYNAMIC_DRAW);
        //  Bind Buffers

        //  Map the vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //  Map the vertex attributes

        //  Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //  Unbind buffers
    }

    Sphere(const Sphere&) = delete;
    Sphere& operator=(const Sphere&) = delete;
    ~Sphere() { glDeleteBuffers(1, &VBO); glDeleteBuffers(1, &EBO); glDeleteVertexArrays(1, &VAO); }

    /*  Draw Sphere  */
    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (unsigned int) sphereIndices.size(), GL_UNSIGNED_INT, 0);
    }
    /*  Draw Sphere  */
};

#endif// !SPHERE_H
