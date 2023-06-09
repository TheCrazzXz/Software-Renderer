#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

#include <iostream>

#define M_PI 3.14159

#define NEAR_Z 0.5
#define FAR_Z 2.5

struct Vector
{
    float x;
    float y;
    float z;
    float w;

    Vector() : x( 0 ), y( 0 ), z( 0 ), w( 1 ) {}
    Vector( float a, float b, float c ) : x( a ), y( b ), z( c ), w( 1 ) {}

    /* Assume proper operator overloads here, with vectors and scalars */
    float Length() const
    {
        return std::sqrt( x*x + y*y + z*z );
    }
    Vector& operator*=(float fac) noexcept
    {
        x *= fac;
        y *= fac;
        z *= fac;
        return *this;
    }
    Vector  operator*(float fac) const noexcept
    {
        return Vector(*this)*=fac;
    }
    Vector& operator/=(float div) noexcept
    {
        return operator*=(1/div);   // avoid divisions: they are much
                                    // more costly than multiplications
    }

    Vector Unit() const
    {
        const float epsilon = 1e-6;
        float mag = Length();
        if (mag < epsilon) {
            std::out_of_range e( "" );
            throw e;
        }
        return Vector(*this)/=mag;
    }
};

inline float Dot( const Vector& v1, const Vector& v2 )
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

class Matrix
{
public:
    Matrix() : data( 16 )
    {
        Identity();
    }
    void Identity()
    {
        std::fill( data.begin(), data.end(), float( 0 ) );
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }
    float& operator[]( size_t index )
    {
        if (index >= 16) {
            std::out_of_range e( "" );
            throw e;
        }
        return data[index];
    }
    const float& operator[]( size_t index ) const
    {
        if (index >= 16) {
            std::out_of_range e( "" );
            throw e;
        }
        return data[index];
    }
    Matrix operator*( const Matrix& m ) const
    {
        Matrix dst;
        int col;
        for (int y = 0; y<4; ++y) {
            col = y * 4;
            for (int x = 0; x<4; ++x) {
                for (int i = 0; i<4; ++i) {
                    dst[x + col] += m[i + col] * data[x + i * 4];
                }
            }
        }
        return dst;
    }
    Matrix& operator*=( const Matrix& m )
    {
        *this = (*this) * m;
        return *this;
    }

    /* The interesting stuff */
    void SetupClipMatrix( float fov, float aspectRatio )
    {
        Identity();
        float f = 1.0f / std::tan( fov * 0.5f );
        data[0] = f*aspectRatio;
        data[5] = f;
        data[10] = (FAR_Z + NEAR_Z) / (FAR_Z- NEAR_Z);
        data[11] = 1.0f; /* this 'plugs' the old z into w */
        data[14] = (2.0f*NEAR_Z*FAR_Z) / (NEAR_Z - FAR_Z);
        data[15] = 0.0f;
    }

    std::vector<float> data;
};


inline Vector operator*( const Vector& v, Matrix& m )
{
    Vector dst;
    dst.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + v.w*m[12];
    dst.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + v.w*m[13];
    dst.z = v.x*m[2] + v.y*m[6] + v.z*m[10] + v.w*m[14];
    dst.w = v.x*m[3] + v.y*m[7] + v.z*m[11] + v.w*m[15];
    return dst;
}

typedef std::vector<Vector> VecArr;
VecArr ProjectAndClip( int width, int height, const VecArr& vertex )
{
    float halfWidth = (float)width * 0.5f;
    float halfHeight = (float)height * 0.5f;
    float aspect = (float)width / (float)height;
    Vector v;
    Matrix clipMatrix;
    VecArr dst;
    clipMatrix.SetupClipMatrix( 60.0f * (M_PI / 180.0f), aspect);
    /*  Here, after the perspective divide, you perform Sutherland-Hodgeman clipping
    by checking if the x, y and z components are inside the range of [-w, w].
    One checks each vector component seperately against each plane. Per-vertex
    data like colours, normals and texture coordinates need to be linearly
    interpolated for clipped edges to reflect the change. If the edge (v0,v1)
    is tested against the positive x plane, and v1 is outside, the interpolant
    becomes: (v1.x - w) / (v1.x - v0.x)
    I skip this stage all together to be brief.
    */
    for (VecArr::const_iterator i = vertex.begin(); i != vertex.end(); ++i) {
        v = (*i) * clipMatrix;
        v /= v.w; /* Don't get confused here. I assume the divide leaves v.w alone.*/
        dst.push_back( v );
    }

    /* TODO: Clipping here */

    for (VecArr::iterator i = dst.begin(); i != dst.end(); ++i) {
        i->x = (i->x * (float)width) / (2.0f * i->w) + halfWidth;
        i->y = (i->y * (float)height) / (2.0f * i->w) + halfHeight;
    }
    return dst;
}

int main(void)
{
    VecArr vertices = {Vector{0, 0, 0}};

    VecArr Verts_2d = ProjectAndClip(800, 640, vertices);

    for (VecArr::iterator i = Verts_2d.begin(); i != Verts_2d.end(); ++i) {
        std::cout << "Vector : x=" << i->x << " ; y=" << i->y << " ; z=" << i->z << " ;w=" << i->w << std::endl;
    }
}