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
// ns3d.cpp: program that tests navier-stokes-3d against known source term
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <tests.h>
#include <cmath>

using namespace std;
using namespace MASA;

template<typename Scalar>
Scalar anQ_p (Scalar x,Scalar y,Scalar z,Scalar p_0,Scalar p_x,Scalar p_y,Scalar p_z,Scalar a_px,Scalar a_py,Scalar a_pz,Scalar L)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar exact_p = p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L);
  return exact_p;
}

template<typename Scalar>
Scalar anQ_u (Scalar x,Scalar y,Scalar z,Scalar u_0,Scalar u_x,Scalar u_y,Scalar u_z,Scalar a_ux,Scalar a_uy,Scalar a_uz,Scalar L)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar exact_u = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L);  
  return exact_u;
} 

template<typename Scalar> 
Scalar anQ_v (Scalar x,Scalar y,Scalar z,Scalar v_0,Scalar v_x,Scalar v_y,Scalar v_z,Scalar a_vx,Scalar a_vy,Scalar a_vz,Scalar L)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar exact_v = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L);
  return exact_v;
}

template<typename Scalar>
Scalar anQ_w (Scalar x,Scalar y,Scalar z,Scalar w_0,Scalar w_x,Scalar w_y,Scalar w_z,Scalar a_wx,Scalar a_wy,Scalar a_wz,Scalar L)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar exact_w = w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L);
  return exact_w;
}

template<typename Scalar>
Scalar anQ_rho (Scalar x,Scalar y,Scalar z,Scalar rho_0,Scalar rho_x,Scalar rho_y,Scalar rho_z,Scalar a_rhox,Scalar a_rhoy,Scalar a_rhoz,Scalar L)
{ 
  const Scalar pi = std::acos(Scalar(-1));
  Scalar exact_rho = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L);
  return exact_rho;
}

