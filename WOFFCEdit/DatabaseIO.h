#pragma once
#include "sqlite3.h"
#include "SceneObject.h"
#include "ChunkObject.h"
#include <vector>

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
