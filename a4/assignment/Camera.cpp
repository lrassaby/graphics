#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Reset() {

}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
}

Matrix Camera::GetProjectionMatrix() {
	Matrix m;
	return m;
}


void Camera::SetViewAngle (double viewAngle) {
}

void Camera::SetNearPlane (double nearPlane) {
}

void Camera::SetFarPlane (double farPlane) {
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
}

Matrix Camera::GetModelViewMatrix() {
	Matrix m;
	return m;
}

void Camera::RotateV(double angle) {
}

void Camera::RotateU(double angle) {
}

void Camera::RotateW(double angle) {
}

void Camera::Translate(const Vector &v) {
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
}


Point Camera::GetEyePoint() {
	Point p;
	return p;
}

Vector Camera::GetLookVector() {
	Vector v;
	return v;
}

Vector Camera::GetUpVector() {
	Vector v;
	return v;
}

double Camera::GetViewAngle() {
	return 0;
}

double Camera::GetNearPlane() {
	return 0;
}

double Camera::GetFarPlane() {
	return 0;
}

int Camera::GetScreenWidth() {
	return 0;
}

int Camera::GetScreenHeight() {
	return 0;
}

double Camera::GetFilmPlanDepth() {
	return 0;
}

double Camera::GetScreenWidthRatio() {
	return 0;
}
