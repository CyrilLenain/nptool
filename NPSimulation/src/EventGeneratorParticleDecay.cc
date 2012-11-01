/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : May 2012                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated the particle decay of nuclei   *
 * generated by previous event generator. Multiple cases are supported:      *
 *  - Only one particle is emmited, in this case a Cross section can be given*
 *  - If Multiple particle are emitted, a Phase Space generator is used      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventGeneratorParticleDecay.hh"

// NPS
#include "Particle.hh"

// G4
#include "G4ParticleTable.hh"

// NPL
#include "NPNucleus.h"
using namespace NPL;

// ROOT
#include "TLorentzVector.h"
#include "TVector3.h"
EventGeneratorParticleDecay::EventGeneratorParticleDecay(){
  m_ParticleStack = ParticleStack::getInstance();
}

EventGeneratorParticleDecay::~EventGeneratorParticleDecay(){
  
}

void EventGeneratorParticleDecay::ReadConfiguration(string Path,int Occurence){
  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;
  istringstream LineStream;
  int TokkenOccurence = 0;
  //////// Setting needs///////
  bool ReadingStatusParticleDecay  = false ;
  
  bool check_Daughter = false ;
  bool check_CrossSection = false ;
  bool check_ExcitationEnergy = false ;
  bool check_shoot = false ;
  bool check_created = false ;
  
  // Instantiate new variable for the up coming Particle
  vector<string> DaughterName;
  vector<double> ExcitationEnergy;
  vector<bool>   shoot;
  string CSPath = "TGenPhaseSpace";
  
  //////////////////////////////////////////////////////////////////////////////////////////
  ifstream InputFile;
  InputFile.open(Path.c_str());
  
  if (InputFile.is_open()) {}
  
  else {
    return;
  }
  
  while (!InputFile.eof()&& !check_created) {
    //Pick-up next line
    getline(InputFile, LineBuffer);
    
    if (LineBuffer.compare(0, 13, "ParticleDecay") == 0) {
      TokkenOccurence++;
      if(TokkenOccurence==Occurence){
        ReadingStatusParticleDecay = true ;
        G4cout << "///////////////////////////////////////// " << G4endl;
        // Get the nuclei name
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        DataBuffer.erase();
        LineStream >> DataBuffer;
        m_MotherNucleiName = DataBuffer ;
        G4cout << "Particle Decay for " << m_MotherNucleiName << G4endl;
      }
    }
    
    ///////////////////////////////
    /// Gamma Decay case
    while(ReadingStatusParticleDecay){
      
      InputFile >> DataBuffer;
      //Search for comment Symbol %
      if (DataBuffer.compare(0, 1, "%") == 0) {
        InputFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }
      
      else if (DataBuffer == "Daughter=") {
        check_Daughter = true ;
        LineStream.clear();
        LineStream.str(LineBuffer);
        
        getline(InputFile, LineBuffer);
        LineStream.clear();
        LineStream.str(LineBuffer);
        G4cout << "    Daughter: " ;
        while(LineStream >> DataBuffer){
          DaughterName.push_back(DataBuffer);
          G4cout << DataBuffer << " " ;
        }
        
        G4cout << G4endl;
        
      }
      
      else if(DataBuffer == "ExcitationEnergy=") {
        check_ExcitationEnergy = true;
        LineStream.clear();
        LineStream.str(LineBuffer);
        
        getline(InputFile, LineBuffer);
        LineStream.clear();
        LineStream.str(LineBuffer);
        G4cout << "    Excitation Energy: " ;
        while(LineStream >> DataBuffer){
          ExcitationEnergy.push_back( atof(DataBuffer.c_str()) );
          G4cout << DataBuffer << " " ;
        }
        
        G4cout << G4endl;
      }
      
      else if(DataBuffer == "DifferentialCrossSection=") {
        check_CrossSection = true;
        InputFile >> CSPath ;
        G4cout << "    Cross Section: " << CSPath << G4endl;
      }
      
      else if(DataBuffer == "shoot=") {
        check_shoot = true;
        LineStream.clear();
        LineStream.str(LineBuffer);
        
        getline(InputFile, LineBuffer);
        LineStream.clear();
        LineStream.str(LineBuffer);
        G4cout << "    Shoot Particle: " ;
        while(LineStream >> DataBuffer){
          shoot.push_back( atof(DataBuffer.c_str()) );
          G4cout << DataBuffer << " " ;
        }
        
        G4cout << G4endl;
      }
      
      
      
      //////////////////////////////////////////////////////
      // If no Token and no comment, toggle out //
      else
          {ReadingStatusParticleDecay = false; G4cout << "ERROR : Wrong Token Sequence: Getting out " << G4endl ;
            exit(1);
          }
      
      // Decay ended
      if(check_Daughter && check_shoot){
        SetDecay(DaughterName,shoot,ExcitationEnergy,CSPath);
        ReadingStatusParticleDecay = false;
        check_created=true;
      }
    }
  }
  
  G4cout << "///////////////////////////////////////// " << G4endl;
  InputFile.close();
}


