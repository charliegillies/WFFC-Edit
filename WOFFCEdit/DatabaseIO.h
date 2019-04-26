#pragma once
#include "sqlite3.h"
#include "SceneObject.h"
#include "ChunkObject.h"
#include <vector>

// Handles the connection, reading and writing of the scene graph
// and chunk, to and from the local database. This helps keep 
// the implementation nicely encapsulated from the tool, 
// and easy to change later on, if required.
class DatabaseIO {
private:
	sqlite3 *m_databaseConnection;	//sqldatabase handle

public:
	DatabaseIO();
	~DatabaseIO();

	void closeConnection();
	bool tryOpenConnection();

	void read(std::vector<SceneObject>* graph, ChunkObject* chunk);
	void writeGraph(std::vector<SceneObject>* graph);
};
