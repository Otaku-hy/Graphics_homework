
#include "../scene/particles.h"
#include "../rays/pathtracer.h"

bool Scene_Particles::Particle::update(const PT::Object& scene, float dt, float radius) {

    auto ProjectLength = [](Vec3 normal, Vec3 dir)
    {
        return dot(normal,-dir);
    };

    auto AdjustNormal = [](Vec3& normal, Vec3 dir)
    {
        if(dot(normal,-dir) < 0.f) 
            normal = -1 * normal;
    };

    float tLeft = dt; // time left for collision looping
    float eps = 1e-3; // minimum time to continue loop

    while(tLeft > 0) {
        Ray ray(pos,velocity.unit());

        auto intersection = scene.hit(ray);
        AdjustNormal(intersection.normal,ray.dir);
        
        if(intersection.hit)
        {
            float v = velocity.norm();
            if(eps * v + radius > intersection.distance)
            {
                velocity = velocity + 2 * ProjectLength(intersection.normal,velocity) * intersection.normal;
            }
            else
            {
                pos = pos + velocity * eps;
                velocity = velocity + acceleration * eps;
            }
        }
        else
        {
            pos = pos + velocity * tLeft;
            velocity = velocity + acceleration * tLeft;
            break;
        }
    // TODO: ray from particle origin; velocity is always unit
    // TODO: how far the particle will travel
    // TODO: hit something?
    // TODO: if hit something?
    // TODO: calculate new pos and velocity, and new simulation time.
    // TODO: if not hit
    // use Forward Euler to calculate new pos and velocity and break loop
        tLeft -= eps;
    }

    age -= dt;

    return age > 0.f; // dead particle?
}
