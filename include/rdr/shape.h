#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "rdr/rdr.h"
#include "rdr/accel.h"
#include "rdr/interaction.h"
#include "rdr/math_utils.h"

RDR_NAMESPACE_BEGIN

class Shape : public ConfigurableObject {
public:
  Shape(const Properties &props) : ConfigurableObject(props) {}
  virtual ~Shape() = default;

  virtual bool intersect(Ray &ray, SurfaceInteraction &interaction) const = 0;
  virtual AABB getBound() const = 0;
  virtual Float area() const = 0;
  virtual SurfaceInteraction sample(Sampler &sampler) const = 0;
  virtual Float pdf(const SurfaceInteraction &interaction) const = 0;
};

class Sphere : public Shape {
public:
  Sphere(const Properties &props);
  bool intersect(Ray &ray, SurfaceInteraction &interaction) const override;
  AABB getBound() const override;
  Float area() const override;
  SurfaceInteraction sample(Sampler &sampler) const override;
  Float pdf(const SurfaceInteraction &interaction) const override;

private:
  Vec3f center;
  Float radius;
};

struct TriangleMeshResource {
  std::vector<Vec3f> vertices;
  std::vector<Vec3f> normals;
  std::vector<Vec2f> texture_coordinates;
  std::vector<uint32_t> v_indices;
  std::vector<uint32_t> n_indices;
  std::vector<uint32_t> t_indices;
  
  bool has_normal = false;
  bool has_texture = false;

  Vec3f getVertex(uint32_t i) const { return vertices[v_indices[i]]; }
};

class TriangleMesh : public Shape {
public:
  TriangleMesh(const Properties &props);
  bool intersect(Ray &ray, SurfaceInteraction &interaction) const override;
  AABB getBound() const override;
  Float area() const override;
  SurfaceInteraction sample(Sampler &sampler) const override;
  Float pdf(const SurfaceInteraction &interaction) const override;

  ref<TriangleMeshResource> mesh;
  ref<Accel> accel;
  std::vector<Float> areas;
  Float total_area = 0;
  ref<Distribution1D> dist;
};

RDR_NAMESPACE_END

#endif
