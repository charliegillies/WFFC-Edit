#include "DatabaseIO.h"
#include <string>
#include <sstream>

#include "SceneGraph.h"

DatabaseIO::DatabaseIO()
{
	m_databaseConnection = nullptr;
}

DatabaseIO::~DatabaseIO()
{
}

void DatabaseIO::closeConnection()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}

bool DatabaseIO::tryOpenConnection()
{
	// Attempt to open the database with read/write permissions.
	int rc = sqlite3_open_v2("database/test.db", &m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);
	return rc == SQLITE_OK;
}

void DatabaseIO::read(SceneGraph* graph, ChunkObject* chunk)
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);

	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);

		//send completed object to scenegraph
		graph->getObjects()->push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	chunk->ID = sqlite3_column_int(pResultsChunk, 0);
	chunk->name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	chunk->chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	chunk->chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	chunk->chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	chunk->heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	chunk->tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	chunk->tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	chunk->tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	chunk->tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	chunk->tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	chunk->tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	chunk->render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	chunk->render_normals = sqlite3_column_int(pResultsChunk, 13);
	chunk->tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	chunk->tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	chunk->tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	chunk->tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	chunk->tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);
}

void DatabaseIO::writeGraph(SceneGraph* graph)
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = graph->size();	//Loop thru the scengraph.

	std::vector<SceneObject>* objects = graph->getObjects();

	for (int i = 0; i < numObjects; i++)
	{
		SceneObject& obj = objects->at(i);

		std::stringstream command;
		command << "INSERT INTO Objects "
			<< "VALUES(" << obj.ID << ","
			<< obj.chunk_ID << ","
			<< "'" << obj.model_path << "'" << ","
			<< "'" << obj.tex_diffuse_path << "'" << ","
			<< obj.posX << ","
			<< obj.posY << ","
			<< obj.posZ << ","
			<< obj.rotX << ","
			<< obj.rotY << ","
			<< obj.rotZ << ","
			<< obj.scaX << ","
			<< obj.scaY << ","
			<< obj.scaZ << ","
			<< obj.render << ","
			<< obj.collision << ","
			<< "'" << obj.collision_mesh << "'" << ","
			<< obj.collectable << ","
			<< obj.destructable << ","
			<< obj.health_amount << ","
			<< obj.editor_render << ","
			<< obj.editor_texture_vis << ","
			<< obj.editor_normals_vis << ","
			<< obj.editor_collision_vis << ","
			<< obj.editor_pivot_vis << ","
			<< obj.pivotX << ","
			<< obj.pivotY << ","
			<< obj.pivotZ << ","
			<< obj.snapToGround << ","
			<< obj.AINode << ","
			<< "'" << obj.audio_path << "'" << ","
			<< obj.volume << ","
			<< obj.pitch << ","
			<< obj.pan << ","
			<< obj.one_shot << ","
			<< obj.play_on_init << ","
			<< obj.play_in_editor << ","
			<< obj.min_dist << ","
			<< obj.max_dist << ","
			<< obj.camera << ","
			<< obj.path_node << ","
			<< obj.path_node_start << ","
			<< obj.path_node_end << ","
			<< obj.parent_id << ","
			<< obj.editor_wireframe << ","
			<< "'" << obj.name << "'" << ","

			<< obj.light_type << ","
			<< obj.light_diffuse_r << ","
			<< obj.light_diffuse_g << ","
			<< obj.light_diffuse_b << ","
			<< obj.light_specular_r << ","
			<< obj.light_specular_g << ","
			<< obj.light_specular_b << ","
			<< obj.light_spot_cutoff << ","
			<< obj.light_constant << ","
			<< obj.light_linear << ","
			<< obj.light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);
	}
}
