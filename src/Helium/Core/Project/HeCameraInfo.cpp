#include <Helium/Core/Project/Project.h>
#include <Helium/Graphics/Graphics.h>

namespace ArtificialNature {

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const string& cameraInfoFile)
	{
		HeFile file;
		if (file.Open(cameraInfoFile, true))
		{
			int size = file.GetFileLength();
			char* buffer = new char[size];
			file.Read(buffer, size);
			file.Close();

			int index = 0;
			memcpy(&this->frameIndex, buffer + index, 4); index += 4;
			memcpy(&this->imageWidth, buffer + index, 4); index += 4;
			memcpy(&this->imageHeight, buffer + index, 4); index += 4;

			memcpy(&this->fx, buffer + index, 4); index += 4;
			memcpy(&this->fy, buffer + index, 4); index += 4;
			memcpy(&this->ox, buffer + index, 4); index += 4;
			memcpy(&this->oy, buffer + index, 4); index += 4;

			float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
			float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
			float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
			float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;
			//float projection_matrix[16]; memcpy(&projection_matrix, buffer + index, 64); index += 64;
			//float view_projection_matrix[16]; memcpy(&view_projection_matrix, buffer + index, 64); index += 64;

			delete[] buffer;

			glm::mat4 tm = glm::make_mat4(transform_matrix);
			auto p = glm::vec3(glm::row(tm, 3));
			auto r = glm::mat3(glm::transpose(tm)) * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			this->transformMatrix = glm::translate(glm::mat4(r), p);
			this->transformMatrix = glm::transpose(this->transformMatrix);

			this->viewMatrix = glm::make_mat4(view_matrix);
			this->viewMatrixInversed = glm::inverse(this->viewMatrix);

			//this->viewMatrix = glm::transpose(glm::make_mat4(view_matrix));

			glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
			intrinsic_matrix[2][2] = 1.0f;

			this->intrinsicMatrix = intrinsic_matrix;
			this->fx = this->intrinsicMatrix[0][0];
			this->fy = this->intrinsicMatrix[1][1];
			this->ox = this->intrinsicMatrix[0][2];
			this->oy = this->intrinsicMatrix[1][2];


			this->localToWorldMatrix = glm::inverse(glm::flip_axes(this->viewMatrixInversed));

			//this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

			this->extrinsicMatrix = this->localToWorldMatrix;

			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			//this->projectionMatrix = glm::transpose(glm::make_mat4(projection_matrix));

			//this->viewProjectionMatrix = glm::make_mat4(view_projection_matrix);

			this->position = p;
			this->rotation = r;

			auto r0 = glm::row(r, 0);
			auto r1 = -glm::row(r, 1);
			auto r2 = -glm::row(r, 2);

			glm::mat3 rr;
			rr[0] = r0;
			rr[1] = r1;
			rr[2] = r2;

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}
	}

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const string& cameraInfoFile, const json& reconstructionInfo)
	{
		HeFile file;
		if (file.Open(cameraInfoFile, true))
		{
			int size = file.GetFileLength();
			char* buffer = new char[size];
			file.Read(buffer, size);
			file.Close();

			int index = 0;
			memcpy(&this->frameIndex, buffer + index, 4); index += 4;
			memcpy(&this->imageWidth, buffer + index, 4); index += 4;
			memcpy(&this->imageHeight, buffer + index, 4); index += 4;

			memcpy(&this->fx, buffer + index, 4); index += 4;
			memcpy(&this->fy, buffer + index, 4); index += 4;
			memcpy(&this->ox, buffer + index, 4); index += 4;
			memcpy(&this->oy, buffer + index, 4); index += 4;

			float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
			float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
			float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
			float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;
			//float projection_matrix[16]; memcpy(&projection_matrix, buffer + index, 64); index += 64;
			//float view_projection_matrix[16]; memcpy(&view_projection_matrix, buffer + index, 64); index += 64;

			delete[] buffer;

			glm::mat4 tm = glm::make_mat4(transform_matrix);
			auto p = glm::vec3(glm::row(tm, 3));
			auto r = glm::mat3(glm::transpose(tm)) * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			this->transformMatrix = glm::mat4(r);
			this->transformMatrix[3] = glm::vec4(p, 1.0f);
			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			this->viewMatrix = glm::make_mat4(view_matrix);
			this->viewMatrixInversed = glm::inverse(this->viewMatrix);

			//this->viewMatrix = glm::transpose(glm::make_mat4(view_matrix));

			glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
			intrinsic_matrix[2][2] = 1.0f;

			this->intrinsicMatrix = intrinsic_matrix;
			this->fx = this->intrinsicMatrix[0][0];
			this->fy = this->intrinsicMatrix[1][1];
			this->ox = this->intrinsicMatrix[0][2];
			this->oy = this->intrinsicMatrix[1][2];


			this->localToWorldMatrix = glm::inverse(glm::flip_axes(this->viewMatrixInversed));

			//this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

			this->extrinsicMatrix = this->localToWorldMatrix;

			//this->projectionMatrix = glm::transpose(glm::make_mat4(projection_matrix));

			//this->viewProjectionMatrix = glm::make_mat4(view_projection_matrix);

			this->position = p;
			this->rotation = r;

			auto r0 = glm::row(r, 0);
			auto r1 = -glm::row(r, 1);
			auto r2 = -glm::row(r, 2);

			glm::mat3 rr;
			rr[0] = r0;
			rr[1] = r1;
			rr[2] = r2;

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}

		{
			auto& info = reconstructionInfo["frames"][frameInfo->GetFrameIndex()];
			this->imageWidth = info["image_width"].get<int>();
			this->imageHeight = info["image_height"].get<int>();
			this->fx = info["fx"].get<float>();
			this->fy = info["fy"].get<float>();
			this->ox = info["ox"].get<float>();
			this->oy = info["oy"].get<float>();
			this->intrinsicMatrix = glm::make_mat4(info["intrinsic_matrix"].get<vector<float>>().data());
			this->extrinsicMatrix = glm::make_mat4(info["extrinsic_matrix"].get<vector<float>>().data());
			this->position = glm::vec3(glm::row(glm::inverse(this->extrinsicMatrix), 3));
			this->rotation = glm::mat3(this->extrinsicMatrix);

			this->transformMatrix = glm::mat4(this->rotation);
			this->transformMatrix[3] = glm::vec4(this->position, 1.0f);
			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);

			auto rr = this->rotation * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			rr[2] = -rr[2];

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}
	}

	HeCameraInfo::~HeCameraInfo()
	{
		delete this->frustum;
	}

	glm::vec3 RayPlaneIntersection(const glm::vec3& planePoint, const glm::vec3& planeNormal, const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
	{
		auto ndotu = glm::dot(planeNormal, rayDirection);

		if (abs(ndotu) < FLT_EPSILON) {
			cout << "No intersection or line is within plane." << endl;
		}

		auto w = rayOrigin - planePoint;
		auto si = -glm::dot(planeNormal, w) / ndotu;
		return w + si * rayDirection + planePoint;
	}

	glm::vec2 HeCameraInfo::WorldToUV(const glm::vec3& worldPosition) const
	{
		//auto pp = this->viewProjectionMatrix * glm::vec4(worldPosition, 1.0f);

		auto ip4 = this->transformMatrixInversed * glm::vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1);
		//auto uv4 = this->projectionMatrix * this->transformMatrixInversed * glm::vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1);
		glm::vec3 planePoint = glm::vec3(0, 0, this->fx);
		glm::vec3 planeNormal = glm::vec3(0, 0, 1);
		glm::vec3 rayOrigin = glm::vec3(0, 0, 0);
		glm::vec3 rayDirection = glm::normalize(glm::vec3(ip4.x, ip4.y, ip4.z));
		glm::vec3 intersection = RayPlaneIntersection(planePoint, planeNormal, rayOrigin, rayDirection);
		float u = ((intersection.x / (this->imageWidth * 0.5f)) * 0.5f + 0.5f);
		float v = 1 - ((intersection.y / (this->imageHeight * 0.5f)) * 0.5f + 0.5f);
		return glm::vec2(u, v);
	}
}
