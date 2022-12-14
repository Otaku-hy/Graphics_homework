
#include "../rays/shapes.h"
#include "debug.h"

namespace PT {

const char* Shape_Type_Names[(int)Shape_Type::count] = {"None", "Sphere"};

BBox Sphere::bbox() const {

    BBox box;
    box.enclose(Vec3(-radius));
    box.enclose(Vec3(radius));
    return box;
}

Trace Sphere::hit(const Ray& ray) const {

    // TODO (PathTracer): Task 2
    // Intersect this ray with a sphere of radius Sphere::radius centered at the origin.

    // If the ray intersects the sphere twice, ret should
    // represent the first intersection, but remember to respect
    // ray.dist_bounds! For example, if there are two intersections,
    // but only the _later_ one is within ray.dist_bounds, you should
    // return that one!

    // Trace ret;
    // ret.origin = ray.point;
    // ret.hit = false;       // was there an intersection?
    // ret.distance = 0.0f;   // at what distance did the intersection occur?
    // ret.position = Vec3{}; // where was the intersection?
    // ret.normal = Vec3{};   // what was the surface normal at the intersection?
    // return ret;

    Trace ret;
    ret.origin = ray.point;
    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(ray.point, ray.dir);
    float c = dot(ray.point, ray.point) - radius * radius;
    float delta = b * b - 4 * a * c;

    ret.hit = false;       // was there an intersection?
    ret.distance = 0.0f;   // at what distance did the intersection occur?
    ret.position = Vec3{}; // where was the intersection?
    ret.normal = Vec3{};   // what was the surface normal at the intersection?

    if(delta < 0) return ret;
    float min_t = (-b - sqrt(delta)) / (2 * a);
    float max_t = (-b + sqrt(delta)) / (2 * a);
    if(min_t >= ray.dist_bounds[0] && min_t <= ray.dist_bounds[1]) {
        ret.hit = true;
        ret.distance = min_t;
        ret.position = ray.point + min_t * ray.dir;
        ret.normal = ret.position.unit();
        return ret;
    }
    if(max_t >= ray.dist_bounds[0] && max_t <= ray.dist_bounds[1]) {
        ret.hit = true;
        ret.distance = max_t;
        ret.position = ray.point + max_t * ray.dir;
        ret.normal = ret.position.unit();
        return ret;
    }
    
    return ret;
}

} // namespace PT
