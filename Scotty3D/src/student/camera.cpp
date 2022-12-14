
#include "../util/camera.h"
#include "../rays/samplers.h"
#include "debug.h"

Ray Camera::generate_ray(Vec2 screen_coord) const {

    // TODO (PathTracer): Task 1
    // compute the position of the input sensor sample coordinate on the
    // canonical sensor plane one unit away from the pinhole.
    // Tip: Compute the ray direction in camera space and use
    // the camera transform to transform it back into world space.

    Vec3 Camera_W = (looking_at - position).normalize();
    Vec3 Camera_U = cross(Vec3(0,1,0),-Camera_W).normalize();
    Vec3 Camera_V = cross(-Camera_W,Camera_U).normalize();

    Camera_V = Camera_V * tan(Radians(vert_fov * 0.5f));
    Camera_U = Camera_U * tan(Radians(vert_fov * 0.5f)) * aspect_ratio;

    float Px =  (2 * screen_coord.x -1.f);
    float Py =  (2 * screen_coord.y -1.f);

    Vec3 rayPos = Px * Camera_U + Py * Camera_V + 1.f * Camera_W;
    Vec3 rayDir = (rayPos - Vec3(0,0,0)).normalize();

    return Ray(position,rayDir);
}
