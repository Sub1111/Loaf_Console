#include "Vec2.h"
#include "Vec3.h"
#include <math.h>

using namespace std;

#define MAX_STEPS 100
#define MAX_DIST 100.0f
#define SURF_DIST 0.01f

float clamp(float a, float b, float c){ return min(c, max(a, b));}

float length(vec2 const& v) { return sqrt(v.x * v.x + v.y * v.y);}
float length(vec3 const& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);}

vec3 normalize(vec3 v) { return v / length(v);}
float dot(vec3 const& v, vec3 const& u) { return (v.x * u.x + v.y * u.y + v.z * u.z);}
vec3 abs(vec3 v) { return vec3(abs(v.x), abs(v.y), abs(v.z));}
vec3 max(vec3 v, vec3 u) { return vec3(max(v.x, u.x), max(v.y, u.y), max(v.z, u.z));}

float smin( float a, float b, float k ){
    float h = max(k-abs(a-b),0.0f);
    return min(a, b) - h*h*0.25/k;
}
float smax( float a, float b, float k ){ return -smin(-a,-b,k);}
vec3 rotateX(vec3 a, float angle){
    vec3 b = a;
    b.z = a.z * cos(angle) - a.y * sin(angle);
    b.y = a.z * sin(angle) + a.y * cos(angle);
    return b;
}
vec3 rotateY(vec3 a, float angle){
    vec3 b = a;
    b.x = a.x * cos(angle) - a.z * sin(angle);
    b.z = a.x * sin(angle) + a.z * cos(angle);
    return b;
}
vec3 rotateZ(vec3 a, float angle){
    vec3 b = a;
    b.x = a.x * cos(angle) - a.y * sin(angle);
    b.y = a.x * sin(angle) + a.y * cos(angle);
    return b;
}

float sdSphere(vec3 p, vec3 const& pos, float r){ return length(p - pos) - r;}
float sdPlane(vec3 p, float y){ return p.y - y;}
float sdBox( vec3 p, vec3 b ){
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0f);
}
float sdEllipsoid(vec3 p, vec3 c, vec3 r){
    return (length( (p-c)/r ) - 1.0) * min(min(r.x,r.y),r.z);
}
float sdLoaf(vec3 p, vec3 pos, float scale){
    float dS1 = sdSphere(p, pos, 1.5f*scale);
    float dEllipsoid = sdEllipsoid(p, pos, vec3(1.5f, 0.70f, 0.65f) * scale);
    float dShortEl = smax(dEllipsoid, dS1, 0.5f);
    float dP = sdPlane(p, pos.y-0.05*scale);
    float dBase = smax(-dP, dShortEl, 0.5);
    
    // Бороздки
    float dEl1 = sdEllipsoid(p, pos + vec3(0.f, 0.6f, 0.f), vec3(0.15f, 0.1f, 10.f) * scale);
    float dEl2 = sdEllipsoid(p, pos + vec3(0.5f, 0.6f, 0.f), vec3(0.15f, 0.1f, 10.f) * scale);
    float dEl3 = sdEllipsoid(p, pos + vec3(-0.5f, 0.6f, 0.f), vec3(0.15f, 0.1f, 10.f) * scale);
    float dEl4 = sdEllipsoid(p, pos + vec3(0.9f, 0.53f, 0.f), vec3(0.15f, 0.1f, 10.f) * scale);
    float dEl5 = sdEllipsoid(p, pos + vec3(-0.9f, 0.53f, 0.f), vec3(0.15f, 0.1f, 10.f) * scale);
    
    float dResult = max(-dEl3, max(-dEl2, max(-dEl1, dBase)));
    dResult = max(-dEl5, max(-dEl4, dResult));
    return dResult;
}

float RayMarch(vec3 ro, vec3 rd, float t);
float GetDist(vec3 p, float t){
    // float dS2 = sdSphere(p, vec3(sin(t)*3.0f, 1, 4), 0.5);
    /*vec3 loafPos = vec3(0, 1.f, 4);
    loafPos = rotateZ(rotateY(loafPos, t), t/2.f);
    float dLoaf = sdLoaf(rotateZ(rotateY(p, t), t/2.f), loafPos, 1.);*/
    vec3 bp = p - vec3(0, 1, 4);
    bp = rotateZ(rotateY(bp, t), t/2.f);
    float dBox = sdBox(bp, vec3(1, 1, 1));

    float d = min(dBox, 9999.f);
    return d;
}

vec3 GetNormal(vec3 p, float t){
    float d = GetDist(p, t);
    vec3 n = vec3(
        d - GetDist(p - vec3(0.1f, 0, 0), t),
        d - GetDist(p - vec3(0, 0.1f, 0), t),
        d - GetDist(p - vec3(0, 0, 0.1f), t)
    );

    return normalize(n);
}

float GetLight(vec3 p, float t){
    vec3 lightPos1 = vec3(0, 10, -6);
    vec3 l1 = normalize(lightPos1 - p);
    vec3 n = GetNormal(p, t);
    float dif = clamp(dot(l1, n), 0., 1.);
    
    vec3 lightPos2 = vec3(0, -10, -6);
    vec3 l2 = normalize(lightPos2 - p);
    n = GetNormal(p, t);
    dif += clamp(dot(l2, n)/15., 0., 1.);

    float d = RayMarch(p + n * SURF_DIST * 2.0f, l1, t);
    if (d < length(lightPos1-p)) dif *= 0.1f;
    return dif;
}

float RayMarch(vec3 ro, vec3 rd, float t){
    float dO = 0.0f;

    for(int i = 0; i<MAX_STEPS; i++){
        vec3 p = ro + rd * dO;
        float dS = GetDist(p, t);
        dO += dS;
        if(dO > MAX_DIST || dS < SURF_DIST) break;
    }

    return dO;
}