template<typename Scalar>
Scalar SourceQ_e (
  Scalar x,
  Scalar y,
  Scalar z,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar u_z,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar v_z,
  Scalar w_0,
  Scalar w_x,
  Scalar w_y,
  Scalar w_z,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar rho_z,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar p_z,
  Scalar a_px,
  Scalar a_py,
  Scalar a_pz,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_rhoz,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_uz,
  Scalar a_vx,
  Scalar a_vy,
  Scalar a_vz,
  Scalar a_wx,
  Scalar a_wy,
  Scalar a_wz,
  Scalar mu,
  Scalar Gamma,
  Scalar L,
  Scalar R,
  Scalar k)
  {
    const Scalar pi = std::acos(Scalar(-1));    
    // onkars generated routines
    Scalar Q_e = std::cos(a_rhox * pi * x / L) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * (std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1)) + std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1))) * rho_x * a_rhox * pi / L / Scalar(0.2e1) - std::sin(a_rhoy * pi * y / L) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * (std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1)) + std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1))) * rho_y * a_rhoy * pi / L / Scalar(0.2e1) + std::cos(a_rhoz * pi * z / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1)) + std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1))) * rho_z * a_rhoz * pi / L / Scalar(0.2e1) + ((std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1)) + Scalar(0.3e1) * std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1))) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) / L / Scalar(0.2e1) + Gamma * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) / L / (Gamma - Scalar(0.1e1))) * u_x * std::cos(a_ux * pi * x / L) * a_ux * pi + ((std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1)) + std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + Scalar(0.3e1) * std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1))) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) / L / Scalar(0.2e1) + Gamma * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) / L / (Gamma - Scalar(0.1e1))) * v_y * std::cos(a_vy * pi * y / L) * a_vy * pi + (-(Scalar(0.3e1) * std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) + std::pow(v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0, Scalar(0.2e1)) + std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1))) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) / L / Scalar(0.2e1) - Gamma * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) / L / (Gamma - Scalar(0.1e1))) * w_z * std::sin(a_wz * pi * z / L) * a_wz * pi + Scalar(0.4e1) / Scalar(0.3e1) * (-std::pow(cos(a_ux * pi * x / L), Scalar(0.2e1)) * u_x + std::sin(a_ux * pi * x / L) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L))) * mu * u_x * a_ux * a_ux * pi * pi * std::pow(L, Scalar(-0.2e1)) + (-std::pow(sin(a_uy * pi * y / L), Scalar(0.2e1)) * u_y + std::cos(a_uy * pi * y / L) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L))) * mu * u_y * a_uy * a_uy * pi * pi * std::pow(L, Scalar(-0.2e1)) + (-std::pow(sin(a_uz * pi * z / L), Scalar(0.2e1)) * u_z + std::cos(a_uz * pi * z / L) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L))) * mu * u_z * a_uz * a_uz * pi * pi * std::pow(L, Scalar(-0.2e1)) - (std::pow(sin(a_vx * pi * x / L), Scalar(0.2e1)) * v_x - std::cos(a_vx * pi * x / L) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0)) * mu * v_x * a_vx * a_vx * pi * pi * std::pow(L, Scalar(-0.2e1)) - Scalar(0.4e1) / Scalar(0.3e1) * (std::pow(cos(a_vy * pi * y / L), Scalar(0.2e1)) * v_y - std::sin(a_vy * pi * y / L) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0)) * mu * v_y * a_vy * a_vy * pi * pi * std::pow(L, Scalar(-0.2e1)) - (std::pow(cos(a_vz * pi * z / L), Scalar(0.2e1)) * v_z - std::sin(a_vz * pi * z / L) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0)) * mu * v_z * a_vz * a_vz * pi * pi * std::pow(L, Scalar(-0.2e1)) + (-std::pow(cos(a_wx * pi * x / L), Scalar(0.2e1)) * w_x + std::sin(a_wx * pi * x / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L))) * mu * w_x * a_wx * a_wx * pi * pi * std::pow(L, Scalar(-0.2e1)) + (-std::pow(cos(a_wy * pi * y / L), Scalar(0.2e1)) * w_y + std::sin(a_wy * pi * y / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L))) * mu * w_y * a_wy * a_wy * pi * pi * std::pow(L, Scalar(-0.2e1)) + Scalar(0.4e1) / Scalar(0.3e1) * (-std::pow(sin(a_wz * pi * z / L), Scalar(0.2e1)) * w_z + std::cos(a_wz * pi * z / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L))) * mu * w_z * a_wz * a_wz * pi * pi * std::pow(L, Scalar(-0.2e1)) + std::sin(a_py * pi * y / L) * k * p_y * a_py * a_py * pi * pi * std::pow(L, Scalar(-0.2e1)) / R / (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) - Scalar(0.2e1) * std::cos(a_rhox * pi * x / L) * rho_x * std::sin(a_px * pi * x / L) * k * p_x * a_px * a_rhox * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.2e1)) - Scalar(0.2e1) * std::sin(a_rhoy * pi * y / L) * rho_y * std::cos(a_py * pi * y / L) * k * p_y * a_py * a_rhoy * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.2e1)) - (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * u_y * std::sin(a_uy * pi * y / L) * a_uy * pi / L + (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * v_z * std::cos(a_vz * pi * z / L) * a_vz * pi / L + std::cos(a_px * pi * x / L) * k * p_x * a_px * a_px * pi * pi * std::pow(L, Scalar(-0.2e1)) / R / (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) + std::cos(a_pz * pi * z / L) * k * p_z * a_pz * a_pz * pi * pi * std::pow(L, Scalar(-0.2e1)) / R / (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) + (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * w_x * std::cos(a_wx * pi * x / L) * a_wx * pi / L - (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * v_x * std::sin(a_vx * pi * x / L) * a_vx * pi / L - (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * u_z * std::sin(a_uz * pi * z / L) * a_uz * pi / L + (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * w_y * std::cos(a_wy * pi * y / L) * a_wy * pi / L - Scalar(0.2e1) * std::cos(a_rhoz * pi * z / L) * rho_z * std::sin(a_pz * pi * z / L) * k * p_z * a_pz * a_rhoz * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.2e1)) - (Scalar(0.2e1) * std::pow(cos(a_rhox * pi * x / L), Scalar(0.2e1)) * rho_x + std::sin(a_rhox * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L))) * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) * k * rho_x * a_rhox * a_rhox * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.3e1)) - (Scalar(0.2e1) * std::pow(sin(a_rhoy * pi * y / L), Scalar(0.2e1)) * rho_y + std::cos(a_rhoy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L))) * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) * k * rho_y * a_rhoy * a_rhoy * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.3e1)) - (Scalar(0.2e1) * std::pow(cos(a_rhoz * pi * z / L), Scalar(0.2e1)) * rho_z + std::sin(a_rhoz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L))) * (p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L) + p_z * std::cos(a_pz * pi * z / L)) * k * rho_z * a_rhoz * a_rhoz * pi * pi * std::pow(L, Scalar(-0.2e1)) / R * std::pow(rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L), Scalar(-0.3e1)) + Scalar(0.4e1) / Scalar(0.3e1) * mu * u_x * v_y * std::cos(a_ux * pi * x / L) * std::cos(a_vy * pi * y / L) * a_ux * a_vy * pi * pi * std::pow(L, Scalar(-0.2e1)) - Scalar(0.4e1) / Scalar(0.3e1) * mu * u_x * w_z * std::cos(a_ux * pi * x / L) * std::sin(a_wz * pi * z / L) * a_ux * a_wz * pi * pi * std::pow(L, Scalar(-0.2e1)) - Scalar(0.2e1) * mu * u_y * v_x * std::sin(a_uy * pi * y / L) * std::sin(a_vx * pi * x / L) * a_uy * a_vx * pi * pi * std::pow(L, Scalar(-0.2e1)) + Scalar(0.2e1) * mu * u_z * w_x * std::cos(a_wx * pi * x / L) * std::sin(a_uz * pi * z / L) * a_uz * a_wx * pi * pi * std::pow(L, Scalar(-0.2e1)) - Scalar(0.4e1) / Scalar(0.3e1) * mu * v_y * w_z * std::cos(a_vy * pi * y / L) * std::sin(a_wz * pi * z / L) * a_vy * a_wz * pi * pi * std::pow(L, Scalar(-0.2e1)) - Scalar(0.2e1) * mu * v_z * w_y * std::cos(a_vz * pi * z / L) * std::cos(a_wy * pi * y / L) * a_vz * a_wy * pi * pi * std::pow(L, Scalar(-0.2e1)) - Gamma * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * std::sin(a_px * pi * x / L) * p_x * a_px * pi / L / (Gamma - Scalar(0.1e1)) + Gamma * (v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L) + v_0) * std::cos(a_py * pi * y / L) * p_y * a_py * pi / L / (Gamma - Scalar(0.1e1)) - Gamma * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * std::sin(a_pz * pi * z / L) * p_z * a_pz * pi / L / (Gamma - Scalar(0.1e1));  
    return(Q_e);
}

