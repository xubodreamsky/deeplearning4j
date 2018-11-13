/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//  @author raver119@gmail.com
//  @author Yurii Shyrma (iuriish@yahoo.com)

#ifndef PAIRWISE_CU
#define PAIRWISE_CU

#ifdef __CUDACC__

#include "../pairwise_transform.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <op_boilerplate.h>
#include <helpers/TAD.h>
#include <types/float16.h>
#include <helpers/DebugHelper.h>

#include "../legacy_ops.h"

using namespace simdOps;

namespace functions           {
namespace pairwise_transforms {

////////////////////////////////////////////////////////////////////////////////
	
template<typename X, typename Y, typename Z>
template<typename OpType>
__device__ void PairWiseTransform<X,Y,Z>::transformCuda(Nd4jLong len,
														void *vx, void *vy,
														Nd4jLong xEws, Nd4jLong yEws,
														void *vparams,
														void *vz, Nd4jLong zEws,
														int *allocationPointer, 
														UnifiedSharedMemory *manager,
														Nd4jLong *tadOnlyShapeInfo) {
	auto x = static_cast<X*>(vx);
	auto y = static_cast<Y*>(vy);
	auto result = static_cast<Z*>(vz);
	auto params = static_cast<Z*>(vparams);

	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	if(xEws == yEws && yEws == zEws && xEws == 1) {
		for (Nd4jLong i = tid; i < len; i += gridDim.x * blockDim.x) 
				result[i] = OpType::op(x[i], y[i], params);
	}
	else {
		for (Nd4jLong i = tid; i < len; i += gridDim.x * blockDim.x) 
			result[i * zEws] = OpType::op(x[i * xEws], y[i * yEws], params);			
	}
}






}
}

/**
 * The api for the driver interface
 * @param opNum the op number
 * @param len the length of the problem
 * @param xOffset the offset for x
 * @param yOffset the offset for y
 * @param resultOffset the offset for result
 * @param x the input
 * @param y the pair wise array
 * @param xEws the stride for x
 * @param yEws the stride for y
 * @param params the parameters for the problem
 * @param result the result buffer
 * @param zEws the result stride
 * @param blockSize the block size
 */

// template<typename X, typename Y, typename Z, typename opType>
// __device__ void pairWiseTransformGeneric(
// 		void *x,
// 		void *y,
// 		void *params,
// 		void *result,
// 		Nd4jLong *xShapeInfo, int xRank,
// 		Nd4jLong *yShapeInfo, int yRank,
// 		Nd4jLong *resultShapeInfo, int zRank, 
// 		int *allocationPointer, 
// 		Nd4jLong *tadOnlyShapeInfo) {

// 	functions::pairwise_transforms::PairWiseTransform<X,Y,Z>::template transformCuda<opType>(
// 	    x,
// 	    xShapeInfo,
// 	    y,
// 	    yShapeInfo,
// 	    result,
// 	    resultShapeInfo,
// 	    params,
// 	    allocationPointer,
// 	    nullptr, tadOnlyShapeInfo);
// }

// /**
//  * The api for the driver interface
//  * @param opNum the op number
//  * @param len the length of the problem
//  * @param xOffset the offset for x
//  * @param yOffset the offset for y
//  * @param resultOffset the offset for result
//  * @param x the input
//  * @param y the pair wise array
//  * @param xEws the stride for x
//  * @param yEws the stride for y
//  * @param params the parameters for the problem
//  * @param result the result buffer
//  * @param zEws the result stride
//  * @param blockSize the block size
//  */

// /*
// extern "C" __global__ void pairWiseTransformDouble(
// 		int opNum,
// 		double *x,
// 		double *y,
// 		double *params,
// 		double *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<double>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo);

// }

// extern "C" __global__ void pairWiseTransformFloat(
// 		int opNum,
// 		float *x,
// 		float *y,
// 		float *params,
// 		float *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<float>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo);

// }

// extern "C" __global__ void pairWiseTransformHalf(
// 		int opNum,
// 		float16 *x,
// 		float16 *y,
// 		float16 *params,
// 		float16 *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<float16>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo);
// }

// */

