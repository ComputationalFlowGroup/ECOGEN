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

#ifndef MODUEQ_H
#define MODUEQ_H

#include "../Model.h"
#include "../../Order1/Cell.h"
#include "MixUEq.h"
#include "PhaseUEq.h"
#include "GradPhaseUEq.h"
#include "GradMixUEq.h"

class ModUEq;

#include "FluxUEq.h"

//! \class     ModUEq
//! \brief     Model class for the velocity-equilibrium system of equations
class ModUEq : public Model
{
  public:
    //! \brief     UEq model constructor
    //! \param     numbTransports    number of additional transport equations
    //! \param     numbPhases        number of phases
    ModUEq(const int& numbTransports, const int& numbPhases);
    //! \brief     Generic model constructor (used by derived classes)
    //! \param     name              model name
    //! \param     numbTransports    number of additional transport equations
    ModUEq(const std::string& name, const int& numbTransports);
    virtual ~ModUEq();

    virtual void allocateCons(Flux** cons);
    virtual void allocatePhase(Phase** phase);
    virtual void allocateMixture(Mixture** mixture);
    virtual void allocatePhaseGradient(GradPhase** phase);
    virtual void allocateMixtureGradient(GradMixture** mixture);

    //! \details    Complete multiphase state from volume fractions, pressures, densities and velocity
    virtual void fulfillState(Phase** phases, Mixture* mixture);

    //! \details    Does nothing for this model
    virtual void fulfillStateRestart(Phase** /*phases*/, Mixture* /*mixture*/) {};

    //! \details    Does nothing for this model
    virtual void initializeAugmentedVariables(Cell* /*cell*/) {};

    //Hydrodynamic Riemann solvers
    //----------------------------
    virtual void solveRiemannIntern(Cell& cellLeft, Cell& cellRight, const double& dxLeft, const double& dxRight, double& dtMax, std::vector<double> &boundData = DEFAULT_VEC_INTERFACE_DATA) const; // Riemann between two computed cells
    virtual void solveRiemannInternMRF(Cell& cellLeft, Cell& cellRight, const double& dxLeft, const double& dxRight, double& dtMax, const Coord& omega, const Coord& normal, const Coord& tangent, const Coord& binormal, const Coord& position) const;
    virtual void solveRiemannWall(Cell& cellLeft, const double& dxLeft, double& dtMax, std::vector<double> &boundData) const; // Riemann between left cell and wall
    virtual void solveRiemannInletTank(Cell& cellLeft, const double& dxLeft, double& dtMax, const double* ak0, const double* rhok0, const double& p0, const double& /*T0*/, std::vector<double> &boundData) const;
    virtual void solveRiemannInletInjStagState(Cell& cellLeft, const double& dxLeft, double& dtMax, const double m0, const double* ak0, const double* rhok0, const double* pk0, std::vector<double> &boundData) const;
    virtual void solveRiemannInletInjTemp(Cell& cellLeft, const double& dxLeft, double& dtMax, const double m0, const double* Tk0, const double* ak0, std::vector<double> &boundData) const; 
    virtual void solveRiemannOutletPressure(Cell& cellLeft, const double& dxLeft, double& dtMax, const double p0, std::vector<double> &boundData) const;
    virtual void solveRiemannOutletMassflow(Cell& cellLeft, const double& dxLeft, double& dtMax, const double m0, std::vector<double>& boundData) const;
    virtual void solveRiemannNullFlux() const;

    //Transports Riemann solvers
    //--------------------------
    virtual void solveRiemannTransportIntern(Cell& cellLeft, Cell& cellRight);
    virtual void solveRiemannTransportWall();
    virtual void solveRiemannTransportPiston(Cell& cellLeft, double uPiston);
    virtual void solveRiemannTransportInletTank(Cell& cellLeft, double* valueTransports);
    virtual void solveRiemannTransportInletInjStagState(Cell& cellLeft, double* valueTransports);
    virtual void solveRiemannTransportOutletPressure(Cell& cellLeft, double* valueTransport);

    virtual void reverseProjection(const Coord normal, const Coord tangent, const Coord binormal) const;

    //Low-Mach preconditioning
    //------------------------
    virtual void lowMachSoundSpeed(double& machRef, const double& uL, double& cL, const double& uR = Errors::defaultDouble, double& cR = Tools::uselessDouble) const;
    virtual void setLowMach(const bool& lowMach) { m_lowMach = lowMach; };
    virtual void setMachRefMin(const double& machRefMin) { m_machRefMin = machRefMin; };

    //Moving Reference Frame
    //----------------------
    virtual void addNonConsMrfFlux(Phase** phases);
    virtual void reverseProjectionMrfFlux(const Coord normal, const Coord tangent, const Coord binormal) const;

    //Accessors
    //---------
    //! \brief  Select a specific scalar variable
    //! \param  phases         phases array variables
    //! \param  mixture        mixture variables
    //! \param  vecTransports  vector of transport variables
    //! \param  nameVariables  Name of the variable to select
    //! \param  numPhases      Phases number's
    virtual double selectScalar(Phase** phases, Mixture* mixture, Transport* transports, Variable nameVariable, int num = 0) const;
    virtual const double& getSM();
    virtual const Coord& getVelocity(const Cell* cell) const { return cell->getMixture()->getVelocity(); };
    virtual Coord& getVelocity(Cell* cell) { return cell->getMixture()->getVelocity(); };

    virtual const std::string& whoAmI() const { return m_name; };

    virtual void setSmoothCrossSection1d(const bool& applySmooth) { m_smoothCrossSection1d = applySmooth; };

  private:
    static const std::string NAME;

    friend class FluxUEq;
};

#endif // MODUEQ_H
