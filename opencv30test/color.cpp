#include "color.h"

Color operator + ( const Color& A , const Color& B ) {
	double r = A.color[2] + B.color[2];
	double g = A.color[1] + B.color[1];
	double b = A.color[0] + B.color[0];
	return Color(r, g, b);
}

Color operator - ( const Color& A , const Color& B ) {
	double r = A.color[2] - B.color[2];
	double g = A.color[1] - B.color[1];
	double b = A.color[0] - B.color[0];
	return Color(r, g, b);
}

Color operator * ( const Color& A , const Color& B ) {
	double r = A.color[2] * B.color[2];
	double g = A.color[1] * B.color[1];
	double b = A.color[0] * B.color[0];
	return Color(r, g, b);
}

Color operator * ( const double& k , const Color& A) {
	return Color(k * A.color[2], k * A.color[1], k * A.color[0]);
}