// /**
//  * The api for the driver interface
//  * @param opNum the op number
//  * @param len the length of the problem
//  * @param xOffset the offset for x
//  * @param yOffset the offset for y
//  * @param resultOffset the offset for result
//  * @param x the input
//  * @param y the pair wise array
//  * @param xEws the stride for x
//  * @param yEws the stride for y
//  * @param params the parameters for the problem
//  * @param result the result buffer
//  * @param zEws the result stride
//  * @param blockSize the block size
//  */

// /*
// template <typename T>
// __device__ void pairWiseTransformGeneric(
// 		int opNum,
// 		T *x,
// 		T *y,
// 		T *params,
// 		T *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank,
// 		int *xIndexes,
// 		int *yIndexes,
// 		int *resultIndexes, int *allocationPointer, int *tadOnlyShapeInfo) {

// 	__shared__ UnifiedSharedMemory *manager;

//      if (threadIdx.x == 0) {
//         extern __shared__ unsigned char shmem[];
//         manager = new(shmem) UnifiedSharedMemory((int *) shmem);
// 	    manager->init(sizeof(UnifiedSharedMemory), 0, sizeof(functions::pairwise_transforms::PairWiseTransform<T>), sizeof(shape::TAD), xRank);
//     }
//     __syncthreads();

// 	functions::pairwise_transforms::PairWiseTransform<T>::transformCuda(
// 			opNum,
// 			x,
// 			xShapeInfo,
// 			y,
// 			yShapeInfo,
// 			result,
// 			resultShapeInfo,
// 			params,
// 			xIndexes,
// 			yIndexes,
// 			resultIndexes,
// 			allocationPointer,
// 			manager,
// 			tadOnlyShapeInfo);

// }
// */

// /**
//  * The api for the driver interface
//  * @param opNum the op number
//  * @param len the length of the problem
//  * @param xOffset the offset for x
//  * @param yOffset the offset for y
//  * @param resultOffset the offset for result
//  * @param x the input
//  * @param y the pair wise array
//  * @param xEws the stride for x
//  * @param yEws the stride for y
//  * @param params the parameters for the problem
//  * @param result the result buffer
//  * @param zEws the result stride
//  * @param blockSize the block size
//  */
// /*
// extern "C" __global__ void pairWiseTransformDoubleIndex(
// 		int opNum,
// 		double *x,
// 		double *y,
// 		double *params,
// 		double *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank,
// 		int *xIndexes,
// 		int *yIndexes,
// 		int *resultIndexes, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<double>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank,
// 			xIndexes,
// 			yIndexes,
// 			resultIndexes, allocationPointer, tadOnlyShapeInfo);

// }

// extern "C" __global__ void pairWiseTransformFloatIndex(
// 		int opNum,
// 		float *x,
// 		float *y,
// 		float *params,
// 		float *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank,
// 		int *xIndexes,
// 		int *yIndexes,
// 		int *resultIndexes, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<float>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank,
// 			xIndexes,
// 			yIndexes,
// 			resultIndexes, allocationPointer, tadOnlyShapeInfo);
// }

// extern "C" __global__ void pairWiseTransformHalfIndex(
// 		int opNum,
// 		float16 *x,
// 		float16 *y,
// 		float16 *params,
// 		float16 *result,
// 		int *xShapeInfo, int xRank,
// 		int *yShapeInfo, int yRank,
// 		int *resultShapeInfo, int zRank,
// 		int *xIndexes,
// 		int *yIndexes,
// 		int *resultIndexes, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformGeneric<float16>(
// 			opNum,
// 			x,
// 			y,
// 			params,
// 			result,
// 			xShapeInfo, xRank,
// 			yShapeInfo, yRank,
// 			resultShapeInfo, zRank,
// 			xIndexes,
// 			yIndexes,
// 			resultIndexes, allocationPointer, tadOnlyShapeInfo);
// }

// */

// /**
//  * The api for the driver interface
//  * @param opNum the op number
//  * @param len the length of the problem
//  * @param xOffset the offset for x
//  * @param yOffset the offset for y
//  * @param resultOffset the offset for result
//  * @param x the input
//  * @param y the pair wise array
//  * @param xEws the stride for x
//  * @param yEws the stride for y
//  * @param params the parameters for the problem
//  * @param result the result buffer
//  * @param zEws the result stride
//  * @param blockSize the block size
//  */
// template<typename X, typename Y, typename Z, typename opType>
// __device__ void pairWiseTransformStridedGeneric(
// 		Nd4jLong len,
// 		void *x,
// 		void *y,
// 		Nd4jLong xEws,
// 		Nd4jLong yEws,
// 		void *params,
// 		void *result,
// 		Nd4jLong zEws, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo) {

