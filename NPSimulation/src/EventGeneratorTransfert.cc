/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : January 2011                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body TransfertReaction.    *
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <iostream>
#include <fstream>
#include <limits>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4RotationMatrix.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool headers
#include "EventGeneratorTransfert.hh"
#include "RootOutput.h"
#include "Particle.hh"
#include "ParticleStack.hh"
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::EventGeneratorTransfert()
   :  m_ShootLight(0),
      m_ShootHeavy(0),
      m_Target(0),
      m_Reaction(new Reaction),
      m_HalfOpenAngleMin(0),
      m_HalfOpenAngleMax(180),
      m_BeamEnergy(0),
      m_BeamEnergySpread(0),
      m_SigmaX(0),
      m_SigmaY(0),
      m_SigmaThetaX(0),
      m_SigmaPhiY(0)
{
   //------------- Default Constructor -------------
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::SetTarget(Target* Target) 
{
   if (Target != 0) {
      m_Target = Target;
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::~EventGeneratorTransfert()
{
   //------------- Default Destructor ------------
   delete m_Reaction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTransfert::EventGeneratorTransfert(  string name1                  ,  // Beam nuclei
                                                   string name2                  ,  // Target nuclei
                                                   string name3                  ,  // Product of reaction
                                                   string name4                  ,  // Product of reaction
                                                   double BeamEnergy             ,  // Beam Energy
                                                   double ExcitationEnergyLight  ,  // Excitation of Light Nuclei
                                                   double ExcitationEnergyHeavy  ,  // Excitation of Heavy Nuclei
                                                   double BeamEnergySpread       ,
                                                   double SigmaX                 ,
                                                   double SigmaY                 ,
                                                   double SigmaThetaX            ,
                                                   double SigmaPhiY              ,
                                                   bool   ShootLight             ,
                                                   bool   ShootHeavy             ,
                                                   string Path                   ,
                                                   double CSThetaMin             ,
                                                   double CSThetaMax)  // Path of the differentiel Cross Section
{
 SetEverything(   name1,       
                  name2,        
                  name3,       
                  name4,        
                  BeamEnergy,        
                  ExcitationEnergyLight,        
                  ExcitationEnergyHeavy,
                  BeamEnergySpread,
                  SigmaX,
                  SigmaY,
                  SigmaThetaX,
                  SigmaPhiY,
                  ShootLight,
                  ShootHeavy,
                  Path,
                  CSThetaMin,
                  CSThetaMax);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::InitializeRootOutput()
{

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::Print() const
{

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//    Inherit from VEventGenerator

void EventGeneratorTransfert::ReadConfiguration(string Path)
{
////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

////////Reaction Setting needs///////
   string Beam, Target, Heavy, Light, CrossSectionPath ;
   G4double BeamEnergy = 0 , ExcitationEnergyLight = 0, ExcitationEnergyHeavy = 0;
   G4double BeamEnergySpread = 0 , SigmaX = 0 , SigmaY = 0 , SigmaThetaX = 0 , SigmaPhiY=0;
   G4double CSHalfOpenAngleMin = 0, CSHalfOpenAngleMax = 180;
   bool  ShootLight = false ;
   bool  ShootHeavy = false ;
   
   bool ReadingStatus = false ;
   bool check_Beam = false ;
   bool check_Target = false ;
   bool check_Light = false ;
   bool check_Heavy = false ;
   bool check_ExcitationEnergyLight = false ;
   bool check_ExcitationEnergyHeavy = false ;
   bool check_BeamEnergy = false ;
   bool check_BeamEnergySpread = false ;
   bool check_FWHMX = false ;
   bool check_FWHMY = false ;
   bool check_EmmitanceTheta = false ;
   bool check_EmmitancePhi = false ;
   bool check_CrossSectionPath = false ;
   bool check_ShootLight = false ;
   bool check_ShootHeavy = false ;
   
//////////////////////////////////////////////////////////////////////////////////////////
   ifstream ReactionFile;
   ReactionFile.open(Path.c_str());

   if (ReactionFile.is_open()) {} else {
      return;
   }

   while (!ReactionFile.eof()) {
      //Pick-up next line
      getline(ReactionFile, LineBuffer);

      

      if (LineBuffer.compare(0, 9, "Transfert") == 0) { 
          ReadingStatus = true ;
      
          G4cout << "////////// Transfert ///////////" << G4endl;
      }


   while(ReadingStatus){
          
        ReactionFile >> DataBuffer;
        
        //Search for comment Symbol %
          if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
         else if (DataBuffer=="Beam=") {
            check_Beam = true ;
            ReactionFile >> DataBuffer;
            Beam = DataBuffer;
            G4cout << "Beam " << Beam << G4endl;
         }

         else if (DataBuffer=="Target=") {
            check_Target = true ;
            ReactionFile >> DataBuffer;
            Target = DataBuffer;
            G4cout << "Target " << Target << G4endl;
         }

         else if (DataBuffer=="Light=") {
            check_Light = true ;
            ReactionFile >> DataBuffer;
            Light = DataBuffer;
            G4cout << "Light " << Light << G4endl;
         }

        else if  (DataBuffer=="Heavy=") {
            check_Heavy = true ;
            ReactionFile >> DataBuffer;
            Heavy = DataBuffer;
            G4cout << "Heavy " << Heavy << G4endl;
         }

        else if  (DataBuffer=="ExcitationEnergy3=" || DataBuffer=="ExcitationEnergyLight=") {
           check_ExcitationEnergyLight = true ;
            ReactionFile >> DataBuffer;
            ExcitationEnergyLight = atof(DataBuffer.c_str()) * MeV;
            G4cout << "Excitation Energy Nuclei 3: " << ExcitationEnergyLight / MeV << " MeV" << G4endl;
         }

        else if  (DataBuffer=="ExcitationEnergy4=" || DataBuffer=="ExcitationEnergyHeavy=") {
           check_ExcitationEnergyHeavy = true ;
            ReactionFile >> DataBuffer;
            ExcitationEnergyHeavy = atof(DataBuffer.c_str()) * MeV;
            G4cout << "Excitation Energy Nuclei 4: " << ExcitationEnergyHeavy / MeV << " MeV" << G4endl;
         }

        else if  (DataBuffer=="BeamEnergy=") {
           check_BeamEnergy = true ;
            ReactionFile >> DataBuffer;
            BeamEnergy = atof(DataBuffer.c_str()) * MeV;
            G4cout << "Beam Energy " << BeamEnergy / MeV << " MeV" << G4endl;
         }

        else if  (DataBuffer=="BeamEnergySpread=") {
           check_BeamEnergySpread = true ;
            ReactionFile >> DataBuffer;
            BeamEnergySpread = atof(DataBuffer.c_str()) * MeV;
            G4cout << "Beam Energy Spread " << BeamEnergySpread / MeV << " MeV" << G4endl;
         }

        else if  (DataBuffer=="SigmaX=") {
           check_FWHMX = true ;
            ReactionFile >> DataBuffer;
            SigmaX = atof(DataBuffer.c_str()) * mm;
            G4cout << "Beam FWHM X " << SigmaX << " mm" << G4endl;
         }

        else if  (DataBuffer=="SigmaY=") {
           check_FWHMY = true ;
            ReactionFile >> DataBuffer;
            SigmaY = atof(DataBuffer.c_str()) * mm;
            G4cout << "Beam FWHM Y " << SigmaX << " mm" << G4endl;
         }

        else if  (DataBuffer=="SigmaThetaX=") {
           check_EmmitanceTheta = true ;
            ReactionFile >> DataBuffer;
            SigmaThetaX = atof(DataBuffer.c_str()) * deg;
            G4cout << "Beam Emmitance Theta " << SigmaThetaX / deg << " deg" << G4endl;
         }
         
        else if  (DataBuffer=="SigmaPhiY=") {
           check_EmmitancePhi = true ;
            ReactionFile >> DataBuffer;
            SigmaPhiY = atof(DataBuffer.c_str()) * deg;
            G4cout << "Beam Emmitance Phi " << SigmaPhiY / deg << " deg" << G4endl;
         }

        else if  (DataBuffer=="CrossSectionPath=") {
           check_CrossSectionPath = true ;
            ReactionFile >> CrossSectionPath;
            G4cout << "Cross Section File: " << CrossSectionPath << G4endl ;
         }

         else if (DataBuffer=="HalfOpenAngleMin=") {
            ReactionFile >> DataBuffer;
            CSHalfOpenAngleMin = atof(DataBuffer.c_str()) * deg;
            G4cout << "HalfOpenAngleMin " << CSHalfOpenAngleMin / deg << " degree" << G4endl;
         }

         else if (DataBuffer=="HalfOpenAngleMax=") {
            ReactionFile >> DataBuffer;
            CSHalfOpenAngleMax = atof(DataBuffer.c_str()) * deg;
            G4cout << "HalfOpenAngleMax " << CSHalfOpenAngleMax / deg << " degree" << G4endl;
         }

        else if  (DataBuffer=="ShootLight=") {
           check_ShootLight = true ;
            ReactionFile >> DataBuffer;
            if (atof(DataBuffer.c_str()) == 1) ShootLight = true ;
            if (ShootLight)    G4cout << "Shoot Light particle      : yes" << G4endl;
            else           G4cout << "Shoot Light particle      : no"  << G4endl;
         }

        else if  (DataBuffer=="ShootHeavy=") {
           check_ShootHeavy = true ;
            ReactionFile >> DataBuffer;
            if (atof(DataBuffer.c_str()) == 1) ShootHeavy = true ;
            if (ShootHeavy)    G4cout << "Shoot Heavy particle      : yes" << G4endl;
            else           G4cout << "Shoot Heavy particle      : no"  << G4endl;
         }

        
         ///////////////////////////////////////////////////
      //   If no Transfert Token and no comment, toggle out
         else 
            {ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
            
         ///////////////////////////////////////////////////
      //   If all Token found toggle out
         if(check_Beam && check_Target && check_Light && check_Heavy && check_ExcitationEnergyLight && check_ExcitationEnergyHeavy 
            &&  check_BeamEnergy && check_BeamEnergySpread && check_FWHMX && check_FWHMY && check_EmmitanceTheta 
            &&  check_EmmitancePhi && check_CrossSectionPath && check_ShootLight && check_ShootHeavy){
            ReadingStatus = false ;  
            G4cout << "////////////////////////////////" << G4endl;
         }
   }
           

   }
   
   SetEverything( Beam,
                  Target,
                  Light,
                  Heavy,
                  BeamEnergy,
                  ExcitationEnergyLight,
                  ExcitationEnergyHeavy,
                  BeamEnergySpread,
                  SigmaX,
                  SigmaY,
                  SigmaThetaX,
                  SigmaPhiY,
                  ShootLight,
                  ShootHeavy,
                  CrossSectionPath,
                  CSHalfOpenAngleMin/deg,
                  CSHalfOpenAngleMax/deg);
         
   ReactionFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::GenerateEvent(G4Event* anEvent)
{   
   // If first time, write the DeDx table
   if (anEvent->GetEventID() == 0) {
      //-------------- Before living, wrtie the DeDx Table -------------------
      G4int LightZx = m_Reaction->GetNucleus3()->GetZ();
      G4int LightAx = m_Reaction->GetNucleus3()->GetA();

      G4int BeamZx = m_Reaction->GetNucleus1()->GetZ();
      G4int BeamAx = m_Reaction->GetNucleus1()->GetA();

      if (m_Target != 0) {
         m_Target->WriteDEDXTable(G4ParticleTable::GetParticleTable()->GetIon(LightZx,LightAx, 0.) ,0, m_BeamEnergy+4*m_BeamEnergySpread);
         m_Target->WriteDEDXTable(G4ParticleTable::GetParticleTable()->GetIon(BeamZx,BeamAx, 0.)   ,0, m_BeamEnergy+4*m_BeamEnergySpread);
      }
   }

   //////////////////////////////////////////////////
   //////Define the kind of particle to shoot////////
   //////////////////////////////////////////////////
   // Light
   G4int LightZ = m_Reaction->GetNucleus3()->GetZ() ;
   G4int LightA = m_Reaction->GetNucleus3()->GetA() ;

   G4ParticleDefinition* LightName
   = G4ParticleTable::GetParticleTable()->GetIon(LightZ, LightA, m_Reaction->GetExcitation3()*MeV);

   // Recoil
   G4int HeavyZ = m_Reaction->GetNucleus4()->GetZ() ;
   G4int HeavyA = m_Reaction->GetNucleus4()->GetA() ;

   G4ParticleDefinition* HeavyName
   = G4ParticleTable::GetParticleTable()->GetIon(HeavyZ, HeavyA, m_Reaction->GetExcitation4()*MeV);

   // Beam
   G4int BeamZ = m_Reaction->GetNucleus1()->GetZ();
   G4int BeamA = m_Reaction->GetNucleus1()->GetA();
   G4ParticleDefinition* BeamName = G4ParticleTable::GetParticleTable()->GetIon(BeamZ, BeamA, 0);

   ///////////////////////////////////////////////////////////////////////
   ///// Calculate the incident beam direction as well as the vertex /////
   ///// of interaction in target and Energy Loss of the beam within /////
   ///// the target.                                                 /////
   ///////////////////////////////////////////////////////////////////////
    G4ThreeVector InterCoord;
    
    G4double Beam_thetaX = 0, Beam_phiY = 0;
    G4double Beam_theta  = 0, Beam_phi  = 0;
    G4double FinalBeamEnergy = 0 ;
    G4double InitialBeamEnergy = RandGauss::shoot(m_BeamEnergy, m_BeamEnergySpread);
    
    m_Target->CalculateBeamInteraction( 0, m_SigmaX, 0, m_SigmaThetaX,
                                       0, m_SigmaY, 0, m_SigmaPhiY,
                                       InitialBeamEnergy,
                                       BeamName,
                                       InterCoord, Beam_thetaX, Beam_phiY,
                                       Beam_theta, Beam_phi,
                                       FinalBeamEnergy);
    
    m_Reaction->SetBeamEnergy(FinalBeamEnergy);
    
    
    // write vertex position to ROOT file
    G4double x0 = InterCoord.x();
    G4double y0 = InterCoord.y();
    G4double z0 = InterCoord.z();
    
    
    //////////////////////////////////////////////////////////
    ///// Build rotation matrix to go from the incident //////
    ///// beam frame to the "world" frame               //////
    //////////////////////////////////////////////////////////
    G4ThreeVector col1(cos(Beam_theta) * cos(Beam_phi),
                       cos(Beam_theta) * sin(Beam_phi),
                       -sin(Beam_theta));
    G4ThreeVector col2(-sin(Beam_phi),
                       cos(Beam_phi),
                       0);
    G4ThreeVector col3(sin(Beam_theta) * cos(Beam_phi),
                       sin(Beam_theta) * sin(Beam_phi),
                       cos(Beam_theta));
    G4RotationMatrix BeamToWorld(col1, col2, col3);
    
    /////////////////////////////////////////////////////////////////
    ///// Angles for emitted particles following Cross Section //////
    ///// Angles are in the beam frame                         //////
    /////////////////////////////////////////////////////////////////
    
    // Angles
    RandGeneral CrossSectionShoot(m_Reaction->GetCrossSection(), m_Reaction->GetCrossSectionSize());
    G4double ThetaCM = (m_Reaction->GetCrossSectionAngleMin() + CrossSectionShoot.shoot() * (m_Reaction->GetCrossSectionAngleMax() - m_Reaction->GetCrossSectionAngleMin())) * deg;
    
    G4double phi     = RandFlat::shoot() * 2*pi;
    
    //////////////////////////////////////////////////
    /////  Momentum and angles from  kinematics  /////
    /////  Angles are in the beam frame          /////
    //////////////////////////////////////////////////
    // Variable where to store results
    G4double ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy;
    // Set the Theta angle of reaction
    m_Reaction->SetThetaCM(ThetaCM);
    // Compute Kinematic using previously defined ThetaCM
    m_Reaction->KineRelativistic(ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy);
    // Momentum in beam frame for light particle
    G4ThreeVector momentum_kineLight_beam(sin(ThetaLight) * cos(phi),
                                          sin(ThetaLight) * sin(phi),
                                          cos(ThetaLight));
    // Momentum in beam frame for heavy particle
    G4ThreeVector momentum_kineHeavy_beam(sin(ThetaHeavy) * cos(phi+pi),
                                          sin(ThetaHeavy) * sin(phi+pi),
                                          cos(ThetaHeavy));
    
    //////////////////////////////////////////////////
    ///////// Set up everything for shooting /////////
    //////////////////////////////////////////////////
    // Case of light particle
    // Instantiate a new particle
    Particle LightParticle;
    
    // Particle type
    LightParticle.SetParticleDefinition(LightName);
    // Particle energy
    LightParticle.SetParticleKineticEnergy(EnergyLight);
    // Particle vertex position
    LightParticle.SetParticlePosition(G4ThreeVector(x0, y0, z0));
    // Particle direction
    // Kinematical angles in the beam frame are transformed 
    // to the "world" frame
    G4ThreeVector momentum_kine_world = BeamToWorld * momentum_kineLight_beam;
    //Set the Momentum Direction
    LightParticle.SetParticleMomentumDirection(momentum_kine_world);
    // Set the shoot status
    LightParticle.SetShootStatus(m_ShootLight) ;
    //Add the particle to the particle stack
    ParticleStack::getInstance()->AddParticleToStack(LightParticle); 
   
    
   // Case of heavy particle
   // Instantiate a new particle
    Particle HeavyParticle;
    
    // Particle type
    HeavyParticle.SetParticleDefinition(HeavyName);
    // Particle energy
    HeavyParticle.SetParticleKineticEnergy(EnergyHeavy);
    // Particle vertex position
    HeavyParticle.SetParticlePosition(G4ThreeVector(x0, y0, z0));
    // Particle direction
    // Kinematical angles in the beam frame are transformed 
    // to the "world" frame
    momentum_kine_world = BeamToWorld * momentum_kineHeavy_beam;
    //Set the Momentum Direction
    HeavyParticle.SetParticleMomentumDirection(momentum_kine_world);
    // Set the shoot status
    HeavyParticle.SetShootStatus(m_ShootHeavy) ;
    //Add the particle to the particle stack
    ParticleStack::getInstance()->AddParticleToStack(HeavyParticle); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTransfert::SetEverything(string name1,                // Beam nuclei
                                            string name2,                // Target nuclei
                                            string name3,                // Product of reaction
                                            string name4,                // Product of reaction
                                            double BeamEnergy,           // Beam Energy
                                            double ExcitationEnergyLight,// Excitation of Light Nuclei
                                            double ExcitationEnergyHeavy,// Excitation of Heavy Nuclei
                                            double BeamEnergySpread,
                                            double SigmaX,
                                            double SigmaY,
                                            double SigmaThetaX,
                                            double SigmaPhiY,
                                            bool   ShootLight,
                                            bool   ShootHeavy,
                                            string Path,
                                            double CSThetaMin,
                                            double CSThetaMax) 
{
   m_Reaction = new Reaction(name1, name2, name3, name4, BeamEnergy, ExcitationEnergyLight, ExcitationEnergyHeavy, Path, CSThetaMin, CSThetaMax);

   m_BeamEnergy       = BeamEnergy;
   m_BeamEnergySpread = BeamEnergySpread;
   m_SigmaX           = SigmaX;
   m_SigmaY           = SigmaY;
   m_SigmaThetaX      = SigmaThetaX;
   m_SigmaPhiY        = SigmaPhiY;
   m_ShootLight       = ShootLight;
   m_ShootHeavy       = ShootHeavy;
   m_HalfOpenAngleMin = CSThetaMin;
   m_HalfOpenAngleMax = CSThetaMax;
}
 
