#ifndef CVECTOR3_H
#define CVECTOR3_H

#include <float.h>
#include <math.h>

namespace cAudio
{
	inline bool float_equals(const float a, const float b)
	{
		return (a + FLT_EPSILON >= b) && (a - FLT_EPSILON <= b);
	}

	class cVector3
	{
	public:
		float x, y, z;

		cVector3(void) : x(0), y(0), z(0)
		{
		}

		cVector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
		{
		}

		cVector3(float n) : x(n), y(n), z(n)
		{
		}

		cVector3(const cVector3& other) : x(other.x), y(other.y), z(other.z)
		{
		}

		cVector3 operator-() const { return cVector3(-x, -y, -z); }
		cVector3& operator=(const cVector3& other) { x = other.x; y = other.y; z = other.z; return *this; }
		cVector3 operator+(const cVector3& other) const { return cVector3(x + other.x, y + other.y, z + other.z); }
		cVector3& operator+=(const cVector3& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
		cVector3 operator+(const float val) const { return cVector3(x + val, y + val, z + val); }
		cVector3& operator+=(const float val) { x+=val; y+=val; z+=val; return *this; }

		cVector3 operator-(const cVector3& other) const { return cVector3(x - other.x, y - other.y, z - other.z); }
		cVector3& operator-=(const cVector3& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
		cVector3 operator-(const float val) const { return cVector3(x - val, y - val, z - val); }
		cVector3& operator-=(const float val) { x-=val; y-=val; z-=val; return *this; }

		cVector3 operator*(const cVector3& other) const { return cVector3(x * other.x, y * other.y, z * other.z); }
		cVector3& operator*=(const cVector3& other) { x*=other.x; y*=other.y; z*=other.z; return *this; }
		cVector3 operator*(const float v) const { return cVector3(x * v, y * v, z * v); }
		cVector3& operator*=(const float v) { x*=v; y*=v; z*=v; return *this; }

		cVector3 operator/(const cVector3& other) const { return cVector3(x / other.x, y / other.y, z / other.z); }
		cVector3& operator/=(const cVector3& other) { x/=other.x; y/=other.y; z/=other.z; return *this; }
		cVector3 operator/(const float v) const { float i=(float)1.0/v; return cVector3(x * i, y * i, z * i); }
		cVector3& operator/=(const float v) { float i=(float)1.0/v; x*=i; y*=i; z*=i; return *this; }

		bool operator<=(const cVector3& other) const { return x<=other.x && y<=other.y && z<=other.z;}
		bool operator>=(const cVector3& other) const { return x>=other.x && y>=other.y && z>=other.z;}
		bool operator<(const cVector3& other) const { return x<other.x && y<other.y && z<other.z;}
		bool operator>(const cVector3& other) const { return x>other.x && y>other.y && z>other.z;}

		bool operator==(const cVector3& other) const
		{
			return float_equals(x, other.x) &&
					float_equals(y, other.y) &&
					float_equals(z, other.z);
		}

		bool operator!=(const cVector3& other) const
		{
			return !(float_equals(x, other.x) &&
					float_equals(y, other.y) &&
					float_equals(z, other.z));
		}

		operator const float*() const { return &x; }

		operator float*() { return &x; }

		const float operator[] ( int i ) const { return ( ( float* ) &x ) [i]; }

		float &operator[] ( int i ) { return ( ( float* ) &x ) [i]; }

		float length() const
		{
			return sqrtf( x*x + y*y + z*z );
		}

		void normalize()
		{
			float invLen = 1.0f / length();
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		float dot( const cVector3& other ) const
		{
			return ( x * other.x + y * other.y + z * other.z );
		}

		cVector3 cross( const cVector3& other ) const
		{
			return cVector3( y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x );
		}

		void set( float nx, float ny, float nz )
		{
			x = nx;
			y = ny;
			z = nz;
		}

		void set( float n )
		{
			x = y = z = n;
		}

		void set( const cVector3& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		void getAsArray(float* output)
		{
			output[0] = x;
			output[1] = y;
			output[2] = z;
		}
	};
};

#endif //! CVECTOR3_H