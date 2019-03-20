#include "IOManager.h"

using namespace std;
using namespace cv;

void IOManager::sendObjectData(std::vector<ObjectTracked>& o, std::string filename)
{
	//Pytanie czy sens jest siê bawiæ z FileStorage bo jest to wygodne do zapisywanie macierzy ale przy takim zapisywaniu punktów chyba ju¿ nie jest takie wygodne albo ja nie umiem tego dobrze zrobic.
	//mysle ze tak wystarczy
	fstream file;

	file.open("database.txt", ios::out | ios::app);
	if (file.good())
	{
		for (unsigned int i = 0; i < o.size(); i++)
		{
			if (o[i].good())
			{
				//jesli obiekt nadal istnieje, pobieramy jego dane i wysylamy do jakiegos pliku
				int number = i;
				Rect r = o[i].getCoordinates();

				file << i << r.x << " " << r.y << endl;
			}
		}
		file.close();
	}
	else
		cout << "Error: file cannot be opened" << endl;
}
