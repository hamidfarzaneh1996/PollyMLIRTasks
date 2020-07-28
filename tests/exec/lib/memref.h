#ifndef MEMREF_H
#define MEMREF_H

#include <stdint.h>
#include <stdlib.h>

#define DECL_VECND_STRUCT(ndims, name, eltype)	\
  struct name {					\
    eltype *allocatedPtr;			\
    eltype *alignedPtr;				\
    int64_t offset;				\
    int64_t sizes[ndims];			\
    int64_t strides[ndims];			\
  };

#define DECL_VEC1D_STRUCT(name, eltype)	\
  DECL_VECND_STRUCT(1, name, eltype)

#define DECL_VEC2D_STRUCT(name, eltype) \
  DECL_VECND_STRUCT(2, name, eltype)

/* Generates a comma-separated list of arguments from the fields of a
 * 1d memref */
#define VEC1D_ARGS(v)                           \
  (v)->allocatedPtr,                            \
    (v)->alignedPtr,                            \
    (v)->offset,                                \
    (v)->sizes[0],                              \
    (v)->strides[0]

/* Forwards all arguments declared with DECL_VEC1D_FUNC_ARGS to
 * another function */
#define VEC1D_FWD_ARGS(prefix)			    \
  prefix##_allocatedPtr,                            \
    prefix##_alignedPtr,                            \
    prefix##_offset,                                \
    prefix##_size0,				    \
    prefix##_stride0

/* Generates a comma-separated list of arguments from the fields of a
 * 2d memref */
#define VEC2D_ARGS(v)                           \
  (v)->allocatedPtr,                            \
    (v)->alignedPtr,                            \
    (v)->offset,                                \
    (v)->sizes[0],                              \
    (v)->sizes[1],                              \
    (v)->strides[0],                            \
    (v)->strides[1]

/* Forwards all arguments declared with DECL_VEC2D_FUNC_ARGS to
 * another function */
#define VEC2D_FWD_ARGS(prefix)			    \
  prefix##_allocatedPtr,                            \
    prefix##_alignedPtr,                            \
    prefix##_offset,                                \
    prefix##_size0,				    \
    prefix##_size1,				    \
    prefix##_stride0,				    \
    prefix##_stride1

#define DECL_VEC1D_FUNC_ARGS(prefix, eltype, cst)	     \
  cst eltype* prefix##_allocatedPtr,			     \
    cst eltype* prefix##_alignedPtr,			     \
    int64_t prefix##_offset,				     \
    int64_t prefix##_size0,				     \
    int64_t prefix##_stride0

#define DECL_VEC1D_FUNC_IN_ARGS(prefix, eltype)	\
  DECL_VEC1D_FUNC_ARGS(prefix, eltype, const)

#define DECL_VEC1D_FUNC_OUT_ARGS(prefix, eltype) \
  DECL_VEC1D_FUNC_ARGS(prefix, eltype, )

#define DECL_VEC2D_FUNC_ARGS(prefix, eltype, cst)	     \
  cst eltype* prefix##_allocatedPtr,			     \
    cst eltype* prefix##_alignedPtr,			     \
    int64_t prefix##_offset,				     \
    int64_t prefix##_size0,				     \
    int64_t prefix##_size1,				     \
    int64_t prefix##_stride0,				     \
    int64_t prefix##_stride1

#define DECL_VEC2D_FUNC_IN_ARGS(prefix, eltype)	\
  DECL_VEC2D_FUNC_ARGS(prefix, eltype, const)

#define DECL_VEC2D_FUNC_OUT_ARGS(prefix, eltype) \
  DECL_VEC2D_FUNC_ARGS(prefix, eltype, )

