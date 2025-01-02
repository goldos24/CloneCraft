#pragma once

constexpr inline const int getVoxelSubdivision() {
	return 8;
}

constexpr inline const float getFaceSize() {
	return 1.0 / getVoxelSubdivision();
}
