/// \ingroup newmat
///@{

/// \file hholder.cpp
/// QR related decompositions
/// QRZ, QRZT decompositions
/// QR update and extend orthogonal functions

// Copyright (C) 1991,2,3,4: R B Davies

#define WANT_MATH
//#define WANT_STREAM

#include "include.h"

#include "newmatap.h"

#ifdef use_namespace
namespace NEWMAT {
#endif

#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,16); ++ExeCount; }
#else
#define REPORT {}
#endif


/*************************** QR decompositions ***************************/

inline Real square(Real x) { return x*x; }

void QRZT(Matrix& X, LowerTriangularMatrix& L)
{
   REPORT
	 Tracer et("QRZT(1)");
   int n = X.Ncols(); int s = X.Nrows(); L.resize(s);
   if (n == 0 || s == 0) { L = 0.0; return; }
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real sum = 0.0;
      Real* xi0=xi; k=n; while(k--) { sum += square(*xi++); }
      sum = sqrt(sum);
      if (sum == 0.0)
      {
         REPORT
         k=n; while(k--) { *xi0++ = 0.0; }
         for (int j=i; j<s; j++) L.element(j,i) = 0.0;
      }
      else
      {
         L.element(i,i) = sum;
         Real* xj0=xi0; k=n; while(k--) { *xj0++ /= sum; }
         for (int j=i+1; j<s; j++)
         {
            sum=0.0;
            xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
            xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
            L.element(j,i) = sum;
         }
      }
   }
}

void QRZT(const Matrix& X, Matrix& Y, Matrix& M)
{
   REPORT
   Tracer et("QRZT(2)");
   int n = X.Ncols(); int s = X.Nrows(); int t = Y.Nrows();
   if (Y.Ncols() != n)
      { Throw(ProgramException("Unequal row lengths",X,Y)); }
   M.resize(t,s);
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real* xj0 = Y.Store(); Real* xi0 = xi;
      for (int j=0; j<t; j++)
      {
         Real sum=0.0;
         xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
         xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
         M.element(j,i) = sum;
      }
   }
}

/*
void QRZ(Matrix& X, UpperTriangularMatrix& U)
{
	Tracer et("QRZ(1)");
	int n = X.Nrows(); int s = X.Ncols(); U.resize(s);
	Real* xi0 = X.Store(); int k;
	for (int i=0; i<s; i++)
	{
		Real sum = 0.0;
		Real* xi = xi0; k=n; while(k--) { sum += square(*xi); xi+=s; }
		sum = sqrt(sum);
		U.element(i,i) = sum;
		if (sum==0.0) Throw(SingularException(U));
		Real* xj0=xi0; k=n; while(k--) { *xj0 /= sum; xj0+=s; }
		xj0 = xi0;
		for (int j=i+1; j<s; j++)
		{
			sum=0.0;
			xi=xi0; k=n; xj0++; Real* xj=xj0;
			while(k--) { sum += *xi * *xj; xi+=s; xj+=s; }
			xi=xi0; k=n; xj=xj0;
			while(k--) { *xj -= sum * *xi; xj+=s; xi+=s; }
			U.element(i,j) = sum;
		}
		xi0++;
	}
}
*/

void QRZ(Matrix& X, UpperTriangularMatrix& U)
{
   REPORT
   Tracer et("QRZ(1)");
   int n = X.Nrows(); int s = X.Ncols(); U.resize(s); U = 0.0;
   if (n == 0 || s == 0) return;
   Real* xi0 = X.Store(); Real* u0 = U.Store(); Real* u;
   int j, k; int J = s; int i = s;
   while (i--)
   {
      Real* xj0 = xi0; Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         u = u0; Real Xi = *xi; Real* xj = xj0;
         j = J; while(j--) *u++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += s;
      }

      Real sum = sqrt(*u0); *u0 = sum; u = u0+1;
      if (sum == 0.0)
      {
         REPORT
         j = J - 1; while(j--) *u++ = 0.0;

         xj0 = xi0++; k = n;
         if (k) for (;;)
         {
            *xj0 = 0.0;
            if (!(--k)) break;
	          xj0 += s;
         }
         u0 += J--;
      }
      else
      {
         int J1 = J-1; j = J1; while(j--) *u++ /= sum;

         xj0 = xi0; xi = xi0++; k = n;
         if (k) for (;;)
         {
            u = u0+1; Real Xi = *xi; Real* xj = xj0;
            Xi /= sum; *xj++ = Xi;
            j = J1; while(j--) *xj++ -= *u++ * Xi;
            if (!(--k)) break;
	          xi += s; xj0 += s;
         }
         u0 += J--;
      }
   }
}

