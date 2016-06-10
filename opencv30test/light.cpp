#include "light.h"
#include <cstdlib>

const int BoxLight::SAMPLEGRIDSIZE = 16;
const int BoxLight::SAMPLEGRIDSIZEMOD = 15;
const int BoxLight::SAMPLEGRIDSIZESQRT = 4;
const double BoxLight::SAMPLEGRIDSIZESQRTDOWN = 1.0 / SAMPLEGRIDSIZESQRT;
void BoxLight::initSampleGrid()
{
	if (sampleGrid == 0)
		sampleGrid = new double [SAMPLEGRIDSIZE << 1];
	int seq[SAMPLEGRIDSIZE];
	for (int i = 0; i < SAMPLEGRIDSIZE; i++)
		seq[i] = i;
	std::random_shuffle(seq, seq + SAMPLEGRIDSIZE);
	for (int i = 0; i < SAMPLEGRIDSIZE; i++){
		sampleGrid[i << 1] = (seq[i] / SAMPLEGRIDSIZESQRT) * box.getSize().x / SAMPLEGRIDSIZESQRT + box.pos.x;
		sampleGrid[(i << 1) ^ 1] = (seq[i] % SAMPLEGRIDSIZESQRT) * box.getSize().z / SAMPLEGRIDSIZESQRT + box.pos.z; 
	}	
}