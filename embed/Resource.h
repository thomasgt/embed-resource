#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace embed {

class Resource {
public:
  Resource(const uint8_t *start, const size_t len)
      : resource_data(start), data_len(len) {}

  const uint8_t *const &data() const {
    return resource_data;
  }
  const size_t &size() const {
    return data_len;
  }

  const uint8_t *begin() const {
    return resource_data;
  }
  const uint8_t *end() const {
    return resource_data + data_len;
  }

private:
  const uint8_t *resource_data;
  const size_t data_len;
};

}  // namespace embed

#define LOAD_RESOURCE(RESOURCE) \
  ([]() { \
    extern const unsigned char _resource_##RESOURCE[]; \
    extern const size_t _resource_##RESOURCE##_len; \
    return embed::Resource(_resource_##RESOURCE, _resource_##RESOURCE##_len); \
  })()
