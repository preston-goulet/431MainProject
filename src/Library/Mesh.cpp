#include "Mesh.h"
#include "noise.h"

// OBJ file - str to int
int Mesh::StrToInt(const string &str) {
	int i;
	if (sscanf_s(str.c_str(), "%i", &i) == 1) return i;
	else return 0;
}

// OBJ file - split string
vector<string> Mesh::split_string(const string& str, const string& split_str) {
	vector<string> stlv_string;
	string part_string("");
	string::size_type i;
	i = 0;
	while (i < str.size()) {
		if (split_str.find(str[i]) != string::npos) {
			stlv_string.push_back(part_string);
			part_string = "";
			while (split_str.find(str[i]) != string::npos) {
				++i;
			}
		}
		else {
			part_string += str[i];
			++i;
		}
	}
	if (!part_string.empty())
		stlv_string.push_back(part_string);
	return stlv_string;
}

// OBJ file - load file
void Mesh::loadFile(const char* file) {
	dot_vertex.clear();
	face_index_vertex.clear();
	ifstream infile(file);
	if (infile.fail()) {
		cout << "Error opening file " << file;
		return;
	}
	char current_line[1024];
	while (!infile.eof()) {
		infile.getline(current_line, 1024);
		switch (current_line[0]) {
		case'v':
			float x, y, z;
			switch (current_line[1]) {
			case 'n':
				sscanf_s(current_line, "vn %f %f %f", &x, &y, &z);
				dot_normalPerFace.push_back(Vec3f(x, y, z));
				dot_normalPerVertex.push_back(Vec3f(x, y, z));
				break;
			case 't':
				sscanf_s(current_line, "vt %f %f", &x, &y);
				dot_texture.push_back(Vec2f(x, y));
				break;
			default:
				sscanf_s(current_line, "v %f %f %f", &x, &y, &z);
				dot_vertex.push_back(Vec3f(x, y, z));
				break;
			}
			break;
		case 'f': {
			vector<string> faces = split_string(current_line, " \t.\r\n");
			int vnt[3][3] = { { -1, -1, -1 },{ -1, -1, -1 },{ -1, -1, -1 } };
			string::size_type begin, end;
			for (int i = 0; i < 2; i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i + 1].find_first_of("/", begin);
					if (begin < end) {
						vnt[i][j] = StrToInt(faces[i + 1].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[i][j] = -1;
					}
					begin = end + 1;
					j++;
				} while (end != string::npos);
			}
			for (unsigned int i = 3; i < faces.size(); i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i].find_first_of("/", begin);
					if (begin < end) {
						vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[2][j] = -1;
					}
					begin = end + 1;
					j++;
				} while (end != string::npos);
				for (int j = 0; j < 3; j++) {
					face_index_vertex.push_back(vnt[j][0]);
					if (vnt[j][1] != -1) face_index_texture.push_back(vnt[j][1]);
					if (vnt[j][2] != -1) face_index_normalPerFace.push_back(vnt[j][2]);
					if (vnt[j][2] != -1) face_index_normalPerVertex.push_back(vnt[j][2]);
				}
				memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
			}
		}
				  break;
		default:
			break;
		}
	}
}

// create a triangulated diamond
void Mesh::createCube() {
	
	// Vertexes
	int BOX_WIDTH = 1800;
	int BOX_HEIGHT = 1800;
	int BOX_DEPTH = 1800;

	dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, BOX_DEPTH));

	dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, 0.0));

	// face 1
	face_index_vertex.push_back(0);//0
	face_index_vertex.push_back(2);//1
	face_index_vertex.push_back(1);//2
	face_index_vertex.push_back(0);//0
	face_index_vertex.push_back(3);//2
	face_index_vertex.push_back(2);//3
										 // face 2
	face_index_vertex.push_back(0);//0
	face_index_vertex.push_back(5);//1
	face_index_vertex.push_back(4);//5
	face_index_vertex.push_back(0);//0
	face_index_vertex.push_back(1);//5
	face_index_vertex.push_back(5);//4
										 // face 3
	face_index_vertex.push_back(1);//1
	face_index_vertex.push_back(6);//2
	face_index_vertex.push_back(5);//6
	face_index_vertex.push_back(1);//1
	face_index_vertex.push_back(2);//6
	face_index_vertex.push_back(6);//5
										 // face 4
	face_index_vertex.push_back(2);//2
	face_index_vertex.push_back(7);//3
	face_index_vertex.push_back(6);//7
	face_index_vertex.push_back(2);//2
	face_index_vertex.push_back(3);//7
	face_index_vertex.push_back(7);//6
										 // face 5
	face_index_vertex.push_back(4);//0
	face_index_vertex.push_back(3);//3
	face_index_vertex.push_back(0);//7
	face_index_vertex.push_back(4);//0
	face_index_vertex.push_back(7);//7
	face_index_vertex.push_back(3);//4
										 // face 6
	face_index_vertex.push_back(5);//4 
	face_index_vertex.push_back(7);//5
	face_index_vertex.push_back(4);//6
	face_index_vertex.push_back(5);//4
	face_index_vertex.push_back(6);//6
	face_index_vertex.push_back(7);//7

										 // texture
	dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	for (int t = 0; t<6; t++) {
		face_index_texture.push_back(0);//0
		face_index_texture.push_back(2);//1
		face_index_texture.push_back(1);//2
		face_index_texture.push_back(0);//0
		face_index_texture.push_back(3);//2
		face_index_texture.push_back(2);//3
	}

	
}

