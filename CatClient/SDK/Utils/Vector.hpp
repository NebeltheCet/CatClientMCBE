#pragma once

class Vector3 {
public:
	float x, y, z;
	Vector3(float x, float y, float z) {
		Vector3(x, y, z);
	}
	Vector3(int x, int y, int z) {
		Vector3(x, y, z);
	}
	Vector3() {
		Vector3(0.f, 0.f, 0.);
	}
};