#include "Camera.h"

Camera::Camera(glm::vec3 camera_pos,glm::vec3 look_pos,glm::vec3 camera_up)
{
	eye = glm::vec4(camera_pos,1.0f);
	at = glm::vec4(look_pos,1.0f);
	up = glm::vec4(glm::normalize(camera_up),1.0f);

	move_param.forward = glm::normalize(at-eye);
	move_param.up = up;
	move_param.left = glm::normalize(glm::cross(move_param.up,move_param.forward));
};

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
	return this->lookAt(eye, at, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	if (this-> cam_mode == 1) {
		return this->ortho(-scale, scale, -scale, scale, this->zNear, this->zFar);
	}
	else {
		return this->perspective(fov, aspect, this->zNear, this->zFar);
	}
}

glm::mat4 Camera::lookAt(const glm::vec4& camera_pos, const glm::vec4& look_pos, const glm::vec4& up)
{
	// ================@TODO: Task1 补全相机观察矩阵的计算====================
	// use glm.
	// compute directly.
	// 获得相机方向。	
	// 获得右(x)轴方向。
	// std::cout << at.x <<" "<<at.y<<" "<<at.z<<"  ";
	glm::vec3 up_3 = up;

	glm::vec3 z = glm::normalize(camera_pos - look_pos);
	glm::vec3 x = glm::normalize(glm::cross(up_3,z));
	glm::vec3 y = glm::normalize(glm::cross(z,x));
	
	
	glm::mat4 rotate = glm::mat4(1.0f);
	rotate[0][0] = x.x;
	rotate[1][0] = x.y;
	rotate[2][0] = x.z;
	rotate[0][1] = y.x;
	rotate[1][1] = y.y;
	rotate[2][1] = y.z;
	rotate[0][2] = z.x;
	rotate[1][2] = z.y;
	rotate[2][2] = z.z;

	glm::mat4 translate = glm::mat4(1.f);
	translate[3][0] = -camera_pos.x;
	translate[3][1] = -camera_pos.y;
	translate[3][2] = -camera_pos.z;

	glm::mat4 view = rotate * translate;
	return view;							// 计算最后需要沿-eye方向平移
	//return glm::lookAt(glm::vec3(camera_pos),glm::vec3(look_pos),glm::vec3(up));
}

glm::mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// =============@TODO: Task1 补全正交投影矩阵的计算=================
	glm::mat4 ortho_scale = glm::mat4(1.f);
	ortho_scale[0][0] = 2.f / std::abs(right - left);
	ortho_scale[1][1] = 2.f / std::abs(top - bottom);
	ortho_scale[2][2] = 2.f / std::abs(zNear - zFar);

	glm::mat4 ortho_trans = glm::mat4(1.f);
	ortho_trans[3][0] = - (left + right) * 0.5f;
	ortho_trans[3][1] = - (top + bottom) * 0.5f;
	ortho_trans[3][2] = - (zFar + zNear) * 0.5f;
	ortho_trans[2][2] = -1.f;

	return ortho_scale* ortho_trans;
	//return glm::ortho(left,right,bottom,top);
}

glm::mat4 Camera::perspective(const GLfloat fov, const GLfloat aspect_ratio,
	const GLfloat zNear, const GLfloat zFar)
{
	//================@TODO: Task1 补全透视投影矩阵的计算===============

	GLfloat height = 2 * tan(fov * 0.5) * zNear;
	GLfloat width = height * aspect_ratio;

	glm::mat4 persp = glm::mat4(0.0f);
	persp[0][0] = zNear;
	persp[1][1] = zNear;
	persp[2][2] = -std::abs(zNear + zFar);
	persp[3][2] = zNear * zFar;
	persp[2][3] = -1.f;

	glm::mat4 ortho = glm::mat4(1.f);
	ortho[0][0] = 2.f / width;
	ortho[1][1] = 2.f / height;
	ortho[2][2] = 2.f / std::abs(zNear - zFar);
	ortho[3][2] = - (zNear + zFar) / std::abs(zNear - zFar);
	
	/*
	OpenGL NDC 空间为[-1,1]
	glm::mat4 NDC_scale = glm::mat4(1.f);
	NDC_scale[0][0] = 0.5f;
	NDC_scale[1][1] = 0.5f;
	NDC_scale[2][2] = 0.5f;

	glm::mat4 NDC_trans = glm::mat4(1.f);
	NDC_trans[0][3] = 0.5f;
	NDC_trans[1][3] = 0.5f;
	NDC_trans[2][3] = 0.5f;
	*/                           
	//return ortho * persp;
	return glm::perspective(fov,aspect,zNear,zFar);
}

