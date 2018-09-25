#include <string.h>
#include <stdlib.h>

#include "mat.h"
#include "mvcv_types.h"

void divide(const Mat& src1, const Mat& src2, Mat& dst, float scale = 1)
{
    const float* s1;
    const float* s2;
    float* d;
    int w = src1.cols;
    int h = src1.rows;

    //Check images have same size
    assert((src1.cols == src2.cols) && (src1.rows == src2.rows));
    assert((src2.cols == dst.cols) && (src2.rows == dst.rows));

    for (int i = 0; i < h; i++)
    {
        //Move row pointers
        s1 = (const float*)src1.ptr(i);
        s2 = (const float*)src2.ptr(i);
        d = (float*)dst.ptr(i);

        //Compute
        for (int j = 0; j < w; j++)
            d[j] = s1[j] * scale / s2[j];
    }
}

static void setSize(Mat& m)
{
    size_t esz = CV_ELEM_SIZE(m.flags);

    m.size[0] = m.rows;
    m.size[1] = m.cols;
    m.step0[0] = m.cols * esz;
    m.step0[1] = esz;
    m.step = m.step0[0];
}

static void initHeader(Mat& m, int _rows, int _cols, int _type)
{
    m.rows = _rows;
    m.cols = _cols;
    m.type = _type;
    m.data = NULL;
    m.flags = (m.type & CV_MAT_TYPE_MASK) | MAGIC_VAL;

    setSize(m);
}

static void createMat(Mat& m, int _rows, int _cols, int _type, bool allocate)
{
    assert(_rows > 0 && _cols > 0);

    initHeader(m, _rows, _cols, _type);

    if (allocate)
    {
        m.data = (u8*)malloc(m.rows * m.cols * m.step0[1]);
        assert(m.data != NULL);
    }

    m.allocated = allocate;
}

mvCvMat mvcvMat(int height, int width, int type)
{
    mvCvMat m;

    type = 0;
    m.type = 0x42420000 | (1<<14) | type;
    m.width = width;
    m.height = height;
    m.step = m.width*CV_ELEM_SIZE(type);
    m.ptr = NULL;

    return m;
}

Mat::Mat() : rows(0), cols(0), type(0), data(0)
{
}

Mat::Mat(const Mat& m)
{
    this->rows = m.rows;
    this->cols = m.cols;
    this->type = m.type;
    this->flags = m.flags;
    this->step0[0] = m.step0[0];
    this->step0[1] = m.step0[1];
    this->step = m.step0[0];
    this->size[0] = m.step0[0];
    this->size[1] = m.step0[1];
    this->data = m.data;

    /*if (this->data != NULL)
    {
        this->data = (u8*)malloc(rows * cols * step0[1]);
        assert(this->data != NULL);
        memcpy(this->data, m.data, rows * cols * step0[1]);
    }*/
}

Mat::Mat(s32 _rows, s32 _cols, s32 _type, u8* _data, bool allocate, size_t _step)
{
    createMat(*this, _rows, _cols, _type, (_data == NULL) ? allocate : false);
    if (_data != NULL)
        data = _data;
    if (_step != 0)
    {
        this->step0[0] = _step;
        this->step = this->step0[0];
    }
}


Mat::Mat(s32 _rows, s32 _cols, s32 _type, mvScalar s, u8* _data, bool allocate)
{
    createMat(*this, _rows, _cols, _type, (_data == NULL) ? allocate : false);
    if (_data != NULL)
        data = _data;

    //Fill the image with Scalar value
    u8* p_data = (u8*)data;
    for (int i = 0; i < rows; i++)
    {
        //p_data += i * cols;
        p_data = ptr(i);
        for (int j = 0; j < cols; j++)
            p_data[j] = s[0];
    }
}

Mat::Mat(const Mat& m, const mvRange& rowRange, const mvRange& colRange)
{
    //Create ROI image header
    createMat(*this, rowRange.end - rowRange.start, colRange.end - colRange.start, m.type, false);

    //Pointer to a ROI inside the big image
    this->data = m.data + rowRange.start * m.step0[0] + colRange.start * m.step0[1];

    //Step must be set according to the big image which holds the actual data
    size_t esz = CV_ELEM_SIZE(m.flags);
    this->step0[0] = m.cols * esz;
    this->step = this->step0[0];
}

