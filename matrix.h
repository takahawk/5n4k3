#ifndef MATRIX_H_
#define MATRIX_H_
#include <stddef.h>

typedef struct {
	size_t w;
	size_t h;
	size_t elem_size;
	void *data;
} Matrix;

Matrix*
AllocMatrix(size_t w, size_t h, size_t elem_size);

void*
MatrixGet(Matrix *m, size_t x, size_t y);

// TODO: add method to compare elements

void
MatrixSet(Matrix *m, size_t x, size_t y, void *val);

void
FreeMatrix(Matrix*);
#endif
