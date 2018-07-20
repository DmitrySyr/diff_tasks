#pragma once

#include <cstddef>
#include <thread>
#include <mutex>
#include <memory>

#include "Handler.h"

namespace async {

using handle_t = void *;

void* connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);
void clear();

}
