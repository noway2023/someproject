#include "tgaimage.h"
#include <iostream>
#include <chrono>
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);
const TGAColor yellow   = TGAColor(255, 255,   0,   255);

int count =0;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
	bool sheep = false;
	if(std::abs(x0-x1)<std::abs(y0-y1)){
		std::swap(x0, y0);
		std::swap(x1, y1);
		sheep = true;
	}

	if(x0>x1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int y =y0;
	int dx = x1-x0;
	int dy = y1-y0;
	int offset = std::abs(dy)*2;
	int error = 0;
	const int incre = (y1>y0)?1:-1;
	if(sheep){
		for(int x=x0;x<x1;x++){
			image.set(y, x, color);
			error += offset;
			if(error > dx){
				y +=incre;
				error -= dx*2;
			}
		}
	}
	else{
		
		for(int x=x0;x<x1;x++){
			image.set(x, y, color); 
			error += offset;
			if(error > dx){
				y +=incre;
				error -= dx*2;
			}
		}
		
	}
}

void divid(TGAImage& image){
	for(int i=0;i<image.get_width();i++){
		for(int j=0;j<image.get_height();j++){
			if((i/2+j/2)%2){
				image.set(i, j, black);
			}
			else{
				image.set(i, j, white);
			}
		}
	}

} 

bool line(Vec2i u, Vec2i v, TGAImage& image, TGAColor color){
	line(u.x, u.y, v.x, v.y, image, color);
	return true;
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color){
	if (t0.y==t1.y && t0.y==t2.y) return;
	if(t0.y>t1.y) std::swap(t0, t1);
	if(t0.y>t2.y) std::swap(t0, t2);
	if(t1.y>t2.y) std::swap(t1, t2);
	int totol_height = t2.y - t0.y;
	int temp_height = t1.y - t0.y;
	for(int y=t0.y;y<t1.y;y++){
		float a = (float)(y - t0.y)/totol_height;
		float b = (float)(y - t0.y)/temp_height;
		Vec2i xa = t0 + (t2 - t0) * a;
		Vec2i xb = t0 + (t1 - t0) * b;
		//image.set(xa.x, y, red);
		//image.set(xb.x, y, green);
		//line(xa.x, y, xb.x, y, image, color);
		if(xa.x>xb.x) std::swap(xa, xb);
		for(int x = xa.x;x<=xb.x;x++){
			image.set(x, y, color);
		}
	}
	temp_height = t2.y - t1.y;
	for(int y=t1.y;y<t2.y;y++){
	float a = (float)(y - t0.y)/totol_height;
	float b = (float)(y - t1.y)/temp_height;
	Vec2i xa = t0 + (t2 - t0) * a;
	Vec2i xb = t1 + (t2 - t1) * b;
	//image.set(xa.x, y, red);
	//image.set(xb.x, y, green);
	//line(xa.x, y, xb.x, y, image, color);
	if(xa.x>xb.x) std::swap(xa, xb);
	for(int x = xa.x;x<=xb.x;x++){
		image.set(x, y, color);
	}
	}


}

