//  
//       ,---.     ,--,    .---.     ,--,    ,---.    .-. .-. 
//       | .-'   .' .')   / .-. )  .' .'     | .-'    |  \| | 
//       | `-.   |  |(_)  | | |(_) |  |  __  | `-.    |   | | 
//       | .-'   \  \     | | | |  \  \ ( _) | .-'    | |\  | 
//       |  `--.  \  `-.  \ `-' /   \  `-) ) |  `--.  | | |)| 
//       /( __.'   \____\  )---'    )\____/  /( __.'  /(  (_) 
//      (__)              (_)      (__)     (__)     (__)     
//      Official webSite: https://code-mphi.github.io/ECOGEN/
//
//  This file is part of ECOGEN.
//
//  ECOGEN is the legal property of its developers, whose names 
//  are listed in the copyright file included with this source 
//  distribution.
//
//  ECOGEN is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published 
//  by the Free Software Foundation, either version 3 of the License, 
//  or (at your option) any later version.
//  
//  ECOGEN is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with ECOGEN (file LICENSE).  
//  If not, see <http://www.gnu.org/licenses/>.

#ifndef ELEMENTTETRAHEDRON_H
#define ELEMENTTETRAHEDRON_H

#include <fstream>

#include "ElementNS.h"
#include "FaceTriangle.h"

class ElementTetrahedron : public ElementNS
{
public:
  ElementTetrahedron();
  virtual ~ElementTetrahedron();

  virtual void construitFaces(const Coord* nodes, FaceNS** faces, int& indexMaxFaces, int** facesBuff, int* sumNodesBuff); 
  virtual void construitFacesSimplifie(int& iMax, int** facesBuff, int* sumNodesBuff);
  virtual void attributFaceCommunicante(FaceNS** faces, const int& indexMaxFaces, const int& numberNoeudsInternes);
  virtual int compteFaceCommunicante(std::vector<int*>& faces, std::vector<int>& sumNodesBuff);
  virtual int compteFaceCommunicante(int& iMax, int** faces, int* sumNodesBuff);

private:
  virtual void computeVolume(const Coord* nodes);
  virtual void computeLCFL(const Coord* nodes);

  static const int TYPEGMSH;
  static const int NUMBERNODES;
  static const int NUMBERFACES; /* ici il s'agit de triangles*/
  static const int TYPEVTK;
};

#endif // ELEMENTTETRAHEDRON_H
