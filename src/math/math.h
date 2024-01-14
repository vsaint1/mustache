#pragma once
#include <cmath>

#define M_PI 3.14159265358979323846264338327950288419716939937510
#define M_DEG2RAD(x) ((x) * M_PI / 180.0f)
#define M_RAD2DEG(x) ((x) * 180.0f / M_PI)

namespace math {

class ViewMatrix {
public:
  float matrix[4][4]; // or matrix[16]

  float *operator[](int index) { return matrix[index]; }

};

class Vector {
public:
  float x, y, z;

  constexpr Vector(const float x = 0.f, const float y = 0.f, const float z = 0.f) noexcept : x(x), y(y), z(z) {}

  constexpr const Vector &operator-(const Vector &other) const noexcept { return Vector{x - other.x, y - other.y, z - other.z}; }

  constexpr const Vector &operator+(const Vector &other) const noexcept { return Vector{x + other.x, y + other.y, z + other.z}; }

  constexpr const Vector &operator/(const float factor) const noexcept { return Vector{x / factor, y / factor, z / factor}; }

  constexpr const Vector &operator*(const float factor) const noexcept { return Vector{x * factor, y * factor, z * factor}; }

  constexpr const bool operator>(const Vector &other) const noexcept { return x > other.x && y > other.y && z > other.z; }

  constexpr const bool operator>=(const Vector &other) const noexcept { return x >= other.x && y >= other.y && z >= other.z; }

  constexpr const bool operator<(const Vector &other) const noexcept { return x < other.x && y < other.y && z < other.z; }

  constexpr const bool operator<=(const Vector &other) const noexcept { return x <= other.x && y <= other.y && z <= other.z; }

  constexpr const bool operator==(const Vector &other) const noexcept { return x == other.x && y == other.y && z == other.z; }

  double distance(const Vector &other) const noexcept { return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2)); }

  constexpr bool isZero() const noexcept { return x == 0 && y == 0 && z == 0; }

  // vm: view_matrix, int: window_width, int: window_height
  Vector worldToScreen(ViewMatrix matrix, int ww, int wh) {
    float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
    float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

    float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];

    float inv_w = 1.f / w;
    _x *= inv_w;
    _y *= inv_w;

    float screen_x = ww * 0.5f;
    float screen_y = wh * 0.5f;

    screen_x += 0.5f * _x * ww + 0.5f;
    screen_y -= 0.5f * _y * wh + 0.5f;

    return {screen_x, screen_y, w};
  }
};

float radToDeg(float x) { return x * 180 / M_PI; }

float degToRad(float x) { return x * M_PI / 180; }

// vec3: pos , vec3: out, vm: view_matrix, int: window_width, int: window_height
bool worldToScreen(const Vector &pos, Vector &out, ViewMatrix matrix, int ww, int wh) {
  out.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
  out.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

  float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3]; // x,y,z,w matrix

  if (w < 0.01f)
    return false;

  const float inv_w = 1.f / w;
  out.x *= inv_w;
  out.y *= inv_w;

  float x = ww * 0.5f;
  float y = wh * 0.5f;

  x += 0.5f * out.x * ww + 0.5f;
  y -= 0.5f * out.y * wh + 0.5f;

  out.x = x;
  out.y = y;

  return true;
}
}; // namespace math