template<typename Scalar>
Scalar SourceQ_u (
  Scalar x,
  Scalar y,
  Scalar z,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar u_z,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar v_z,
  Scalar w_0,
  Scalar w_x,
  Scalar w_y,
  Scalar w_z,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar rho_z,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar p_z,
  Scalar a_px,
  Scalar a_py,
  Scalar a_pz,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_rhoz,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_uz,
  Scalar a_vx,
  Scalar a_vy,
  Scalar a_vz,
  Scalar a_wx,
  Scalar a_wy,
  Scalar a_wz,
  Scalar mu,
  Scalar L,
  Scalar R,
  Scalar k)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar Q_u;
  Q_u = Scalar(0.4e1) / Scalar(0.3e1) * mu * u_x * std::sin(a_ux * pi * x / L) * a_ux * a_ux * pi * pi * std::pow(L, Scalar(-0.2e1)) + mu * u_y * std::cos(a_uy * pi * y / L) * a_uy * a_uy * pi * pi * std::pow(L, Scalar(-0.2e1)) + mu * u_z * std::cos(a_uz * pi * z / L) * a_uz * a_uz * pi * pi * std::pow(L, Scalar(-0.2e1)) - p_x * std::sin(a_px * pi * x / L) * a_px * pi / L + rho_x * std::cos(a_rhox * pi * x / L) * std::pow(u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L), Scalar(0.2e1)) * a_rhox * pi / L - rho_y * std::sin(a_rhoy * pi * y / L) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_rhoy * pi / L + rho_z * std::cos(a_rhoz * pi * z / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_rhoz * pi / L + Scalar(0.2e1) * u_x * std::cos(a_ux * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_ux * pi / L - u_y * std::sin(a_uy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_uy * pi / L - u_z * std::sin(a_uz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_uz * pi / L + v_y * std::cos(a_vy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_vy * pi / L - w_z * std::sin(a_wz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_wz * pi / L;
  return(Q_u);
}

template<typename Scalar>
Scalar SourceQ_v (
  Scalar x,
  Scalar y,
  Scalar z,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar u_z,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar v_z,
  Scalar w_0,
  Scalar w_x,
  Scalar w_y,
  Scalar w_z,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar rho_z,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar p_z,
  Scalar a_px,
  Scalar a_py,
  Scalar a_pz,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_rhoz,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_uz,
  Scalar a_vx,
  Scalar a_vy,
  Scalar a_vz,
  Scalar a_wx,
  Scalar a_wy,
  Scalar a_wz,
  Scalar mu,
  Scalar L,
  Scalar R,
  Scalar k)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar Q_v;
  Q_v = mu * v_x * std::cos(a_vx * pi * x / L) * a_vx * a_vx * pi * pi * std::pow(L, Scalar(-0.2e1)) + Scalar(0.4e1) / Scalar(0.3e1) * mu * v_y * std::sin(a_vy * pi * y / L) * a_vy * a_vy * pi * pi * std::pow(L, Scalar(-0.2e1)) + mu * v_z * std::sin(a_vz * pi * z / L) * a_vz * a_vz * pi * pi * std::pow(L, Scalar(-0.2e1)) + p_y * std::cos(a_py * pi * y / L) * a_py * pi / L + rho_x * std::cos(a_rhox * pi * x / L) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_rhox * pi / L - rho_y * std::sin(a_rhoy * pi * y / L) * std::pow(v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L), Scalar(0.2e1)) * a_rhoy * pi / L + rho_z * std::cos(a_rhoz * pi * z / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_rhoz * pi / L + u_x * std::cos(a_ux * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_ux * pi / L - v_x * std::sin(a_vx * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_vx * pi / L + Scalar(0.2e1) * v_y * std::cos(a_vy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_vy * pi / L + v_z * std::cos(a_vz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_vz * pi / L - w_z * std::sin(a_wz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_wz * pi / L;
  return(Q_v);
}

template<typename Scalar>
Scalar SourceQ_w (
  Scalar x,
  Scalar y,
  Scalar z,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar u_z,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar v_z,
  Scalar w_0,
  Scalar w_x,
  Scalar w_y,
  Scalar w_z,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar rho_z,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar p_z,
  Scalar a_px,
  Scalar a_py,
  Scalar a_pz,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_rhoz,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_uz,
  Scalar a_vx,
  Scalar a_vy,
  Scalar a_vz,
  Scalar a_wx,
  Scalar a_wy,
  Scalar a_wz,
  Scalar mu,
  Scalar L,
  Scalar R,
  Scalar k)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar Q_w;
  Q_w = mu * w_x * std::sin(a_wx * pi * x / L) * a_wx * a_wx * pi * pi * std::pow(L, Scalar(-0.2e1)) + mu * w_y * std::sin(a_wy * pi * y / L) * a_wy * a_wy * pi * pi * std::pow(L, Scalar(-0.2e1)) + Scalar(0.4e1) / Scalar(0.3e1) * mu * w_z * std::cos(a_wz * pi * z / L) * a_wz * a_wz * pi * pi * std::pow(L, Scalar(-0.2e1)) - p_z * std::sin(a_pz * pi * z / L) * a_pz * pi / L + rho_x * std::cos(a_rhox * pi * x / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_rhox * pi / L - rho_y * std::sin(a_rhoy * pi * y / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_rhoy * pi / L + rho_z * std::cos(a_rhoz * pi * z / L) * std::pow(w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L), Scalar(0.2e1)) * a_rhoz * pi / L + u_x * std::cos(a_ux * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_ux * pi / L + v_y * std::cos(a_vy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_vy * pi / L + w_x * std::cos(a_wx * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_wx * pi / L + w_y * std::cos(a_wy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_wy * pi / L - Scalar(0.2e1) * w_z * std::sin(a_wz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_wz * pi / L;
  return(Q_w);
}

template<typename Scalar>
Scalar SourceQ_rho(
  Scalar x,
  Scalar y,
  Scalar z,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar u_z,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar v_z,
  Scalar w_0,
  Scalar w_x,
  Scalar w_y,
  Scalar w_z,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar rho_z,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar p_z,
  Scalar a_px,
  Scalar a_py,
  Scalar a_pz,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_rhoz,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_uz,
  Scalar a_vx,
  Scalar a_vy,
  Scalar a_vz,
  Scalar a_wx,
  Scalar a_wy,
  Scalar a_wz,
  Scalar mu,
  Scalar L,
  Scalar R,
  Scalar k)
{
  const Scalar pi = std::acos(Scalar(-1));
  Scalar Q_rho;
  Q_rho = rho_x * std::cos(a_rhox * pi * x / L) * (u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L) + u_z * std::cos(a_uz * pi * z / L)) * a_rhox * pi / L - rho_y * std::sin(a_rhoy * pi * y / L) * (v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L) + v_z * std::sin(a_vz * pi * z / L)) * a_rhoy * pi / L + rho_z * std::cos(a_rhoz * pi * z / L) * (w_0 + w_x * std::sin(a_wx * pi * x / L) + w_y * std::sin(a_wy * pi * y / L) + w_z * std::cos(a_wz * pi * z / L)) * a_rhoz * pi / L + u_x * std::cos(a_ux * pi * x / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * a_ux * pi / L + v_y * std::cos(a_vy * pi * y / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * a_vy * pi / L - w_z * std::sin(a_wz * pi * z / L) * (rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L) + rho_z * std::sin(a_rhoz * pi * z / L)) * a_wz * pi / L;
  return(Q_rho);
}

template<typename Scalar>
int run_regression()
{

  const Scalar threshold = 5 * numeric_limits<Scalar>::epsilon();

  //variables
  Scalar u_0;
  Scalar u_x;
  Scalar u_y;
  Scalar u_z;
  Scalar v_0;
  Scalar v_x;
  Scalar v_y;
  Scalar v_z;
  Scalar w_0;
  Scalar w_x;
  Scalar w_y;
  Scalar w_z;
  Scalar rho_0;
  Scalar rho_x;
  Scalar rho_y;
  Scalar rho_z;
  Scalar p_0;
  Scalar p_x;
  Scalar p_y;
  Scalar p_z;
  Scalar a_px;
  Scalar a_py;
  Scalar a_pz;
  Scalar a_rhox;
  Scalar a_rhoy;
  Scalar a_rhoz;
  Scalar a_ux;
  Scalar a_uy;
  Scalar a_uz;
  Scalar a_vx;
  Scalar a_vy;
  Scalar a_vz;
  Scalar a_wx;
  Scalar a_wy;
  Scalar a_wz;
  Scalar mu;
  Scalar Gamma;
  Scalar L;    

  Scalar R;
  Scalar K;    

  // parameters
  Scalar x;
  Scalar y;
  Scalar z;

  // solutions
  Scalar ufield,ufield2,ufield3;
  Scalar vfield,vfield2,vfield3;
  Scalar wfield,wfield2,wfield3;
  Scalar efield,efield2,efield3;
  Scalar rho,rho2,rho3;
  Scalar gradx,grady,gradz;

  Scalar exact_u,exact_u2,exact_u3;
  Scalar exact_v,exact_v2,exact_v3;
  Scalar exact_w,exact_w2,exact_w3;
  Scalar exact_p,exact_p2,exact_p3;
  Scalar exact_rho,exact_rho2,exact_rho3;

  // initalize
  int nx = 20;             // number of points
  int ny = 10;
  int nz = 30;
  int lx=1;                // length
  int ly=2; 
  int lz=3;     
  Scalar dx=Scalar(lx)/Scalar(nx);
  Scalar dy=Scalar(ly)/Scalar(ny);
  Scalar dz=Scalar(lz)/Scalar(nz);

  masa_init<Scalar>("navier-stokes-test","navierstokes_3d_compressible");

  // set params -- do not call default values for this problem!
  masa_set_param<Scalar>("R",1.01);
  //masa_set_param<Scalar>("k",1.38);
  masa_set_param<Scalar>("u_0",2.27);
  masa_set_param<Scalar>("u_x",6.00);
  masa_set_param<Scalar>("u_y",1.35);
  masa_set_param<Scalar>("u_z",5.34);
  masa_set_param<Scalar>("v_0",3.46);
  masa_set_param<Scalar>("v_x",6.13);
  masa_set_param<Scalar>("v_y",.54);
  masa_set_param<Scalar>("v_z",.30);
  masa_set_param<Scalar>("w_0",.411);
  masa_set_param<Scalar>("w_x",3.14);
  masa_set_param<Scalar>("w_y",5.68);
  masa_set_param<Scalar>("w_z",6.51);
  masa_set_param<Scalar>("rho_0",1.63);
  masa_set_param<Scalar>("rho_x",4.7);
  masa_set_param<Scalar>("rho_y",0.85);
  masa_set_param<Scalar>("rho_z",12.15);
  masa_set_param<Scalar>("p_0",1.135);
  masa_set_param<Scalar>("p_x",.73);
  masa_set_param<Scalar>("p_y",4.9);
  masa_set_param<Scalar>("p_z",1.8);
  masa_set_param<Scalar>("a_px",8.8);
  masa_set_param<Scalar>("a_py",0.1);
  masa_set_param<Scalar>("a_pz",38.5);
  masa_set_param<Scalar>("a_rhox",.82);
  masa_set_param<Scalar>("a_rhoy",.41);
  masa_set_param<Scalar>("a_rhoz",.44);
  masa_set_param<Scalar>("a_ux",.46);
  masa_set_param<Scalar>("a_uy",.425);
  masa_set_param<Scalar>("a_uz",.42);
  masa_set_param<Scalar>("a_vx",.52);
  masa_set_param<Scalar>("a_vy",.23);
  masa_set_param<Scalar>("a_vz",1.2);
  masa_set_param<Scalar>("a_wx",1.05);
  masa_set_param<Scalar>("a_wy",1.8);
  masa_set_param<Scalar>("a_wz",13.6);
  masa_set_param<Scalar>("Gamma",2.5);
  masa_set_param<Scalar>("mu",2.01);
  masa_set_param<Scalar>("L",1.0);

  // now set reference values for maple compare
  u_0 = masa_get_param<Scalar>("u_0");
  u_x = masa_get_param<Scalar>("u_x");
  u_y = masa_get_param<Scalar>("u_y");
  u_z = masa_get_param<Scalar>("u_z");

  v_0 = masa_get_param<Scalar>("v_0");
  v_x = masa_get_param<Scalar>("v_x");
  v_y = masa_get_param<Scalar>("v_y");
  v_z = masa_get_param<Scalar>("v_z");

  w_0 = masa_get_param<Scalar>("w_0");
  w_x = masa_get_param<Scalar>("w_x");
  w_y = masa_get_param<Scalar>("w_y");
  w_z = masa_get_param<Scalar>("w_z");

  rho_0 = masa_get_param<Scalar>("rho_0");
  rho_x = masa_get_param<Scalar>("rho_x");
  rho_y = masa_get_param<Scalar>("rho_y");
  rho_z = masa_get_param<Scalar>("rho_z");

  p_0 = masa_get_param<Scalar>("p_0");
  p_x = masa_get_param<Scalar>("p_x");
  p_y = masa_get_param<Scalar>("p_y");
  p_z = masa_get_param<Scalar>("p_z");

  a_px = masa_get_param<Scalar>("a_px");
  a_py = masa_get_param<Scalar>("a_py");
  a_pz = masa_get_param<Scalar>("a_pz");

  a_rhox = masa_get_param<Scalar>("a_rhox");
  a_rhoy = masa_get_param<Scalar>("a_rhoy");
  a_rhoz = masa_get_param<Scalar>("a_rhoz");

  a_ux = masa_get_param<Scalar>("a_ux");
  a_uy = masa_get_param<Scalar>("a_uy");
  a_uz = masa_get_param<Scalar>("a_uz");

  a_vx = masa_get_param<Scalar>("a_vx");
  a_vy = masa_get_param<Scalar>("a_vy");
  a_vz = masa_get_param<Scalar>("a_vz");

  a_wx = masa_get_param<Scalar>("a_wx");
  a_wy = masa_get_param<Scalar>("a_wy");
  a_wz = masa_get_param<Scalar>("a_wz");

  Gamma = masa_get_param<Scalar>("Gamma");
  mu    = masa_get_param<Scalar>("mu");
  L     = masa_get_param<Scalar>("L");

  R = masa_get_param<Scalar>("R");
  K = masa_get_param<Scalar>("k");

  // check all vars are initialized
  int err = masa_sanity_check<Scalar>();
  if(err != 0)
    {
      cout << "MASA :: Sanity Check Failed!\n";
      exit(1);
    }
  
  // evaluate source terms (3D)
  for(int i=0;i<nx;i++)
    for(int j=0;j<ny;j++)    
      for(int k=0;k<nz;k++)
	{
	  x=i*dx;
	  y=j*dy;
	  z=k*dz;

	  // evalulate source terms
	  ufield = masa_eval_source_rho_u  <Scalar>(x,y,z);
	  vfield = masa_eval_source_rho_v  <Scalar>(x,y,z);
	  wfield = masa_eval_source_rho_w  <Scalar>(x,y,z);
	  efield = masa_eval_source_rho_e  <Scalar>(x,y,z);
	  rho    = masa_eval_source_rho<Scalar>(x,y,z);
	  
	  // evaluate analytical terms
	  exact_u = masa_eval_exact_u        <Scalar>(x,y,z);
	  exact_v = masa_eval_exact_v        <Scalar>(x,y,z);
	  exact_w = masa_eval_exact_w        <Scalar>(x,y,z);
	  exact_p = masa_eval_exact_p        <Scalar>(x,y,z);
	  exact_rho = masa_eval_exact_rho    <Scalar>(x,y,z);

	  // eval gradient terms
	  gradx = masa_eval_grad_u<Scalar>(x,y,z,1);
	  grady = masa_eval_grad_u<Scalar>(x,y,z,2);
	  gradz = masa_eval_grad_u<Scalar>(x,y,z,3);

	  gradx = masa_eval_grad_v<Scalar>(x,y,z,1);
	  grady = masa_eval_grad_v<Scalar>(x,y,z,2);
	  gradz = masa_eval_grad_v<Scalar>(x,y,z,3);
	  
	  gradx = masa_eval_grad_w<Scalar>(x,y,z,1);
	  grady = masa_eval_grad_w<Scalar>(x,y,z,2);
	  gradz = masa_eval_grad_w<Scalar>(x,y,z,3);

	  gradx = masa_eval_grad_p<Scalar>(x,y,z,1);
	  grady = masa_eval_grad_p<Scalar>(x,y,z,2);
	  gradz = masa_eval_grad_p<Scalar>(x,y,z,3);

	  gradx = masa_eval_grad_rho<Scalar>(x,y,z,1);
	  grady = masa_eval_grad_rho<Scalar>(x,y,z,2);
	  gradz = masa_eval_grad_rho<Scalar>(x,y,z,3);

	  // check against maple output
	  ufield2   = SourceQ_u  (x,y,z,u_0,u_x,u_y,u_z,v_0,v_x,v_y,v_z,w_0,w_x,w_y,w_z,rho_0,rho_x,rho_y,rho_z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,a_rhox,a_rhoy,a_rhoz,a_ux,a_uy,a_uz,a_vx,a_vy,a_vz,a_wx,a_wy,a_wz,mu,L,R,K);
	  vfield2   = SourceQ_v  (x,y,z,u_0,u_x,u_y,u_z,v_0,v_x,v_y,v_z,w_0,w_x,w_y,w_z,rho_0,rho_x,rho_y,rho_z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,a_rhox,a_rhoy,a_rhoz,a_ux,a_uy,a_uz,a_vx,a_vy,a_vz,a_wx,a_wy,a_wz,mu,L,R,K);
	  wfield2   = SourceQ_w  (x,y,z,u_0,u_x,u_y,u_z,v_0,v_x,v_y,v_z,w_0,w_x,w_y,w_z,rho_0,rho_x,rho_y,rho_z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,a_rhox,a_rhoy,a_rhoz,a_ux,a_uy,a_uz,a_vx,a_vy,a_vz,a_wx,a_wy,a_wz,mu,L,R,K);
	  rho2      = SourceQ_rho(x,y,z,u_0,u_x,u_y,u_z,v_0,v_x,v_y,v_z,w_0,w_x,w_y,w_z,rho_0,rho_x,rho_y,rho_z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,a_rhox,a_rhoy,a_rhoz,a_ux,a_uy,a_uz,a_vx,a_vy,a_vz,a_wx,a_wy,a_wz,mu,L,R,K);
	  efield2   = SourceQ_e  (x,y,z,u_0,u_x,u_y,u_z,v_0,v_x,v_y,v_z,w_0,w_x,w_y,w_z,rho_0,rho_x,rho_y,rho_z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,a_rhox,a_rhoy,a_rhoz,a_ux,a_uy,a_uz,a_vx,a_vy,a_vz,a_wx,a_wy,a_wz,mu,Gamma,L,R,K);

	  exact_u2     = anQ_u   (x,y,z,u_0,u_x,u_y,u_z,a_ux,a_uy,a_uz,L);
	  exact_v2     = anQ_v   (x,y,z,v_0,v_x,v_y,v_z,a_vx,a_vy,a_vz,L);
	  exact_w2     = anQ_w   (x,y,z,w_0,w_x,w_y,w_z,a_wx,a_wy,a_wz,L);
	  exact_rho2   = anQ_rho (x,y,z,rho_0,rho_x,rho_y,rho_z,a_rhox,a_rhoy,a_rhoz,L);
	  exact_p2     = anQ_p   (x,y,z,p_0,p_x,p_y,p_z,a_px,a_py,a_pz,L);

	  // test the result is roughly zero
	  // choose between abs and rel error
#ifdef MASA_STRICT_REGRESSION

	  ufield3 = std::abs(ufield-ufield2);
	  vfield3 = std::abs(vfield-vfield2);
	  wfield3 = std::abs(wfield-wfield2);
	  efield3 = std::abs(efield-efield2);
	  rho3    = std::abs(rho-rho2);

	  exact_u3   = std::abs(exact_u-exact_u2);
	  exact_v3   = std::abs(exact_v-exact_v2);
	  exact_w3   = std::abs(exact_w-exact_w2);
	  exact_rho3 = std::abs(exact_rho-exact_rho2);
	  exact_p3   = std::abs(exact_p-exact_p2);

#else
	  ufield3 = std::abs(ufield-ufield2)/std::abs(ufield2);
	  vfield3 = std::abs(vfield-vfield2)/std::abs(vfield2);
	  wfield3 = std::abs(wfield-wfield2)/std::abs(wfield2);
	  efield3 = std::abs(efield-efield2)/std::abs(efield2);
	  rho3    = std::abs(rho-rho2)/std::abs(rho2);

	  exact_u3   = std::abs(exact_u-exact_u2)/std::abs(exact_u2);
	  exact_v3   = std::abs(exact_v-exact_v2)/std::abs(exact_v2);
	  exact_w3   = std::abs(exact_w-exact_w2)/std::abs(exact_w2);
	  exact_rho3 = std::abs(exact_rho-exact_rho2)/std::abs(exact_rho2);
	  exact_p3   = std::abs(exact_p-exact_p2)/std::abs(exact_p2);
#endif

	  threshcheck(ufield3,threshold);
	  threshcheck(vfield3,threshold);
	  threshcheck(wfield3,threshold);
	  threshcheck(efield3,threshold);
	  threshcheck(rho3,threshold);
	  
	  threshcheck(exact_u3,threshold);
	  threshcheck(exact_v3,threshold);
	  threshcheck(exact_w3,threshold);
	  threshcheck(exact_rho3,threshold);
	  threshcheck(exact_p3,threshold);

	  nancheck(gradx);
	  nancheck(grady);
	  nancheck(gradz);
	  
	} // done iterating

  // reroute stdout for regressions: TODO remove when logger mechanism
  // is used inside masa; these tests currently just verify functions
  // run successfully.
  freopen("/dev/null","w",stdout);

  // test gradient error terms
  Scalar derr = masa_eval_grad_u<Scalar>(0,0,0,0);
  test_grad(derr);

  derr = masa_eval_grad_u<Scalar>(0,0,0,4);
  test_grad(derr);

  // v
  derr = masa_eval_grad_v<Scalar>(0,0,0,0);
  test_grad(derr);

  derr = masa_eval_grad_v<Scalar>(0,0,0,4);
  test_grad(derr);

  // w
  derr = masa_eval_grad_w<Scalar>(0,0,0,0);
  test_grad(derr);  

  derr = masa_eval_grad_w<Scalar>(0,0,0,4);
  test_grad(derr);

  // p
  derr = masa_eval_grad_p<Scalar>(0,0,0,0);
  test_grad(derr);  

  derr = masa_eval_grad_p<Scalar>(0,0,0,4);
  test_grad(derr);

  // rho
  derr = masa_eval_grad_rho<Scalar>(0,0,0,0);
  test_grad(derr);  

  derr = masa_eval_grad_rho<Scalar>(0,0,0,4);
  test_grad(derr);

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
