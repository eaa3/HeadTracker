#include "Model3D.h"


Model3D::Model3D()
{
	this->angle[0] = this->angle[1] = this->angle[2] = 0.0f;
	this->scaleFactor = 1.0f;
}

Model3D::Model3D(string filename)
{
	this->angle[0] = this->angle[1] = this->angle[2] = 0.0f;
	this->scaleFactor = 1.0f;

	this->load(filename);
}

Model3D::~Model3D()
{
	//dtor
}

void Model3D::load(string filename)
{
	this->vertex.clear();
	this->normal.clear();
	this->face.clear();

	Vector3 vtemp;
	string line;
	string format = "%d//%d";
	int vi, vin;
	Face facetemp;

	ifstream in(filename.c_str());

	while( getline(in, line) )
	{
		
		if( line[0] == '#' ) continue;

		if( line[0] == 'v' && line[1] == 'n' )
		{
			sscanf(line.substr(2).c_str(), "%f %f %f", &vtemp[0], &vtemp[1], &vtemp[2]);

			this->normal.push_back(vtemp);
		}
		else if( line[0] == 'v')
		{
			sscanf(line.substr(1).c_str(), "%f %f %f", &vtemp[0], &vtemp[1], &vtemp[2]);
			this->vertex.push_back(vtemp);
		}
		else if( line[0] == 'f' )
		{
			facetemp.v.clear();
			facetemp.vn.clear();
			format = "%d//%d";

			while( sscanf(line.substr(1).c_str(), format.c_str(), &vi, &vin) == 2 )
			{
				vi--; vin--;
				facetemp.v.push_back(vi);
				facetemp.vn.push_back(vin);

				format = "%*d//%*d " + format;
			}

			this->face.push_back(facetemp);
		}


	}

	printf("%d %d", face.size());
	in.close();
}

void Model3D::draw()
{

	glPushMatrix();

	glTranslatef(translation[0], translation[1], translation[2]);

	glRotatef(angle[0], 1,0,0);
	glRotatef(angle[1], 0,1,0);
	glRotatef(angle[2], 0,0,1);

	glScalef(scaleFactor, scaleFactor, scaleFactor);
	glScalef(1,1,-1);

	/*MEU DESENHO */
	for(int i = 0; i < face.size(); i++)
	{
		glBegin(GL_POLYGON);
		glColor3f(1.0f,0.5,0.0f);
		for(int j = 0; j < face[i].v.size(); j++ )
		{
			glNormal3fv(normal[face[i].vn[j]]);
			glVertex3fv(vertex[face[i].v[j]]);
		}
		glEnd();
	}
	/*Fim desenho*/

	glPopMatrix();
}