void QRZ(const Matrix& X, Matrix& Y, Matrix& M)
{
   REPORT
   Tracer et("QRZ(2)");
   int n = X.Nrows(); int s = X.Ncols(); int t = Y.Ncols();
   if (Y.Nrows() != n)
      { Throw(ProgramException("Unequal column lengths",X,Y)); }
   M.resize(s,t); M = 0;Real* m0 = M.Store(); Real* m;
   Real* xi0 = X.Store();
   int j, k; int i = s;
   while (i--)
   {
      Real* xj0 = Y.Store(); Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         m = m0; Real Xi = *xi; Real* xj = xj0;
         j = t; while(j--) *m++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += t;
      }

      xj0 = Y.Store(); xi = xi0++; k = n;
      if (k) for (;;)
      {
         m = m0; Real Xi = *xi; Real* xj = xj0;
         j = t; while(j--) *xj++ -= *m++ * Xi;
         if (!(--k)) break;
         xi += s; xj0 += t;
      }
      m0 += t;
   }
}

/*

void QRZ(const Matrix& X, Matrix& Y, Matrix& M)
{
	Tracer et("QRZ(2)");
	int n = X.Nrows(); int s = X.Ncols(); int t = Y.Ncols();
	if (Y.Nrows() != n)
	{ Throw(ProgramException("Unequal column lengths",X,Y)); }
	M.resize(s,t);
	Real* xi0 = X.Store(); int k;
	for (int i=0; i<s; i++)
	{
		Real* xj0 = Y.Store();
		for (int j=0; j<t; j++)
		{
			Real sum=0.0;
			Real* xi=xi0; Real* xj=xj0; k=n;
			while(k--) { sum += *xi * *xj; xi+=s; xj+=t; }
			xi=xi0; k=n; xj=xj0++;
			while(k--) { *xj -= sum * *xi; xj+=t; xi+=s; }
			M.element(i,j) = sum;
		}
		xi0++;
	}
}
*/

void updateQRZT(Matrix& X, LowerTriangularMatrix& L)
{
   REPORT
	 Tracer et("updateQRZT");
   int n = X.Ncols(); int s = X.Nrows();
   if (s != L.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,L));
   if (n == 0 || s == 0) return;
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real r = L.element(i,i);
      Real sum = 0.0;
      Real* xi0=xi; k=n; while(k--) { sum += square(*xi++); }
      sum = sqrt(sum + square(r));
      if (sum == 0.0)
      {
         REPORT
         k=n; while(k--) { *xi0++ = 0.0; }
         for (int j=i; j<s; j++) L.element(j,i) = 0.0;
      }
      else
      {
         Real frs = fabs(r) + sum;
         Real a0 = sqrt(frs / sum); Real alpha = a0 / frs;
         if (r <= 0) { REPORT L.element(i,i) = sum; alpha = -alpha; }
         else { REPORT L.element(i,i) = -sum; }
         Real* xj0=xi0; k=n; while(k--) { *xj0++ *= alpha; }
         for (int j=i+1; j<s; j++)
         {
            sum = 0.0;
            xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
            sum += a0 * L.element(j,i);
            xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
            L.element(j,i) -= sum * a0;
         }
      }
   }
}

