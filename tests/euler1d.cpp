// -*-c++-*-
//
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MASA - Manufactured Analytical Solutions Abstraction Library
//
// Copyright (C) 2010,2011,2012,2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// $Author$
// $Id$
//
// euler1d.cpp :program that tests masa against known source term
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <tests.h>
#include <cmath>

using namespace MASA;
using namespace std;

template<typename Scalar>
Scalar anQ_p (Scalar x,Scalar p_0,Scalar p_x,Scalar a_px,Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_p = p_0 + p_x * std::cos(a_px * pi * x / L);
  return exact_p;
}
  
template<typename Scalar>
Scalar anQ_u (Scalar x,Scalar u_0,Scalar u_x,Scalar a_ux,Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_u = u_0 + u_x * std::sin(a_ux * pi * x / L);
  return exact_u;
} 
 
template<typename Scalar>
Scalar anQ_rho (Scalar x,Scalar rho_0,Scalar rho_x,Scalar a_rhox,Scalar L)
{ 
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_rho = rho_0 + rho_x * std::sin(a_rhox * pi * x / L);
  return exact_rho;
}

template<typename Scalar>
Scalar SourceQ_e ( // 12
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar p_0,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar Gamma,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_e;

  Scalar RHO;
  Scalar U;
  Scalar P;
  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L);
  P = p_0 + p_x * std::cos(a_px * pi * x / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L);
  Q_e = std::cos(a_rhox * pi * x / L) * std::pow(U, Scalar(0.3e1)) * a_rhox * pi * rho_x / L / Scalar(0.2e1) + std::cos(a_ux * pi * x / L) * P * a_ux * pi * u_x * Gamma / L / (Gamma - Scalar(0.1e1)) + Scalar(0.3e1) / Scalar(0.2e1) * std::cos(a_ux * pi * x / L) * RHO * U * U * a_ux * pi * u_x / L - std::sin(a_px * pi * x / L) * U * a_px * pi * p_x * Gamma / L / (Gamma - Scalar(0.1e1));

  return(Q_e);
}

template<typename Scalar>
Scalar SourceQ_u ( // should be 10
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_u;
  Scalar RHO;
  Scalar U;

  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L);

  Q_u = Scalar(0.2e1) * std::cos(a_ux * pi * x / L) * RHO * U * a_ux * pi * u_x / L + std::cos(a_rhox * pi * x / L) * U * U * a_rhox * pi * rho_x / L - std::sin(a_px * pi * x / L) * a_px * pi * p_x / L;

  return(Q_u);
}

template<typename Scalar>
Scalar SourceQ_rho (
  Scalar x,
  Scalar u_0,
  Scalar u_x,
  Scalar rho_0,
  Scalar rho_x,
  Scalar a_rhox,
  Scalar a_ux,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_rho;
  Scalar RHO;
  Scalar U;

  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L);

  Q_rho = std::cos(a_ux * pi * x / L) * RHO * a_ux * pi * u_x / L + std::cos(a_rhox * pi * x / L) * U * a_rhox * pi * rho_x / L;

  return(Q_rho);
}