// create a triangulated box
void Mesh::createSkybox(int w) {
	// Vertexes
	int BOX_WIDTH = w;
	int BOX_HEIGHT = w;
	int BOX_DEPTH = w;
	dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, BOX_DEPTH));
	dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, BOX_DEPTH));

	dot_vertex.push_back(Vec3<GLfloat>(0.0, BOX_HEIGHT, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, BOX_HEIGHT, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(BOX_WIDTH, 0.0, 0.0));
	dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, 0.0));

	// face 1
	face_index_vertex.push_back(1);//0
	face_index_vertex.push_back(2);//1
	face_index_vertex.push_back(0);//2
	face_index_vertex.push_back(2);//0
	face_index_vertex.push_back(3);//2
	face_index_vertex.push_back(0);//3
										 // face 2
	face_index_vertex.push_back(4);//0
	face_index_vertex.push_back(5);//1
	face_index_vertex.push_back(0);//5
	face_index_vertex.push_back(5);//0
	face_index_vertex.push_back(1);//5
	face_index_vertex.push_back(0);//4
										 // face 3
	face_index_vertex.push_back(5);//1
	face_index_vertex.push_back(6);//2
	face_index_vertex.push_back(1);//6
	face_index_vertex.push_back(6);//1
	face_index_vertex.push_back(2);//6
	face_index_vertex.push_back(1);//5
										 // face 4
	face_index_vertex.push_back(6);//2
	face_index_vertex.push_back(7);//3
	face_index_vertex.push_back(2);//7
	face_index_vertex.push_back(7);//2
	face_index_vertex.push_back(3);//7
	face_index_vertex.push_back(2);//6
										 // face 5
	face_index_vertex.push_back(0);//0
	face_index_vertex.push_back(3);//3
	face_index_vertex.push_back(4);//7
	face_index_vertex.push_back(3);//0
	face_index_vertex.push_back(7);//7
	face_index_vertex.push_back(4);//4
										 // face 6
	face_index_vertex.push_back(4);//4 
	face_index_vertex.push_back(7);//5
	face_index_vertex.push_back(5);//6
	face_index_vertex.push_back(7);//4
	face_index_vertex.push_back(6);//6
	face_index_vertex.push_back(5);//7
										 
										 //face with number 2
	dot_texture.push_back(Vec2<GLfloat>(0.0, 2.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.25, 2.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.25, 1.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0 / 3.0));
	face_index_texture.push_back(1);//0
	face_index_texture.push_back(2);//1
	face_index_texture.push_back(0);//2
	face_index_texture.push_back(2);//0
	face_index_texture.push_back(3);//2
	face_index_texture.push_back(0);//3
										  //face with number 1
	dot_texture.push_back(Vec2<GLfloat>(0.25, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 0.66));
	dot_texture.push_back(Vec2<GLfloat>(0.25, 0.66));
	face_index_texture.push_back(5);//0
	face_index_texture.push_back(6);//1
	face_index_texture.push_back(4);//2
	face_index_texture.push_back(6);//0
	face_index_texture.push_back(7);//2
	face_index_texture.push_back(4);//3
										  //face with number 3
	dot_texture.push_back(Vec2<GLfloat>(0.25, 2.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 2.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 1.0 / 3.0));
	dot_texture.push_back(Vec2<GLfloat>(0.25, 1.0 / 3.0));
	face_index_texture.push_back(9);//0
	face_index_texture.push_back(10);//1
	face_index_texture.push_back(8);//2
	face_index_texture.push_back(10);//0
	face_index_texture.push_back(11);//2
	face_index_texture.push_back(8);//3
										  //face with number 6
	dot_texture.push_back(Vec2<GLfloat>(0.25, 1.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 1.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 0.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.25, 0.0 / 3));
	face_index_texture.push_back(13);//0
	face_index_texture.push_back(14);//1
	face_index_texture.push_back(12);//2
	face_index_texture.push_back(14);//0
	face_index_texture.push_back(15);//2
	face_index_texture.push_back(12);//3
										   //face with number 5
	dot_texture.push_back(Vec2<GLfloat>(0.75, 2.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 2.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.75, 1.0 / 3));

	face_index_texture.push_back(17);//0
	face_index_texture.push_back(18);//1
	face_index_texture.push_back(16);//2
	face_index_texture.push_back(18);//0
	face_index_texture.push_back(19);//2
	face_index_texture.push_back(16);//3
										   //face with number 4
	dot_texture.push_back(Vec2<GLfloat>(0.50, 2.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.75, 2.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.75, 1.0 / 3));
	dot_texture.push_back(Vec2<GLfloat>(0.50, 1.0 / 3));
	face_index_texture.push_back(21);//0
	face_index_texture.push_back(22);//1
	face_index_texture.push_back(20);//2
	face_index_texture.push_back(22);//0
	face_index_texture.push_back(23);//2
	face_index_texture.push_back(20);//3
}