void updateQRZ(Matrix& X, UpperTriangularMatrix& U)
{
   REPORT
   Tracer et("updateQRZ");
   int n = X.Nrows(); int s = X.Ncols();
   if (s != U.Ncols())
      Throw(ProgramException("Incompatible dimensions",X,U));
   if (n == 0 || s == 0) return;
   Real* xi0 = X.Store(); Real* u0 = U.Store(); Real* u;
   RowVector V(s); Real* v0 = V.Store(); Real* v; V = 0.0;
   int j, k; int J = s; int i = s;
   while (i--)
   {
      Real* xj0 = xi0; Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         v = v0; Real Xi = *xi; Real* xj = xj0;
         j = J; while(j--) *v++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += s;
      }

      Real r = *u0;
      Real sum = sqrt(*v0 + square(r));

      if (sum == 0.0)
      {
         REPORT
         u = u0; v = v0;
         j = J; while(j--) { *u++ = 0.0; *v++ = 0.0; }
         xj0 = xi0++; k = n;
         if (k) for (;;)
         {
            *xj0 = 0.0;
            if (!(--k)) break;
	          xj0 += s;
         }
         u0 += J--;
      }
      else
      {
         Real frs = fabs(r) + sum;
         Real a0 = sqrt(frs / sum); Real alpha = a0 / frs;
         if (r <= 0) { REPORT alpha = -alpha; *u0 = sum; }
         else { REPORT *u0 = -sum; }

         j = J - 1; v = v0 + 1; u = u0 + 1;
         while (j--)
            { *v = a0 * *u + alpha * *v; *u -= a0 * *v; ++v; ++u; }

         xj0 = xi0; xi = xi0++; k = n;
         if (k) for (;;)
         {
            v = v0 + 1; Real Xi = *xi; Real* xj = xj0;
            Xi *= alpha; *xj++ = Xi;
            j = J - 1; while(j--) *xj++ -= *v++ * Xi;
            if (!(--k)) break;
	          xi += s; xj0 += s;
         }

         j = J; v = v0;
         while (j--) *v++ = 0.0;

         u0 += J--;
      }
   }
}

// Following previous transformation,
// now apply the same orthogonal transformation to (MX & MU)
// Need the X Matrix but not the U.
// Not optimised for accessing consecutive memory

void updateQRZ(const Matrix& X, Matrix& MX, Matrix& MU)
{
   REPORT
   Tracer et("updateQRZ(2)");
   int s = X.Ncols(); int n = X.Nrows();
   if (n != MX.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,MX));
   if (s != MU.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,MU));
   int t = MX.Ncols();
   if (t != MU.Ncols())
      Throw(ProgramException("Incompatible dimensions",MX,MU));

   if (s == 0) return;

   const Real* xi0 = X.data(); Real* mx = MX.data(); Real* muj = MU.data();
   for (int i=1; i<=s; ++i)
   {
		  Real sum = 0.0;
			{
         const Real* xi=xi0; int k=n;
			   while(k--) { sum += square(*xi); xi+= s;}
			}
      Real a0 = sqrt(2.0 - sum); Real* mxj0 = mx;
      for (int j=1; j<=t; ++j)
      {
         // [Derek Huang] use outer sum instead of shadowing
         sum = 0.0;
         const Real* xi=xi0; Real* mxj=mxj0; int k=n;
         while(--k) { sum += *xi * *mxj; xi += s; mxj += t; }
         sum += *xi * *mxj;    // last line of loop
         sum += a0 * *muj;
         xi=xi0; mxj=mxj0; k=n;
         while(--k) { *mxj -= sum * *xi; xi += s; mxj += t; }
         *mxj -= sum * *xi;    // last line of loop
         *muj -= sum * a0; ++mxj0; ++muj;
      }
			++xi0;
   }
}



// same thing as updateQRZ(Matrix& X, UpperTriangularMatrix& U)
// except that X is upper triangular
// contents of X are destroyed - results are in U
// assume we can access efficiently by columns
// e.g. X and U will fit in cache memory