void line2(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

bool triangle(Vec2i t0, Vec2i t1,Vec2i t2, TGAImage& image){
	if(t0.y>t1.y) std::swap(t0, t1);
	if(t0.y>t2.y) std::swap(t0, t2);
	if(t1.y>t2.y) std::swap(t1, t2);
	line2(t0, t1, image, green);
	line2(t1, t2, image, green);
	line2(t2, t0, image, red);
	return true;
}

Vec3f bayercoor(std::vector<Vec2i> ps, Vec2i P){
	Vec3f u = Vec3f(ps[1].x-ps[0].x, ps[2].x-ps[0].x, ps[0].x - P.x)^ \
			Vec3f(ps[1].y-ps[0].y, ps[2].y-ps[0].y, ps[0].y - P.y);
	if(std::abs(u.z)<1) return Vec3f(-1., 1., 1.);
	else return  Vec3f(1.-(u.x+u.y)/u.z, u.x/u.z, u.y/u.z);
}

void trianglebb(std::vector<Vec2i> ps, TGAImage& image, TGAColor color){
	Vec2i bbmin(image.get_width()-1, image.get_height()-1); 
	Vec2i bbmax(0, 0);
	Vec2i cul(image.get_width()-1, image.get_height()-1);
	for(int i=0;i<3;i++){
		bbmin.x = std::max(0, std::min(ps[i].x, bbmin.x));
		bbmin.y = std::max(0, std::min(ps[i].y, bbmin.y));
		bbmax.x = std::min(cul.x, std::max(ps[i].x, bbmax.x));
		bbmax.y = std::min(cul.y, std::max(ps[i].y, bbmax.y));
	}
	Vec2i P;
	for(P.x=bbmin.x;P.x<bbmax.x;P.x++){
		for(P.y=bbmin.y;P.y<bbmax.y;P.y++){
			Vec3f bc = bayercoor(ps, P);
			if(bc.x<0 || bc.y<0 || bc.z<0) continue;
			image.set(P.x, P.y, color);
		}
	}

}

Matrix projection(const int width, const int height, const int depth){
	Matrix projection = Matrix::identity(4);
	projection[0][3] = width/2.;
	projection[1][3] = height/2.;
	projection[2][3] = depth/2.;
	projection[0][0] = width*3/8.;
	projection[1][1] = height*3/8.;
	projection[2][2] = depth/2.;
	return projection;
}

Matrix Tcm(Vec3f eye, Vec3f center , Vec3f up){
	Vec3f z = (eye - center).normalize();
	Vec3f x = (up^z).normalize();
	Vec3f y = (z^x).normalize();
	Matrix R = Matrix::identity(4);
	Matrix t = Matrix::identity(4);
	for(int i=0;i<3;i++){
		R[0][i] = x[i];
		R[1][i] = y[i];
		R[2][i] = z[i];
		//R[i][3] = -center[i];
		t[i][3] = -center[i];
	}
	return R*t;
}

void raster(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color, int *zbuff){
	if(p0.x> p1.x) std::swap(p0, p1);

	for(int i=p0.x;i<p1.x;i++){
		float t = (i - p0.x)/(float)(p1.x - p0.x);
		int y = p0.y * (1.-t) + p1.y *t;
		if(zbuff[i]<y){
			zbuff[i] = y;
			image.set(i, 0, color);
		}
	}
}


void trianglezb(std::vector<Vec2i> &ps, std::vector<float>& z,const int width, float* zbuff, TGAImage &image, TGAColor color){
	Vec2i bbmax(0, 0);
	Vec2i bbmin(image.get_width()-1, image.get_height()-1);
	for(int i=0;i<3;i++){
		bbmax.x = std::max(bbmax.x, ps[i].x);
		bbmax.y = std::max(bbmax.y, ps[i].y);
		bbmin.x = std::min(bbmin.x, ps[i].x);
		bbmin.y = std::min(bbmin.y, ps[i].y);
	}

	Vec2i P;

	for(P.x=bbmin.x;P.x<bbmax.x;P.x++){
		for(P.y=bbmin.y;P.y<bbmax.y;P.y++){
			Vec3f bs = bayercoor(ps, P);
			if(bs.x<0 || bs.y<0 || bs.z<0) continue;
			float tempz = 0.;
			for(int i=0;i<3;i++){
				tempz += z[i] * bs[i];
			}
			if(zbuff[P.x+P.y*width]<tempz){
				zbuff[P.x+P.y*width] = tempz;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void triangleuv(std::vector<Vec2i> &ps, std::vector<float>& z,const int width, float* zbuff, TGAImage &image, std::vector<TGAColor> &uvmap){
	Vec2i bbmin(image.get_width() - 1, image.get_height() - 1);
	Vec2i bbmax(0, 0);
	Vec2i cul(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		bbmin.x = std::max(0, std::min(ps[i].x, bbmin.x));
		bbmin.y = std::max(0, std::min(ps[i].y, bbmin.y));
		bbmax.x = std::min(cul.x, std::max(ps[i].x, bbmax.x));
		bbmax.y = std::min(cul.y, std::max(ps[i].y, bbmax.y));
	}

	Vec2i P;

	for(P.x=bbmin.x;P.x<bbmax.x;P.x++){
		for(P.y=bbmin.y;P.y<bbmax.y;P.y++){
			Vec3f bs = bayercoor(ps, P);
			if(bs.x<0 || bs.y<0 || bs.z<0) continue;
			float tempz = 0.;
			for(int i=0;i<3;i++){
				tempz += z[i] * bs[i];
			}
			
			if(zbuff[P.x+P.y*width]<tempz){
				float* tempcolor = new float[3];
				for(int i=0;i<3;i++){
					tempcolor[i] = 0.;
					for(int j=0;j<3;j++){
						tempcolor[i] += (int)uvmap[j].raw[2-i] * bs.raw[j];
					}
				}
				TGAColor color(tempcolor[0], tempcolor[1], tempcolor[2], 255);
				zbuff[P.x+P.y*width] = tempz;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void trianglenormal(std::vector<Vec2i> &ps, std::vector<float>& z,const int width, int* zbuff, TGAImage &image, std::vector<TGAColor> &uvmap, float* instensity){
	Vec2i bbmin(image.get_width() - 1, image.get_height() - 1);
	Vec2i bbmax(0, 0);
	Vec2i cul(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		bbmin.x = std::max(0, std::min(ps[i].x, bbmin.x));
		bbmin.y = std::max(0, std::min(ps[i].y, bbmin.y));
		bbmax.x = std::min(cul.x, std::max(ps[i].x, bbmax.x));
		bbmax.y = std::min(cul.y, std::max(ps[i].y, bbmax.y));
	}

	Vec2i P;

	for(P.x=bbmin.x;P.x<bbmax.x;P.x++){
		for(P.y=bbmin.y;P.y<bbmax.y;P.y++){
			Vec3f bs = bayercoor(ps, P);
			if(bs.x<0 || bs.y<0 || bs.z<0) continue;
			float tempz = 0.;
			float tempi = 0.;
			for(int i=0;i<3;i++){
				tempz += z[i] * bs[i];
				tempi += instensity[i]*bs[i];
			}

			if(zbuff[P.x+P.y*width]<(int)tempz){
				float* tempcolor = new float[3];
				for(int i=0;i<3;i++){
					tempcolor[i] = 0.;
					for(int j=0;j<3;j++){
						tempcolor[i] += (int)uvmap[j].raw[2-i] * bs.raw[j] * tempi;
					}
				}
				TGAColor color(tempcolor[0], tempcolor[1], tempcolor[2], 255);
				TGAColor color2(255*tempi, 255*tempi, 255*tempi, 255);
				zbuff[P.x+P.y*width] = (int)tempz;
				if (tempi<=0)
					color2 = black;
				image.set(P.x, P.y, color2);
			}
		}
	}
}

// int main(int argc, char** argv) {
// 	const int width = 200;
// 	const int height = 200;
// 	TGAImage image(width, height, TGAImage::RGB);
// 	// divid(image);
// 	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
// 	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
// 	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
// 	std::vector<Vec2i> ps0{Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
// 	std::vector<Vec2i> ps1{Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(20, 150)};
// 	std::vector<Vec2i> ps2{Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
// 	auto start = std::chrono::steady_clock::now();
// 	//triangle(t0[0], t0[1],t0[2], image, white);
// 	//triangle(t1[0], t1[1],t1[2], image, white);
// 	//triangle(t2[0], t2[1],t2[2], image, white);
// 	trianglebb(ps0, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
// 	trianglebb(ps1, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
// 	trianglebb(ps2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
// 	triangle(t0[0], t0[1],t0[2], image);
// 	triangle(t1[0], t1[1],t1[2], image);
// 	triangle(t2[0], t2[1],t2[2], image);
	
// 	auto end = std::chrono::steady_clock::now();
// 	std::cout << "Elapsed time in microseconds: " 
//     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
//     << " us";
// 	image.flip_vertically(); 
// 	image.write_tga_file("output.tga");
// 	return 0;
// }

// int main(int argc, char** argv) {
// 	const int width = 800;
// 	const int height = 800;
// 	TGAImage image(width, height, TGAImage::RGB);
// 	// divid(image);
// 	Model* model = NULL;
// 	if (2==argc) {
//         model = new Model(argv[1]);
//     } else {
//         model = new Model("obj/african_head.obj");
//     }
// 	auto start = std::chrono::steady_clock::now();
// 	Vec3f light(1., -1., -1.);
// 	for(int i=0;i<model->nfaces();i++){
// 		std::vector<int> face = model->face(i);
// 		std::vector<Vec2i> tempv(3);
// 		std::vector<Vec3f> modelps(3);
		
// 		// for(int j=0;j<3;j++){
// 		// 	Vec3f u = model->vert(face[j]);
// 		// 	Vec3f v = model->vert(face[(j+1)%3]);
// 		// 	int x0 = (u.x+1.0)*(width/2.0);
// 		// 	int x1 = (v.x+1.0)*(width/2.0);
// 		// 	int y0 = (u.y+1.0)*(height/2.0);
// 		// 	int y1 = (v.y+1.0)*(height/2.0);
// 		// 	line(x0, y0, x1, y1, image, white);
// 		// }
// 		for(int k=0;k<3;k++){
// 			Vec3f u = model->vert(face[k]);
// 		  	tempv[k] = Vec2i((u.x+1.0)*(width/2.0),(u.y+1.0)*(height/2.0));
// 			modelps[k] = u;
// 		}
// 		Vec3f n = (modelps[2] - modelps[0]) ^ (modelps[1] - modelps[0]);
// 		n.normalize();
// 		float intensity = n*light;
// 		if(intensity>0)
// 		trianglebb(tempv, image, TGAColor(intensity*255,intensity*255,intensity*255, 255));
// 		//triangle2(tempv[0], tempv[1], tempv[2], image, TGAColor(intensity*255,intensity*255,intensity*255, 255));
// 	}
// 	auto end = std::chrono::steady_clock::now();
// 	std::cout << "Elapsed time in microseconds: " 
//     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
//     << " us";
// 	image.flip_vertically(); 
// 	image.write_tga_file("scene.tga");
// 	return 0;
// }

// int main(){
// 	const int width = 800;
// 	const int height = 800;

// 	TGAImage scene(width, height, TGAImage::RGB);
// 	Vec2i p0[2] = {Vec2i(20, 34),   Vec2i(744, 400)};
// 	Vec2i p1[2] = {Vec2i(120, 434), Vec2i(444, 400)};
// 	Vec2i p2[2] = {Vec2i(330, 463), Vec2i(594, 200)};
// 	line(p0[0], p0[1], scene, red);
// 	line(p1[0], p1[1], scene, green);
// 	line(p2[0], p2[1], scene, blue);

// 	line(Vec2i(10,10), Vec2i(790, 10), scene, white);
// 	scene.flip_vertically();
// 	scene.write_tga_file("scene.tga");

	// {
	// 	TGAImage render(width, 16, TGAImage::RGB);
	// 	int ybuff[width];
	// 	for(int i=0;i<width;i++){
	// 		ybuff[i] = std::numeric_limits<int>::min();
	// 	}
	// 	raster(p0[0], p0[1],render, red, ybuff);
	// 	raster(p1[0], p1[1],render, green, ybuff);
	// 	raster(p2[0], p2[1],render, blue, ybuff);

	// 	for(int i=0;i<width;i++){
	// 		for(int j=1;j<16;j++){
	// 			render.set(i, j, render.get(i, 0));
	// 		}
	// 	}

	// 	render.flip_vertically();
	// 	render.write_tga_file("render.tga");
	// }
	// return 0;
// }



int main(){
	const int width = 800;
	const int height = 800;
	const int depth = 255;

	TGAImage scene(width, height, TGAImage::RGB);
	TGAImage texture;
	texture.read_tga_file("obj/african_head_diffuse.tga");
	//texture.read_tga_file("D:/project/myProject/tinyrenderer/obj/african_head_diffuse.tga");
	int texwidth = texture.get_width();
	int texheight = texture.get_height();
	texture.flip_vertically();
	Model* model = new Model("obj/african_head.obj");
	//Model* model = new Model("D:/project/myProject/tinyrenderer/obj/african_head.obj");
	int* zbuff = new int[width * height];
	for (int i=0; i<width*height; i++){
		zbuff[i] = std::numeric_limits<int>::min();
	}
	Vec3f light(1., -1., 1.);
	light.normalize();
	//Vec3f camera(0.,0.,100.);
	Vec3f eye(1, 1, 3), center(0., 0., 0.);
	Matrix proj = Matrix::identity(4);
	//proj[3][2] = -1./ camera.z;
	proj[3][2] = -1./ (eye-center).norm();
	Matrix view = Tcm(eye, center, Vec3f(0.,1.,0.));
	Matrix vp = projection(width, height, depth);
	auto start = std::chrono::steady_clock::now();
	for(int i = 0;i<model->nfaces();i++){
		std::vector<Vec2i> vs;
		std::vector<float> z;
		std::vector<int> face = model->face(i);
		std::vector<int> uvs = model->uvs(i);
		std::vector<Vec3f> modelps;
		std::vector<TGAColor> uvmap;
		float instensitys[3];
		for(int j=0;j<3;j++){
			Vec3f u = model->vert(face[j]);
			Matrix tempu(4,1);
			tempu[0][0] = u.x;
			tempu[1][0] = u.y;
			tempu[2][0] = u.z;
			tempu[3][0] = 1.;
			Matrix newxy = vp*proj*view*tempu;
			vs.push_back(Vec2i(newxy[0][0]/newxy[3][0], newxy[1][0]/newxy[3][0]));
			z.push_back(newxy[2][0]/newxy[3][0]);

			//vs.push_back(Vec2i((u.x+1.0)*(width/2.0),(u.y+1.0)*(height/2.0)));
			//z.push_back(u.z);
			modelps.push_back(u);

			Vec3f uv = model->vt(uvs[j]);
			//TGAColor tempcolor = texture.get(uv.x*texwidth, uv.y*texheight);
			//uvmap.push_back(tempcolor);
			uvmap.push_back(texture.get((int)(uv.x*texwidth)%texwidth, (int)(uv.y*texheight)%texheight));
		
			instensitys[j] = model->vn(i, j)*light;
		}
		trianglenormal(vs, z, width, zbuff, scene, uvmap, instensitys);
		// Vec3f n = (modelps[2] - modelps[0])^(modelps[1] - modelps[0]);
		// n.normalize();
		// float instensity = n * light;

		// if(instensity>0){
		// 	for(int k=0;k<uvmap.size();k++){
		// 		uvmap[k] = uvmap[k] * instensity;
		// 	} 
		// 	//trianglebb(vs, scene, TGAColor(instensity*255,instensity*255,instensity*255, 255));
		// 	//triangle2(vs[0], vs[1], vs[2], scene, TGAColor(instensity* 255, instensity*255, instensity*255, 255));
		// 	//trianglezb(vs, z, width, zbuff, scene, TGAColor(instensity* 255, instensity*255, instensity*255, 255));
		// 	triangleuv(vs, z, width, zbuff, scene, uvmap);
		// }
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in microseconds: " 
    << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
    << " us";

	scene.flip_vertically();
	scene.write_tga_file("scene.tga");
	delete model;



	{ // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i=0; i<width; i++) {
            for (int j=0; j<height; j++) {
                zbimage.set(i, j, TGAColor(zbuff[i+j*width]));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }
	delete [] zbuff;
	return 0;
}


// int main(){
// 	const int width = 400;
// 	const int height = 400;

// 	TGAImage scene(width, height, TGAImage::RGB);
// 	Model* model = new Model("obj/cube.obj");
	
// 	Vec2i VP(width-5, height/2);
// 	Vec2i plane[2] = {Vec2i(width/4, 0), Vec2i(width/4, height)};

// 	Vec3f ps[5]={Vec3f(width/16., height*3./4, 1.),Vec3f(width/4., height*3./4, 1.), \
// 				Vec3f(width/2., height/2., 1.),Vec3f(width/2., height/4., 1.),Vec3f(width/16., height/4., 1.) };

// 	Vec3f axis[3] = {Vec3f(width/4., height/2., 1.),Vec3f(width/4., height*3./4, 1.), \
// 				Vec3f(width/2., height/2., 1.)};
// 	auto start = std::chrono::steady_clock::now();
// 	line(plane[0], plane[1], scene, white);
// 	for(int i=0;i<5;i++){
// 		if(ps[i].x>axis[0][0]){
// 			float tempy = (ps[i].x - axis[0].x)*VP.y -(VP.x - axis[0].x)* ps[i].y;
// 			tempy = tempy/(ps[i].x - VP.x);
// 			line(VP, Vec2i(axis[0].x, tempy), scene, yellow);
// 		}
// 		else 
// 		line(VP, Vec2i(ps[i].x, ps[i].y), scene, yellow);
// 		//line(Vec2i(ps[i].x, ps[i].y), Vec2i(ps[(i+1)%5].x, ps[(i+1)%5].y), scene, white);
// 	}
// 	{
// 		line(Vec2i(axis[0].x, axis[0].y), Vec2i(axis[1].x, axis[1].y), scene, green);
// 		line(Vec2i(axis[0].x, axis[0].y), Vec2i(axis[2].x, axis[2].y), scene, red);

// 	}
// 	Matrix trans =Matrix::identity(3);
// 	//trans[2][0] = -0.2/(axis[2].x - axis[0].x);
// 	trans[2][0] = -1./(VP.x - axis[0].x);
// 	Matrix pts(3,5);
// 	for(int i=0;i<3;i++){
// 		for(int j=0;j<5;j++){
// 			pts[i][j] = ps[j][i];
// 			if(i<2) pts[i][j] -= axis[0][i];
// 		}
// 	}
// 	Matrix new_pts = trans * pts;
// 	for(int j=0;j<5;j++){
// 		new_pts[0][j] = new_pts[0][j]/new_pts[2][j];
// 		new_pts[1][j] = new_pts[1][j]/new_pts[2][j];
// 		new_pts[2][j] = new_pts[2][j]/new_pts[2][j];
// 	}
// 	std::cout<<trans<<std::endl;
// 	std::cout<<pts<<std::endl;
// 	std::cout<<new_pts<<std::endl;
// 	for(int j=0;j<5;j++){
// 		new_pts[0][j] += axis[0][0];
// 		new_pts[1][j] += axis[0][1];
// 	}
// 	std::cout<<new_pts<<std::endl;
// 	{
// 		for(int i=0;i<5;i++){
// 			//line(VP, Vec2i(new_pts[0][i], new_pts[1][i]), scene, yellow);
// 			line(Vec2i(new_pts[0][i], new_pts[1][i]), Vec2i(new_pts[0][(i+1)%5], new_pts[1][(i+1)%5]), scene, red);
// 		}
// 	}
// 	auto end = std::chrono::steady_clock::now();
// 	std::cout << "Elapsed time in microseconds: " 
//     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
//     << " us";

// 	scene.flip_vertically();
// 	scene.write_tga_file("proj.tga");
// 	delete model;
// 	return 0;
// }