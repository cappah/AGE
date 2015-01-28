#pragma once

#include "SerializationMacros.hpp"
#include <glm/fwd.hpp>

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION()

SERIALIZATION_SAVE_DECLARATION(glm::mat2);
SERIALIZATION_LOAD_DECLARATION(glm::mat2);
SERIALIZATION_SAVE_DECLARATION(glm::mat3);
SERIALIZATION_LOAD_DECLARATION(glm::mat3);
SERIALIZATION_SAVE_DECLARATION(glm::mat4);
SERIALIZATION_LOAD_DECLARATION(glm::mat4);
