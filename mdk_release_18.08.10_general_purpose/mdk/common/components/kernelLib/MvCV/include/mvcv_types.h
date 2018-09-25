#ifndef __CV_TYPES_H__
#define __CV_TYPES_H__

//Guard against inclusion in applications which contain standard OpenCV code
//#ifndef __OPENCV_OLD_CV_H__


#ifndef _WIN32
#include <stddef.h>
#endif

#include <assert.h>
#include <VectorTypes.h>
#include <mv_types.h>

///TODO
#ifndef __MOVICOMPILE__
#define MVCV_STATIC_DATA __attribute((section(".mvcv_static_data")))
#else
#define MVCV_STATIC_DATA
#endif

#ifndef DBL_EPSILON
	#define DBL_EPSILON     2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */
#endif
#define CV_PI   3.1415926535897932384626433832795

/** Flags used by Optical Flow */
#define MVCV_TERMCRIT_ITER	1
#define MVCV_TERMCRIT_EPS		2

#define  MVCV_LKFLOW_PYR_A_READY       1
#define  MVCV_LKFLOW_PYR_B_READY       2
#define  MVCV_LKFLOW_INITIAL_GUESSES   4
#define  MVCV_LKFLOW_GET_MIN_EIGENVALS 8
#define  MVCV_LKFLOW_PADDED_PYRAMIDS  16

#define MVCV_OK				0
#define MVCV_BADFLAG_ERR		-12
#define	MVCV_BADRANGE_ERR		-44
#define	MVCV_NULLPTR_ERR		-2
#define MVCV_BADSIZE_ERR		-1
/******************************************************************/

#define	DIM					4104
#define VecOp			8
#define castOp(x)		((x + (1 << (VecOp - 1))) >> VecOp)

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

#ifndef MVCV_MAT_DEPTH
#define MVCV_MAT_DEPTH(flags)     ((flags) & ((1 << 3) - 1))
#endif
#ifndef MVCV_MAKETYPE
#define MVCV_MAKETYPE(depth,cn)	(MVCV_MAT_DEPTH(depth) + (((cn)-1) << 3))
#endif

/* 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem)) */
#define MVCV_ELEM_SIZE(type)		(MVCV_MAT_CN(type) << ((((sizeof(size_t)/4+1)*16384|0x3a50) >> MVCV_MAT_DEPTH(type)*2) & 3))

#define MVCV_MAT_CN_MASK          ((MVCV_CN_MAX - 1) << MVCV_CN_SHIFT)
#define MVCV_MAT_CN(flags)        ((((flags) & MVCV_MAT_CN_MASK) >> MVCV_CN_SHIFT) + 1)
#define MVCV_MAT_TYPE_MASK        (MVCV_DEPTH_MAX*MVCV_CN_MAX - 1)
#define MVCV_MAT_TYPE(flags)      ((flags) & MVCV_MAT_TYPE_MASK)
#define MVCV_MAT_CONT_FLAG_SHIFT  14
#define MVCV_MAT_CONT_FLAG        (1 << MVCV_MAT_CONT_FLAG_SHIFT)
#define MVCV_IS_MAT_CONT(flags)   ((flags) & MVCV_MAT_CONT_FLAG)
#define MVCV_IS_CONT_MAT          MVCV_IS_MAT_CONT
#define MVCV_SUBMAT_FLAG_SHIFT    15
#define MVCV_SUBMAT_FLAG          (1 << MVCV_SUBMAT_FLAG_SHIFT)
#define MVCV_IS_SUBMAT(flags)     ((flags) & MVCV_MAT_SUBMAT_FLAG)

enum { MAGIC_VAL = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = MVCV_MAT_CONT_FLAG, SUBMATRIX_FLAG = MVCV_SUBMAT_FLAG };


/******************************** CvSize's & CvBox **************************************/

/**
 * Point types mapped on vector types
 */
#ifdef __MOVICOMPILE__

typedef int2	CLPoint32i;
typedef short2	CLPoint16i;
typedef ushort2	CLPoint16ui;
typedef float2	CLPoint32f;
typedef half2	CLPoint16f;