Mat::~Mat()
{
    if (data != NULL && allocated)
        free(data);
}

void Mat::copyTo(Mat& dst)
{
    //Create matrix header and allocate data
    dst.create(this->rows, this->cols, this->type);

    //Blindly copy all data
    memcpy(dst.data, this->data, this->size[0] * this->step0[0]);
}

void Mat::create(int _rows, int _cols, int _type)
{
    if (data != NULL)
        free(data);

    createMat(*this, _rows, _cols, _type, true);
}

void Mat::convertTo(Mat& m, int rtype, float alpha, float beta) const
{
    const u8* s;
    float* d;
    rtype += 1; //use that param only to not interfere with function API
    //Check images have same size
    assert(cols == m.cols && rows == m.rows);

    for (int i = 0; i < rows; i++)
    {
        //Move row pointers
        s = (const u8*)ptr(i);
        d = (float*)m.ptr(i);

        //Convert to float
        for (int j = 0; j < cols; j++)
            d[j] = (float)s[j] * alpha + beta;
    }
}

Mat Mat::mul(Mat& m, float scale)
{
    //Create new matrix to hold the result
    Mat r(m.rows, m.cols, m.type);
    int w = m.cols;
    int h = m.rows;
    float* row_a;
    float* row_b;
    float* row_c;

    for (int i = 0; i < h; i++)
    {
        //Advance row pointers
        row_a = (float*)this->ptr(i);
        row_b = (float*)m.ptr(i);
        row_c = (float*)r.ptr(i);

        //Multiply elements
        for (int j = 0; j < w; j++)
            row_c[j] = row_a[j] * row_b[j] * scale;
    }

    //Pass by value - calls copy constructor which creates a deep copy
    return r;
}

size_t Mat::depth() const
{
    return CV_MAT_DEPTH(this->flags);
}

size_t Mat::step1() const
{
    return this->step0[0];
}

Mat& Mat::operator = (const Mat& m)
{
//  createMat(*this, m.rows, m.cols, m.type, false);
//  this->data = m.data;
    memcpy(this, &m, sizeof(m));
    return *this;
}

Mat Mat::operator () (const mvRect& r)
{
    u8* p_data = this->data;

    p_data += r.y * this->step0[0] + r.x * this->step0[1];

    Mat tmp(r.height, r.width, this->type, p_data, false, this->step0[0]);

    return tmp;
}

Mat Mat::operator () (const mvRange& rowRange, const mvRange& colRange)
{
    return Mat(*this, rowRange, colRange);
}

Mat Mat::operator / (const Mat& m)
{
    Mat res(m.rows, m.cols, m.type);

    divide(*this, m, res);

    return res;
}

Mat Mat::operator / (const float m)
{
    Mat res(this->rows, this->cols, this->type);

    return mul(res, 1 / m);
}

float& Mat::at(int i0, int i1)
{
    return ((float*)(data + step0[0] * i0))[i1];
}

u8* Mat::ptr(int y)
{
    assert(y >= 0);

    return this->data + y * this->step0[0];
}

const u8* Mat::ptr(int y) const
{
    assert(y >= 0);

    return this->data + y * this->step0[0];
}

mvCvMat mvcvMat(mvCvSize size, u8* data, int type)
{
    mvCvMat m = mvcvMat(size.height, size.width, type);

    m.ptr = data;

    return m;
}

// Assigns external data to array
void mvcvSetData(mvCvMat* arr, u8* data, int step)
{
    int pix_size, min_step;

    mvCvMat* mat = arr;

    int type = 0;
    pix_size = CV_ELEM_SIZE(type);
    min_step = mat->width*pix_size;
    mat->step = step;

    mat->ptr = (u8*)data;
    mat->type = 0x42420000 | type | (mat->height == 1 || mat->step == min_step ? (1<<14) : 0);
}

void mvcvSetData1(Mat* mat, int rows, int cols, u8* const data, int type)
{
    createMat(*mat, rows, cols, type, false);
    mat->data = data;
}
