#include "matrix.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static inline void*
_MatrixIndex(Matrix *m, size_t x, size_t y);

Matrix*
AllocMatrix(size_t w, size_t h, size_t elem_size) {
	Matrix *m = malloc(sizeof(Matrix));
	m->w = w;
	m->h = h;
	m->elem_size = elem_size;

	size_t size = w * h * elem_size;
	m->data = malloc(size);
	memset(m->data, 0, size);
	
	return m;
}

void*
MatrixGet(Matrix *m, size_t x, size_t y) {
	// TODO: add debug/verbose prints on error
	if (x >= m->w || y >= m->h)
		return NULL;
	return _MatrixIndex(m, x, y); 
}

void
MatrixSet(Matrix *m, size_t x, size_t y, void *val) {
	// TODO: add debug/verbose prints on error
	if (x >= m->w || y >= m->h)
		return;
	memcpy(_MatrixIndex(m, x, y), val, m->elem_size);
}

void
FreeMatrix(Matrix* m) {
	free(m->data);
	free(m);
}

static inline void*
_MatrixIndex(Matrix *m, size_t x, size_t y) {
	return (char *) m->data + (x + y * m->w) * m->elem_size; 
}

