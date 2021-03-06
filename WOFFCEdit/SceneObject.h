#pragma once

#include <string>


//This object should accurately and totally reflect the information stored in the object table


class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	int ID;
	int chunk_ID;
	std::string model_path;
	std::string tex_diffuse_path;

	int parent_id;
	std::string name;

	// Transform data!
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	float scaX, scaY, scaZ;
	float pivotX, pivotY, pivotZ;

	bool render, collision;
	std::string collision_mesh;
	bool collectable, destructable;
	int health_amount;
	bool editor_render, editor_texture_vis;
	bool editor_normals_vis, editor_collision_vis, editor_pivot_vis;
	bool snapToGround;
	bool AINode;
	std::string audio_path;
	float volume;
	float pitch;
	float pan;
	bool one_shot;
	bool play_on_init, play_in_editor;
	int min_dist, max_dist;

	bool camera;

	bool path_node;
	bool path_node_start;
	bool path_node_end;

	bool editor_wireframe;

	int light_type;
	float light_diffuse_r, light_diffuse_g, light_diffuse_b;
	float light_specular_r, light_specular_g, light_specular_b;
	float light_spot_cutoff;
	float light_constant;
	float light_linear;
	float light_quadratic;

	void setPosition(const float x, const float y, const float z);
	void setScale(const float x, const float y, const float z);
	void setRotation(const float x, const float y, const float z);

	static SceneObject CreatePrimitive();
};

