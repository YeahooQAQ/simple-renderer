#include"material.hpp"

namespace LemonCube{

Material::~Material(){}

Lambertian::Lambertian(const Vec3f& _color) : Material(_color){};
Lambertian::~Lambertian(){};
bool Lambertian::Scatter(
    const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
) const { 
    //Vec3f rand_vector = Vec3<float>::RandomInSphere();
    Vec3f rand_vector = Vec3<float>::RandomInSphere().normalized();
    if(rand_vector * info.normal < 0.0f){
        rand_vector *= -1.0f;
    }

    attenuation = color * (rand_vector * info.normal);
    scattered.position = info.hit_point;
    scattered.direction = rand_vector;
    return true;
}


Metal::Metal(const Vec3f& _color, const float& _fuzz) : Material(_color), fuzz(_fuzz){};
Metal::~Metal(){};
bool Metal::Scatter(
    const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
) const{ 
    Vec3f reflect = Reflect(ray.direction, info.normal) 
                    + Vec3<float>::RandomInSphere().normalized() * fuzz;
    attenuation = color;
    scattered.position = info.hit_point;
    scattered.direction = reflect;
    return (scattered.direction * info.normal) > 0.0f;
}

Dielectric::Dielectric(const Vec3f& _color, const float& _refraction_index, const float& _fuzz)
    : 
    Metal(_color, _fuzz), 
    refraction_index(_refraction_index)
{}
Dielectric::~Dielectric(){};

bool Dielectric::Scatter(
    const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
) const{
    float refraction = info.hit_on_face ? (1.0f / refraction_index) : refraction_index;
    float cosine_theta = ray.direction * info.normal;
    float sine_theta = sqrt(1.0f - cosine_theta * cosine_theta);
    attenuation = color;
    scattered.position = info.hit_point;

    if(refraction * sine_theta < 1.0f){
        scattered.direction = Refract(ray.direction, info.normal, refraction) 
                            + Vec3<float>::RandomInSphere().normalized() * fuzz;
    }
    else{
        scattered.direction = Reflect(ray.direction, info.normal)
                    + Vec3<float>::RandomInSphere().normalized() * fuzz;
    }
    return true;
}


}