template<typename Scalar>
int run_regression()
{
  
  Scalar threshold = 5 * numeric_limits<Scalar>::epsilon();

  //variables 
  Scalar u_0;
  Scalar u_x;
  Scalar rho_0;
  Scalar rho_x;
  Scalar p_0;
  Scalar p_x;
  Scalar a_px;
  Scalar a_rhox;
  Scalar a_ux;
  Scalar Gamma;
  Scalar L;

  // parameters
  Scalar x;

  //problem size
  int nx = 200;  // number of points
  int lx=10;     // length
  Scalar dx=Scalar(lx)/Scalar(nx);

  // solutions
  Scalar ufield,ufield2,ufield3;
  Scalar efield,efield2,efield3;
  Scalar rho,rho2,rho3;
  Scalar gradx,gradp,gradrho;

  Scalar exact_u,exact_u2,exact_u3;
  Scalar exact_p,exact_p2,exact_p3;
  Scalar exact_rho,exact_rho2,exact_rho3;

  // initalize
  masa_init<Scalar>("euler-test","euler_1d");

  // initialize the default parameters
  masa_init_param<Scalar>();

  // get defaults for comparison to source terms
  u_0 = masa_get_param<Scalar>("u_0");
  u_x = masa_get_param<Scalar>("u_x");
  rho_0 = masa_get_param<Scalar>("rho_0");
  rho_x = masa_get_param<Scalar>("rho_x");

  p_0 = masa_get_param<Scalar>("p_0");
  p_x = masa_get_param<Scalar>("p_x");

  a_px = masa_get_param<Scalar>("a_px");
  a_rhox = masa_get_param<Scalar>("a_rhox");
  a_ux = masa_get_param<Scalar>("a_ux");

  Gamma = masa_get_param<Scalar>("Gamma");
  L     = masa_get_param<Scalar>("L");

  // check that all terms have been initialized
  masa_sanity_check<Scalar>();

  // evaluate MMS (1D)
  for(int i=0;i<nx;i++)
    {
      x=i*dx;
      	
      //evalulate source terms
      ufield = masa_eval_source_rho_u  <Scalar>(x);
      efield = masa_eval_source_rho_e  <Scalar>(x);
      rho    = masa_eval_source_rho<Scalar>(x);
      
      //evaluate manufactured analytical solution terms
      exact_u = masa_eval_exact_u        <Scalar>(x);
      exact_p = masa_eval_exact_p        <Scalar>(x);
      exact_rho = masa_eval_exact_rho    <Scalar>(x);

      // eval gradient terms
      gradx   = masa_eval_grad_u  (x);
      gradp   = masa_eval_grad_p  (x);
      gradrho = masa_eval_grad_rho(x);

      // get fundamental source term solution
      ufield2   = SourceQ_u  (x,u_0,u_x,rho_0,rho_x,p_x,a_px,a_rhox,a_ux,L);
      rho2      = SourceQ_rho(x,u_0,u_x,rho_0,rho_x,a_rhox,a_ux,L);
      efield2   = SourceQ_e  (x,u_0,u_x,rho_0,rho_x,p_0,p_x,a_px,a_rhox,a_ux,Gamma,L);
  
      exact_u2   = anQ_u   (x,u_0,u_x,a_ux,L);
      exact_rho2 = anQ_rho (x,rho_0,rho_x,a_rhox,L);
      exact_p2   = anQ_p   (x,p_0,p_x,a_px,L);

      // test the result is roughly zero
      // choose between abs and rel error
#ifdef MASA_STRICT_REGRESSION

      ufield3 = std::abs(ufield-ufield2);
      efield3 = std::abs(efield-efield2);
      rho3    = std::abs(rho-rho2);

      exact_u3   = std::abs(exact_u-exact_u2);
      exact_rho3 = std::abs(exact_rho-exact_rho2);
      exact_p3   = std::abs(exact_p-exact_p2);

#else

      ufield3 = std::abs(ufield-ufield2)/std::abs(ufield2);
      efield3 = std::abs(efield-efield2)/std::abs(efield2);
      rho3    = std::abs(rho-rho2)/std::abs(rho2);

      exact_u3   = std::abs(exact_u-exact_u2)/std::abs(exact_u2);
      exact_rho3 = std::abs(exact_rho-exact_rho2)/std::abs(exact_rho2);
      exact_p3   = std::abs(exact_p-exact_p2)/std::abs(exact_p2);

#endif

      threshcheck(ufield3,threshold);
      threshcheck(efield3,threshold);
      threshcheck(rho3,threshold);
      
      threshcheck(exact_u3,threshold);
      threshcheck(exact_rho3,threshold);
      threshcheck(exact_p3,threshold);

      nancheck(gradx);
      nancheck(gradp);
      nancheck(gradrho);
      
      // adding a new error check: ensure physical results are coming out!
      if(0 > rho)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "RHO\n";
	  cout << rho << endl;
	  exit(1);
	}

      if(0 > exact_rho)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "RHO analytical\n";
	  exit(1);
	}

      if(0 > exact_p)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "Pressure is negative!\n";
	  exit(1);
	}

      if(0 > efield)
	{
	  cout << "\nMASA REGRESSION TEST FAILED: Euler-1d\n";
   	  cout << "Initial Variables are returning non-physical results!\n";
	  cout << "Energy is negative!\n";
	  exit(1);
	}
    } // done interating 

  // tests passed
  return 0;
}

int main()
{
  int err=0;

  err += run_regression<double>();
  err += run_regression<long double>();

  return err;
}
