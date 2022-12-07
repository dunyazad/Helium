#define BUILDING_CUDA_CODE

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeGeometryTriangleSoup;
	class HeCameraInfo;

	void ComputeFaceNormals(HeGeometryTriangleSoup* pGeometry);

	void RayIntersect(HeGeometryTriangleSoup* pGeometry, const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

	void ComputeOcclusionInfo(HeGeometryTriangleSoup* pGeometry, const vector<const HeCameraInfo*>& cameraInfos);
}