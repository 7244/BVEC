#if defined(BVEC_set_MultipleType_Sizes)
  inline
  static
  const
  BVEC_set_NodeSizeType
  _BVEC_P(_MultipleType_Sizes)[] = {BVEC_set_MultipleType_Sizes};
  #define _BVEC_MultipleType_Amount \
    (sizeof(_BVEC_P(_MultipleType_Sizes)) / sizeof(_BVEC_P(_MultipleType_Sizes)[0]))
#elif defined(BVEC_set_MultipleType_SizeArray)
  inline
  static
  const
  auto
  _BVEC_P(_MultipleType_Sizes) = BVEC_set_MultipleType_SizeArray;
  #define _BVEC_MultipleType_Amount \
    _BVEC_P(_MultipleType_Sizes).size()
#elif defined(BVEC_set_NodeData)
  typedef BVEC_set_NodeData _BVEC_P(Node_t);
#else
  typedef void _BVEC_P(Node_t);
#endif

typedef struct{
  BVEC_set_NodeType Current;
  BVEC_set_NodeType Possible;
  #if defined(BVEC_set_MultipleType)
    uint8_t *ptr[_BVEC_MultipleType_Amount];
  #else
    _BVEC_P(Node_t) *ptr;
    #ifndef BVEC_set_NodeData
      BVEC_set_NodeSizeType NodeSize;
      #if BVEC_set_BufferingFormat == 0
        BVEC_set_NodeType BufferAmount;
      #endif
    #endif
  #endif
}_BVEC_P(t);

static
BVEC_set_NodeSizeType
_BVEC_P(GetNodeSize)
(
  _BVEC_P(t) *List
  #if defined(BVEC_set_MultipleType)
    , uintptr_t PointerIndex
  #endif
){
  #if defined(BVEC_set_MultipleType)
    return _BVEC_P(_MultipleType_Sizes)[PointerIndex];
  #elif defined(BVEC_set_NodeData)
    return sizeof(BVEC_set_NodeData);
  #else
    return List->NodeSize;
  #endif
}

static
#if defined(BVEC_set_MultipleType)
  void *
#else
  _BVEC_P(Node_t) *
#endif
_BVEC_P(GetNode)
(
  _BVEC_P(t) *List,
  BVEC_set_NodeType NR
  #if defined(BVEC_set_MultipleType)
    , uintptr_t PointerIndex
  #endif
){
  #if defined(BVEC_set_MultipleType)
    return (void *)(List->ptr[PointerIndex] + NR * _BVEC_P(GetNodeSize)(List, PointerIndex));
  #elif defined(BVEC_set_NodeData)
    return &List->ptr[NR];
  #else
    return (_BVEC_P(Node_t) *)&((uint8_t *)List->ptr)[NR * _BVEC_P(GetNodeSize)(List)];
  #endif
}

static
void
_BVEC_P(Close)
(
  _BVEC_P(t) *List
){
  #if BVEC_set_HandleAllocate == 1
    #if defined(BVEC_set_MultipleType)
      for(uintptr_t i = 0; i < _BVEC_MultipleType_Amount; i++){
        BVEC_set_alloc_close(List->ptr[i]);
      }
    #else
      BVEC_set_alloc_close(List->ptr);
    #endif
  #endif
}
static
void
_BVEC_P(Open)
(
  _BVEC_P(t) *List
  #if !defined(BVEC_set_MultipleType)
    #ifndef BVEC_set_NodeData
      , BVEC_set_NodeSizeType NodeSize
    #endif
  #endif
){
  List->Current = 0;
  List->Possible = 0;
  #if defined(BVEC_set_MultipleType)
    for(uintptr_t i = 0; i < _BVEC_MultipleType_Amount; i++){
      List->ptr[i] = NULL;
    }
  #else
    List->ptr = NULL;
    #ifndef BVEC_set_NodeData
      List->NodeSize = NodeSize;
      #if BVEC_set_BufferingFormat == 0
        List->BufferAmount = BVEC_set_WantedBufferByteAmount / List->NodeSize;
        if(List->BufferAmount == 0){
          List->BufferAmount = 1;
        }
      #endif
    #endif
  #endif
}
static
void
_BVEC_P(Clear)
(
  _BVEC_P(t) *List
){
  List->Current = 0;
}