// creating a triangulated plane
void Mesh::createPlane(int arena_width, int arena_depth, int arena_cell) {
	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;

	// vertices
	for (int i = 0; i<n; i++) {
		for (int j = 0; j < m; j++) {
			dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, 0.0, j*arena_cell));
		}
	}
	//texture
	dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	// faces
	for (int i = 0; i<(n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		face_index_vertex.push_back(i); face_index_vertex.push_back(i + 1);
		face_index_vertex.push_back(i + n);
		face_index_vertex.push_back(i + 1); face_index_vertex.push_back(i + n + 1);
		face_index_vertex.push_back(i + n);
		// index for texture
		for (int t = 0; t<6; t++) {
			face_index_texture.push_back(3);//0
			face_index_texture.push_back(2);//2
			face_index_texture.push_back(1);//1
			face_index_texture.push_back(2);//0
			face_index_texture.push_back(0);//3
			face_index_texture.push_back(1);//2
		}
	}
}

float** Mesh::createPerlinPlane(int arena_width, int arena_depth, int arena_cell) {
	int n = (arena_cell + arena_width) / arena_cell;
	int m = (arena_cell + arena_depth) / arena_cell;

	ImprovedNoise ynoise;
	float **heightMap;
	int xLength, yLength;

	xLength = n;
	yLength = m;

	heightMap = new float*[n];
	for (int x = 0; x < n; x++) {
		heightMap[x] = new float[m];
	}

	// vertices
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			float y = ynoise.perlinMultiscale(i * 20, j * 20);
			heightMap[i][j] = y;
			dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, y, j*arena_cell));
		}
	}
	//texture
	dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	// faces
	for (int i = 0; i < (n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		face_index_vertex.push_back(i); face_index_vertex.push_back(i + 1);
		face_index_vertex.push_back(i + n);
		face_index_vertex.push_back(i + 1); face_index_vertex.push_back(i + n + 1);
		face_index_vertex.push_back(i + n);
		// index for texture
		for (int t = 0; t < 6; t++) {
			face_index_texture.push_back(3);//0
			face_index_texture.push_back(2);//2
			face_index_texture.push_back(1);//1
			face_index_texture.push_back(2);//0
			face_index_texture.push_back(0);//3
			face_index_texture.push_back(1);//2
		}
	}
	return heightmap;
}

// normal per face
//TODO: check
void Mesh::calculateNormalPerFace() {
	Vec3f v1, v2, v3, v4, v5;
	for (unsigned int i = 0; i < face_index_vertex.size(); i += 3) {
		v1 = dot_vertex.at(face_index_vertex.at(i));
		v2 = dot_vertex.at(face_index_vertex.at(i+1));
		v3 = dot_vertex.at(face_index_vertex.at(i+2));
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();
		dot_normalPerFace.push_back(v4);
		int pos = dot_normalPerFace.size() - 1;
		// same normal for all vertex in this face
		face_index_normalPerFace.push_back(pos);
		face_index_normalPerFace.push_back(pos);
		face_index_normalPerFace.push_back(pos);
	}
}

// calculate normal per vertex
void Mesh::calculateNormalPerVertex() {
	dot_normalPerVertex.clear();
	face_index_normalPerVertex.clear();
	Vec3f suma; suma.x = 0; suma.y = 0; suma.z = 0;
	//initialize
	for (unsigned int val = 0; val < dot_vertex.size(); val++) {
		dot_normalPerVertex.push_back(suma);
	}
	// calculate sum for vertex
	for (unsigned long pos = 0; pos < face_index_vertex.size(); pos++) {
		dot_normalPerVertex.at(face_index_vertex.at(pos)) +=
			dot_normalPerFace.at(face_index_normalPerFace.at(pos));
	}
	// normalize for vertex 
	for (unsigned int val = 0; val < dot_normalPerVertex.size(); val++) {
		dot_normalPerVertex.at(val) = dot_normalPerVertex.at(val).normalize();
	}
	//normalVertexIndex is the same that vertexIndex
	for (unsigned int pos = 0; pos < face_index_vertex.size(); pos++) {
		face_index_normalPerVertex.push_back((face_index_vertex[pos]));
	}
}
