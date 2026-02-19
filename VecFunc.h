#include "Vec2.h"
#include "Vec3.h"
#include <math.h>

using namespace std;

#define MAX_STEPS 100
#define MAX_DIST 100.0f
#define SURF_DIST 0.01f

float length(vec2 const& v) { return sqrt(v.x * v.x + v.y * v.y);}
float length(vec3 const& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);}

vec3 normalize(vec3 v) { return v / length(v);}

float sdSphere(vec3 p, vec3 const& pos, float r){ return length(p - pos) - r;}
float sdPlane(vec3 p, float y){ return p.y - y;}

float GetDist(vec3 const& p){
    float dS = sdSphere(p, vec3(0, 1, 4), 1);
    float dP = sdPlane(p, 0);

    float d = min(dS, dP);
    return d;
}

vec3 GetNormal(vec3 p){
    float d = GetDist(p);
    vec3 n = vec3(
        d - GetDist(p - vec3(0.1f, 0, 0)),
        d - GetDist(p - vec3(0, 0.1f, 0)),
        d - GetDist(p - vec3(0, 0, 0.1f))
    );

    return normalize(n);
}

float RayMarch(vec3 ro, vec3 rd){
    float dO = 0.0f;

    for(int i = 0; i<MAX_STEPS; i++){
        vec3 p = ro + rd * dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO > MAX_DIST || dS < SURF_DIST) break;
    }

    return dO;
}