glm::mat4 Camera::frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	c = glm::transpose(c);
	return c;
}


void Camera::updateCamera()
{

}


void Camera::keyboard(int key, int action, int mode)
{
	glm::vec3 move_flag = glm::vec3(0.f);
	if (key == GLFW_KEY_U && action == GLFW_PRESS && mode == 0x0000)
	{
		float current_rotateAngle = rotateAngle;
		rotateAngle += 5.0;
		CameraRotateEvent(move_param.up,rotateAngle - current_rotateAngle);
	}
	else if (key == GLFW_KEY_U && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		float current_rotateAngle = rotateAngle;
		rotateAngle -= 5.0;
		CameraRotateEvent(move_param.up, rotateAngle - current_rotateAngle);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS && mode == 0x0000)
	{
		float current_upAngle = upAngle;
		upAngle += 5.0;
		if (upAngle > 88)
			upAngle = 88;
		CameraRotateEvent(-move_param.left,upAngle - current_upAngle);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		float current_upAngle = upAngle;
		upAngle -= 5.0;
		if (upAngle < -88)
			upAngle = -88;
		CameraRotateEvent(-move_param.left,upAngle - current_upAngle);
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS && mode == 0x0000)
	{
		radius += 0.1;
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		radius -= 0.1;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && mode == 0x0000)
	{
		radius = 4.0;
		rotateAngle = 0.0;
		upAngle = 0.0;
		fov = 45.0;
		aspect = 1.0;
		scale = 1.5;

		eye = glm::vec4(0,0,-1,1.0f);
		at = glm::vec4(0,0,0,1.0f);
		up = glm::vec4(0,1,0,0.0f);

		move_param.forward = glm::normalize(at-eye);
		move_param.up = up;
		move_param.left = glm::normalize(glm::cross(move_param.up,move_param.forward));
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS && mode == 0x0000)
	{
		cam_mode = 0;
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		cam_mode = 1;
	}
	/*
		add control option: A move left; D move right; W move forwards; D move backwards
							Q move above    ; E move up
	*/
	if (key == GLFW_KEY_W && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.z = 1.f;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.z = 1.f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.z = -1.f;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.z = -1.f;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.x = 1.f;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.x = 1.f;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.x = -1.f;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.x = -1.f;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.y = 1.f;
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.y = 1.f;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS && mode == 0x0000)
	{
		move_param.move_speed = 0.2f;
		move_flag.y = -1.f;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		move_param.move_speed = 1.f;
		move_flag.y = -1.f;
	}
	if(move_flag.x != 0 || move_flag.y != 0 || move_flag.z != 0)
		CameraMoveEvent(move_flag);
}

void Camera::CameraMoveEvent(glm::vec3 move_flag)
{
	glm::vec3 horizontal_move_vec = move_flag.x * move_param.left;
	glm::vec3 vertical_move_vec = move_flag.y * move_param.up;
	glm::vec3 forward_move_vec = move_flag.z * move_param.forward;

	glm::vec3 move_vec = move_param.move_speed * glm::normalize(horizontal_move_vec + vertical_move_vec + forward_move_vec);
	eye += glm::vec4(move_vec,0.f);
	at += glm::vec4(move_vec,0.f);
	//std::cout << eye.x <<" "<<eye.y<<" "<<eye.z<<"  ";

	return;
}

void Camera::CameraRotateEvent(glm::vec3 axis, float rotate_angle)
{
	glm::mat4 vertical_rotate = glm::mat4(1.f);
	vertical_rotate = glm::rotate(vertical_rotate,glm::radians(rotate_angle),axis);
	glm::vec4 look_direction = at - eye;
	look_direction = vertical_rotate * look_direction;
	at = eye + look_direction;
	move_param.forward = glm::normalize(look_direction);
	move_param.left = glm::normalize(glm::cross(move_param.up,move_param.forward));	
	//std::cout << rotate_angle<<" ";
	//std::cout << at.x <<" "<<at.y<<" "<<at.z<<"  ";
}