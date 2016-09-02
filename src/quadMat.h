#ifndef __INCLUDED_QUADMAT_H__
#define __INCLUDED_QUADMAT_H__

class QuadMat 
{
public:
    QuadMat ( double const (&mat_)[4][4] );
    void leftMul ( QuadMat const& qMat2 );
    void rightMul ( QuadMat const& qMat2 );
	void transformVert ( double& x, double& y, double& z ) const;
private: 
    double mat [4][4];
};

#endif 