void EventGeneratorParticleDecay::GenerateEvent(G4Event* anEvent){
  // Look for the decaying nucleus
  Particle decayingParticle = m_ParticleStack->SearchAndRemoveParticle(m_MotherNucleiName);
  if(decayingParticle.GetParticleDefinition()==NULL){
    G4cout << "Gamma Decay Warning: The decaying particle " << m_MotherNucleiName
    << " was not found in the particle stack " << G4endl;
    return ;
  }
  
  G4ParticleDefinition* decayingParticleDefinition = decayingParticle.GetParticleDefinition();
  // Build the decaying particle four momenta vector:
  
  double NucleiEnergy= decayingParticle.GetParticleKineticEnergy()+decayingParticleDefinition->GetPDGMass();
  
  double NucleiMomentum=sqrt(NucleiEnergy*NucleiEnergy -
                             decayingParticleDefinition->GetPDGMass()*decayingParticleDefinition->GetPDGMass());
  
  G4ThreeVector Momentum = decayingParticle.GetParticleMomentumDirection().unit();
  
  // Case of one particle decaying with a user given cross section
  if(m_DifferentialCrossSection!="TGenPhaseSpace"){
    TLorentzVector NucleiLV( NucleiMomentum*Momentum.x(),
                            NucleiMomentum*Momentum.y(),
                            NucleiMomentum*Momentum.z(),
                            NucleiEnergy);
    // Shoot the angle in Center of Mass (CM) frame
    G4double ThetaCM = (m_CrossSectionThetaMin + m_CrossSectionShoot->shoot() * (m_CrossSectionThetaMax - m_CrossSectionThetaMin)) * deg;
    G4double phi     = RandFlat::shoot()*2.*pi;
    
    // Build daughter particule CM LV
    // Pre compute variable for the decay
    double M  = decayingParticleDefinition->GetPDGMass();
    double m1 = m_DaughterNuclei[0]->GetPDGMass();
    double m2 = m_DaughterNuclei[1]->GetPDGMass();
    
    if(M<(m1+m2))
      cout << "Warning: Particle Decay forbiden by kinematic, no particle emitted "<<endl;
    
    else {
      double Energy = ( 1./(2.*M) )*( M*M + m1*m1 - m2*m2);
      double Momentum = sqrt(Energy*Energy - m1*m1);
      
      TVector3 FirstDaughterMomentum = Momentum * TVector3( sin(ThetaCM) * cos(phi),
                                                           sin(ThetaCM) * sin(phi),
                                                           cos(ThetaCM));
      
      TLorentzVector FirstDaughterLV(FirstDaughterMomentum,Energy);
      
      FirstDaughterLV.Boost( NucleiLV.BoostVector() );
      TLorentzVector SecondDaughterLV = NucleiLV - FirstDaughterLV;
      
      G4ThreeVector DaughterDirection = G4ThreeVector( FirstDaughterLV.X()   ,
                                                      FirstDaughterLV.Y()   ,
                                                      FirstDaughterLV.Z()   );
      
      Particle FirstDaughterParticle( m_DaughterNuclei[0],
                                     FirstDaughterLV.E()-m_DaughterNuclei[0]->GetPDGMass(),
                                     DaughterDirection.unit(),
                                     decayingParticle.GetParticlePosition(),
                                     m_shoot[0]);
      
      DaughterDirection = G4ThreeVector( SecondDaughterLV.X()   ,
                                        SecondDaughterLV.Y()   ,
                                        SecondDaughterLV.Z()   );
      
      Particle SecondDaughterParticle( m_DaughterNuclei[1],
                                      SecondDaughterLV.E()-m_DaughterNuclei[1]->GetPDGMass(),
                                      DaughterDirection.unit(),
                                      decayingParticle.GetParticlePosition(),
                                      m_shoot[1]);
      
      ParticleStack::getInstance()->AddParticleToStack(FirstDaughterParticle);
      ParticleStack::getInstance()->AddParticleToStack(SecondDaughterParticle);
    }
  }
  
  // Case of a TGenPhaseSpace
  else{
    TLorentzVector NucleiLV( NucleiMomentum*Momentum.x()/GeV,
                            NucleiMomentum*Momentum.y()/GeV,
                            NucleiMomentum*Momentum.z()/GeV,
                            NucleiEnergy/GeV);
    
    if( !m_TPhaseSpace.SetDecay(NucleiLV, m_DaughterNuclei.size(), m_Masses) )
      cout << "Warning: Phase Space Decay forbiden by kinematic, or more than 18 particles, no particle emitted "<<endl;
    
    else{
      m_TPhaseSpace.Generate();
      
      TLorentzVector* daughterLV ;
      double KineticEnergy;
      
      for (unsigned int i = 0 ;  i < m_DaughterNuclei.size(); i++) {
        
        daughterLV = m_TPhaseSpace.GetDecay(i);
        G4ThreeVector daughterDirection = G4ThreeVector( daughterLV->X()   ,
                                                        daughterLV->Y()   ,
                                                        daughterLV->Z()   );
        
        KineticEnergy   = daughterLV->E()-m_Masses[i] ;
        
        Particle daughterParticle( m_DaughterNuclei[i],
                                  KineticEnergy*GeV,
                                  daughterDirection.unit(),
                                  decayingParticle.GetParticlePosition(),
                                  m_shoot[i]);
        ParticleStack::getInstance()->AddParticleToStack(daughterParticle);
      }
    }
  }
}


