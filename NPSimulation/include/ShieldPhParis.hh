/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:  M. Labiche  contact address: marc.labiche@atfc.ac.uk    *
 *                                                                           *
 * Creation Date  : 15/08/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define the shield around a PARIS phoswich module              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef ShieldPhParis_h
#define ShieldPhParis_h 1

// C++ headers
#include <vector>

// NPTool header
//#include "ParisModule.hh"
#include "ShieldModule.hh"
#include "TInteractionCoordinates.h"

using namespace std;



class ShieldPhParis : public ShieldModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   ShieldPhParis();
   virtual ~ShieldPhParis();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4ThreeVector TL,
                  G4ThreeVector BL,
                  G4ThreeVector BR,
                  G4ThreeVector CT);

   // By Angle Method
   void AddModule(G4double R,
                  G4double Theta,
                  G4double Phi,
                  G4double beta_u,
                  G4double beta_v,
                  G4double beta_w); 

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int             DetecNumber,
                    G4ThreeVector     MMpos,
                    G4RotationMatrix* MMrot,
                    G4LogicalVolume*  world);


   ///////////////////////////////////////////
   ////  Inherite from ParisModule class /////
   ///////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path);

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world);

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput();

   // Initialize all scorers necessary for the detector
   void InitializeScorers();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);

   // Give the static TInteractionCoordinates from VDetector to the classes
   // deriving from ParisModule
   // This is mandatory since the Paris*** does not derive from VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()	{return ms_InterCoord;};


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Interaction Coordinates coming from VDetector through the 
   // SetInteractionCoordinatesPointer method
   TInteractionCoordinates* ms_InterCoord;

   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_X1_Y1     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_X1_Y128   ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y1   ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y128 ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  > Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |

  G4ThreeVector momentum;
};



namespace PARISPHSHIELD
{
   // Resolution
//   const G4double ResoFirstStage  = 0;	// = 50 keV of Resolution   //   Unit is MeV/2.35
   const G4double ResoFirstStage  = 0.0366;	// = 3% at .662 MeV of Resolution   //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957;// = 500ps                 //   Unit is  ns/2.35

   // Geometry for the mother volume containing the different layers of your dummy shape module
  //   const G4double FaceFront          = 16.9*cm;
  //   const G4double FaceBack           = 16.9*cm;

   const G4double Length             = 15.*cm;

  // central hole for Phoswich  
  const G4double PhoswichFace       = 5.8*cm; // 5.7+0.1 cm

   // CsI
  //   const G4double CsIFace      = LaBr3Face;
  //   const G4double CsIThickness = 15.24*cm;

   // Starting form the LaBr3 and going to the CsI
  //   const G4double LaBr3Stage_PosZ  = Length* -0.5 + 0.5*LaBr3Thickness;
  //   const G4double CsIStage_PosZ    = Length* -0.5 + LaBr3Thickness + 0.5*CsIThickness;
}

#endif
