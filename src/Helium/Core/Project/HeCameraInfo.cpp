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
			memcpy(&this->depthImageWidth, buffer + index, 4); index += 4;
			memcpy(&this->depthImageHeight, buffer + index, 4); index += 4;
			this->colorImageWidth = int(depthImageWidth * 7.5);
			this->colorImageHeight = int(depthImageHeight * 7.5);

			memcpy(&this->original_fx, buffer + index, 4); index += 4;
			memcpy(&this->original_fy, buffer + index, 4); index += 4;
			memcpy(&this->original_ox, buffer + index, 4); index += 4;
			memcpy(&this->original_oy, buffer + index, 4); index += 4;

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
			this->scaled_fx = this->intrinsicMatrix[0][0];
			this->scaled_fy = this->intrinsicMatrix[1][1];
			this->scaled_ox = this->intrinsicMatrix[0][2];
			this->scaled_oy = this->intrinsicMatrix[1][2];


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
			this->frustum = new HeFrustum(this->transformMatrix, int(this->colorImageWidth), int(this->colorImageHeight), this->original_fx, this->original_fy);
		}
	}

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const string& cameraInfoFile, const json& reconstructionInfo)
	{
		//HeFile file;
		//if (file.Open(cameraInfoFile, true))
		//{
		//	int size = file.GetFileLength();
		//	char* buffer = new char[size];
		//	file.Read(buffer, size);
		//	file.Close();

		//	int index = 0;
		//	memcpy(&this->frameIndex, buffer + index, 4); index += 4;
		//	memcpy(&this->imageWidth, buffer + index, 4); index += 4;
		//	memcpy(&this->imageHeight, buffer + index, 4); index += 4;

		//	memcpy(&this->fx, buffer + index, 4); index += 4;
		//	memcpy(&this->fy, buffer + index, 4); index += 4;
		//	memcpy(&this->ox, buffer + index, 4); index += 4;
		//	memcpy(&this->oy, buffer + index, 4); index += 4;

		//	float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
		//	float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
		//	float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
		//	float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;
		//	//float projection_matrix[16]; memcpy(&projection_matrix, buffer + index, 64); index += 64;
		//	//float view_projection_matrix[16]; memcpy(&view_projection_matrix, buffer + index, 64); index += 64;

		//	delete[] buffer;

		//	glm::mat4 tm = glm::make_mat4(transform_matrix);
		//	auto p = glm::vec3(glm::row(tm, 3));
		//	auto r = glm::mat3(glm::transpose(tm)) * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
		//	this->transformMatrix = glm::mat4(r);
		//	this->transformMatrix[3] = glm::vec4(p, 1.0f);
		//	this->transformMatrixInversed = glm::inverse(this->transformMatrix);

		//	this->viewMatrix = glm::make_mat4(view_matrix);
		//	this->viewMatrixInversed = glm::inverse(this->viewMatrix);

		//	//this->viewMatrix = glm::transpose(glm::make_mat4(view_matrix));

		//	glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
		//	intrinsic_matrix[2][2] = 1.0f;

		//	this->intrinsicMatrix = intrinsic_matrix;
		//	this->fx = this->intrinsicMatrix[0][0];
		//	this->fy = this->intrinsicMatrix[1][1];
		//	this->ox = this->intrinsicMatrix[0][2];
		//	this->oy = this->intrinsicMatrix[1][2];


		//	this->localToWorldMatrix = glm::inverse(glm::flip_axes(this->viewMatrixInversed));

		//	//this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

		//	this->extrinsicMatrix = this->localToWorldMatrix;

		//	//this->projectionMatrix = glm::transpose(glm::make_mat4(projection_matrix));

		//	//this->viewProjectionMatrix = glm::make_mat4(view_projection_matrix);

		//	this->position = p;
		//	this->rotation = r;

		//	auto r0 = glm::row(r, 0);
		//	auto r1 = -glm::row(r, 1);
		//	auto r2 = -glm::row(r, 2);

		//	glm::mat3 rr;
		//	rr[0] = r0;
		//	rr[1] = r1;
		//	rr[2] = r2;

		//	//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
		//	this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		//}

		{
			auto& info = reconstructionInfo["frames"][frameInfo->GetFrameIndex()];
			this->frameIndex = frameInfo->GetFrameIndex();
			this->depthImageWidth = info["image_width"].get<int>();
			this->depthImageHeight = info["image_height"].get<int>();
			this->colorImageWidth = int(this->depthImageWidth * 7.5f);
			this->colorImageHeight = int(this->depthImageHeight * 7.5f);
			this->scaled_fx = info["fx"].get<float>();
			this->scaled_fy = info["fy"].get<float>();
			this->scaled_ox = info["ox"].get<float>();
			this->scaled_oy = info["oy"].get<float>();
			this->original_fx = this->scaled_fx * 7.5f;
			this->original_fy = this->scaled_fy * 7.5f;
			this->original_ox = this->scaled_ox * 7.5f;
			this->original_oy = this->scaled_oy * 7.5f;
			this->intrinsicMatrix = glm::make_mat4(info["intrinsic_matrix"].get<vector<float>>().data());
			this->extrinsicMatrix = glm::make_mat4(info["extrinsic_matrix"].get<vector<float>>().data());
			this->position = glm::vec3(glm::row(glm::inverse(this->extrinsicMatrix), 3)) * 1000.0f;
			this->rotation = glm::mat3(this->extrinsicMatrix);

			this->transformMatrix = glm::mat4(this->rotation);
			this->transformMatrix[3] = glm::vec4(this->position, 1.0f);
			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			if (info.contains("ambient_color_temperature")) {
				this->ambientColorTemperature = info["ambient_color_temperature"].get<float>();
			}
			if (info.contains("ambient_intensity")) {
				this->ambientIntensity = info["ambient_intensity"].get<float>();
			}

			this->transformMatrix[1] = -this->transformMatrix[1];
			//this->transformMatrix[2] = -this->transformMatrix[2];

			this->frustum = new HeFrustum(this->transformMatrix, this->colorImageWidth, this->colorImageHeight, this->original_fx, this->original_fy);
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
		glm::vec3 planePoint = glm::vec3(0, 0, this->original_fx);
		glm::vec3 planeNormal = glm::vec3(0, 0, 1);
		glm::vec3 rayOrigin = glm::vec3(0, 0, 0);
		glm::vec3 rayDirection = glm::normalize(glm::vec3(ip4.x, ip4.y, ip4.z));
		glm::vec3 intersection = RayPlaneIntersection(planePoint, planeNormal, rayOrigin, rayDirection);
		float u = ((intersection.x / (this->colorImageWidth * 0.5f)) * 0.5f + 0.5f);
		float v = 1 - ((intersection.y / (this->colorImageHeight * 0.5f)) * 0.5f + 0.5f);
		return glm::vec2(u, v);

		//auto intrinsic = glm::mat3(this->scaled_fx, 0, this->scaled_ox, 0, this->scaled_fy, this->scaled_oy, 0, 0, 1);
		//auto extrinsic = this->transformMatrix;
		//auto world = glm::vec4(worldPosition, 1);
		//auto s = intrinsic * glm::vec3(extrinsic * world);
		//s /= s.z;
		//return glm::vec2(s.x, s.y);
	}

	glm::vec3 HeCameraInfo::UVToWorld(const glm::vec2& uv) const
	{
		auto p = glm::vec3((uv.x - 0.5f) * this->colorImageWidth, ((1.0f - uv.y) - 0.5f) * this->colorImageHeight, this->original_fx);
		auto dir = glm::normalize(p);
		auto rp = dir * this->original_fx;
		return this->position + this->rotation * rp;
	}
}
