/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

namespace FloatVectorHelpers
{
    #define JUCE_INCREMENT_SRC_DEST         dest += (16 / sizeof (*dest)); src += (16 / sizeof (*dest));
    #define JUCE_INCREMENT_SRC1_SRC2_DEST   dest += (16 / sizeof (*dest)); src1 += (16 / sizeof (*dest)); src2 += (16 / sizeof (*dest));
    #define JUCE_INCREMENT_DEST             dest += (16 / sizeof (*dest));

   #if JUCE_USE_SSE_INTRINSICS
    static bool sse2Present = false;

    static bool isSSE2Available() noexcept
    {
        if (sse2Present)
            return true;

        sse2Present = SystemStats::hasSSE2();
        return sse2Present;
    }

    inline static bool isAligned (const void* p) noexcept
    {
        return (((pointer_sized_int) p) & 15) == 0;
    }

    struct BasicOps32
    {
        typedef float Type;
        typedef __m128 ParallelType;
        typedef __m128 IntegerType;
        enum { numParallel = 4 };

        // Integer and parallel types are the same for SSE. On neon they have different types
        static forcedinline IntegerType toint (ParallelType v) noexcept                 { return v; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { return v; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return _mm_load1_ps (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return _mm_load_ps (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return _mm_loadu_ps (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { _mm_store_ps (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { _mm_storeu_ps (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return _mm_add_ps (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return _mm_sub_ps (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return _mm_mul_ps (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return _mm_max_ps (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return _mm_min_ps (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  { return _mm_and_ps (a, b); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  { return _mm_andnot_ps (a, b); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  { return _mm_or_ps (a, b); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  { return _mm_xor_ps (a, b); }

        static forcedinline Type max (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1], v[2], v[3]); }
        static forcedinline Type min (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1], v[2], v[3]); }
    };

    struct BasicOps64
    {
        typedef double Type;
        typedef __m128d ParallelType;
        typedef __m128d IntegerType;
        enum { numParallel = 2 };

        // Integer and parallel types are the same for SSE. On neon they have different types
        static forcedinline IntegerType toint (ParallelType v) noexcept                 { return v; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { return v; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return _mm_load1_pd (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return _mm_load_pd (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return _mm_loadu_pd (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { _mm_store_pd (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { _mm_storeu_pd (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return _mm_add_pd (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return _mm_sub_pd (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return _mm_mul_pd (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return _mm_max_pd (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return _mm_min_pd (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  { return _mm_and_pd (a, b); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  { return _mm_andnot_pd (a, b); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  { return _mm_or_pd (a, b); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  { return _mm_xor_pd (a, b); }

        static forcedinline Type max (ParallelType a) noexcept  { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1]); }
        static forcedinline Type min (ParallelType a) noexcept  { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1]); }
    };



    #define JUCE_BEGIN_VEC_OP \
        typedef FloatVectorHelpers::ModeType<sizeof(*dest)>::Mode Mode; \
        if (FloatVectorHelpers::isSSE2Available()) \
        { \
            const int numLongOps = num / Mode::numParallel;

    #define JUCE_FINISH_VEC_OP(normalOp) \
            num &= (Mode::numParallel - 1); \
            if (num == 0) return; \
        } \
        for (int i = 0; i < num; ++i) normalOp;

    #define JUCE_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest))   JUCE_VEC_LOOP (vecOp, dummy, Mode::loadA, Mode::storeA, locals, JUCE_INCREMENT_DEST) \
        else                                        JUCE_VEC_LOOP (vecOp, dummy, Mode::loadU, Mode::storeU, locals, JUCE_INCREMENT_DEST) \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src)) JUCE_VEC_LOOP (vecOp, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
            else                                     JUCE_VEC_LOOP (vecOp, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
        }\
        else \
        { \
            if (FloatVectorHelpers::isAligned (src)) JUCE_VEC_LOOP (vecOp, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
            else                                     JUCE_VEC_LOOP (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        } \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadU, Mode::storeA, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeA, locals, increment) \
            } \
        } \
        else \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadA, Mode::storeU, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadA, Mode::storeU, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
        } \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
            } \
        } \
        else \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
                else                                        JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
        } \
        JUCE_FINISH_VEC_OP (normalOp)


    //==============================================================================
   #elif JUCE_USE_ARM_NEON

    struct BasicOps32
    {
        typedef float Type;
        typedef float32x4_t ParallelType;
        typedef uint32x4_t IntegerType;
        enum { numParallel = 4 };

        static forcedinline IntegerType toint (ParallelType v) noexcept                 { union { ParallelType f; IntegerType i; } u; u.f = v; return u.i; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { union { ParallelType f; IntegerType i; } u; u.i = v; return u.f; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return vld1q_dup_f32 (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return vld1q_f32 (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return vld1q_f32 (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { vst1q_f32 (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { vst1q_f32 (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return vaddq_f32 (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return vsubq_f32 (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return vmulq_f32 (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return vmaxq_f32 (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return vminq_f32 (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  {  return toflt (vandq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  {  return toflt (vbicq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  {  return toflt (vorrq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  {  return toflt (veorq_u32 (toint (a), toint (b))); }

        static forcedinline Type max (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1], v[2], v[3]); }
        static forcedinline Type min (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1], v[2], v[3]); }
    };

    struct BasicOps64
    {
        typedef double Type;
        typedef double ParallelType;
        typedef uint64 IntegerType;
        enum { numParallel = 1 };

        static forcedinline IntegerType toint (ParallelType v) noexcept                 { union { ParallelType f; IntegerType i; } u; u.f = v; return u.i; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { union { ParallelType f; IntegerType i; } u; u.i = v; return u.f; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return v; }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return *v; }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return *v; }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { *dest = a; }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { *dest = a; }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return a + b; }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return a - b; }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return a * b; }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return jmax (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return jmin (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) & toint (b)); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  {  return toflt ((~toint (a)) & toint (b)); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) | toint (b)); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) ^ toint (b)); }

        static forcedinline Type max (ParallelType a) noexcept  { return a; }
        static forcedinline Type min (ParallelType a) noexcept  { return a; }
    };

    #define JUCE_BEGIN_VEC_OP \
        typedef FloatVectorHelpers::ModeType<sizeof(*dest)>::Mode Mode; \
        if (Mode::numParallel > 1) \
        { \
            const int numLongOps = num / Mode::numParallel;

    #define JUCE_FINISH_VEC_OP(normalOp) \
            num &= (Mode::numParallel - 1); \
            if (num == 0) return; \
        } \
        for (int i = 0; i < num; ++i) normalOp;

    #define JUCE_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        JUCE_VEC_LOOP (vecOp, dummy, Mode::loadU, Mode::storeU, locals, JUCE_INCREMENT_DEST) \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        JUCE_VEC_LOOP (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        JUCE_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        JUCE_FINISH_VEC_OP (normalOp)

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        JUCE_BEGIN_VEC_OP \
        setupOp \
        JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        JUCE_FINISH_VEC_OP (normalOp)


    //==============================================================================
   #else
    #define JUCE_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        for (int i = 0; i < num; ++i) normalOp;

    #define JUCE_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (int i = 0; i < num; ++i) normalOp;

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (int i = 0; i < num; ++i) normalOp;

    #define JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (int i = 0; i < num; ++i) normalOp;

   #endif

    //==============================================================================
    #define JUCE_VEC_LOOP(vecOp, srcLoad, dstLoad, dstStore, locals, increment) \
        for (int i = 0; i < numLongOps; ++i) \
        { \
            locals (srcLoad, dstLoad); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define JUCE_VEC_LOOP_TWO_SOURCES(vecOp, src1Load, src2Load, dstStore, locals, increment) \
        for (int i = 0; i < numLongOps; ++i) \
        { \
            locals (src1Load, src2Load); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define JUCE_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD(vecOp, src1Load, src2Load, dstLoad, dstStore, locals, increment) \
        for (int i = 0; i < numLongOps; ++i) \
        { \
            locals (src1Load, src2Load, dstLoad); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define JUCE_LOAD_NONE(srcLoad, dstLoad)
    #define JUCE_LOAD_DEST(srcLoad, dstLoad)                        const Mode::ParallelType d = dstLoad (dest);
    #define JUCE_LOAD_SRC(srcLoad, dstLoad)                         const Mode::ParallelType s = srcLoad (src);
    #define JUCE_LOAD_SRC1_SRC2(src1Load, src2Load)                 const Mode::ParallelType s1 = src1Load (src1), s2 = src2Load (src2);
    #define JUCE_LOAD_SRC1_SRC2_DEST(src1Load, src2Load, dstLoad)   const Mode::ParallelType d = dstLoad (dest), s1 = src1Load (src1), s2 = src2Load (src2);
    #define JUCE_LOAD_SRC_DEST(srcLoad, dstLoad)                    const Mode::ParallelType d = dstLoad (dest), s = srcLoad (src);

   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    template<int typeSize> struct ModeType    { typedef BasicOps32 Mode; };
    template<>             struct ModeType<8> { typedef BasicOps64 Mode; };

    template <typename Mode>
    struct MinMax
    {
        typedef typename Mode::Type Type;
        typedef typename Mode::ParallelType ParallelType;

        static Type findMinOrMax (const Type* src, int num, const bool isMinimum) noexcept
        {
            int numLongOps = num / Mode::numParallel;

           #if JUCE_USE_SSE_INTRINSICS
            if (numLongOps > 1 && isSSE2Available())
           #else
            if (numLongOps > 1)
           #endif
            {
                ParallelType val;

               #if ! JUCE_USE_ARM_NEON
                if (isAligned (src))
                {
                    val = Mode::loadA (src);

                    if (isMinimum)
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::min (val, Mode::loadA (src));
                        }
                    }
                    else
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::max (val, Mode::loadA (src));
                        }
                    }
                }
                else
               #endif
                {
                    val = Mode::loadU (src);

                    if (isMinimum)
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::min (val, Mode::loadU (src));
                        }
                    }
                    else
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::max (val, Mode::loadU (src));
                        }
                    }
                }

                Type result = isMinimum ? Mode::min (val)
                                        : Mode::max (val);

                num &= (Mode::numParallel - 1);
                src += Mode::numParallel;

                for (int i = 0; i < num; ++i)
                    result = isMinimum ? jmin (result, src[i])
                                       : jmax (result, src[i]);

                return result;
            }

            return isMinimum ? juce::findMinimum (src, num)
                             : juce::findMaximum (src, num);
        }

        static Range<Type> findMinAndMax (const Type* src, int num) noexcept
        {
            int numLongOps = num / Mode::numParallel;

           #if JUCE_USE_SSE_INTRINSICS
            if (numLongOps > 1 && isSSE2Available())
           #else
            if (numLongOps > 1)
           #endif
            {
                ParallelType mn, mx;

               #if ! JUCE_USE_ARM_NEON
                if (isAligned (src))
                {
                    mn = Mode::loadA (src);
                    mx = mn;

                    while (--numLongOps > 0)
                    {
                        src += Mode::numParallel;
                        const ParallelType v = Mode::loadA (src);
                        mn = Mode::min (mn, v);
                        mx = Mode::max (mx, v);
                    }
                }
                else
               #endif
                {
                    mn = Mode::loadU (src);
                    mx = mn;

                    while (--numLongOps > 0)
                    {
                        src += Mode::numParallel;
                        const ParallelType v = Mode::loadU (src);
                        mn = Mode::min (mn, v);
                        mx = Mode::max (mx, v);
                    }
                }

                Range<Type> result (Mode::min (mn),
                                    Mode::max (mx));

                num &= (Mode::numParallel - 1);
                src += Mode::numParallel;

                for (int i = 0; i < num; ++i)
                    result = result.getUnionWith (src[i]);

                return result;
            }

            return Range<Type>::findMinAndMax (src, num);
        }
    };
   #endif
}

//==============================================================================
void JUCE_CALLTYPE FloatVectorOperations::clear (float* dest, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vclr (dest, 1, (size_t) num);
   #else
    zeromem (dest, (size_t) num * sizeof (float));
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::clear (double* dest, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vclrD (dest, 1, (size_t) num);
   #else
    zeromem (dest, (size_t) num * sizeof (double));
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::fill (float* dest, float valueToFill, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vfill (&valueToFill, dest, 1, (size_t) num);
   #else
    JUCE_PERFORM_VEC_OP_DEST (dest[i] = valueToFill, val, JUCE_LOAD_NONE,
                              const Mode::ParallelType val = Mode::load1 (valueToFill);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::fill (double* dest, double valueToFill, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vfillD (&valueToFill, dest, 1, (size_t) num);
   #else
    JUCE_PERFORM_VEC_OP_DEST (dest[i] = valueToFill, val, JUCE_LOAD_NONE,
                              const Mode::ParallelType val = Mode::load1 (valueToFill);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::copy (float* dest, const float* src, int num) noexcept
{
    memcpy (dest, src, (size_t) num * sizeof (float));
}

void JUCE_CALLTYPE FloatVectorOperations::copy (double* dest, const double* src, int num) noexcept
{
    memcpy (dest, src, (size_t) num * sizeof (double));
}

void JUCE_CALLTYPE FloatVectorOperations::copyWithMultiply (float* dest, const float* src, float multiplier, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsmul (src, 1, &multiplier, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier, Mode::mul (mult, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::copyWithMultiply (double* dest, const double* src, double multiplier, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsmulD (src, 1, &multiplier, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier, Mode::mul (mult, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (float* dest, float amount, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsadd (dest, 1, &amount, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_DEST (dest[i] += amount, Mode::add (d, amountToAdd), JUCE_LOAD_DEST,
                              const Mode::ParallelType amountToAdd = Mode::load1 (amount);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (double* dest, double amount, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_DEST (dest[i] += amount, Mode::add (d, amountToAdd), JUCE_LOAD_DEST,
                              const Mode::ParallelType amountToAdd = Mode::load1 (amount);)
}

void JUCE_CALLTYPE FloatVectorOperations::add (float* dest, float* src, float amount, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsadd (src, 1, &amount, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] + amount, Mode::add (am, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType am = Mode::load1 (amount);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (double* dest, double* src, double amount, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsaddD (src, 1, &amount, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] + amount, Mode::add (am, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType am = Mode::load1 (amount);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (float* dest, const float* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vadd (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i], Mode::add (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (double* dest, const double* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vaddD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i], Mode::add (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vadd (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] + src2[i], Mode::add (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::add (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vaddD (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] + src2[i], Mode::add (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::subtract (float* dest, const float* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsub (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i], Mode::sub (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::subtract (double* dest, const double* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsubD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i], Mode::sub (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::subtract (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsub (src2, 1, src1, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] - src2[i], Mode::sub (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::subtract (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsubD (src2, 1, src1, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] - src2[i], Mode::sub (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::addWithMultiply (float* dest, const float* src, float multiplier, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsma (src, 1, &multiplier, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i] * multiplier, Mode::add (d, Mode::mul (mult, s)),
                                  JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::addWithMultiply (double* dest, const double* src, double multiplier, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i] * multiplier, Mode::add (d, Mode::mul (mult, s)),
                                  JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
}

void JUCE_CALLTYPE FloatVectorOperations::addWithMultiply (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vma ((float*) src1, 1, (float*) src2, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] += src1[i] * src2[i], Mode::add (d, Mode::mul (s1, s2)),
                                             JUCE_LOAD_SRC1_SRC2_DEST,
                                             JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::addWithMultiply (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmaD ((double*) src1, 1, (double*) src2, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] += src1[i] * src2[i], Mode::add (d, Mode::mul (s1, s2)),
                                             JUCE_LOAD_SRC1_SRC2_DEST,
                                             JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (float* dest, const float* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmul (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] *= src[i], Mode::mul (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (double* dest, const double* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmulD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] *= src[i], Mode::mul (d, s), JUCE_LOAD_SRC_DEST, JUCE_INCREMENT_SRC_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmul (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] * src2[i], Mode::mul (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmulD (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] * src2[i], Mode::mul (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (float* dest, float multiplier, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsmul (dest, 1, &multiplier, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_DEST (dest[i] *= multiplier, Mode::mul (d, mult), JUCE_LOAD_DEST,
                              const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (double* dest, double multiplier, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vsmulD (dest, 1, &multiplier, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_DEST (dest[i] *= multiplier, Mode::mul (d, mult), JUCE_LOAD_DEST,
                              const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (float* dest, const float* src, float multiplier, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier, Mode::mul (mult, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
}

void JUCE_CALLTYPE FloatVectorOperations::multiply (double* dest, const double* src, double multiplier, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier, Mode::mul (mult, s),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
}

void FloatVectorOperations::negate (float* dest, const float* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vneg ((float*) src, 1, dest, 1, (vDSP_Length) num);
   #else
    copyWithMultiply (dest, src, -1.0f, num);
   #endif
}

void FloatVectorOperations::negate (double* dest, const double* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vnegD ((double*) src, 1, dest, 1, (vDSP_Length) num);
   #else
    copyWithMultiply (dest, src, -1.0f, num);
   #endif
}

void FloatVectorOperations::abs (float* dest, const float* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vabs ((float*) src, 1, dest, 1, (vDSP_Length) num);
   #else
    union { float f; uint32 i; } signMask;
    signMask.i = 0x7fffffffUL;
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = fabsf (src[i]), Mode::bit_and (s, mask),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mask = Mode::load1 (signMask.f);)

    ignoreUnused (signMask);
   #endif
}

void FloatVectorOperations::abs (double* dest, const double* src, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vabsD ((double*) src, 1, dest, 1, (vDSP_Length) num);
   #else
    union {double d; uint64 i;} signMask;
    signMask.i = 0x7fffffffffffffffULL;

    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = fabs (src[i]), Mode::bit_and (s, mask),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mask = Mode::load1 (signMask.d);)

    ignoreUnused (signMask);
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::convertFixedToFloat (float* dest, const int* src, float multiplier, int num) noexcept
{
   #if JUCE_USE_ARM_NEON
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                  vmulq_n_f32 (vcvtq_f32_s32 (vld1q_s32 (src)), multiplier),
                                  JUCE_LOAD_NONE, JUCE_INCREMENT_SRC_DEST, )
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                  Mode::mul (mult, _mm_cvtepi32_ps (_mm_loadu_si128 ((const __m128i*) src))),
                                  JUCE_LOAD_NONE, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::convertFixedToFloat(double* dest, const int* src, const double multiplier, int num) noexcept
{
/*
//#if JUCE_USE_SSE_INTRINSICS
	const __m128d mult = _mm_load1_pd(&multiplier);
//#endif

	JUCE_PERFORM_SSE_OP_SRC_DEST(dest[i] = src[i] * multiplier,
		_mm_mul_pd(mult, _mm_cvtepi32_pd(_mm_loadu_si128((const __m128i*) src))),
		JUCE_LOAD_NONE, JUCE_INCREMENT_SRC_DEST)*/

#if JUCE_USE_ARM_NEON
	JUCE_PERFORM_VEC_OP_SRC_DEST(dest[i] = src[i] * multiplier,
		vmulq_n_f32(vcvtq_f32_s32(vld1q_s32(src)), multiplier),
		JUCE_LOAD_NONE, JUCE_INCREMENT_SRC_DEST, )
	#else
	JUCE_PERFORM_VEC_OP_SRC_DEST(dest[i] = src[i] * multiplier,
		Mode::mul(mult, _mm_cvtepi32_pd(_mm_loadu_si128((const __m128i*) src))),
		JUCE_LOAD_NONE, JUCE_INCREMENT_SRC_DEST,
		const Mode::ParallelType mult = Mode::load1(multiplier);)
	#endif

}

void JUCE_CALLTYPE FloatVectorOperations::min (float* dest, const float* src, float comp, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmin (src[i], comp), Mode::min (s, cmp),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType cmp = Mode::load1 (comp);)
}

void JUCE_CALLTYPE FloatVectorOperations::min (double* dest, const double* src, double comp, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmin (src[i], comp), Mode::min (s, cmp),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType cmp = Mode::load1 (comp);)
}



void JUCE_CALLTYPE FloatVectorOperations::min (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmin ((float*) src1, 1, (float*) src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmin (src1[i], src2[i]), Mode::min (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::min (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vminD ((double*) src1, 1, (double*) src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmin (src1[i], src2[i]), Mode::min (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::max (float* dest, const float* src, float comp, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (src[i], comp), Mode::max (s, cmp),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType cmp = Mode::load1 (comp);)
}

void JUCE_CALLTYPE FloatVectorOperations::max (double* dest, const double* src, double comp, int num) noexcept
{
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (src[i], comp), Mode::max (s, cmp),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType cmp = Mode::load1 (comp);)
}

void JUCE_CALLTYPE FloatVectorOperations::max (float* dest, const float* src1, const float* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmax ((float*) src1, 1, (float*) src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmax (src1[i], src2[i]), Mode::max (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::max (double* dest, const double* src1, const double* src2, int num) noexcept
{
   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vmaxD ((double*) src1, 1, (double*) src2, 1, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmax (src1[i], src2[i]), Mode::max (s1, s2), JUCE_LOAD_SRC1_SRC2, JUCE_INCREMENT_SRC1_SRC2_DEST, )
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::clip (float* dest, const float* src, float low, float high, int num) noexcept
{
    jassert(high >= low);

   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vclip ((float*) src, 1, &low, &high, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (jmin (src[i], high), low), Mode::max (Mode::min (s, hi), lo),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType lo = Mode::load1 (low); const Mode::ParallelType hi = Mode::load1 (high);)
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::clip (double* dest, const double* src, double low, double high, int num) noexcept
{
    jassert(high >= low);

   #if JUCE_USE_VDSP_FRAMEWORK
    vDSP_vclipD ((double*) src, 1, &low, &high, dest, 1, (vDSP_Length) num);
   #else
    JUCE_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (jmin (src[i], high), low), Mode::max (Mode::min (s, hi), lo),
                                  JUCE_LOAD_SRC, JUCE_INCREMENT_SRC_DEST,
                                  const Mode::ParallelType lo = Mode::load1 (low); const Mode::ParallelType hi = Mode::load1 (high);)
   #endif
}

Range<float> JUCE_CALLTYPE FloatVectorOperations::findMinAndMax (const float* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinAndMax (src, num);
   #else
    return Range<float>::findMinAndMax (src, num);
   #endif
}

Range<double> JUCE_CALLTYPE FloatVectorOperations::findMinAndMax (const double* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinAndMax (src, num);
   #else
    return Range<double>::findMinAndMax (src, num);
   #endif
}

float JUCE_CALLTYPE FloatVectorOperations::findMinimum (const float* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinOrMax (src, num, true);
   #else
    return juce::findMinimum (src, num);
   #endif
}

double JUCE_CALLTYPE FloatVectorOperations::findMinimum (const double* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinOrMax (src, num, true);
   #else
    return juce::findMinimum (src, num);
   #endif
}

float JUCE_CALLTYPE FloatVectorOperations::findMaximum (const float* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinOrMax (src, num, false);
   #else
    return juce::findMaximum (src, num);
   #endif
}

double JUCE_CALLTYPE FloatVectorOperations::findMaximum (const double* src, int num) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS || JUCE_USE_ARM_NEON
    return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinOrMax (src, num, false);
   #else
    return juce::findMaximum (src, num);
   #endif
}

void JUCE_CALLTYPE FloatVectorOperations::enableFlushToZeroMode (bool shouldEnable) noexcept
{
   #if JUCE_USE_SSE_INTRINSICS
    if (FloatVectorHelpers::isSSE2Available())
        _MM_SET_FLUSH_ZERO_MODE (shouldEnable ? _MM_FLUSH_ZERO_ON : _MM_FLUSH_ZERO_OFF);
   #endif
    ignoreUnused (shouldEnable);
}

//==============================================================================
//==============================================================================
#if JUCE_UNIT_TESTS

class FloatVectorOperationsTests  : public UnitTest
{
public:
    FloatVectorOperationsTests() : UnitTest ("FloatVectorOperations") {}

    template <typename ValueType>
    struct TestRunner
    {
        static void runTest (UnitTest& u, Random random)
        {
            const int range = random.nextBool() ? 500 : 10;
            const int num = random.nextInt (range) + 1;

            HeapBlock<ValueType> buffer1 ((size_t) num + 16), buffer2 ((size_t) num + 16);
            HeapBlock<int> buffer3 ((size_t) num + 16);

           #if JUCE_ARM
            ValueType* const data1 = buffer1;
            ValueType* const data2 = buffer2;
            int* const int1 = buffer3;
           #else
            ValueType* const data1 = addBytesToPointer (buffer1.getData(), random.nextInt (16));
            ValueType* const data2 = addBytesToPointer (buffer2.getData(), random.nextInt (16));
            int* const int1 = addBytesToPointer (buffer3.getData(), random.nextInt (16));
           #endif

            fillRandomly (random, data1, num);
            fillRandomly (random, data2, num);

            Range<ValueType> minMax1 (FloatVectorOperations::findMinAndMax (data1, num));
            Range<ValueType> minMax2 (Range<ValueType>::findMinAndMax (data1, num));
            u.expect (minMax1 == minMax2);

            u.expect (valuesMatch (FloatVectorOperations::findMinimum (data1, num), juce::findMinimum (data1, num)));
            u.expect (valuesMatch (FloatVectorOperations::findMaximum (data1, num), juce::findMaximum (data1, num)));

            u.expect (valuesMatch (FloatVectorOperations::findMinimum (data2, num), juce::findMinimum (data2, num)));
            u.expect (valuesMatch (FloatVectorOperations::findMaximum (data2, num), juce::findMaximum (data2, num)));

            FloatVectorOperations::clear (data1, num);
            u.expect (areAllValuesEqual (data1, num, 0));

            FloatVectorOperations::fill (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 2));

            FloatVectorOperations::add (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 4));

            FloatVectorOperations::copy (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 4));

            FloatVectorOperations::add (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 8));

            FloatVectorOperations::copyWithMultiply (data2, data1, (ValueType) 4, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 16));

            FloatVectorOperations::addWithMultiply (data2, data1, (ValueType) 4, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 32));

            FloatVectorOperations::multiply (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 8));

            FloatVectorOperations::multiply (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 256));

            FloatVectorOperations::negate (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) -256));

            FloatVectorOperations::subtract (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 512));

            FloatVectorOperations::abs (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 256));

            FloatVectorOperations::abs (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 256));

            fillRandomly (random, int1, num);
            doConversionTest (u, data1, data2, int1, num);

            FloatVectorOperations::fill (data1, (ValueType) 2, num);
            FloatVectorOperations::fill (data2, (ValueType) 3, num);
            FloatVectorOperations::addWithMultiply (data1, data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 8));
        }

        static void doConversionTest (UnitTest& u, float* data1, float* data2, int* const int1, int num)
        {
            FloatVectorOperations::convertFixedToFloat (data1, int1, 2.0f, num);
            convertFixed (data2, int1, 2.0f, num);
            u.expect (buffersMatch (data1, data2, num));
        }

        static void doConversionTest (UnitTest&, double*, double*, int*, int) {}

        static void fillRandomly (Random& random, ValueType* d, int num)
        {
            while (--num >= 0)
                *d++ = (ValueType) (random.nextDouble() * 1000.0);
        }

        static void fillRandomly (Random& random, int* d, int num)
        {
            while (--num >= 0)
                *d++ = random.nextInt();
        }

        static void convertFixed (float* d, const int* s, ValueType multiplier, int num)
        {
            while (--num >= 0)
                *d++ = *s++ * multiplier;
        }

        static bool areAllValuesEqual (const ValueType* d, int num, ValueType target)
        {
            while (--num >= 0)
                if (*d++ != target)
                    return false;

            return true;
        }

        static bool buffersMatch (const ValueType* d1, const ValueType* d2, int num)
        {
            while (--num >= 0)
                if (! valuesMatch (*d1++, *d2++))
                    return false;

            return true;
        }

        static bool valuesMatch (ValueType v1, ValueType v2)
        {
            return std::abs (v1 - v2) < std::numeric_limits<ValueType>::epsilon();
        }
    };

    void runTest() override
    {
        beginTest ("FloatVectorOperations");

        for (int i = 1000; --i >= 0;)
        {
            TestRunner<float>::runTest (*this, getRandom());
            TestRunner<double>::runTest (*this, getRandom());
        }
    }
};

static FloatVectorOperationsTests vectorOpTests;

#endif
