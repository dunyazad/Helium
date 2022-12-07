#include <cuda_runtime.h>

#include <Helium/Graphics/Geometry/Geometry.cuh>

#include <Helium/Graphics/Geometry/HeGeometryTriangleSoup.h>
#include <Helium/Graphics/HeVertexBufferObject.hpp>
#include <Helium/Core/Project/HeCameraInfo.h>

namespace ArtificialNature {
	
	union Triangle {
		struct {
			glm::vec3 v0;
			glm::vec3 v1;
			glm::vec3 v2;
		};
		struct {
			glm::vec3 n0;
			glm::vec3 n1;
			glm::vec3 n2;
		};
	};

	__global__ void CUDA_ComputeFaceNormals(Triangle* inputFaces, Triangle* outputFaceNormals, int faceCount)
	{
		int threadID = blockDim.x * blockIdx.x + threadIdx.x;

		if (threadID < faceCount)
		{
			//glm::vec3 d01 = glm::normalize(inputFaces[threadID].v1 - inputFaces[threadID].v0);
			//glm::vec3 d21 = glm::normalize(inputFaces[threadID].v2 - inputFaces[threadID].v0);
			//glm::vec3 n = glm::cross(d01, d21);

			//outputFaceNormals[threadID].n0 = n;
			//outputFaceNormals[threadID].n1 = n;
			//outputFaceNormals[threadID].n2 = n;

			outputFaceNormals[threadID].n0 = inputFaces[threadID].v0;
			outputFaceNormals[threadID].n1 = inputFaces[threadID].v1;
			outputFaceNormals[threadID].n2 = inputFaces[threadID].v2;
		}
	}

	void ComputeFaceNormals(HeGeometryTriangleSoup* pGeometry)
	{
		int faceCount = pGeometry->GetFaceCount();

		auto vbo = pGeometry->GetVBO();
		auto elements = vbo->GetElements();


		Triangle* inputFaces;
		Triangle* outputFaceNormals;

		cudaMalloc((void**)&inputFaces, faceCount * sizeof(Triangle));
		cudaMalloc((void**)&outputFaceNormals, faceCount * sizeof(Triangle));

		cudaMemcpy(inputFaces, &(elements[0]), faceCount * sizeof(Triangle), cudaMemcpyHostToDevice);

		dim3 blockSize(512, 1, 1);
		dim3 gridSize(512 / faceCount + 1, 1, 1);

		CUDA_ComputeFaceNormals << <gridSize, blockSize >> > (inputFaces, outputFaceNormals, faceCount);

		vector<glm::vec3> vertexNormals;
		vertexNormals.resize(faceCount * 3);

		cudaMemcpy(&vertexNormals[0], outputFaceNormals, faceCount * sizeof(Triangle), cudaMemcpyDeviceToHost);

		auto vnbo = pGeometry->GetVNBO();
		vnbo->SetElements(vertexNormals);
	}

	__global__ void CUDA_RayIntersect(Triangle* inputFaces, bool* outputIntersection, int faceCount, const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
	{
		int threadID = blockDim.x * blockIdx.x + threadIdx.x;

		if (threadID < faceCount)
		{
			glm::vec2 baricenter;
			float distance = 0.0f;
			if (glm::intersectRayTriangle(rayOrigin, rayDirection, inputFaces[threadID].v0, inputFaces[threadID].v1, inputFaces[threadID].v2, baricenter, distance))
			{
				if (distance > 0) {
					outputIntersection[threadID] = true;
				}
			}
		}
	}

	void RayIntersect(HeGeometryTriangleSoup* pGeometry, const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
	{
		int faceCount = pGeometry->GetFaceCount();

		auto vbo = pGeometry->GetVBO();
		auto elements = vbo->GetElements();

		Triangle* inputFaces;
		bool* outputIntersection;

		cudaMalloc((void**)&inputFaces, faceCount * sizeof(Triangle));
		cudaMalloc((void**)&outputIntersection, faceCount * sizeof(bool));

		cudaMemcpy(inputFaces, &(elements[0]), faceCount * sizeof(Triangle), cudaMemcpyHostToDevice);

		dim3 blockSize(512, 1, 1);
		dim3 gridSize(512 / faceCount + 1, 1, 1);

		CUDA_RayIntersect << <gridSize, blockSize >> > (inputFaces, outputIntersection, faceCount, rayOrigin, rayDirection);

		vector<bool> results;
		results.resize(faceCount);

		cudaMemcpy(&results[0], outputIntersection, faceCount * sizeof(bool), cudaMemcpyDeviceToHost);
	}

	__global__ void CUDA_ComputeOcclusionInfo(Triangle* inputFaces, bool* outputIntersection, int faceCount, glm::vec3* camInfos, int camInfoCount)
	{
		int threadID = blockDim.x * blockIdx.x + threadIdx.x;

		if (threadID < faceCount)
		{
			for (size_t j = 0; j < camInfoCount / 2; j++)
			{
				auto cameraPosition = camInfos[j * 2];
				auto cameraFront = camInfos[j * 2 + 1];

				for (int i = 0; i < faceCount; i++)
				{
					glm::vec2 baricenter;
					float distance = 0.0f;
					if (glm::intersectRayTriangle(cameraPosition, cameraFront, inputFaces[i].v0, inputFaces[i].v1, inputFaces[i].v2, baricenter, distance))
					{
						if (distance > 0) {
							outputIntersection[threadID] = true;
						}
					}
				}
			}
		}
	}

	void ComputeOcclusionInfo(HeGeometryTriangleSoup* pGeometry, const vector<const HeCameraInfo*>& cameraInfos)
	{
		int faceCount = pGeometry->GetFaceCount();

		auto vbo = pGeometry->GetVBO();
		auto elements = vbo->GetElements();

		Triangle* inputFaces_dev;

		cudaMalloc((void**)&inputFaces_dev, faceCount * sizeof(Triangle));
		cudaMemcpy(inputFaces_dev, &(elements[0]), faceCount * sizeof(Triangle), cudaMemcpyHostToDevice);

		bool* outputIntersection_dev;
		cudaMalloc((void**)&outputIntersection_dev, faceCount * cameraInfos.size() * sizeof(bool));

		vector<glm::vec3> cam_infos;
		cam_infos.resize(cameraInfos.size() * 2);
		for (size_t i = 0; i < cameraInfos.size(); i++)
		{
			auto info = cameraInfos[i];
			auto cameraPosition = info->GetPosition();
			auto cameraFront = glm::vec3(info->GetViewMatrix()[3]);
			cam_infos[i * 2] = cameraPosition;
			cam_infos[i * 2 + 1] = cameraFront;
		}

		glm::vec3* cameraInfos_dev;
		cudaMalloc((void**)&cameraInfos_dev, cameraInfos.size() * sizeof(glm::vec3) * 2);
		cudaMemcpy(cameraInfos_dev, &(cameraInfos_dev[0]), cameraInfos.size() * sizeof(glm::vec3) * 2, cudaMemcpyHostToDevice);


		dim3 blockSize(512, 1, 1);
		dim3 gridSize(512 / faceCount + 1, 1, 1);

		CUDA_ComputeOcclusionInfo << <gridSize, blockSize >> > (inputFaces_dev, outputIntersection_dev, faceCount, cameraInfos_dev, cameraInfos.size() * 2);

		vector<bool> results;
		results.resize(faceCount * cameraInfos.size());
		cudaMemcpy(&results[0], outputIntersection_dev, faceCount * cameraInfos.size() * sizeof(bool), cudaMemcpyDeviceToHost);
	}
}