//Working vectorized types
typedef int4 CLPoint32iW;
typedef short4 CLPoint16iW;
typedef ushort4 CLPoint16uiW;
typedef float4 CLPoint32fW;
typedef half4 CLPoint16fW;
#else

#ifdef __PC__

template<typename T> class ClPoint
{
public:
	T x;
    T y;

	inline ClPoint()
	{}

	inline ClPoint(T x0, T y0)
	{
		x = x0;
		y = y0;
	}
};

typedef ClPoint<u32>	CLPoint32u;
typedef ClPoint<s32>	CLPoint32i;
typedef ClPoint<u16>	CLPoint16ui;
typedef ClPoint<fp32>	CLPoint32f;
typedef ClPoint<fp16>	CLPoint16f;

template<typename T> ClPoint<T> operator + (ClPoint<T> a, ClPoint<T> b)
{
	ClPoint<T> res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;

	return res;
}

template<typename T> ClPoint<T> operator - (ClPoint<T> a, ClPoint<T> b)
{
	ClPoint<T> res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;

	return res;
}

template<typename T> ClPoint<T> operator - (ClPoint<T> a, T b)
{
	ClPoint<T> res;

	res.x = a.x - b;
	res.y = a.y - b;

    return res;
}

template<typename T> ClPoint<T> operator + (ClPoint<T> a, T b)
{
	ClPoint<T> res;

	res.x = a.x + b;
	res.y = a.y + b;

    return res;
}
//template<typename T> ClPoint<T> operator * (fp32 s)
//{
//	ClPoint<T> res;
//
//	res.x = a.x * s;
//	res.y = a.y * s;
//
//	return res;
//}

#endif

#endif

/*struct CvPoint
{
    int x;
    int y;

	inline CvPoint()
	{}

	inline CvPoint(int x0, int y0)
	{
		x = x0;
		y = y0;
	}
};

struct CvPoint2D32f
{
    float x;
    float y;

	inline CvPoint2D32f()
	{}

	inline CvPoint2D32f(float x0, float y0)
	{
		x = x0;
		y = y0;
	}
};*/

struct mvCvRect
{
    int x;
    int y;
    int width;
    int height;
};

#ifndef __PC__

typedef int2 ClSize;
//Working version
typedef int4 ClSizeW;

#else

struct ClSize
{
    int x;
    int y;

	inline ClSize()
	{}

	inline ClSize(int width0, int height0)
	{
		x = width0;
		y = height0;
	}

    template<typename T>
    ClSize& operator = (const ClPoint<T>& x) {
        this->x = x.x;
        this->y = x.y;
        return *this;
    }
};

inline ClSize operator * (ClSize a, int s)
{
	ClSize res;

	res.x = a.x * s;
	res.y = a.y * s;

	return res;
}

inline ClSize operator + (ClSize a, int s)
{
	ClSize res;

	res.x = a.x + s;
	res.y = a.y + s;

	return res;
}

template<typename T> ClPoint<T> operator - (ClPoint<T> a, ClSize b)
{
	ClPoint<T> res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;

	return res;
}

template<typename T> ClPoint<T>& operator -= (ClPoint<T>& a, const ClSize& b)
{
    a.x = a.x - b.x;
	a.y = a.y - b.y;

	return a;
}

#endif

struct mvCvSize
{
    int width;
    int height;

	mvCvSize();

	mvCvSize(int width0, int height0);
};


#ifdef __MOVICOMPILE__

typedef float4 mvScalar;

#else

struct mvScalar
{
	float val[4];

	inline mvScalar()
	{}

	inline mvScalar(float val0, float val1, float val2, float val3)
	{
		val[0] = val0;
		val[1] = val1;
		val[2] = val2;
		val[3] = val3;
	}

	inline mvScalar(float val0)
	{
		val[0] = val0;
	}

	inline float& operator [] (const int i)
	{
		return val[i];
	}
};

#endif

