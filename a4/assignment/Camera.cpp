#include "Camera.h"
#include <iostream>
using namespace std;
enum VectorComponent {X, Y, Z};

double RadiantoDegree(double radian)
{
        return ((radian / M_PI) * 180.0f);
}

double DegreetoRadian(double degree)
{
        return ((degree / 180.0f) * M_PI);
}


Camera::Camera() {
    Reset();
}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    Vector look = focus - eye;
    Orient(eye, look, up); 
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    eye_point = eye;
    look_vector = normalize(look);
    up_vector = normalize(up);

    u_vector = cross(look, up);
    u_vector = normalize(u_vector);
    v_vector = cross(u_vector, look);
    v_vector = normalize(v_vector);
    w_vector = -look_vector;
}

Matrix Camera::GetProjectionMatrix() {
    double c = -near_plane / far_plane;
	Matrix n(1, 0,          0,         0,
             0, 1,          0,         0,
             0, 0, -1/(c + 1), c/(c + 1),
             0, 0,         -1,        0);

    /* use trig to calculate angles */

    double theta_h = DegreetoRadian(view_angle); 

    double far     = screen_height / (2 * tan (theta_h / 2));
    double theta_w = 2 * atan( screen_width / (2 * far) );

    Matrix s(1/(tan((theta_w) / 2) * far_plane), 0, 0, 0,
             0,  1/(tan((theta_h)/ 2) * far_plane), 0, 0,
             0,  0,                     1 / far_plane, 0,
             0,  0,                                 0, 1);

    return n * s;
}


void Camera::SetViewAngle (double viewAngle) {
    view_angle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    near_plane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    far_plane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    screen_width = screenWidth;
    screen_height = screenHeight;
}

Matrix Camera::GetModelViewMatrix() {
    Matrix r(u_vector[X], u_vector[Y], u_vector[Z], 0,
             v_vector[X], v_vector[Y], v_vector[Z], 0,
             w_vector[X], w_vector[Y], w_vector[Z], 0,
                       0,           0,           0, 1);

    Matrix t(1, 0, 0, -eye_point[X],
             0, 1, 0, -eye_point[Y], 
             0, 0, 1, -eye_point[Z],
             0, 0, 0,            1);

    return r * t; 
}

#define SIN sin(angle)
#define COS cos(angle)
Matrix rotXMatrix(double angle) {
    return Matrix(1,   0,    0, 0,
                  0, COS, -SIN, 0,
                  0, SIN,  COS, 0,
                  0,   0,    0, 1);
}
Matrix rotYMatrix(double angle) {
    return Matrix( COS, 0, SIN, 0,
                     0, 1,   0, 0,
                  -SIN, 0, COS, 0,
                     0, 0,   0, 1);
}
Matrix rotZMatrix(double angle) {
    return Matrix(COS, -SIN, 0, 0,
                  SIN,  COS, 0, 0,
                    0,    0, 1, 0,
                    0,    0, 0, 1);
}


void Camera::RotateV(double angle) {
    // modify u and w
    angle = DegreetoRadian(-angle);
    Vector temp;
    temp =  COS * u_vector + SIN * w_vector;   
    w_vector = -SIN * u_vector + COS * w_vector;   
    u_vector = temp; 
}

void Camera::RotateU(double angle) {
    // modify v and w
    angle = DegreetoRadian(-angle);
    Vector temp;
    temp = COS * v_vector - SIN * w_vector;    
    w_vector = SIN * v_vector + COS * w_vector;    
    v_vector = temp;
}

void Camera::RotateW(double angle) {
    // modify u and v
    angle = DegreetoRadian(-angle);
    Vector temp;
    temp = COS * u_vector - SIN * v_vector;    
    v_vector = SIN * u_vector + COS * v_vector;    
    u_vector = temp;
}
#undef SIN
#undef COS


void Camera::Translate(const Vector &v) {
    eye_point = eye_point + v;
}

/* point rotations around origin */
Point Camera::CalcRotate(Point p, Vector axis, double gamma) {
    double theta = atan2(axis[Z], axis[X]);
    double phi = -atan2(axis[Y], sqrt(axis[X] * axis[X] + axis[Z] * axis[Z]));
    return rotXMatrix(gamma) * rotYMatrix(theta) * rotZMatrix(phi) * p;
}

/* vector rotations around origin */
Vector Camera::CalcRotate(Vector v, Vector axis, double degree) {
    Point p(v[X], v[Y], v[Z]);
    return CalcRotate(p, axis, degree) - Point(0, 0, 0);
}

Point Camera::GetEyePoint() {
	return eye_point;
}

Vector Camera::GetLookVector() {
	return look_vector;
}

Vector Camera::GetUpVector() {
	return up_vector;
}

double Camera::GetViewAngle() {
	return view_angle;
}

double Camera::GetNearPlane() {
	return near_plane;
}

double Camera::GetFarPlane() {
	return far_plane;
}

int Camera::GetScreenWidth() {
	return screen_width;
}

int Camera::GetScreenHeight() {
	return screen_height;
}

double Camera::GetFilmPlaneDepth() {
	return -1/tan(DegreetoRadian(view_angle) / 2.0); 
}

double Camera::GetScreenWidthRatio() {
	return (double) screen_width / (double) screen_height;
}

void Camera::Reset() 
{   
    screen_width = 500;
    screen_height = 500;
    near_plane = 0.001;
    far_plane = 30;
    view_angle = 45;
    u_vector = Vector(1, 0, 0);
    v_vector = Vector(0, 1, 0);
    w_vector = Vector(0, 0, 1);
}
