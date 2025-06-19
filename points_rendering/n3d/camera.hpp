#ifndef _N3D_CAMERA_HPP_
#define _N3D_CAMERA_HPP_

#include <cstring>

#include "matrix.hpp"
#include "vector.hpp"

namespace n3d {
	enum CamMoveDirection{
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD,
		UP,
		DOWN
	};

	class Camera
	{
	public:

		Camera(){
			moveSpeed = 0.5f;
			turnSpeed = 1.25f;
			isViewMatDirty = true;
			isProjMatDirty = true;
		}

		inline bool IsPerspective() {
			return isPerspective;
		}

		inline void SetPerspectiveCameraParams(
			float fovy, 
			float aspectRatio, 
			float near, float far) 
		{
			this->fovy = fovy;
			this->aspectRatio = aspectRatio;
			this->near = near;
			this->far = far;
			isPerspective = true;
			isProjMatDirty = true;
		}

		inline void SetOrthoCameraParams(
			float left, float right, 
			float bottom, float top, 
			float near, float far)
		{
			orthoLeft = left;
			orthoRight = right;
			orthoBottom = bottom;
			orthoTop = top;
			orthoNear = near;
			orthoFar = far;
			isPerspective = false;
			isProjMatDirty = true;
		}

		void TurnLeft() {
			yaw -= turnSpeed;
			UpdateAtBasedOnYawAndPitch();
			isViewMatDirty = true;
		}

		void TurnRight() {
			yaw += turnSpeed;
			UpdateAtBasedOnYawAndPitch();
			isViewMatDirty = true;
		}


		void LookUp() {
			pitch -= turnSpeed;
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
			UpdateAtBasedOnYawAndPitch();
			isViewMatDirty = true;
		}

		void LookDown() {
			pitch += turnSpeed;
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
			UpdateAtBasedOnYawAndPitch();
			isViewMatDirty = true;
		}


		void MoveCamera(CamMoveDirection direction) {
			float dx = 0.0f;
			float dy = 0.0f;
			float dz = 0.0f;
			switch (direction) {
			case LEFT:
				dx = -moveSpeed;
				break;
			case RIGHT:
				dx = moveSpeed;
				break;
			case FORWARD:
				dz = -moveSpeed;
				break;
			case BACKWARD:
				dz = moveSpeed;
				break;
			case UP:
				dy = moveSpeed;
				break;
			case DOWN:
				dy = -moveSpeed;
				break;
			}
			vec3f u, v, n, tu, tn, tv;
			mat4x4_get_axis_u(&u, viewMat);
			mat4x4_get_axis_v(&v, viewMat);
			mat4x4_get_axis_n(&n, viewMat);

			//eye = eye + dx * u 
			//at = at + dx * u 
			vec3f_norm(&tu, &u);
			vec3f_scale(&tu, &tu, dx);
			vec3f_add(&eye, &eye, &tu);
			vec3f_add(&at, &at, &tu);

			//eye = eye + dz * n
			//at = at + dz * n
			vec3f_norm(&tn, &n);
			vec3f_scale(&tn, &tn, dz);
			vec3f_add(&eye, &eye, &tn);
			vec3f_add(&at, &at, &tn);

			//eye = eye + dy * v
			//at = at + dy * v
			vec3f_norm(&tv, &v);
			vec3f_scale(&tv, &tv, dy);
			vec3f_add(&eye, &eye, &tv);
			vec3f_add(&at, &at, &tv);

			isViewMatDirty = true;
		}

		inline void Sync(mat4x4 viewMatDst, mat4x4 projMatDst){
			if (isViewMatDirty){
				mat4x4_set_look_at(viewMat, &eye, &at, &up);
				isViewMatDirty = false;
				memcpy(viewMatDst, viewMat, 16 * sizeof(float));
			}
			if (isProjMatDirty){
				if (isPerspective){
					mat4x4_set_perspective(
						projMat, 
						fovy, 
						aspectRatio, 
						near, 
						far
					);
				}
				else {
					mat4x4_set_ortho(projMat,
						orthoLeft,
						orthoRight,
						orthoBottom,
						orthoTop,
						orthoNear,
						orthoFar
					);
				}
				memcpy(projMatDst, projMat, 16 * sizeof(float));
				isProjMatDirty = false;
			}
		}

		inline void SetViewParams(vec3f eye, vec3f at, vec3f up)
		{
			this->eye = eye;
			this->at = at;
			this->up = up;
			vec3f cameraForward;
			vec3f_sub(&cameraForward, &at, &eye);
			cameraFwdVecMag = vec3f_mag(&cameraForward);
			vec3f_norm(&cameraForward, &cameraForward);
			pitch = -TODEGREES(asin(cameraForward.y));
			yaw = -TODEGREES(atan2(cameraForward.z, cameraForward.x));
		}

	private:
		void UpdateAtBasedOnYawAndPitch() {
			vec3f cameraForward;
			cameraForward.x = cos(TORADIANS(yaw)) * cos(TORADIANS(pitch));
			cameraForward.y = sin(TORADIANS(pitch));
			cameraForward.z = sin(TORADIANS(yaw)) * cos(TORADIANS(pitch));
			vec3f_norm(&cameraForward, &cameraForward);
			vec3f_scale_and_add(&at, cameraFwdVecMag, &cameraForward, &eye);
		}

	private:
		mat4x4 viewMat;
		mat4x4 projMat;
		bool isPerspective;
		bool isViewMatDirty;
		bool isProjMatDirty;
		float cameraFwdVecMag;
		vec3f eye;
		vec3f at;
		vec3f up;
		float moveSpeed;
		float turnSpeed;

		float fovy;
		float aspectRatio;
		float near;
		float far;
		float yaw;
		float pitch;

		float orthoNear;
		float orthoFar;
		float orthoLeft;
		float orthoRight;
		float orthoBottom;
		float orthoTop;
	};

}

#endif