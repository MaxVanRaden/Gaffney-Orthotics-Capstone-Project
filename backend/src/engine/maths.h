#ifndef MATHS_H
#define MATHS_H

#include "defines.h"

global const f32 PI = (f32)3.141592653589793238;

internal inline
f32 deg_to_rad(f32 deg) {
    return deg * PI / 180.0f;
}

internal inline
f32 rad_to_deg(f32 rad) {
    return rad * 180.0f / PI;
}

union vec2 {
    f32 e[2];
    struct {
        f32 x;
        f32 y;
    };
};

union vec3 {
    f32 e[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    struct {
        vec2 xy;
        f32 _pad0;
    };
    struct {
        f32 _pad1;
        vec2 yz;
    };
};

union vec4 {
    f32 e[4];
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct {
        vec3 xyz;
        f32 _pad0;
    };
    struct {
        vec2 xy;
        f32 _pad1;
        f32 _pad2;
    };
    struct {
        f32 _pad3;
        vec2 yz;
        f32 _pad4;
    };
    struct {
        f32 _pad5;
        f32 _pad6;
        vec2 zw;
    };
};

union Quaternion {
    f32 e[4];
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct {
        f32 a;
        f32 k;
        f32 j;
        f32 i;
    };
    struct {
        vec3 xyz;
        f32 _pad0;
    };
    struct {
        vec2 xy;
        f32 _pad1;
        f32 _pad2;
    };
    struct {
        f32 _pad3;
        vec2 yz;
        f32 _pad4;
    };
    struct {
        f32 _pad5;
        f32 _pad6;
        vec2 zw;
    };
    vec4 xyzw;
};

union mat4 {
    f32 elements[4 * 4];
    vec4 columns[4];
    struct {
        f32 m00, m01, m02, m03;
        f32 m10, m11, m12, m13;
        f32 m20, m21, m22, m23;
        f32 m30, m31, m32, m33;
    };
};


internal inline
vec2 V2(f32 x, f32 y) {
    vec2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

internal inline
vec3 V3(f32 x, f32 y, f32 z) {
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

internal inline
vec3 V3(vec2 v2, f32 z) {
    vec3 vec;
    vec.x = v2.x;
    vec.y = v2.y;
    vec.z = z;
    return vec;
}

internal inline
vec4 V4(f32 x, f32 y, f32 z, f32 w) {
    vec4 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = w;
    return vec;
}

internal inline
vec4 V4(vec3 v3, f32 w) {
    vec4 vec;
    vec.x = v3.x;
    vec.y = v3.y;
    vec.z = v3.z;
    vec.w = w;
    return vec;
}

internal inline
vec4 V4(vec2 v21, vec2 v22) {
    vec4 vec;
    vec.x = v21.x;
    vec.y = v21.y;
    vec.z = v22.x;
    vec.w = v22.y;
    return vec;
}

internal inline
f32 length(vec2 vec) {
#ifdef USE_SSE
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
    return sqrt(vec.x * vec.x + vec.y * vec.y);
#endif
}

internal inline
void normalize(vec2* vec) {
    f32 len = length(*vec);
    if (len != 0) {
        vec->x /= len;
        vec->y /= len;
    }
}

internal inline
vec2 normalize(vec2 vec) {
    f32 len = length(vec);
    if (len != 0) {
        return V2(vec.x / len, vec.y / len);
    }
    return vec;
}

internal inline
f32 dot(vec2 a, vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

internal inline
f32 cross(vec2 a, vec2 b) {
    return (a.x * b.y) - (a.y * b.x);
}

internal inline
f32 length(vec3 vec) {
#ifdef USE_SSE
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
#endif
}

internal inline
void normalize(vec3* vec) {
    f32 len = length(*vec);
    if (len != 0) {
        vec->x /= len;
        vec->y /= len;
        vec->z /= len;
    }
}

internal inline
vec3 normalize(vec3 vec) {
    f32 len = length(vec);
    if (len != 0) {
        return V3(vec.x / len, vec.y / len, vec.z / len);
    }
    return vec;
}

internal inline
f32 dot(vec3 a, vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

internal inline
vec3 cross(vec3 a, vec3 b) {
    return V3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

internal inline
f32 length(vec4 vec) {
#ifdef USE_SSE
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(SimdReals, SimdReals, 0x71)));
#else
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
#endif
}

internal inline
void normalize(vec4* vec) {
    f32 len = length(*vec);
    if (len != 0) {
        vec->x /= len;
        vec->y /= len;
        vec->z /= len;
        vec->w /= len;
    }
}

internal inline
vec4 normalize(vec4 vec) {
    f32 len = length(vec);
    if (len != 0) {
        return V4(vec.x / len, vec.y / len, vec.z / len, vec.w / len);
    }
    return vec;
}

internal inline
f32 dot(vec4 a, vec4 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

//OPERATOR OVERLOADS
// VEC2

internal inline
vec2 operator+(vec2 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

internal inline
vec2 operator-(vec2 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

internal inline
vec2 operator*(vec2 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    return c;
}

internal inline
vec2 operator*(f32 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a * b.x;
    c.y = a * b.y;
    return c;
}

internal inline
vec2 operator*(vec2 a, f32 b) {
    vec2 c = { 0 };
    c.x = a.x * b;
    c.y = a.y * b;
    return c;
}

internal inline
vec2 operator/(vec2 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    return c;
}

internal inline
vec2 operator/(f32 a, vec2 b) {
    vec2 c = { 0 };
    c.x = a / b.x;
    c.y = a / b.y;
    return c;
}

internal inline
vec2 operator/(vec2 a, f32 b) {
    vec2 c = { 0 };
    c.x = a.x / b;
    c.y = a.y / b;
    return c;
}

// VEC3

internal inline
vec3 operator+(vec3 a, vec3 b) {
    vec3 c = { 0 };
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

internal inline
vec3 operator-(vec3 a, vec3 b) {
    vec3 c = { 0 };
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

internal inline
vec3 operator*(vec3 a, vec3 b) {
    vec3 c = { 0 };
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    return c;
}

internal inline
vec3 operator*(f32 a, vec3 b) {
    vec3 c = { 0 };
    c.x = a * b.x;
    c.y = a * b.y;
    c.z = a * b.z;
    return c;
}

internal inline
vec3 operator*(vec3 b, f32 a) {
    vec3 c = { 0 };
    c.x = a * b.x;
    c.y = a * b.y;
    c.z = a * b.z;
    return c;
}

internal inline
vec3 operator/(vec3 a, vec3 b) {
    vec3 c = { 0 };
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    return c;
}

// VEC4

internal inline
vec4 operator+(vec4 a, vec4 b) {
    vec4 c = { 0 };
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;
    return c;
}

internal inline
vec4 operator-(vec4 a, vec4 b) {
    vec4 c = { 0 };
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w - b.w;
    return c;
}

internal inline
vec4 operator*(vec4 a, vec4 b) {
    vec4 c = { 0 };
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    c.w = a.w * b.w;
    return c;
}

internal inline
vec4 operator*(f32 a, vec4 b) {
    vec4 c = { 0 };
    c.x = a * b.x;
    c.y = a * b.y;
    c.z = a * b.z;
    c.w = a * b.w;
    return c;
}

internal inline
vec4 operator*(vec4 b, f32 a) {
    vec4 c = { 0 };
    c.x = a * b.x;
    c.y = a * b.y;
    c.z = a * b.z;
    c.w = a * b.w;
    return c;
}

//matrix * vector multiplication
internal inline
vec4 operator*(mat4 a, vec4 b) {
    vec4 c = {0};
    c.x = a.m00*b.x + a.m01*b.y + a.m02*b.z + a.m03*b.w;
    c.y = a.m10*b.x + a.m11*b.y + a.m12*b.z + a.m13*b.w;
    c.z = a.m20*b.x + a.m21*b.y + a.m22*b.z + a.m23*b.w;
    c.w = a.m30*b.x + a.m31*b.y + a.m32*b.z + a.m33*b.w;
    return c;
}

internal inline
vec4 operator*(vec4 b, mat4 a) {
    vec4 c = {0};
    c.x = a.m00*b.x + a.m01*b.y + a.m02*b.z + a.m03*b.w;
    c.y = a.m10*b.x + a.m11*b.y + a.m12*b.z + a.m13*b.w;
    c.z = a.m20*b.x + a.m21*b.y + a.m22*b.z + a.m23*b.w;
    c.w = a.m30*b.x + a.m31*b.y + a.m32*b.z + a.m33*b.w;
    return c;
}

//NOTE: Not commutative.
internal inline
Quaternion operator*(Quaternion a, Quaternion b) {
    Quaternion c;
    c.w = ((a.w*b.w) - (a.x*b.x) - (a.y*b.y) - (a.z*b.z));
    c.x = ((a.w*b.x) + (a.x*b.w) + (a.y*b.z) - (a.z*b.y));
    c.y = ((a.w*b.y) - (a.x*b.z) + (a.y*b.w) + (a.z*b.x));
    c.z = ((a.w*b.z) + (a.x*b.y) - (a.y*b.x) + (a.z*b.w));
    return c;
}

internal inline
Quaternion quaternion_transform(Quaternion q, mat4 mat) {
    Quaternion result = {0};

    result.x = mat.m00*q.x + mat.m01*q.y + mat.m02*q.z + mat.m03*q.w;
    result.y = mat.m10*q.x + mat.m11*q.y + mat.m12*q.z + mat.m13*q.w;
    result.z = mat.m20*q.x + mat.m21*q.y + mat.m22*q.z + mat.m23*q.w;
    result.w = mat.m30*q.x + mat.m31*q.y + mat.m32*q.z + mat.m33*q.w;

    return result;
}

internal inline
vec4 operator/(vec4 a, vec4 b) {
    vec4 c = { 0 };
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    c.w = a.w / b.w;
    return c;
}

internal inline
bool operator==(vec2 a, vec2 b) {
    return (a.x == b.x && a.y == b.y);
}

internal inline
bool operator==(vec3 a, vec3 b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

internal inline
bool operator==(vec4 a, vec4 b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

//================================================
//Description: 4x4 matrix for graphics calculations.
//
//Comments: IMPORTANT! Matrices in OpenGL are
//	column-major. Accessing elements of this
//	matrix are done in the following way:
//------------------------------------------------
//		elements[column + row * 4]
//================================================
internal inline
mat4 identity() {
    return
            {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
            };
}

#if 0
internal inline
mat4 operator*(const mat4 a, const mat4 b) {
    mat4 c;
    c.m00 = a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02 + a.m30 * b.m03;
    c.m01 = a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02 + a.m31 * b.m03;
    c.m02 = a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02 + a.m32 * b.m03;
    c.m03 = a.m03 * b.m00 + a.m13 * b.m01 + a.m23 * b.m02 + a.m33 * b.m03;
    c.m10 = a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12 + a.m30 * b.m13;
    c.m11 = a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13;
    c.m12 = a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13;
    c.m13 = a.m03 * b.m10 + a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13;
    c.m20 = a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22 + a.m30 * b.m23;
    c.m21 = a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23;
    c.m22 = a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23;
    c.m23 = a.m03 * b.m20 + a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23;
    c.m30 = a.m00 * b.m30 + a.m10 * b.m31 + a.m20 * b.m32 + a.m30 * b.m33;
    c.m31 = a.m01 * b.m30 + a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33;
    c.m32 = a.m02 * b.m30 + a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33;
    c.m33 = a.m03 * b.m30 + a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33;
    return c;
}

internal inline
void operator*=(mat4& a, const mat4 b) {
    mat4 c;
    c.m00 = a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02 + a.m30 * b.m03;
    c.m01 = a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02 + a.m31 * b.m03;
    c.m02 = a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02 + a.m32 * b.m03;
    c.m03 = a.m03 * b.m00 + a.m13 * b.m01 + a.m23 * b.m02 + a.m33 * b.m03;
    c.m10 = a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12 + a.m30 * b.m13;
    c.m11 = a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13;
    c.m12 = a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13;
    c.m13 = a.m03 * b.m10 + a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13;
    c.m20 = a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22 + a.m30 * b.m23;
    c.m21 = a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23;
    c.m22 = a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23;
    c.m23 = a.m03 * b.m20 + a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23;
    c.m30 = a.m00 * b.m30 + a.m10 * b.m31 + a.m20 * b.m32 + a.m30 * b.m33;
    c.m31 = a.m01 * b.m30 + a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33;
    c.m32 = a.m02 * b.m30 + a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33;
    c.m33 = a.m03 * b.m30 + a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33;
    memcpy(a.elements, c.elements, 16 * sizeof(f32));
}

#else
internal inline
mat4 operator*(const mat4 a, const mat4 b) {
    mat4 c = {};
    c.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30;
    c.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30;
    c.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30;
    c.m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30;
    c.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31;
    c.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    c.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    c.m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    c.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32;
    c.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    c.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    c.m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    c.m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33;
    c.m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;
    c.m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;
    c.m33 = a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
    return c;
}

internal inline
void operator*=(mat4& a, const mat4 b) {
    mat4 c;
    c.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30;
    c.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30;
    c.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30;
    c.m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30;
    c.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31;
    c.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    c.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    c.m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    c.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32;
    c.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    c.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    c.m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    c.m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33;
    c.m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;
    c.m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;
    c.m33 = a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
    memcpy(a.elements, c.elements, 16 * sizeof(f32));
}
#endif

internal inline
mat4 translation(const f32 x, const f32 y, const f32 z) {
    mat4 mat = identity();
    mat.elements[3 + 0 * 4] = x;
    mat.elements[3 + 1 * 4] = y;
    mat.elements[3 + 2 * 4] = z;
    return mat;
}

internal inline
mat4 translation(const vec3 translation_vec) {
    return translation(translation_vec.x, translation_vec.y, translation_vec.z);
}

internal inline
mat4 scale(const f32 x, const f32 y, const f32 z) {
    mat4 mat = { 0 };
    mat.elements[0 + 0 * 4] = x;
    mat.elements[1 + 1 * 4] = y;
    mat.elements[2 + 2 * 4] = z;
    mat.elements[3 + 3 * 4] = 1.0f;
    return mat;
}

internal inline
mat4 scale(const vec3& scale_vec) {
    return scale(scale_vec.x, scale_vec.y, scale_vec.z);
}

internal inline
mat4 rotateX(f32 angle) {
    mat4 mat = identity();
    f32 theta = deg_to_rad(angle);
    f32 s = sin(theta);
    f32 c = cos(theta);

    mat.elements[1 + 1 * 4] = c;
    mat.elements[2 + 2 * 4] = c;
    mat.elements[2 + 1 * 4] = s;
    mat.elements[1 + 2 * 4] = -s;
    return mat;
}

internal inline
mat4 rotateY(f32 angle) {
    mat4 mat = identity();
    f32 theta = deg_to_rad(angle);
    f32 s = sin(theta);
    f32 c = cos(theta);

    mat.elements[0 + 0 * 4] = c;
    mat.elements[2 + 0 * 4] = -s;
    mat.elements[0 + 2 * 4] = s;
    mat.elements[2 + 2 * 4] = c;
    return mat;
}

internal inline
mat4 rotateZ(f32 angle) {
    mat4 mat = identity();
    f32 theta = deg_to_rad(angle);
    f32 s = sin(theta);
    f32 c = cos(theta);

    mat.elements[0 + 0 * 4] = c;
    mat.elements[1 + 0 * 4] = s;
    mat.elements[0 + 1 * 4] = -s;
    mat.elements[1 + 1 * 4] = c;
    return mat;
}

internal inline
mat4 rotation(f32 angle, f32 x, f32 y, f32 z) {
    mat4 mat = { 0 };
    f32 theta = deg_to_rad(angle);
    f32 s = sin(theta);
    f32 c = cos(theta);
    f32 cm1 = (1.0f - c);

    mat.elements[0 + 0 * 4] = x * x * cm1 + c;
    mat.elements[0 + 1 * 4] = y * x * cm1 + z * s;
    mat.elements[0 + 2 * 4] = x * z * cm1 - y * s;
    mat.elements[1 + 0 * 4] = x * y * cm1 - z * s;
    mat.elements[1 + 1 * 4] = y * y * cm1 + c;
    mat.elements[1 + 2 * 4] = y * z * cm1 + x * s;
    mat.elements[2 + 0 * 4] = x * z * cm1 + y * s;
    mat.elements[2 + 1 * 4] = y * z * cm1 - x * s;
    mat.elements[2 + 2 * 4] = z * z * cm1 + c;
    mat.elements[3 + 3 * 4] = 1.0f;
    return mat;
}

internal inline
mat4 rotation(f32 angle, const vec3 axis) {
    return rotation(angle, axis.x, axis.y, axis.z);
}

//2D Projection
internal inline
mat4 orthographic_projection(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane) {
    mat4 mat = { 0 };
    mat.elements[0 + 0 * 4] = 2.0f / (width - x);
    mat.elements[1 + 1 * 4] = 2.0f / (y - height);
    mat.elements[2 + 2 * 4] = -2.0f / (far_plane - near_plane);
    mat.elements[0 + 3 * 4] = -((width + x) / (width - x));
    mat.elements[1 + 3 * 4] = -((y + height) / (y - height));
    mat.elements[2 + 3 * 4] = -((far_plane + near_plane) / (far_plane - near_plane));
    mat.elements[3 + 3 * 4] = 1.0f;
    return mat;
}

//3D Projection
internal inline
mat4 perspective_projection(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
    mat4 mat = { 0 };
    float radFov = deg_to_rad(fov);
    mat.elements[0 + 0 * 4] = (1 / tan(radFov * 0.5f)) / aspect_ratio;
    mat.elements[1 + 1 * 4] = 1 / tan(radFov * 0.5f);
    mat.elements[2 + 2 * 4] = -((far_plane + near_plane) / (far_plane - near_plane));
    mat.elements[3 + 2 * 4] = -((2.0f * near_plane * far_plane) / (far_plane - near_plane));
    mat.elements[2 + 3 * 4] = -1.0f;
    return mat;
}

//Might have to swap the order of these
internal inline
mat4 quaternion_matrix(Quaternion q) {
    mat4 mat = identity();
    //q^2 + x^2 - y^2 - z^2
    mat.elements[0 + 0 * 4] = (q.w*q.w) + (q.x*q.x) - (q.y*q.y) - (q.z*q.z);
    //2xy - 2wz
    mat.elements[0 + 1 * 4] = (2 * (q.x*q.y)) - (2 * (q.w*q.z));
    //2xz + 2wy
    mat.elements[0 + 2 * 4] = (2 * (q.x*q.z)) + (2 * (q.w*q.y));
    //2xy + 2wz
    mat.elements[1 + 0 * 4] = (2 * (q.x*q.y)) + (2 * (q.w*q.z));
    return mat;
}

internal inline
mat4 create_transformation_matrix(f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ) {
    mat4 mat = identity();
    mat *= translation(x, y, z);
    mat *= rotation(rotX, 1, 0, 0);
    mat *= rotation(rotY, 0, 1, 0);
    mat *= rotation(rotZ, 0, 0, 1);
    mat *= scale(scaleX, scaleY, scaleZ);
    return mat;
}

internal inline
mat4 create_transformation_matrix(const vec3 translation, const vec3 rotation, const vec3 scale_vec) {
    return create_transformation_matrix(
            translation.x, translation.y, translation.z,
            rotation.x, rotation.y, rotation.z,
            scale_vec.x, scale_vec.y, scale_vec.z
    );
}

//ALGORITHMS

internal inline
mat4 inverse(const mat4 mat) {
    mat4 result;

    result.elements[0] =
            (mat.elements[5] * mat.elements[10] * mat.elements[15]) -
            (mat.elements[5] * mat.elements[11] * mat.elements[14]) -
            (mat.elements[9] * mat.elements[6] * mat.elements[15]) +
            (mat.elements[9] * mat.elements[7] * mat.elements[14]) +
            (mat.elements[13] * mat.elements[6] * mat.elements[11]) -
            (mat.elements[13] * mat.elements[7] * mat.elements[10]);

    result.elements[1] =
            (-mat.elements[1] * mat.elements[10] * mat.elements[15]) +
            (mat.elements[1] * mat.elements[11] * mat.elements[14]) +
            (mat.elements[9] * mat.elements[2] * mat.elements[15]) -
            (mat.elements[9] * mat.elements[3] * mat.elements[14]) -
            (mat.elements[13] * mat.elements[2] * mat.elements[11]) +
            (mat.elements[13] * mat.elements[3] * mat.elements[10]);

    result.elements[2] =
            (mat.elements[1] * mat.elements[6] * mat.elements[15]) -
            (mat.elements[1] * mat.elements[7] * mat.elements[14]) -
            (mat.elements[5] * mat.elements[2] * mat.elements[15]) +
            (mat.elements[5] * mat.elements[3] * mat.elements[14]) +
            (mat.elements[13] * mat.elements[2] * mat.elements[7]) -
            (mat.elements[13] * mat.elements[3] * mat.elements[6]);

    result.elements[3] =
            (-mat.elements[1] * mat.elements[6] * mat.elements[11]) +
            (mat.elements[1] * mat.elements[7] * mat.elements[10]) +
            (mat.elements[5] * mat.elements[2] * mat.elements[11]) -
            (mat.elements[5] * mat.elements[3] * mat.elements[10]) -
            (mat.elements[9] * mat.elements[2] * mat.elements[7]) +
            (mat.elements[9] * mat.elements[3] * mat.elements[6]);

    result.elements[4] =
            (-mat.elements[4] * mat.elements[10] * mat.elements[15]) +
            (mat.elements[4] * mat.elements[11] * mat.elements[14]) +
            (mat.elements[8] * mat.elements[6] * mat.elements[15]) -
            (mat.elements[8] * mat.elements[7] * mat.elements[14]) -
            (mat.elements[12] * mat.elements[6] * mat.elements[11]) +
            (mat.elements[12] * mat.elements[7] * mat.elements[10]);

    result.elements[5] =
            (mat.elements[0] * mat.elements[10] * mat.elements[15]) -
            (mat.elements[0] * mat.elements[11] * mat.elements[14]) -
            (mat.elements[8] * mat.elements[2] * mat.elements[15]) +
            (mat.elements[8] * mat.elements[3] * mat.elements[14]) +
            (mat.elements[12] * mat.elements[2] * mat.elements[11]) -
            (mat.elements[12] * mat.elements[3] * mat.elements[10]);

    result.elements[6] =
            (-mat.elements[0] * mat.elements[6] * mat.elements[15]) +
            (mat.elements[0] * mat.elements[7] * mat.elements[14]) +
            (mat.elements[4] * mat.elements[2] * mat.elements[15]) -
            (mat.elements[4] * mat.elements[3] * mat.elements[14]) -
            (mat.elements[12] * mat.elements[2] * mat.elements[7]) +
            (mat.elements[12] * mat.elements[3] * mat.elements[6]);

    result.elements[7] =
            (mat.elements[0] * mat.elements[6] * mat.elements[11]) -
            (mat.elements[0] * mat.elements[7] * mat.elements[10]) -
            (mat.elements[4] * mat.elements[2] * mat.elements[11]) +
            (mat.elements[4] * mat.elements[3] * mat.elements[10]) +
            (mat.elements[8] * mat.elements[2] * mat.elements[7]) -
            (mat.elements[8] * mat.elements[3] * mat.elements[6]);

    result.elements[8] =
            (mat.elements[4] * mat.elements[9] * mat.elements[15]) -
            (mat.elements[4] * mat.elements[11] * mat.elements[13]) -
            (mat.elements[8] * mat.elements[5] * mat.elements[15]) +
            (mat.elements[8] * mat.elements[7] * mat.elements[13]) +
            (mat.elements[12] * mat.elements[5] * mat.elements[11]) -
            (mat.elements[12] * mat.elements[7] * mat.elements[9]);

    result.elements[9] =
            (-mat.elements[0] * mat.elements[9] * mat.elements[15]) +
            (mat.elements[0] * mat.elements[11] * mat.elements[13]) +
            (mat.elements[8] * mat.elements[1] * mat.elements[15]) -
            (mat.elements[8] * mat.elements[3] * mat.elements[13]) -
            (mat.elements[12] * mat.elements[1] * mat.elements[11]) +
            (mat.elements[12] * mat.elements[3] * mat.elements[9]);

    result.elements[10] =
            (mat.elements[0] * mat.elements[5] * mat.elements[15]) -
            (mat.elements[0] * mat.elements[7] * mat.elements[13]) -
            (mat.elements[4] * mat.elements[1] * mat.elements[15]) +
            (mat.elements[4] * mat.elements[3] * mat.elements[13]) +
            (mat.elements[12] * mat.elements[1] * mat.elements[7]) -
            (mat.elements[12] * mat.elements[3] * mat.elements[5]);

    result.elements[11] =
            (-mat.elements[0] * mat.elements[5] * mat.elements[11]) +
            (mat.elements[0] * mat.elements[7] * mat.elements[9]) +
            (mat.elements[4] * mat.elements[1] * mat.elements[11]) -
            (mat.elements[4] * mat.elements[3] * mat.elements[9]) -
            (mat.elements[8] * mat.elements[1] * mat.elements[7]) +
            (mat.elements[8] * mat.elements[3] * mat.elements[5]);

    result.elements[12] =
            (-mat.elements[4] * mat.elements[9] * mat.elements[14]) +
            (mat.elements[4] * mat.elements[10] * mat.elements[13]) +
            (mat.elements[8] * mat.elements[5] * mat.elements[14]) -
            (mat.elements[8] * mat.elements[6] * mat.elements[13]) -
            (mat.elements[12] * mat.elements[5] * mat.elements[10]) +
            (mat.elements[12] * mat.elements[6] * mat.elements[9]);

    result.elements[13] =
            (mat.elements[0] * mat.elements[9] * mat.elements[14]) -
            (mat.elements[0] * mat.elements[10] * mat.elements[13]) -
            (mat.elements[8] * mat.elements[1] * mat.elements[14]) +
            (mat.elements[8] * mat.elements[2] * mat.elements[13]) +
            (mat.elements[12] * mat.elements[1] * mat.elements[10]) -
            (mat.elements[12] * mat.elements[2] * mat.elements[9]);

    result.elements[14] =
            (-mat.elements[0] * mat.elements[5] * mat.elements[14]) +
            (mat.elements[0] * mat.elements[6] * mat.elements[13]) +
            (mat.elements[4] * mat.elements[1] * mat.elements[14]) -
            (mat.elements[4] * mat.elements[2] * mat.elements[13]) -
            (mat.elements[12] * mat.elements[1] * mat.elements[6]) +
            (mat.elements[12] * mat.elements[2] * mat.elements[5]);

    result.elements[15] =
            (mat.elements[0] * mat.elements[5] * mat.elements[10]) -
            (mat.elements[0] * mat.elements[6] * mat.elements[9]) -
            (mat.elements[4] * mat.elements[1] * mat.elements[10]) +
            (mat.elements[4] * mat.elements[2] * mat.elements[9]) +
            (mat.elements[8] * mat.elements[1] * mat.elements[6]) -
            (mat.elements[8] * mat.elements[2] * mat.elements[5]);

    f32 determinate =
            mat.elements[0] * result.elements[0] +
            mat.elements[1] * result.elements[4] +
            mat.elements[2] * result.elements[8] +
            mat.elements[3] * result.elements[12];

    if (determinate == 0) {
        LOG(WARNING, "Matrix could not be inverted! Determinate = 0");
        return result;
    }

    determinate = 1.0 / determinate;

    for (u16 i = 0; i < 16; ++i) {
        result.elements[i] *= determinate;
    }

    return result;
}

internal inline
bool point_inside_triangle(vec3 point, vec3 tri1, vec3 tri2, vec3 tri3) {
    vec3 u = tri2 - tri1;
    vec3 v = tri3 - tri1;
    vec3 w = point - tri1;
    vec3 n = cross(u, v);

    f32 y = (dot(cross(u, w), n) / dot(n, n));
    f32 b = (dot(cross(u, w), n) / dot(n, n));
    f32 a = 1 - y - b;
    vec3 p = (a * tri1) + (b * tri2) + (y * tri3);

    return (a >= 0 && a <= 1 && b >= 0 && b <= 1 && y >= 0 && y <= 1);
}

internal inline
mat4 look_at(const vec3 camera, const vec3 center, const vec3 up = V3(0, 1, 0)) {
    mat4 mat = identity();

    vec3 dir = normalize(center - camera);
    vec3 upNorm = normalize(up);
    vec3 right = normalize(cross(dir, upNorm));
    upNorm = cross(right, dir);

    mat.m00 = right.x;
    mat.m01 = right.y;
    mat.m02 = right.z;
    mat.m10 = upNorm.x;
    mat.m11 = upNorm.y;
    mat.m12 = upNorm.z;
    mat.m20 = -dir.x;
    mat.m21 = -dir.y;
    mat.m22 = -dir.z;
    mat.m03 = -dot(right, camera);
    mat.m13 = -dot(upNorm, camera);
    mat.m23 = dot(dir, camera);

    return mat;
}

struct Camera {
    f32 x;
    f32 y;
    f32 z;
    f32 pitch;
    f32 yaw;
    f32 roll;
};

internal inline
mat4 create_view_matrix(Camera cam) {
    mat4 mat = identity();
    mat *= rotation(cam.pitch, 1, 0, 0);
    mat *= rotation(cam.yaw, 0, 1, 0);
    mat *= rotation(cam.roll, 0, 0, 1);
    mat *= translation(-cam.x, -cam.y, -cam.z);
    return mat;
}

internal inline
void move_cam_front(Camera* cam, f32 units) {
    cam->z -= cos(deg_to_rad(cam->yaw))   * units;
    cam->x += sin(deg_to_rad(cam->yaw))   * units;
}

internal inline
void move_cam_behind(Camera* cam, f32 units) {
    cam->z += cos(deg_to_rad(cam->yaw))   * units;
    cam->x -= sin(deg_to_rad(cam->yaw))   * units;
}

internal inline
void move_cam_forward(Camera* cam, f32 units) {
    cam->z -= cos(deg_to_rad(cam->yaw))   * units;
    cam->x += sin(deg_to_rad(cam->yaw))   * units;
    cam->y -= sin(deg_to_rad(cam->pitch)) * units;
}

internal inline
void move_cam_backwards(Camera* cam, f32 units) {
    cam->z += cos(deg_to_rad(cam->yaw))   * units;
    cam->x -= sin(deg_to_rad(cam->yaw))   * units;
    cam->y += sin(deg_to_rad(cam->pitch)) * units;
}

internal inline
void move_cam_right(Camera* cam, f32 units) {
    cam->x += cos(deg_to_rad(cam->yaw)) * units;
    cam->z += sin(deg_to_rad(cam->yaw)) * units;
}

internal inline
void move_cam_left(Camera* cam, f32 units) {
    cam->x -= cos(deg_to_rad(cam->yaw)) * units;
    cam->z -= sin(deg_to_rad(cam->yaw)) * units;
}

internal inline
f32 lerp(f32 t, f32 a1, f32 a2) {
    return a1 * t*(a2-a1);
}

internal inline
f32 fade(f32 t) {
    return ((6*t - 15)*t + 10)*t*t*t;
}

internal inline
vec3 direction_to_angles(vec3 direction) {
    vec3 ret;
    f32 r = length(direction);
    f32 theta = atan2(direction.y, direction.x);
    f32 phi = acos(direction.z / r);

    ret.x = r * cos(theta) * sin(phi);
    ret.y = r * sin(theta) * sin(phi);
    ret.z = r * cos(phi);

    return ret;
}

internal inline
vec3 convert_to_spherical_coords(vec3 direction) {
    vec3 ret;
    f32 r = length(direction);
    f32 theta = atan2(direction.y, direction.x);
    f32 phi = acos(direction.z / r);

    ret.x = r * cos(theta) * sin(phi);
    ret.y = r * sin(theta) * sin(phi);
    ret.z = r * cos(phi);

    return ret;
}

internal inline
vec3 raycast(mat4 projection, Camera camera, vec2 mouse, Rect viewport) {
    mat4 view = create_view_matrix(camera);
    view = look_at({camera.x, camera.y, camera.z}, {0, 0, 0});

    f32 x = (2.0f * mouse.x) / viewport.width - 1.0f;
    f32 y = (2.0f * mouse.y) / viewport.height - 1.0f;
    //f32 y = 1.0f - (2.0f * mouse.y) / viewport.height;
    vec3 nds = {x, -y, 1.0f};

    vec4 clip = {nds.x, nds.y, -1.0f, 1.0f};
    mat4 invertedProj = inverse(projection);
    vec4 eyeCoords = invertedProj * clip;
    vec4 clippedEyeCoords = {eyeCoords.x, eyeCoords.y, -1.0f, 0.0f};

    mat4 invertedView = inverse(view);
    vec4 rayWorld = invertedView * clippedEyeCoords;
    vec3 mouseRay = {rayWorld.x, rayWorld.y, rayWorld.z};
    normalize(&mouseRay);
    return mouseRay;
}

internal inline
bool ray_tri_collision(vec3 o, vec3 d, vec3 tri0, vec3 tri1, vec3 tri2, vec3* intersectionPoint) {
    const float EPSILON = 0.0000001;
    vec3 edge1 = tri1 - tri0;
    vec3 edge2 = tri2 - tri0;
    vec3 h = cross(d, edge2);

    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;

    float f = 1.0f/a;

    vec3 s = o - tri0;
    float u = f * dot(s, h);
    if(u < 0.0 || u > 1.0)
        return false;

    vec3 q = cross(s, edge1);
    float v = f * dot(d, q);

    if(v < 0.0 || u + v > 1.0)
        return false;

    float t = f * dot(edge2, q);
    if(t > EPSILON) {
        if(intersectionPoint != NULL)
            *intersectionPoint = o + d * t;
        return true;
    } else {
        return false;
    }

    return true;
}

internal inline
bool ray_aabb_collision(vec3 o, vec3 d, vec3 min, vec3 max, vec3* intersectionPoint) {
    float t1 = (min.x -o.x) / d.x;
    float t2 = (max.x -o.x) / d.x;
    float t3 = (min.y -o.y) / d.y;
    float t4 = (max.y -o.y) / d.y;
    float t5 = (min.z -o.z) / d.z;
    float t6 = (max.z -o.z) / d.z;

    float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0) {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax) {
        return false;
    }

    if (tmin < 0.0f) {
        return true;
    }
    return tmin;
}

internal inline
bool ray_collides(vec3 camera, vec3 ray, vec3 pos, f32 radius) {
    vec3 OC = camera - pos;
    f32 b = dot(ray, OC);
    f32 c = dot(OC, OC) - (radius * radius);
    //printf("num: %f\n", (b * b) - c);
    return (b * b) - c >= 0;

    //if I wanted to find the points of intersection:
    //t1 = -b + sqrt((b*b)-c);
    //t2 = -b - sqrt((b*b)-c);
}

#include <vector>
#include <cmath>

internal inline
vec2 get_constant_vector(int v) {
    int h = v & 3;

    if(h == 0)
        return {1, 1};
    else if(h == 1)
        return {-1, 1};
    else if(h == 2)
        return {-1, -1};
    else
        return {1, -1};
}

internal inline
f32 noise2D(f32 x, f32 y, std::vector<f32> P) {
    f32 x2 = (i32)std::floor(x) & 255;
    f32 y2 = (i32)std::floor(y) & 255;

    f32 xf = x - std::floor(x);
    f32 yf = y - std::floor(y);

    vec2 topRight = {(f32)xf-1.0f, (f32)yf-1.0f};
    vec2 topLeft = {(f32)xf, (f32)yf-1.0f};
    vec2 bottomRight = {(f32)xf-1.0f, (f32)yf};
    vec2 bottomLeft = {(f32)xf, (f32)yf};

    f32 valueTopRight = P[P[x2+1]+y2+1];
    f32 valueTopLeft = P[P[x2]+y2+1];
    f32 valueBottomRight = P[P[x2+1]+y2];
    f32 valueBottomLeft = P[P[x2]+y2];

    f32 dotTopRight = dot(topRight, get_constant_vector(valueTopRight));
    f32 dotTopLeft = dot(topLeft, get_constant_vector(valueTopLeft));
    f32 dotBottomRight = dot(bottomRight, get_constant_vector(valueBottomRight));
    f32 dotBottomLeft = dot(bottomLeft, get_constant_vector(valueBottomLeft));

    f32 u = fade(xf);
    f32 v = fade(yf);

    return lerp(u, lerp(v, dotBottomLeft, dotTopLeft), lerp(v, dotBottomRight, dotTopRight));
}

internal inline
vec3* perlin_noise(int width, int height) {
    vec3* image = new vec3[width * height];

    std::vector<f32> P;
    for(int i = 0; i < 256; ++i)
        P.push_back(i);

    for(int i = P.size()-1; i > 0; i--) {
        int index = std::floor(rand() % i);
        f32 temp = P[i];
        P[i] = P[index];
        P[index] = temp;
    }

    for(int i = 0; i < 256; ++i)
        P.push_back(P[i]);

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            f32 n = noise2D(x*0.01, y*0.01, P);
            n+=1.0f;
            n/=2.0f;
            int c = std::floor(255*n);
            image[x + y * width] = {(f32)c, (f32)c, (f32)c};
        }
    }
    return image;
}

#endif
