#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
    public:
        Camera();
        ~Camera();
        void Orient(Point& eye, Point& focus, Vector& up);
        void Orient(Point& eye, Vector& look, Vector& up);
        void SetViewAngle (double viewAngle);
        void SetNearPlane (double nearPlane);
        void SetFarPlane (double farPlane);
        void SetScreenSize (int screenWidth, int screenHeight);

        Matrix GetProjectionMatrix();
        Matrix GetModelViewMatrix();

        void RotateV(double angle);
        void RotateU(double angle);
        void RotateW(double angle);

        Point CalcRotate(Point p, Vector axis, double degree);
        Vector CalcRotate(Vector v, Vector axis, double degree);
        void Translate(const Vector &v);

        Point GetEyePoint();
        Vector GetLookVector();
        Vector GetUpVector();
        double GetViewAngle();
        double GetNearPlane();
        double GetFarPlane();
        int GetScreenWidth();
        int GetScreenHeight();

        double GetFilmPlaneDepth();
        double GetScreenWidthRatio();
        void Reset();
    private:
        int screen_width, screen_height;
        double near_plane, far_plane;
        double view_angle;
        Point eye_point;
        Vector look_vector, up_vector;
        Vector u_vector, v_vector, w_vector;
};
#endif