struct mvRect
{
	int x, y, width, height;

	inline mvRect()
	{}

	inline mvRect(int x0, int y0, int width0, int height0)
	{
		x = x0;
		y = y0;
		width = width0;
		height = height0;
	}
};

struct mvRange
{
	int start, end;

	inline mvRange()
	{}

	inline mvRange(int start0, int end0)
	{
		assert(end0 > start0);

		start = start0;
		end = end0;
	}
};

/*********************************** CvTermCriteria *************************************/

struct mvCvTermCriteria
{
    int		type;  /* may be combination of
                     CV_TERMCRIT_ITER
                     CV_TERMCRIT_EPS */
    u32		max_iter;
    fp32	epsilon;
};

struct mvCvMat
{
    int type;
    int step;
    u8* ptr;
    int width;
    int height;
};

union mvCv32suf
{
    int i;
    unsigned u;
    float f;
};

typedef CLPoint32i		mvCvPoint;
typedef CLPoint16ui		mvCvPoint2D16ui;
typedef CLPoint32f		mvCvPoint2D32f;

typedef mvCvPoint2D32f	mvPoint2f;
typedef mvCvPoint			mvPoint;
typedef mvCvPoint			mvPoint2i;

//Working *4 versions
#ifdef __PC__
typedef ClSize          ClSizeW;
typedef CLPoint32i      CLPoint32iW;
typedef CLPoint16ui  	CLPoint16uiW;
typedef CLPoint32f      CLPoint32fW;
#endif
typedef CLPoint32iW		mvCvPointW;
typedef CLPoint16uiW	mvCvPoint2D16uiW;
typedef CLPoint32fW		mvCvPoint2D32fW;

typedef mvCvPoint2D32fW	mvPoint2fW;
typedef mvCvPointW		mvPointW;
typedef mvCvPointW		mvPoint2iW;

/**
 * These functions construct basic data types
 */
mvCvSize mvcvSize(int width, int height);
ClSize clSize(int width, int height);
ClSizeW clSizeW(int width, int height);
mvScalar mvcvScalar(float s0, float s1, float s2, float s3);
mvCvPoint mvcvPoint(int x, int y);
mvCvPointW mvcvPointW(int x, int y);
mvCvPoint2D32f mvcvPoint2D32f(float x, float y);
mvCvPoint2D32fW mvcvPoint2D32fW(float x, float y);
mvCvRect mvcvRect(int x, int y, int width, int height);
mvCvTermCriteria mvcvTermCriteria(int type, int max_iter, float epsilon);
int mvcvRound(float value);
int mvcvFloor(float value);
int mvcvCeil(float value);
int mvcvAlign(int size, int align);

mvCvMat mvcvMat( int height, int width, int type);
mvCvMat mvcvMat(mvCvSize size, u8* data, int type);
extern "C"
void mvcvSetData( mvCvMat* arr, u8* data, int step);

class Mat;
extern "C"
void mvcvSetData1(Mat* mat, int rows, int cols, u8* const data, int type);

//Threshold types
enum
{
    MVCV_THRESH_BINARY      = 0,  /* value = value > threshold ? max_value : 0       */
    MVCV_THRESH_BINARY_INV  = 1,  /* value = value > threshold ? 0 : max_value       */
    MVCV_THRESH_TRUNC       = 2,  /* value = value > threshold ? threshold : value   */
    MVCV_THRESH_TOZERO      = 3,  /* value = value > threshold ? value : 0           */
    MVCV_THRESH_TOZERO_INV  = 4,  /* value = value > threshold ? 0 : value           */
    MVCV_THRESH_MASK        = 7,
    MVCV_THRESH_OTSU        = 8  /* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CV_THRESH_* values */
};

//Border types
#define IPL_BORDER_CONSTANT		0
#define IPL_BORDER_REPLICATE	1
#define IPL_BORDER_REFLECT		2
#define IPL_BORDER_WRAP			3
#define IPL_BORDER_REFLECT_101  4
#define IPL_BORDER_TRANSPARENT  5

#endif