void updateQRZ(UpperTriangularMatrix& X, UpperTriangularMatrix& U)
{
   REPORT
   Tracer et("updateQRZ(3)");
   int s = X.Ncols();
   if (s != U.Ncols())
      Throw(ProgramException("Incompatible dimensions",X,U));
   if (s == 0) return;
   Real* xi0 = X.data(); Real* u = U.data();
   for (int i=1; i<=s; ++i)
   {
		  Real r = *u; Real sum = 0.0;
			{
         Real* xi=xi0; int k=i; int l=s;
			   while(k--) { sum += square(*xi); xi+= --l;}
			}
      sum = sqrt(sum + square(r));
      if (sum == 0.0) { REPORT X.column(i) = 0.0; *u = 0.0; }
      else
      {
         Real frs = fabs(r) + sum;
         Real a0 = sqrt(frs / sum); Real alpha = a0 / frs;
         if (r <= 0) { REPORT *u = sum; alpha = -alpha; }
         else { REPORT *u = -sum; }
         {
            Real* xj0=xi0; int k=i; int l=s;
            while(k--) { *xj0 *= alpha; --l; xj0 += l;}
         }
         Real* xj0=xi0; Real* uj=u;
         for (int j=i+1; j<=s; ++j)
         {
            // [Derek Huang] use outer sum instead of shadowing
            sum = 0.0; ++xj0; ++uj;
            Real* xi=xi0; Real* xj=xj0; int k=i; int l=s;
            while(k--) { sum += *xi * *xj; --l; xi += l; xj += l; }
            sum += a0 * *uj;
            xi=xi0; xj=xj0; k=i; l=s;
            while(k--) { *xj -= sum * *xi; --l; xi += l; xj += l; }
            *uj -= sum * a0;
         }
      }
			++xi0; u += s-i+1;
   }
}

// Following previous transformation,
// now apply the same orthogonal transformation to (MX & MU)
// Need the X UpperTriangularMatrix but not the U.

void updateQRZ(const UpperTriangularMatrix& X, Matrix& MX, Matrix& MU)
{
   REPORT
   Tracer et("updateQRZ(4)");
   int s = X.Ncols();
   if (s != MX.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,MX));
   if (s != MU.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,MU));
   int t = MX.Ncols();
   if (t != MU.Ncols())
      Throw(ProgramException("Incompatible dimensions",MX,MU));
   if (s == 0) return;

   const Real* xi0 = X.data(); Real* mx = MX.data(); Real* muj = MU.data();
   for (int i=1; i<=s; ++i)
   {
		  Real sum = 0.0;
			{
         const Real* xi=xi0; int k=i; int l=s;
			   while(k--) { sum += square(*xi); xi+= --l;}
			}
      Real a0 = sqrt(2.0 - sum); Real* mxj0 = mx;
      for (int j=1; j<=t; ++j)
      {
         // [Derek Huang] use outer sum instead of shadowing
         sum = 0.0;
         const Real* xi=xi0; Real* mxj=mxj0; int k=i; int l=s;
         while(--k) { sum += *xi * *mxj; --l; xi += l; mxj += t; }
         sum += *xi * *mxj;    // last line of loop
         sum += a0 * *muj;
         xi=xi0; mxj=mxj0; k=i; l=s;
         while(--k) { *mxj -= sum * *xi; --l; xi += l; mxj += t; }
         *mxj -= sum * *xi;    // last line of loop
         *muj -= sum * a0; ++mxj0; ++muj;
      }
			++xi0;
   }
}





// Matrix A's first n columns are orthonormal
// so A.Columns(1,n).t() * A.Columns(1,n) is the identity matrix.
// Fill out the remaining columns of A to make them orthonormal
// so A.t() * A is the identity matrix
void extend_orthonormal(Matrix& A, int n)
{
   REPORT
   Tracer et("extend_orthonormal");
   int nr = A.nrows(); int nc = A.ncols();
   if (nc > nr) Throw(IncompatibleDimensionsException(A));
   if (n > nc) Throw(IncompatibleDimensionsException(A));
   ColumnVector SSR;
   { Matrix A1 = A.Columns(1,n); SSR = A1.sum_square_rows(); }
   for (int i = n; i < nc; ++i)
   {
      // pick row with smallest SSQ
      int k; SSR.minimum1(k);
      // orthogonalise column with 1 at element k, 0 elsewhere
      // next line is rather inefficient
      ColumnVector X = - A.Columns(1, i) * A.SubMatrix(k, k, 1, i).t();
      X(k) += 1.0;
      // normalise
      X /= sqrt(X.SumSquare());
      // update row sums of squares
      for (k = 1; k <= nr; ++k) SSR(k) += square(X(k));
      // load new column into matrix
      A.Column(i+1) = X;
   }
}





#ifdef use_namespace
}
#endif


///@}