// 	functions::pairwise_transforms::PairWiseTransform<X,Y,Z>::template transformCuda<opType>(
// 		len,
// 		x,
// 		y,
// 		xEws,
// 		yEws,
// 		params,
// 		result,
// 		zEws,
// 		allocationPointer,
// 		nullptr,
// 		tadOnlyShapeInfo);
// }



// /**
//  * The api for the driver interface
//  * @param opNum the op number
//  * @param len the length of the problem
//  * @param xOffset the offset for x
//  * @param yOffset the offset for y
//  * @param resultOffset the offset for result
//  * @param x the input
//  * @param y the pair wise array
//  * @param xEws the stride for x
//  * @param yEws the stride for y
//  * @param params the parameters for the problem
//  * @param result the result buffer
//  * @param zEws the result stride
//  * @param blockSize the block size
//  */
// /*
// extern "C" __global__ void pairWiseTransformStridedDouble(
// 		int opNum,
// 		Nd4jLong len,
// 		double *x,
// 		double *y,
// 		int xEws,
// 		int yEws,
// 		double *params,
// 		double *result,
// 		int zEws, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformStridedGeneric<double>(
// 			opNum,
// 			len,
// 			x,
// 			y,
// 			xEws,
// 			yEws,
// 			params,
// 			result,
// 			zEws, allocationPointer, tadOnlyShapeInfo);
// }

// extern "C" __global__ void pairWiseTransformStridedFloat(
// 		int opNum,
// 		Nd4jLong len,
// 		float *x,
// 		float *y,
// 		int xEws,
// 		int yEws,
// 		float *params,
// 		float *result,
// 		int zEws, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformStridedGeneric<float>(
// 			opNum,
// 			len,
// 			x,
// 			y,
// 			xEws,
// 			yEws,
// 			params,
// 			result,
// 			zEws, allocationPointer, tadOnlyShapeInfo);
// }


// extern "C" __global__ void pairWiseTransformStridedHalf(
// 		int opNum,
// 		Nd4jLong len,
// 		float16 *x,
// 		float16 *y,
// 		int xEws,
// 		int yEws,
// 		float16 *params,
// 		float16 *result,
// 		int zEws, int *allocationPointer, int *tadOnlyShapeInfo) {
// 	pairWiseTransformStridedGeneric<float16>(
// 			opNum,
// 			len,
// 			x,
// 			y,
// 			xEws,
// 			yEws,
// 			params,
// 			result,
// 			zEws, allocationPointer, tadOnlyShapeInfo);
// }
// */


