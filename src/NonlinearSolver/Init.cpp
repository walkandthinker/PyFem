//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2019
//* https://github.com/walkandthinker/AsFem
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************

#include "NonlinearSolver/NonlinearSolver.h"


void NonlinearSolver::Init(NonlinearSolverBlock &nonlinearsolverblock){
    _SolverType=nonlinearsolverblock._SolverType;
    _MaxIters=nonlinearsolverblock._MaxIters;
    _RAbsTol=nonlinearsolverblock._RAbsTol;
    _RRelTol=nonlinearsolverblock._RRelTol;
    _STol=nonlinearsolverblock._STol;

    SNESCreate(PETSC_COMM_WORLD,&_snes);
    // SNESSetKSP(_snes,_ksp);
    //***********************************
    //**** init ksp
    //***********************************
    SNESGetKSP(_snes,&_ksp);
    KSPSetTolerances(_ksp,1.0e-10,1.0e-10,PETSC_DEFAULT,500000);
    KSPGMRESSetRestart(_ksp,1200);
    KSPGetPC(_ksp,&_pc);
    PCSetType(_pc,PCLU);

    KSPSetFromOptions(_ksp);




    // SNESSetType(_snes,SNESQN);
    // SNESQNSetType(_snes,SNES_QN_LBFGS);

    SNESSetTolerances(_snes,_RAbsTol,_RRelTol,_STol,_MaxIters,-1);
    
    

    if(_SolverType==NonlinearSolverType::NewtonRaphson){
        SNESSetType(_snes,SNESNEWTONLS);
        SNESGetLineSearch(_snes,&_linesearch);
        // SNESSetLineSearch(_snes,SNESLINESEARCHBASIC);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
        SNESLineSearchSetOrder(_linesearch,nonlinearsolverblock._LineSearchOrder);
    }
    else if(_SolverType==NonlinearSolverType::SNESNewtonLs){
        SNESSetType(_snes,SNESNEWTONLS);
        SNESGetLineSearch(_snes,&_linesearch);
        SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
        SNESLineSearchSetOrder(_linesearch,nonlinearsolverblock._LineSearchOrder);
    }
    else if(_SolverType==NonlinearSolverType::SNESNewtonTr){
        SNESSetType(_snes,SNESNEWTONTR);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }
    else if(_SolverType==NonlinearSolverType::SNESLBfgs){
        SNESSetType(_snes,SNESQN);
        SNESQNSetType(_snes,SNES_QN_LBFGS);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }
    else if(_SolverType==NonlinearSolverType::SNESBroyden){
        SNESSetType(_snes,SNESQN);
        SNESQNSetType(_snes,SNES_QN_BROYDEN);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }
    else if(_SolverType==NonlinearSolverType::SNESBadBroyden){
        SNESSetType(_snes,SNESQN);
        SNESQNSetType(_snes,SNES_QN_BADBROYDEN);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }
    else if(_SolverType==NonlinearSolverType::SNESNewtonCG){
        SNESSetType(_snes,SNESNCG);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }
    else if(_SolverType==NonlinearSolverType::SNESNewtonGMRES){
        SNESSetType(_snes,SNESNGMRES);
        SNESGetLineSearch(_snes,&_linesearch);
        if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchDefault){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBackTrace){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBT);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchCP){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHCP);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchL2){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHL2);
        }
        else if(nonlinearsolverblock._LineSearchType==LineSearchType::LineSearchBasic){
            SNESLineSearchSetType(_linesearch,SNESLINESEARCHBASIC);
        }
    }

    SNESLineSearchSetOrder(_linesearch,nonlinearsolverblock._LineSearchOrder);

}