#define DECL_VEC1D_FUNCTIONS(name, eltype, format)			\
  /* Allocates and initializes a 1d memref. Returns 0 on success,	\
   * otherwise 1.							\
   */									\
  static inline int name##_alloc(struct name* v, size_t n)		\
  {									\
    eltype* f;								\
									\
    if(!(f = calloc(n, sizeof(eltype))))				\
      return 1;								\
									\
    v->allocatedPtr = f;						\
    v->alignedPtr = f;							\
    v->offset = 0;							\
    v->sizes[0] = n;							\
    v->strides[0] = 1;							\
									\
    return 0;								\
  }									\
									\
  /* Destroys a 1d memref */						\
  static inline int name##_destroy(struct name* v)			\
  {									\
    free(v->allocatedPtr);						\
  }									\
									\
  /* Returns the element at position `x` of a 1d memref `v` */		\
  static inline eltype name##_get(const struct name* v, int64_t x)	\
  {									\
    return v->allocatedPtr[x];						\
  }									\
									\
  /* Assigns `f` to the element at position `x` of a 1d			\
   * memref `v`								\
   */									\
  static inline void name##_set(struct name* v, int64_t x, eltype f)	\
  {									\
    v->allocatedPtr[x] = f;						\
  }									\
									\
  /* Compares the values of two 1d memrefs. Returns 1 if they are	\
   * equal, otherwise 0.						\
   */									\
  static inline int name##_compare(const struct name* a,		\
				   const struct name* b)		\
  {									\
    /* Compare shapes */						\
    if(a->sizes[0] != b->sizes[0])					\
      return 0;								\
									\
    /* Compare elements */						\
    for(int64_t x = 0; x < a->sizes[1]; x++)				\
      if(name##_get(a, x) != name##_get(b, x))				\
	return 0;							\
									\
    return 1;								\
  }									\
									\
  /* Dumps a 1d `v` to stdout. */					\
  static inline void name##_dump(const struct name* v)			\
  {									\
    for(int64_t x = 0; x < v->sizes[0]; x++) {				\
      printf(format "%s",						\
	     v->allocatedPtr[x],					\
	     x == v->sizes[0]-1 ? "" : " ");				\
     }									\
									\
     puts("");								\
  }

#define DECL_VEC2D_FUNCTIONS(name, eltype, format)			\
  /* Allocates and initializes a 2d memref. Returns 0 on success,	\
   * otherwise 1.							\
   */									\
  static inline int name##_alloc(struct name* v, size_t n, size_t m)	\
  {									\
    eltype* f;								\
									\
    if(!(f = calloc(n*m, sizeof(eltype))))				\
      return 1;								\
									\
    v->allocatedPtr = f;						\
    v->alignedPtr = f;							\
    v->offset = 0;							\
    v->sizes[0] = n;							\
    v->sizes[1] = m;							\
    v->strides[0] = m;							\
    v->strides[1] = 1;							\
									\
    return 0;								\
  }									\
									\
  /* Destroys a 2d memref */						\
  static inline int name##_destroy(struct name* v)			\
  {									\
    free(v->allocatedPtr);						\
  }									\
									\
  /* Returns the element at position (`x`, `y`) of a 2d memref `v` */	\
  static inline eltype name##_get(const struct name* v,			\
				  int64_t x, int64_t y)			\
  {									\
    return *(v->allocatedPtr + y*v->sizes[1] + x);			\
  }									\
									\
  /* Assigns `f` to the element at position (`x`, `y`) of a 2d		\
   * memref `v`								\
   */									\
  static inline void name##_set(struct name* v, int64_t x, int64_t y,	\
				eltype f)				\
  {									\
    *(v->allocatedPtr + y*v->sizes[1] + x) = f;				\
  }									\
									\
  /* Compares the values of two 2d memrefs. Returns 1 if they are \
   * equal, otherwise 0.						\
   */									\
  static inline int name##_compare(const struct name* a,		\
				   const struct name* b)		\
  {									\
    /* Compare shapes */						\
    if(a->sizes[0] != b->sizes[0] ||					\
       a->sizes[1] != b->sizes[1])					\
      {									\
	return 0;							\
      }									\
									\
    /* Compare elements */						\
    for(int64_t y = 0; y < a->sizes[0]; y++)				\
      for(int64_t x = 0; x < a->sizes[1]; x++)				\
	if(name##_get(a, x, y) != name##_get(b, x, y))			\
	  return 0;							\
									\
    return 1;								\
  }									\
									\
  /* Dumps a 2d `v` to stdout. */					\
  static inline void name##_dump(const struct name* v)			\
  {									\
    for(int64_t y = 0; y < v->sizes[0]; y++) {				\
      for(int64_t x = 0; x < v->sizes[1]; x++) {			\
	printf(format "%s",						\
	       *(v->allocatedPtr + y*v->sizes[1] + x),			\
	       x == v->sizes[1]-1 ? "" : " ");				\
      }									\
									\
      puts("");								\
    }									\
  }

DECL_VEC1D_STRUCT(vec_f1d, float)
DECL_VEC2D_STRUCT(vec_f2d, float)

DECL_VEC1D_FUNCTIONS(vec_f1d, float, "%f")
DECL_VEC2D_FUNCTIONS(vec_f2d, float, "%f")

#endif
