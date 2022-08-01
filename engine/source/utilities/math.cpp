/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "utilities/math.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace jng::math {

    bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        // From glm::decompose in matrix_decompose.inl

        glm::mat4 localMatrix{ transform };

        // Normalize the matrix.
        if (glm::epsilonEqual(localMatrix[3][3], 0.f, glm::epsilon<float>()))
            return false;

        // First, isolate perspective. This is the messiest.
        if (
            glm::epsilonNotEqual(localMatrix[0][3], 0.f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[1][3], 0.f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[2][3], 0.f, glm::epsilon<float>()))
        {
            // Clear the perspective partition
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.f;
            localMatrix[3][3] = 1.f;
        }

        // Translation:
        translation = glm::vec3(localMatrix[3]);
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

        glm::vec3 row[3]{};

        // Scale:
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                row[i][j] = localMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], 1.f);
        scale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], 1.f);
        scale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], 1.f);

        // Rotation:
        rotation.y = asin(-row[0][2]);
        if (cos(rotation.y) != 0.f) {
            rotation.x = atan2(row[1][2], row[2][2]);
            rotation.z = atan2(row[0][1], row[0][0]);
        }
        else {
            rotation.x = atan2(-row[2][0], row[1][1]);
            rotation.z = 0.f;
        }

        return true;
    }

} // namespace jng::math
