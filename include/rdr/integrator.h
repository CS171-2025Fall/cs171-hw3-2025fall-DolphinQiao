#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include "rdr/rdr.h"
#include "rdr/scene.h"
#include "rdr/camera.h"
#include "rdr/math_utils.h"

RDR_NAMESPACE_BEGIN

class Integrator : public ConfigurableObject {
public:
  Integrator(const Properties &props) : ConfigurableObject(props) {}
  virtual ~Integrator() = default;

  virtual void render(ref<Camera> camera, ref<Scene> scene) = 0;
};

class IntersectionTestIntegrator : public Integrator {
public:
  IntersectionTestIntegrator(const Properties &props)
      : Integrator(props),
        spp(props.getProperty<int>("spp", 16)),
        max_depth(props.getProperty<int>("max_depth", 5)),
        point_light_position(props.getProperty<Vec3f>("point_light_position", Vec3f(0, 1.9, 0))) {
    if (props.hasProperty("point_light_intensity")) {
      point_light_intensity = props.getProperty<Vec3f>("point_light_intensity");
    } else if (props.hasProperty("point_light_flux")) {
      point_light_intensity =
          props.getProperty<Vec3f>("point_light_flux") / (4 * PI);
    } else {
      point_light_intensity =
          props.getProperty<Vec3f>("point_light_intensity", Vec3f(10, 10, 10));
    }
  }

  void render(ref<Camera> camera, ref<Scene> scene) override;

  Vec3f Li(ref<Scene> scene, DifferentialRay &ray, Sampler &sampler) const;

  Vec3f directLighting(ref<Scene> scene, SurfaceInteraction &interaction) const;

private:
  int spp;
  int max_depth;
  Vec3f point_light_position;
  Vec3f point_light_intensity;
};

class PathIntegrator : public Integrator {
public:
    PathIntegrator(const Properties &props) : Integrator(props) {}
    void render(ref<Camera> camera, ref<Scene> scene) override;
    Vec3f Li(ref<Scene> scene, DifferentialRay &ray, Sampler &sampler) const;
    Vec3f directLighting(ref<Scene> scene, SurfaceInteraction &interaction, Sampler &sampler) const;
};

class IncrementalPathIntegrator : public Integrator {
public:
    IncrementalPathIntegrator(const Properties &props) : Integrator(props) {}
    template <typename PathType>
    Vec3f Li(ref<Scene> scene, DifferentialRay &ray, Sampler &sampler) const;
};

RDR_NAMESPACE_END

#endif
