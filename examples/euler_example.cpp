// -*-c++-*-
//
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MASA - Manufactured Analytical Solutions Abstraction Library
//
// Copyright (C) 2010,2011 The PECOS Development Team
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
// euler_example.cpp:
// this is an example of the API used for calling the 2D euler equation
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//

#include <examples.h>

using namespace MASA;

typedef double Scalar;

int main()
{
  // declarations
  Scalar tempx,tempy;

  Scalar ufield;
  Scalar vfield;
  Scalar efield;
  Scalar rho;

  Scalar exact_u;
  Scalar exact_v;
  Scalar exact_p;
  Scalar exact_rho;

  //problem size
  Scalar lx,ly;
  Scalar dx,dy;
  int nx,ny;

  // initialize
  nx = 10;  // number of points
  ny = 10;  
  lx=1;     // length
  ly=1; 

  // error handling
  int err=0;

  dx=(Scalar)lx/(Scalar)nx;
  dy=(Scalar)ly/(Scalar)ny;

  // initialize the problem
  err += masa_init<Scalar>("euler-example","euler_2d");

  // initialize the default parameters
  err += masa_init_param<Scalar>();

  // intialize the various parameters required for Euler 2D
  // call the sanity check routine 
  // (tests that all variables have been initialized)
  err += masa_sanity_check<Scalar>();

  // evaluate source terms over the domain (0<x<1, 0<y<1) 
  for(int i=0;i<nx;i++)
    for(int j=0;j<nx;j++)
      {  
	tempx=i*dx;
	tempy=j*dy;

	// evaluate source terms
	ufield = masa_eval_source_rho_u<Scalar>  (tempx,tempy);
	vfield = masa_eval_source_rho_v<Scalar>  (tempx,tempy);
	efield = masa_eval_source_rho_e<Scalar>  (tempx,tempy);
	rho    = masa_eval_source_rho  <Scalar>  (tempx,tempy);
	
	//evaluate analytical solution
	exact_u   = masa_eval_exact_u  <Scalar>   (tempx,tempy);
	exact_v   = masa_eval_exact_v  <Scalar>   (tempx,tempy);
	exact_p   = masa_eval_exact_p  <Scalar>   (tempx,tempy);
	exact_rho = masa_eval_exact_rho<Scalar>   (tempx,tempy);

	test(ufield);
	test(vfield);
	test(efield);
	test(rho);

	test(exact_u);
	test(exact_v);
	test(exact_p);
	test(exact_rho);

      }

  return err;

}// end program
