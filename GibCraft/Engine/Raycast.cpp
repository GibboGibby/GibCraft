#include "Raycast.h"

namespace Physics
{

	static float _bin_size = 1.0f;
	void FastVoxelTraversal(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& visited_voxels)
	{
		glm::vec3 current_voxel(std::floor(start[0] / _bin_size),
			std::floor(start[1] / _bin_size),
			std::floor(start[2] / _bin_size));

		glm::vec3 last_voxel(std::floor(end[0] / _bin_size),
			std::floor(end[1] / _bin_size),
			std::floor(end[2] / _bin_size));

		glm::vec3 ray = end - start;

		ray = glm::normalize(ray);

		double stepX = (ray[0] >= 0) ? 1 : -1;
		double stepY = (ray[1] >= 0) ? 1 : -1;
		double stepZ = (ray[2] >= 0) ? 1 : -1;

		double next_voxel_boundary_x = (current_voxel[0] + stepX) * _bin_size; // correct
		double next_voxel_boundary_y = (current_voxel[1] + stepY) * _bin_size; // correct
		double next_voxel_boundary_z = (current_voxel[2] + stepZ) * _bin_size; // correct

		double tMaxX = (ray[0] != 0) ? (next_voxel_boundary_x - start[0]) / ray[0] : DBL_MAX; //
		double tMaxY = (ray[1] != 0) ? (next_voxel_boundary_y - start[1]) / ray[1] : DBL_MAX; //
		double tMaxZ = (ray[2] != 0) ? (next_voxel_boundary_z - start[2]) / ray[2] : DBL_MAX; //

		double tDeltaX = (ray[0] != 0) ? _bin_size / ray[0] * stepX : DBL_MAX;
		double tDeltaY = (ray[1] != 0) ? _bin_size / ray[1] * stepX : DBL_MAX;
		double tDeltaZ = (ray[2] != 0) ? _bin_size / ray[2] * stepX : DBL_MAX;

		glm::vec3 diff(0, 0, 0);

		bool neg_ray = false;

		if (current_voxel[0] != last_voxel[0] && ray[0] < 0) { diff[0]--; neg_ray = true; }
		if (current_voxel[1] != last_voxel[1] && ray[1] < 0) { diff[1]--; neg_ray = true; }
		if (current_voxel[2] != last_voxel[2] && ray[2] < 0) { diff[2]--; neg_ray = true; }

		visited_voxels.push_back(current_voxel);

		if (neg_ray)
		{
			current_voxel += diff;
			visited_voxels.push_back(current_voxel);
		}

		while (last_voxel != current_voxel)
		{
			if (tMaxX < tMaxY)
			{
				if (tMaxX < tMaxZ)
				{
					current_voxel[0] += stepX;
					tMaxX += tDeltaX;
				}
				else
				{
					current_voxel[2] += stepZ;
					tMaxZ += tDeltaZ;
				}
			}
			else
			{
				if (tMaxY < tMaxZ)
				{
					current_voxel[1] += stepY;
					tMaxY += tDeltaY;
				}
				else
				{
					current_voxel[2] += stepZ;
					tMaxZ += tDeltaZ;
				}
			}

			visited_voxels.push_back(current_voxel);
		}
		return;
	}
}