// // pwt shape
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleShaped_, pairWiseTransformGeneric, float, INPUT(float *x, float *y, float *params, float *result, Nd4jLong *xShapeInfo, int xRank, Nd4jLong *yShapeInfo, int yRank, Nd4jLong *resultShapeInfo, int zRank, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(x, y, params, result, xShapeInfo, xRank, yShapeInfo, yRank, resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleShaped_, pairWiseTransformGeneric, double, INPUT(double *x, double *y, double *params, double *result, Nd4jLong *xShapeInfo, int xRank, Nd4jLong *yShapeInfo, int yRank, Nd4jLong *resultShapeInfo, int zRank, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(x, y, params, result, xShapeInfo, xRank, yShapeInfo, yRank, resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleShaped_, pairWiseTransformGeneric, float16, INPUT(float16 *x, float16 *y, float16 *params, float16 *result, Nd4jLong *xShapeInfo, int xRank, Nd4jLong *yShapeInfo, int yRank, Nd4jLong *resultShapeInfo, int zRank, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(x, y, params, result, xShapeInfo, xRank, yShapeInfo, yRank, resultShapeInfo, zRank, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

// // pwt strided
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleStrided_, pairWiseTransformStridedGeneric, float, INPUT(Nd4jLong len, float *x, float *y, Nd4jLong xEws, Nd4jLong yEws, float *params, float *result, Nd4jLong zEws, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(len, x, y, xEws, yEws, params, result, zEws, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleStrided_, pairWiseTransformStridedGeneric, double, INPUT(Nd4jLong len, double *x, double *y, Nd4jLong xEws, Nd4jLong yEws, double *params, double *result, Nd4jLong zEws, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(len, x, y, xEws, yEws, params, result, zEws, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))
// // DISPATCH_KERNEL_SIMPLE(pwtSimpleStrided_, pairWiseTransformStridedGeneric, float16, INPUT(Nd4jLong len, float16 *x, float16 *y, Nd4jLong xEws, Nd4jLong yEws, float16 *params, float16 *result, Nd4jLong zEws, int *allocationPointer, Nd4jLong *tadOnlyShapeInfo), PARAMS(len, x, y, xEws, yEws, params, result, zEws, allocationPointer, tadOnlyShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))





//             template<typename X, typename Y, typename Z>
//             __host__ void PairWiseTransform<X,Y,Z>::execudaCudaStrided(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, void *x, Nd4jLong xStride, void *y, Nd4jLong yStride, void *result, Nd4jLong resultStride, void *extraParams, Nd4jLong len) {
//                 cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	            auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
// 	            auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	            if (nd4j::Environment::getInstance()->isDebugAndVerbose()) {
// 		            printf("F4 opNum:[%i]; <<<X: [%i]; Y: [%i]; Z: [%i]>>>\len", opNum, launchDims.x,launchDims.y, launchDims.z);
// 	            }

// 	            int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

//             	//pairWiseTransformStridedFloat<<<launchDims.x, launchDims.y, launchDims.z, *stream>>> ( opNum, len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo);
//             	// DISPATCH_SIMPLE(pwtSimpleStrided, float, PARAMS(len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

// 	            DEBUG_KERNEL(stream, opNum);
//             }

//            //  template<>
//            //  __host__ void PairWiseTransform<float16>::execudaCudaStrided(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, float16 *x, Nd4jLong xStride, float16 *y, Nd4jLong yStride, float16 *result, Nd4jLong resultStride, float16 *extraParams, Nd4jLong len) {
//            //      cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	          //   auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);

// 	          //   auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	          //   if (nd4j::Environment::getInstance()->isDebugAndVerbose())
// 		         //    printf("H4 opNum:[%i]\len", opNum);

// 	          //   int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

//            //  	//pairWiseTransformStridedHalf<<<launchDims.x,launchDims.y, launchDims.z, *stream>>> ( opNum, len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo);
//            //  	DISPATCH_SIMPLE(pwtSimpleStrided, float16, PARAMS(len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

// 	          //   DEBUG_KERNEL(stream, opNum);
//            //  }


//            // template<>
//            //  __host__ void PairWiseTransform<double>::execudaCudaStrided(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, double *x, Nd4jLong xStride, double *y, Nd4jLong yStride, double *result, Nd4jLong resultStride, double *extraParams, Nd4jLong len) {
//            //      cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	          //   auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
// 	          //   auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	          //   if (nd4j::Environment::getInstance()->isDebugAndVerbose())
// 		         //    printf("D4 opNum:[%i]\len", opNum);

// 	          //   int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

//            //  	//pairWiseTransformStridedDouble<<<launchDims.x,launchDims.y, launchDims.z, *stream>>> ( opNum, len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo);
//            //  	DISPATCH_SIMPLE(pwtSimpleStrided, double, PARAMS(len, x, y, xStride, yStride, extraParams, result, resultStride, allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))


// 	          //   DEBUG_KERNEL(stream, opNum);
//            //  }

//             template<typename X, typename Y, typename Z>
//             __host__ void PairWiseTransform<X,Y,Z>::execudaCudaShaped(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, void *x, Nd4jLong *xShapeInfo, void *y, Nd4jLong *yShapeInfo, void *result, Nd4jLong *resultShapeInfo, void *extraParams) {
//                 cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	            if (nd4j::Environment::getInstance()->isDebugAndVerbose())
// 		            printf("D6 opNum:[%i]\len", opNum);

//             	auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
// 	            auto hostYShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[7]);
// 	            auto hostZShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[8]);
//             	auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	            int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

// 	            //pairWiseTransformFloat<<<launchDims.x,launchDims.y, launchDims.z, *stream>>>( opNum, x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo);
//                 // DISPATCH_SIMPLE(pwtSimpleShaped, float, PARAMS(x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

// 	            DEBUG_KERNEL(stream, opNum);
//             }


//             // template<>
//             // __host__ void PairWiseTransform<float16>::execudaCudaShaped(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, float16 *x, Nd4jLong *xShapeInfo, float16 *y, Nd4jLong *yShapeInfo, float16 *result, Nd4jLong *resultShapeInfo, float16 *extraParams) {
//             //     cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	           //  if (nd4j::Environment::getInstance()->isDebugAndVerbose())
// 		          //   printf("H6 opNum:[%i]\len", opNum);

//             // 	auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
// 	           //  auto hostYShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[7]);
// 	           //  auto hostZShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[8]);
//             // 	auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	           //  int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

// 	           //  //pairWiseTransformHalf<<<launchDims.x,launchDims.y, launchDims.z, *stream>>>( opNum, x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo);
//             //     DISPATCH_SIMPLE(pwtSimpleShaped, float16, PARAMS(x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

// 	           //  DEBUG_KERNEL(stream, opNum);
//             // }


//             // template<>
//             // __host__ void PairWiseTransform<double>::execudaCudaShaped(dim3& launchDims, Nd4jPointer *extraPointers, int opNum, double *x, Nd4jLong *xShapeInfo, double *y, Nd4jLong *yShapeInfo, double *result, Nd4jLong *resultShapeInfo, double *extraParams) {
//             //     cudaStream_t *stream = reinterpret_cast<cudaStream_t *>(&extraPointers[1]);

// 	           //  if (nd4j::Environment::getInstance()->isDebugAndVerbose())
// 		          //   printf("H6 opNum:[%i]\len", opNum);

//             // 	auto hostXShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[0]);
// 	           //  auto hostYShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[7]);
// 	           //  auto hostZShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[8]);
//             // 	auto deviceTADShapeInfo = reinterpret_cast<Nd4jLong *>(extraPointers[10]);

// 	           //  int *allocationPointer = reinterpret_cast<int *>(extraPointers[3]);

// 	           //  //pairWiseTransformDouble<<<launchDims.x,launchDims.y, launchDims.z, *stream>>>( opNum, x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo);
//             //     DISPATCH_SIMPLE(pwtSimpleShaped, double, PARAMS(x, y, extraParams, result, xShapeInfo,  shape::rank(hostXShapeInfo), yShapeInfo,  shape::rank(hostYShapeInfo), resultShapeInfo,  shape::rank(hostZShapeInfo), allocationPointer, deviceTADShapeInfo), OPS_A(PAIRWISE_TRANSFORM_OPS))

//             //     DEBUG_KERNEL(stream, opNum);
//             // }

//             /*
//             template<typename T>
//             __device__ void PairWiseTransform<T>::transformCuda(const int opNum, Nd4jLong len, T *x, T *y, int xEws, int yEws, T *extraParams, T *result, int zEws, int *allocationPointer, UnifiedSharedMemory *manager, int *tadOnlyShapeInfo) {
//                     DISPATCH_BY_OPNUM(transformCuda, PARAMS(len, x, y, xEws, yEws, extraParams, result, zEws, allocationPointer, manager, tadOnlyShapeInfo), PAIRWISE_TRANSFORM_OPS);
// 			}


//             template<typename T>
// 			__device__ void PairWiseTransform<T>::transformCuda(const int opNum, T *x, int *xShapeBuffer, T *y, int *yShapeBuffer, T *result, int *resultShapeBuffer, T *extraParams, int *allocationPointer, UnifiedSharedMemory *manager,int *tadOnlyShapeInfo) {
//                     DISPATCH_BY_OPNUM(transformCuda, PARAMS(x, xShapeBuffer, y, yShapeBuffer, result, resultShapeBuffer, extraParams, allocationPointer, manager, tadOnlyShapeInfo), PAIRWISE_TRANSFORM_OPS);
// 			}
// */

// 			/*
//             template<typename T>
// 			__device__ void PairWiseTransform<T>::transformCuda(const int opNum, T *x, int *xShapeBuffer, T *y, int *yShapeBuffer, T *result, int *resultShapeBuffer, T *extraParams, int *indexes, int *yIndexes, int *resultIndexes, int *allocationPointer, UnifiedSharedMemory *manager, int *tadOnlyShapeInfo) {
//                     DISPATCH_BY_OPNUM(transform, PARAMS(x, xShapeBuffer, y, yShapeBuffer, result, resultShapeBuffer, extraParams, indexes, yIndexes, resultIndexes, allocationPointer, manager, tadOnlyShapeInfo), PAIRWISE_TRANSFORM_OPS);
// 			}
// 			*/


// 			/*
//             template<typename T>
// 			template<typename OpType>
// 	        __device__ void PairWiseTransform<T>::transform(T *x, int *xShapeBuffer, T *y, int *yShapeBuffer, T *result, int *resultShapeBuffer, T *extraParams, int *indexes, int *yIndexes, int *resultIndexes,  int *allocationPointer, UnifiedSharedMemory *manager, int *tadOnlyShapeInfo) {
// 	        	int tid = blockIdx.x * blockDim.x + threadIdx.x;
// 		        Nd4jLong len = shape::length(xShapeBuffer);

// 		        for (Nd4jLong i = tid; i < len; i += gridDim.x * blockDim.x) {
// 			        result[resultIndexes[i]] = OpType::op(x[indexes[i]],y[yIndexes[i]], extraParams);
// 		        }
// 	        }
// 	        */

// 	        /**
// 	 *
// 	 */
// 	    template<typename X, typename Y, typename Z>
//         template<typename OpType>
// 	    __device__ void PairWiseTransform<X,Y,Z>::transformCuda(void *vx, Nd4jLong *xShapeBuffer, void *vy, Nd4jLong *yShapeBuffer, void *vz, Nd4jLong *resultShapeBuffer, void *vextraParams, int *allocationPointer, UnifiedSharedMemory *manager, Nd4jLong *tadOnlyShapeInfo) {

// 	    	auto x = static_cast<X*>(vx);
// 	    	auto y = static_cast<Y*>(vy);
// 	    	auto result = static_cast<Z*>(vz);
// 	    	auto extraParams = static_cast<Z*>(vextraParams);

// 		    int tid = blockIdx.x * blockDim.x + threadIdx.x;

// 		    __shared__ int xRank;
// 		    __shared__ int yRank;
// 		    __shared__ int resultRank;

// 		    __shared__ int xEWS;
// 		    __shared__ int yEWS;
// 		    __shared__ int zEWS;

// 		    __shared__ char xOrder;
// 		    __shared__ char yOrder;
// 		    __shared__ char zOrder;

// 		    __shared__ bool xRow;
// 		    __shared__ bool yRow;
// 		    __shared__ bool zRow;

// 		    if (threadIdx.x == 0) {
// 		        xRank = shape::rank(xShapeBuffer);
//     		    yRank = shape::rank(yShapeBuffer);
// 	    	    resultRank = shape::rank(resultShapeBuffer);

// 		        xEWS = shape::elementWiseStride(xShapeBuffer);
// 		        yEWS = shape::elementWiseStride(yShapeBuffer);
//     		    zEWS = shape::elementWiseStride(resultShapeBuffer);

// 	    	    xOrder = shape::order(xShapeBuffer);
// 		        yOrder = shape::order(yShapeBuffer);
// 		        zOrder = shape::order(resultShapeBuffer);

// 		        xRow = shape::isRowVector(xShapeBuffer);
// 		        yRow = shape::isRowVector(yShapeBuffer);
// 		        zRow = shape::isRowVector(resultShapeBuffer);

// 		    }
// 		    __syncthreads();

// 		    Nd4jLong len = shape::length(xShapeBuffer);
// 		    if((xEWS >= 1 && yEWS == xEWS && zEWS == xEWS &&  xOrder == yOrder && zOrder == xOrder) || (xEWS >= 1 && yEWS == xEWS && zEWS == xEWS && xRow && yRow && zRow)) {
// 			    // TODO: this is wrong, and should be moved to host side
// 			    transformCuda<OpType>(
// 					len,
// 					x,
// 					y,
// 					xEWS,
// 					yEWS,
// 					extraParams,
// 					result,
// 					zEWS, allocationPointer, manager, tadOnlyShapeInfo);

//     		} else {

//     			if (x == result) {
// 					Nd4jLong xCoord[MAX_RANK];
// 					Nd4jLong yCoord[MAX_RANK];

// 	    			for (Nd4jLong i = tid; i < len; i += gridDim.x * blockDim.x) {
// 		    			shape::ind2subC(xRank,shape::shapeOf(xShapeBuffer), i, len, xCoord);
// 			    		shape::ind2subC(yRank,shape::shapeOf(yShapeBuffer), i, len, yCoord);

// 				    	auto xOffset = shape::getOffset(0, shape::shapeOf(xShapeBuffer), shape::stride(xShapeBuffer), xCoord, xRank);
// 					    auto yOffset = shape::getOffset(0, shape::shapeOf(yShapeBuffer), shape::stride(yShapeBuffer), yCoord, yRank);
//     					result[xOffset] = OpType::op(x[xOffset], y[yOffset], extraParams);
// 	    			}
// 		    	} else {
// 					Nd4jLong xCoord[MAX_RANK];
// 			    	Nd4jLong yCoord[MAX_RANK];
//     		    	Nd4jLong resultCoord[MAX_RANK];

//     				for (Nd4jLong i = tid; i < len; i += gridDim.x * blockDim.x) {
// 	    				shape::ind2subC(xRank,shape::shapeOf(xShapeBuffer), i, len, xCoord);
// 		    			shape::ind2subC(yRank,shape::shapeOf(yShapeBuffer), i, len, yCoord);
// 			    		shape::ind2subC(resultRank,shape::shapeOf(resultShapeBuffer), i, len, resultCoord);

//     					auto xOffset = shape::getOffset(0, shape::shapeOf(xShapeBuffer), shape::stride(xShapeBuffer), xCoord, xRank);
// 	    				auto yOffset = shape::getOffset(0, shape::shapeOf(yShapeBuffer), shape::stride(yShapeBuffer), yCoord, yRank);
// 		    			auto resultOffset = shape::getOffset(0, shape::shapeOf(resultShapeBuffer), shape::stride(resultShapeBuffer), resultCoord, resultRank);
// 			    		result[resultOffset] = OpType::op(x[xOffset], y[yOffset], extraParams);
//     				}
//     			}
//     		}
//     	}


//      /*
// 	 template<typename T>
// 	 __device__ void transform(
// 			T *x,
// 			int *xShapeBuffer,
// 			T *y,
// 			int *yShapeBuffer,
// 			T *result,
// 			int *resultShapeBuffer,
// 			T *extraParams,
// 			Nd4jLong len,
// 			int *indexes,
// 			int *allocationPointer,
// 			UnifiedSharedMemory *manager,
// 			int *tadOnlyShapeInfo) {
// 		transform(x,
// 				xShapeBuffer,
// 				y,
// 				yShapeBuffer,
// 				result,
// 				resultShapeBuffer,
// 				extraParams,
// 				indexes,
// 				indexes,
// 				indexes, allocationPointer, manager, tadOnlyShapeInfo);
// 	    }


// 	 template<typename T>
// 	 __device__ void transform(
// 			T *x,
// 			int *xShapeBuffer,
// 			T *y,
// 			int *yShapeBuffer,
// 			T *result,
// 			int *resultShapeBuffer,
// 			T *extraParams,
// 			int *indexes,
// 			int *yIndexes,
// 			int *allocationPointer,
// 			UnifiedSharedMemory *manager,
// 			int *tadOnlyShapeInfo) {
// 		transform(x,
// 				xShapeBuffer,
// 				y,
// 				yShapeBuffer,
// 				result,
// 				resultShapeBuffer,
// 				extraParams,
// 				indexes,
// 				yIndexes,
// 				indexes, allocationPointer, manager, tadOnlyShapeInfo);
// 	    }
// */

// 	    /**
// 	 *
// 	 * @param len
// 	 * @param xOffset
// 	 * @param yOffset
// 	 * @param resultOffset
// 	 * @param x
// 	 * @param y
// 	 * @param xEws
// 	 * @param yEws
// 	 * @param params
// 	 * @param result
// 	 * @param zEws
// 	 * @param blockSize
// 	 */
    





#endif // CUDA_CC

#endif // PAIRWISE_CU