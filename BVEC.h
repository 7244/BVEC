#ifdef BVEC_set_BaseLibrary
  #error BVEC_set_BaseLibrary is no longer supported.
#endif

#ifndef BVEC_set_prefix
  #error ifndef BVEC_set_prefix
#endif

#ifndef BVEC_set_PossibleUpdate
  #define BVEC_set_PossibleUpdate
#endif

#ifndef BVEC_set_NodeType
  #define BVEC_set_NodeType uint32_t
#endif
#ifndef BVEC_set_NodeSizeType
  #define BVEC_set_NodeSizeType uint32_t
#endif

#ifndef BVEC_set_HandleAllocate
  #define BVEC_set_HandleAllocate 1
#endif

#ifndef BVEC_set_BufferingFormat
  #define BVEC_set_BufferingFormat 1
#endif

#ifndef BVEC_set_alloc_open
  #if defined(__generic_malloc)
    #define BVEC_set_alloc_open __generic_malloc
  #else
    #define BVEC_set_alloc_open malloc
  #endif
#endif
#ifndef BVEC_set_alloc_resize
  #if defined(__generic_realloc)
    #define BVEC_set_alloc_resize __generic_realloc
  #else
    #define BVEC_set_alloc_resize realloc
  #endif
#endif
#ifndef BVEC_set_alloc_close
  #if defined(__generic_free)
    #define BVEC_set_alloc_close __generic_free
  #else
    #define BVEC_set_alloc_close free
  #endif
#endif

#ifndef BVEC_set_alloc_RetryAmount
  #define BVEC_set_alloc_RetryAmount 0x10
#endif

#if BVEC_set_BufferingFormat == 0
  #ifndef BVEC_set_BufferingFormat0_WantedBufferByteAmount
    #define BVEC_set_BufferingFormat0_WantedBufferByteAmount 512
  #endif
  #define BVEC_BufferDivide (BVEC_set_BufferingFormat0_WantedBufferByteAmount / sizeof(BVEC_set_NodeData))
  #define BVEC_BufferAmount (BVEC_BufferDivide == 0 ? 1 : BVEC_BufferDivide)
#endif

#define _BVEC_P(p0) CONCAT3(BVEC_set_prefix, _, p0)

#include "internal/rest.h"

#undef _BVEC_P

#if BVEC_set_BufferingFormat == 0
  #undef BVEC_BufferAmount
  #undef BVEC_BufferDivide
  #undef BVEC_set_BufferingFormat0_WantedBufferByteAmount
#endif

#undef BVEC_set_alloc_RetryAmount

#undef BVEC_set_alloc_close
#undef BVEC_set_alloc_resize
#undef BVEC_set_alloc_open

#undef BVEC_set_BufferingFormat

#undef BVEC_set_HandleAllocate

#ifdef BVEC_set_NodeData
  #undef BVEC_set_NodeData
#endif
#undef BVEC_set_NodeSizeType
#undef BVEC_set_NodeType
#ifdef BVEC_set_MultipleType
  #ifdef BVEC_set_MultipleType_Sizes
    #undef BVEC_set_MultipleType_Sizes
  #endif
  #ifdef BVEC_set_MultipleType_SizeArray
    #undef BVEC_set_MultipleType_SizeArray
  #endif
  #undef BVEC_set_MultipleType
#endif
#undef BVEC_set_PossibleUpdate
#undef BVEC_set_prefix
