#include "quadMat.h"

QuadMat::QuadMat ( double const (&mat_)[4][4] )
{
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            this->mat[i][j] = mat_[i][j];
        }
    }
}

void QuadMat::leftMul ( QuadMat const &qMat2 )
{
    auto const& mat2 = qMat2.mat;
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            this->mat[i][j] = mat2[i][0]*this->mat[0][j]+mat2[i][1]*this->mat[1][j]+mat2[i][2]*this->mat[2][j]+mat2[i][3]*this->mat[3][j];
        }
    }
}

void QuadMat::rightMul ( QuadMat const &qMat2 )
{
    auto const& mat2 = qMat2.mat;
    for ( int i = 0; i < 4; ++i ) 
    {
        for ( int j = 0; j < 4; ++j ) 
        {
            this->mat[i][j] = this->mat[i][0]*mat2[0][j]+this->mat[i][1]*mat2[1][j]+this->mat[i][2]*mat2[2][j]+this->mat[i][3]*mat2[3][j];
        }
    }
}

void QuadMat::transformVert ( double& x, double& y, double& z ) const 
{
    double xCpy { x };
    double yCpy { y };
    x = this->mat[0][0]*x+this->mat[0][1]*y+this->mat[0][2]*z+this->mat[0][3];
    y = this->mat[1][0]*xCpy+this->mat[1][1]*y+this->mat[1][2]*z+this->mat[1][3];
    z = this->mat[2][0]*xCpy+this->mat[2][1]*yCpy+this->mat[2][2]*z+this->mat[2][3];
}
