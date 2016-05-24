#ifndef TEXURE_H
#define TEXURE_H

#include "color.h"

class Texture
{
public:
	Texture( Color* a_Bitmap, int a_Width, int a_Height) : m_Bitmap( a_Bitmap ), m_Width( a_Width ), m_Height( a_Height ) {};
	Texture( char* a_File ){
		FILE* f = fopen( a_File, "rb" );
		if (f) {
			// extract width and height from file header
			unsigned char buffer[20];
			fread( buffer, 1, 20, f );
			m_Width = *(buffer + 12) + 256 * *(buffer + 13);
			m_Height = *(buffer + 14) + 256 * *(buffer + 15);
			fclose( f );
			// read pixel data
			f = fopen( a_File, "rb" );
			unsigned char* t = new unsigned char[m_Width * m_Height * 3 + 1024];
			fread( t, 1, m_Width * m_Height * 3 + 1024, f );
			fclose( f );
			// convert RGB 8:8:8 pixel data to floating point RGB
			m_Bitmap = new Color[m_Width * m_Height];
			
			double rec = 1.0 / 256;
			
			for ( int size = m_Width * m_Height, i = 0; i < size; i++ ){
				m_Bitmap[i] = Color( t[i * 3 + 20] * rec, t[i * 3 + 19] * rec, t[i * 3 + 18] * rec );
				
			}
			delete t;
		}
	}

	Color* GetBitmap() { return m_Bitmap; }
	Color GetTexel( double a_U, double a_V ){
		// fetch a bilinearly filtered texel
		double fu = (a_U + 1000.5f) * m_Width;
		double fv = (a_V + 1000.0f) * m_Width;
		int u1 = ((int)fu) % m_Width;
		int v1 = ((int)fv) % m_Height;
		int u2 = (u1 + 1) % m_Width;
		int v2 = (v1 + 1) % m_Height;
		// calculate fractional parts of u and v
		double fracu = fu - floor( fu );
		double fracv = fv - floor( fv );
		// calculate weight factors
		double w1 = (1 - fracu) * (1 - fracv);
		double w2 = fracu * (1 - fracv);
		double w3 = (1 - fracu) * fracv;
		double w4 = fracu *  fracv;
		// fetch four texels
		Color c1 = m_Bitmap[u1 + v1 * m_Width];
		Color c2 = m_Bitmap[u2 + v1 * m_Width];
		Color c3 = m_Bitmap[u1 + v2 * m_Width];
		Color c4 = m_Bitmap[u2 + v2 * m_Width];
		// scale and sum the four colors
		return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
	}

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
private:
	Color* m_Bitmap;
	int m_Width, m_Height;
};


#endif