void EventGeneratorParticleDecay::SetTarget(Target* Target){
  m_Target = Target;
}

void EventGeneratorParticleDecay::SetDecay(vector<string> DaughterName, vector<bool> shoot, vector<double> ExcitationEnergy, string CSPath){
  
  // Check the validity of the given data:
  if (DaughterName.size() != shoot.size() || (DaughterName.size() != ExcitationEnergy.size() && ExcitationEnergy.size()!=0) ) {
    G4cout << "ERROR : Missmatching information: Getting out " << G4endl ;
    exit(1);
  }
  
  if ( DaughterName.size() != 2 && CSPath!="TGenPhaseSpace" ) {
    G4cout << "ERROR 2: Missmatching information: Getting out " << G4endl ;
    exit(1);
  }
  
  m_shoot = shoot ;
  m_ExcitationEnergy= ExcitationEnergy ;
  
  // If the Excitation Energy Token was omitted, then it is set to zero
  if(m_ExcitationEnergy.size()==0)
    for (unsigned int i = 0 ; i < DaughterName.size(); i++) {
      m_ExcitationEnergy.push_back(0);
    }
  
  // used check for mass and charge conservation
  Nucleus*  myNucleus = new Nucleus(m_MotherNucleiName);
  int InitialCharge = myNucleus->GetZ() ; int FinalCharge = 0 ;
  int InitialMass   = myNucleus->GetA() ; int FinalMass   = 0 ;
  delete myNucleus;
  for (unsigned int i = 0 ; i< DaughterName.size(); i++) {
    if(DaughterName[i] == "p"){
      m_DaughterNuclei.push_back(G4ParticleTable::GetParticleTable()->FindParticle("proton"));
      FinalMass++;FinalCharge++;
    }
    
    else if (DaughterName[i] == "n"){
      m_DaughterNuclei.push_back(G4ParticleTable::GetParticleTable()->FindParticle("neutron"));
      FinalMass++;
    }
    
    else{
      Nucleus*  myNucleus = new Nucleus(DaughterName[i]);
      m_DaughterNuclei.push_back(G4ParticleTable::GetParticleTable()->GetIon(myNucleus->GetZ(),
                                                                             myNucleus->GetA(),
                                                                             m_ExcitationEnergy[i]*MeV));
      FinalMass+=myNucleus->GetA();
      FinalCharge+=myNucleus->GetZ();
      delete myNucleus;
    }
  }
  
  // Check mass and charge conservation
  if (InitialMass!=FinalMass || InitialCharge!=FinalCharge) {
    G4cout << "ERROR: Mass and charge are not conserved." << G4endl;
    exit(1);
  }
  
  m_DaughterName = DaughterName;
  
  m_DifferentialCrossSection = CSPath;
  if(CSPath!="TGenPhaseSpace") {
    unsigned int CrossSectionSize = 0;
    double* m_CrossSection = new double[CrossSectionSize] ;
    double  m_CrossSectionThetaMin = 0;
    double  m_CrossSectionThetaMax = 0;
    
    string GlobalPath = getenv("NPTOOL");
    string StandardPath = GlobalPath + "/Inputs/CrossSection/" + m_DifferentialCrossSection;
    ifstream CSFile;
    CSFile.open( StandardPath.c_str() );
    
    if(CSFile.is_open()) cout << "Reading Cross Section File " << m_DifferentialCrossSection << endl;
    
    // In case the file is not found in the standard path, the programm try to interpret the file name as an absolute or relative file path.
    else{
      CSFile.open( m_DifferentialCrossSection.c_str() );
      if(CSFile.is_open()) {
        cout << "Reading Cross Section File " << m_DifferentialCrossSection << endl;
      }
      
      else {
        cout << "ERROR : Cross Section File " << m_DifferentialCrossSection << " not found" << endl;
        exit(1);
      }
    }
    
    double CSBuffer,AngleBuffer;
    vector<double> CrossSectionBuffer;
    m_CrossSectionThetaMin = 200;
    m_CrossSectionThetaMax = -10;
    while(!CSFile.eof()) {
      CSFile >> AngleBuffer;
      CSFile >> CSBuffer;
      double CSFinal = CSBuffer*sin(AngleBuffer*deg);
      CrossSectionBuffer.push_back(CSFinal);
      if (AngleBuffer < m_CrossSectionThetaMin) m_CrossSectionThetaMin = AngleBuffer;
      if (AngleBuffer > m_CrossSectionThetaMax) m_CrossSectionThetaMax = AngleBuffer;
    }
    
    CSFile.close();
    
    m_CrossSectionSize = CrossSectionBuffer.size();
    m_CrossSectionArray = new double[CrossSectionBuffer.size()] ;
    
    for(unsigned int i = 0 ; i < m_CrossSectionSize ; i++ ) {
      m_CrossSectionArray[i] = CrossSectionBuffer[i];
    }
    
    m_CrossSectionShoot = new RandGeneral(m_CrossSectionArray, m_CrossSectionSize);
    
  }
  
  
  else{
    // Set up the array of masses
    m_Masses = new double[m_DaughterNuclei.size()];
    
    // Mass of the daugther nuclei are set once
    for (unsigned int i = 0 ; i < m_DaughterNuclei.size(); i++) {
      m_Masses[i] = m_DaughterNuclei[i]->GetPDGMass()/GeV;
    }
    
  }
  
  // Change the name of the decaying nucleus to G4 standard
  m_MotherNucleiName = m_ParticleStack->ChangeNameToG4Standard(m_MotherNucleiName);
  
}