static
void
_BVEC_P(ClearWithBuffer)
(
  _BVEC_P(t) *List
){
  #if BVEC_set_HandleAllocate == 1
    /* a lot shorter to call this */
    _BVEC_P(Close)(List);
  #endif

  List->Current = 0;
  List->Possible = 0;
  #if defined(BVEC_set_MultipleType)
    for(uintptr_t i = 0; i < _BVEC_MultipleType_Amount; i++){
      List->ptr[i] = NULL;
    }
  #else
    List->ptr = NULL;
  #endif
}

static
void
_BVEC_P(_SetPossible)(
  _BVEC_P(t) *bvec,
  BVEC_set_NodeType Possible
){
  BVEC_set_PossibleUpdate
  bvec->Possible = Possible;
}

static
void
_BVEC_P(SetPossibleWith)(
  _BVEC_P(t) *List,
  BVEC_set_NodeType Size
){
  _BVEC_P(_SetPossible)(List,
  #if BVEC_set_BufferingFormat == 0
    #ifdef BVEC_set_NodeData
      Size + BVEC_BufferAmount
    #else
      Size + List->BufferAmount
    #endif
  #elif BVEC_set_BufferingFormat == 1
    ((uintptr_t)2 << sizeof(uintptr_t) * 8 - __clz(Size | 1)) - 1
  #else
    #error ?
  #endif
  );
}

static
void
_BVEC_P(SetPointer)(
  _BVEC_P(t) *List,
  void *Pointer
){
  List->ptr = (_BVEC_P(Node_t) *)Pointer;
}

#if BVEC_set_HandleAllocate == 1
  static
  void
  _BVEC_P(_Resize)(
    _BVEC_P(t) *List
  ){
    #if defined(BVEC_set_MultipleType)
      for(uintptr_t i = 0; i < _BVEC_MultipleType_Amount; i++){
        for(uint32_t iRetry = 0; iRetry < BVEC_set_alloc_RetryAmount; iRetry++){
          void *np = BVEC_set_alloc_resize(
            List->ptr[i],
            (uintptr_t)List->Possible * _BVEC_P(_MultipleType_Sizes)[i]
          );
          if(np == NULL){
            continue;
          }
          List->ptr[i] = (uint8_t *)np;
          goto gt_NextType;
        }
        __abort();
        gt_NextType:;
      }
    #else
      for(uint32_t iRetry = 0; iRetry < BVEC_set_alloc_RetryAmount; iRetry++){
        void *np = BVEC_set_alloc_resize(
          List->ptr,
          (uintptr_t)List->Possible * _BVEC_P(GetNodeSize)(List)
        );
        if(np == NULL){
          continue;
        }
        List->ptr = (_BVEC_P(Node_t) *)np;
        return;
      }
      __abort();
    #endif
  }

  static
  void
  _BVEC_P(Reserve)
  (
    _BVEC_P(t) *List,
    BVEC_set_NodeType Amount
  ){
    _BVEC_P(_SetPossible)(List, Amount);
    _BVEC_P(_Resize)(List);
  }

  static
  void
  _BVEC_P(_AllocateBufferWith)
  (
    _BVEC_P(t) *List,
    BVEC_set_NodeType Amount
  ){
    _BVEC_P(SetPossibleWith)(List, Amount);
    _BVEC_P(_Resize)(List);
  }
#endif

#if !defined(BVEC_set_MultipleType)
  static
  void
  _BVEC_P(Add)
  (
    _BVEC_P(t) *List,
    _BVEC_P(Node_t) *Node
  ){
    #if BVEC_set_HandleAllocate == 1
      if(List->Current == List->Possible){
        _BVEC_P(_AllocateBufferWith)(List, List->Current);
      }
    #endif

    #ifdef BVEC_set_NodeData
      List->ptr[List->Current] = *Node;
    #else
      __MemoryCopy(Node, _BVEC_P(GetNode)(List, List->Current), List->NodeSize);
    #endif
    ++List->Current;
  }
#endif

static
void
_BVEC_P(AddEmpty)
(
  _BVEC_P(t) *List,
  BVEC_set_NodeType Amount
){
  BVEC_set_NodeType Current = List->Current + Amount;
  #if BVEC_set_HandleAllocate == 1
    if(Current >= List->Possible){
      _BVEC_P(_AllocateBufferWith)(List, Current);
    }
  #endif
  List->Current = Current;
}

#if defined(BVEC_set_MultipleType)
  #undef _BVEC_MultipleType_Amount
#endif
