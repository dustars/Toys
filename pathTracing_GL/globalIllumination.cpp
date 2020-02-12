/*
 This is a global illumination renderer by using unbiased Monte Carlo path tracing.
 The orginal code has only 99 lines of C++(though with poor readability) by Kevin Beason, https://www.kevinbeason.com/smallpt/
 Also, https://drive.google.com/file/d/0B8g97JkuSSBwUENiWTJXeGtTOHFmSm51UC01YWtCZw/view gives a good explanation

 This is a great example to understand how global illumination works, just need to go through several relevant formula of optics.
 And that, my frient, is gonna take some time for a beginner.
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double M_PI = 3.1415926535;
double M_1_PI = 1 / M_PI;

double erand48(unsigned short xsubi[3]) {
	return (double)rand() / (double)RAND_MAX;
}

struct Vector3 { 
	double x, y, z;
	Vector3(double x_ = 0, double y_ = 0, double z_ = 0) {
		x = x_;
		y = y_;
		z = z_;
	}

	Vector3 operator+(const Vector3& b) const {return Vector3(x + b.x, y + b.y, z + b.z);}

	Vector3 operator-(const Vector3& b) const { return Vector3(x - b.x, y - b.y, z - b.z); }

	Vector3 operator*(double b) const { return Vector3(x * b, y * b, z * b); }

	Vector3 mult(const Vector3& b) const { return Vector3(x * b.x, y * b.y, z * b.z); }

	Vector3& norm() { return *this = *this * (1 / sqrt(x * x + y * y + z * z)); }

	double dot(const Vector3& b) const {
		return x * b.x + y * b.y + z * b.z;
	}

	Vector3 operator%(Vector3& b) {
		return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};

struct Ray {
	Vector3 o, d;
	Ray(Vector3 o_, Vector3 d_) : o(o_), d(d_) {}
};

enum Refl_t {
	DIFF,
	SPEC,
	REFR
};  // material types, used in radiance() 

struct Sphere {
	double rad;       // radius 
	Vector3 p, e, c;      // position, emission, color 
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive) 

	Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_, Refl_t refl_) :
		rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}

	double intersect(const Ray& r) const { // returns distance, 0 if nohit 
		Vector3 op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 
		double t, eps = 1e-4;
		double b = op.dot(r.d);
		double det = b * b - op.dot(op) + rad * rad;
		if (det < 0) return 0;
		else det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	}
};

Sphere spheres[] = {//Scene: radius, position, emission, color, material 
  Sphere(1e5, Vector3(1e5 + 1,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
  Sphere(1e5, Vector3(-1e5 + 99,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght 
  Sphere(1e5, Vector3(50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back 
  Sphere(1e5, Vector3(50,40.8,-1e5 + 170), Vector3(),Vector3(),           DIFF),//Frnt 
  Sphere(1e5, Vector3(50, 1e5, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF),//Botm 
  Sphere(1e5, Vector3(50,-1e5 + 81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF),//Top 
  Sphere(16.5,Vector3(27,16.5,47),       Vector3(),Vector3(1,1,1) * .999, SPEC),//Mirr 
  Sphere(16.5,Vector3(73,16.5,78),       Vector3(),Vector3(1,1,1) * .999, REFR),//Glas 
  Sphere(600, Vector3(50,681.6 - .27,81.6),Vector3(12,12,12),  Vector3(), DIFF) //Lite 
};

inline double clamp(double x) {
	return x < 0 ? 0 : x>1 ? 1 : x;
}

inline int toInt(double x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

inline bool intersect(const Ray& r, double& t, int& id) {
	double n = sizeof(spheres) / sizeof(Sphere);
	double d;
	double inf = t = 1e20;

	for (int i = int(n); i--;) {
		if ((d = spheres[i].intersect(r)) && d < t) {
			t = d;
			id = i;
		}
	}
	return t < inf;
}

Vector3 radiance(const Ray& r, int depth, unsigned short* Xi) {
	double t;                               // distance to intersection 
	int id = 0;                             // id of intersected object 
	if (!intersect(r, t, id)) { // if miss, return black 
		return Vector3();
	}
	const Sphere& obj = spheres[id];        // the hit object 

	Vector3 x = r.o + r.d * t;					// ray intersection point
	Vector3 n = (x - obj.p).norm();				// sphere normal 
	Vector3 nl = n.dot(r.d) < 0 ? n : n * -1;	// properly oriented surface normal
	Vector3 f = obj.c;							// object color (BRDF modulator)

	double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl 

	if (++depth > 5 || !p) {
		if (erand48(Xi) < p) {
			f = f * (1 / p);
		}
		else {
			return obj.e; //R.R. 
		}
	}

	if (depth > 50) {
		return obj.e;
	}

	if (obj.refl == DIFF) {                  // Ideal DIFFUSE reflection 
		// Create Unit hemisphere
		Vector3 w = nl;
		Vector3 u = ((fabs(w.x) > .1 ? Vector3(0, 1) : Vector3(1)) % w).norm();
		Vector3 v = w % u;
		// Cast a random ray from the hemisphere
		double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
		Vector3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
		return obj.e + f.mult(radiance(Ray(x, d), depth, Xi));
	}
	else if (obj.refl == SPEC) {           // Ideal SPECULAR reflection 
		return obj.e + f.mult(radiance(Ray(x, r.d - n * 2 * n.dot(r.d)), depth, Xi));
	}

	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));     // Ideal dielectric relection 
	bool into = n.dot(nl) > 0;                // Ray from outside going in? 
	double nc = 1;
	double nt = 1.5;
	double nnt = into ? nc / nt : nt / nc;
	double ddn = r.d.dot(nl);
	double cos2t;

	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection 
		return obj.e + f.mult(radiance(reflRay, depth, Xi));

	Vector3 tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm(); // refracted ray

	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);

	return obj.e + f.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette 
		radiance(reflRay, depth, Xi) * RP : radiance(Ray(x, tdir), depth, Xi) * TP) :
		radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);
}

int main() {
	int w = 1024, h = 768;
	//int samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples 
	int samps = 500;

	Ray cam(Vector3(50, 52, 295.6), Vector3(0, -0.042612, -1).norm()); // cam pos, dir 
	Vector3 cx = Vector3(w * .5135 / h);
	Vector3 cy = (cx % cam.d).norm() * .5135;
	Vector3 r; // color of sample
	Vector3 *c = new Vector3[w * h]; // the image

#pragma omp parallel for schedule(dynamic, 1) private(r)       // OpenMP
	for (int y = 0; y < h; y++) {                       // Loop over image rows 
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * y / (h - 1));
		unsigned short Xi[3] = { 0, 0, y * y * y }; // any purposes?
		for (unsigned short x = 0; x < w; x++) {  // Loop cols 

			int i = (h - y - 1) * w + x; // The index of current pixel being processed
			for (int sy = 0; sy < 2; sy++) {						   // 2x2 subpixel rows 
				for (int sx = 0; sx < 2; sx++, r = Vector3()) {        // 2x2 subpixel cols 
					for (int s = 0; s < samps; s++) {
						// Tent filter
						double r1 = 2 * erand48(Xi);
						double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * erand48(Xi);
						double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						// ray direction
						Vector3 d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) +
									cy * (((sy + .5 + dy) / 2 + y) / h - .5) +
									cam.d;

						r = r + radiance(Ray(cam.o + d * 140, d.norm()), 0, Xi) * (1. / samps); // Average at the end of loop to save calculation

					} // Camera rays are pushed ^^^^^ forward to start in interior 
					c[i] = c[i] + Vector3(clamp(r.x), clamp(r.y), clamp(r.z)) * .25; // ignore this please
				}
			}
		}
	}
	FILE** f = new FILE*;
	fopen_s(f, "image.ppm", "w");         // Write image to PPM file. 
	fprintf(*f, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w * h; i++) {
		fprintf(*f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
	}
	delete f;
}