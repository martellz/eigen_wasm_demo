#include <iostream>
#include <math.h>
#include <Eigen/Core>

void homography_from_4pt(const float *x, const float *y, const float *z, const float *w, float cgret[8])
{
	float t1 = x[0];
	float t2 = z[0];
	float t4 = y[1];
	float t5 = t1 * t2 * t4;
	float t6 = w[1];
	float t7 = t1 * t6;
	float t8 = t2 * t7;
	float t9 = z[1];
	float t10 = t1 * t9;
	float t11 = y[0];
	float t14 = x[1];
	float t15 = w[0];
	float t16 = t14 * t15;
	float t18 = t16 * t11;
	float t20 = t15 * t11 * t9;
	float t21 = t15 * t4;
	float t24 = t15 * t9;
	float t25 = t2 * t4;
	float t26 = t6 * t2;
	float t27 = t6 * t11;
	float t28 = t9 * t11;
	float t30 = 0.1e1 / (-t24 + t21 - t25 + t26 - t27 + t28);
	float t32 = t1 * t15;
	float t35 = t14 * t11;
	float t41 = t4 * t1;
	float t42 = t6 * t41;
	float t43 = t14 * t2;
	float t46 = t16 * t9;
	float t48 = t14 * t9 * t11;
	float t51 = t4 * t6 * t2;
	float t55 = t6 * t14;
	cgret[0] = -(-t5 + t8 + t10 * t11 - t11 * t7 - t16 * t2 + t18 - t20 + t21 * t2) * t30;
	cgret[1] = (t5 - t8 - t32 * t4 + t32 * t9 + t18 - t2 * t35 + t27 * t2 - t20) * t30;
	cgret[2] = t1;
	cgret[3] = (-t9 * t7 + t42 + t43 * t4 - t16 * t4 + t46 - t48 + t27 * t9 - t51) * t30;
	cgret[4] = (-t42 + t41 * t9 - t55 * t2 + t46 - t48 + t55 * t11 + t51 - t21 * t9) * t30;
	cgret[5] = t14;
	cgret[6] = (-t10 + t41 + t43 - t35 + t24 - t21 - t26 + t27) * t30;
	cgret[7] = (-t7 + t10 + t16 - t43 + t27 - t28 - t21 + t25) * t30;
	//cgret[8] = 1;
}

void homography_transform(const float H[3][3], const float a[2], float r[2])
{
	float z = H[2][0]*a[0] + H[2][1]*a[1] + H[2][2];
	r[0] = (H[0][0]*a[0] + H[0][1]*a[1] + H[0][2]) / z;
	r[1] = (H[1][0]*a[0] + H[1][1]*a[1] + H[1][2]) / z;
}

void homography_transform(Eigen::Matrix3f H, float u, float v, float & up, float & vp)
{
	float inv_k = 1.0 / (H(2, 0)*u + H(2, 1)*v + H(2, 2));
	up = float(floor((H(0, 0)*u + H(0, 1)*v + H(0, 2)) * inv_k + 0.5));
	vp = float(floor((H(1, 0)*u + H(1, 1)*v + H(1, 2)) * inv_k + 0.5));
}

void homography_transform(Eigen::Matrix3f H, float u, float v, int & up, int & vp)
{
	float inv_k = 1.0 / (H(2, 0)*u + H(2, 1)*v + H(2, 2));
	up = int(floor((H(0, 0)*u + H(0, 1)*v + H(0, 2)) * inv_k + 0.5));
	vp = int(floor((H(1, 0)*u + H(1, 1)*v + H(1, 2)) * inv_k + 0.5));
}

void homography_transform(Eigen::Matrix3f homo, Eigen::Vector2f a, Eigen::Vector2f r) {
    Eigen::Vector3f homogeneous_a(a[0], a[1], 1.0f);
    Eigen::Vector3f result = homo * homogeneous_a;
    result = result / result[2];
    r[0] = result[0];
    r[1] = result[1];
}

void homography_from_4corresp(Eigen::Matrix3f& R, Eigen::VectorXf u, Eigen::VectorXf up)
{
    const float a[] = {u[0], u[1]};
    const float b[] = {u[2], u[3]};
    const float c[] = {u[4], u[5]};
    const float d[] = {u[6], u[7]};
    const float x[] = {up[0], up[1]};
    const float y[] = {up[2], up[3]};
    const float z[] = {up[4], up[5]};
    const float w[] = {up[6], up[7]};

	float Hr[3][3], Hl[3][3];

	homography_from_4pt(a,b,c,d,&Hr[0][0]);
	homography_from_4pt(x,y,z,w,&Hl[0][0]);

	// the following code computes R = Hl * inverse Hr
	float t2 = Hr[1][1]-Hr[2][1]*Hr[1][2];
	float t4 = Hr[0][0]*Hr[1][1];
	float t5 = Hr[0][0]*Hr[1][2];
	float t7 = Hr[1][0]*Hr[0][1];
	float t8 = Hr[0][2]*Hr[1][0];
	float t10 = Hr[0][1]*Hr[2][0];
	float t12 = Hr[0][2]*Hr[2][0];
	float t15 = 1/(t4-t5*Hr[2][1]-t7+t8*Hr[2][1]+t10*Hr[1][2]-t12*Hr[1][1]);
	float t18 = -Hr[1][0]+Hr[1][2]*Hr[2][0];
	float t23 = -Hr[1][0]*Hr[2][1]+Hr[1][1]*Hr[2][0];
	float t28 = -Hr[0][1]+Hr[0][2]*Hr[2][1];
	float t31 = Hr[0][0]-t12;
	float t35 = Hr[0][0]*Hr[2][1]-t10;
	float t41 = -Hr[0][1]*Hr[1][2]+Hr[0][2]*Hr[1][1];
	float t44 = t5-t8;
	float t47 = t4-t7;
	float t48 = t2*t15;
	float t49 = t28*t15;
	float t50 = t41*t15;
	R(0, 0) = Hl[0][0]*t48+Hl[0][1]*(t18*t15)-Hl[0][2]*(t23*t15);
	R(0, 1) = Hl[0][0]*t49+Hl[0][1]*(t31*t15)-Hl[0][2]*(t35*t15);
	R(0, 2) = -Hl[0][0]*t50-Hl[0][1]*(t44*t15)+Hl[0][2]*(t47*t15);
	R(1, 0) = Hl[1][0]*t48+Hl[1][1]*(t18*t15)-Hl[1][2]*(t23*t15);
	R(1, 1) = Hl[1][0]*t49+Hl[1][1]*(t31*t15)-Hl[1][2]*(t35*t15);
	R(1, 2) = -Hl[1][0]*t50-Hl[1][1]*(t44*t15)+Hl[1][2]*(t47*t15);
	R(2, 0) = Hl[2][0]*t48+Hl[2][1]*(t18*t15)-t23*t15;
	R(2, 1) = Hl[2][0]*t49+Hl[2][1]*(t31*t15)-t35*t15;
	R(2, 2) = -Hl[2][0]*t50-Hl[2][1]*(t44*t15)+t47*t15;
}

bool normalize(Eigen::VectorXf& i1) {
	float mean = i1.mean();
	float square = i1.transpose() * i1;
    float temp = 1.0 / sqrt(square / i1.size() - pow(mean, 2));
    i1 = (i1 - Eigen::VectorXf::Ones(i1.size()) * mean) * temp;
    return true;
}
