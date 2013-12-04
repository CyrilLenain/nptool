/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TiaraHyball treated data                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TTiaraHyballPhysics.h"
using namespace TiaraHyball_LOCAL;

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTiaraHyballPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTiaraHyballPhysics::TTiaraHyballPhysics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TTiaraHyballData ;
    m_PreTreatedData    = new TTiaraHyballData ;
    m_EventPhysics      = this ;
    m_NumberOfDetector = 0 ;
    m_MaximumStripMultiplicityAllowed = 10;
    m_StripEnergyMatchingSigma = 0.060    ;
    m_StripEnergyMatchingNumberOfSigma = 3;

    // Threshold
    m_StripRing_E_RAW_Threshold = 0 ;
    m_StripRing_E_Threshold = 0 ;

    m_StripSector_E_RAW_Threshold = 0 ;
    m_StripSector_E_Threshold = 0 ;

    m_Take_E_Ring=false;
    m_Take_T_Sector=true;
  }

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::BuildPhysicalEvent(){
  PreTreat();

  if( CheckEvent() == 1 ){
    vector< TVector2 > couple = Match_Ring_Sector() ;
    EventMultiplicity = couple.size();

    unsigned int size = couple.size();
    for(unsigned int i = 0 ; i < size ; ++i){

      int N = m_PreTreatedData->GetRingEDetectorNbr(couple[i].X()) ;
      int Ring = m_PreTreatedData->GetRingEStripNbr(couple[i].X()) ;
      int Sector  = m_PreTreatedData->GetSectorEStripNbr(couple[i].Y()) ;

      double Ring_E = m_PreTreatedData->GetRingEEnergy( couple[i].X() ) ;
      double Sector_E  = m_PreTreatedData->GetSectorEEnergy( couple[i].Y() ) ;


      // Search for associate Time:
      double Ring_T = -1000 ;
      unsigned int StripRingTMult = m_PreTreatedData->GetRingTMult(); 
      for(unsigned int t = 0 ; t < StripRingTMult ; ++t ){
        if(  m_PreTreatedData->GetRingEStripNbr( couple[i].X() ) == m_PreTreatedData->GetRingTStripNbr(t)
            &&m_PreTreatedData->GetRingEDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetRingTDetectorNbr(t))
          Ring_T = m_PreTreatedData->GetRingTTime(t);
      }

      // Search for associate Time:
      double Sector_T = -1000 ;
      unsigned int StripSectorTMult = m_PreTreatedData->GetSectorTMult(); 
      for(unsigned int t = 0 ; t < StripSectorTMult ; ++t ){
        if(  m_PreTreatedData->GetSectorEStripNbr( couple[i].X() ) == m_PreTreatedData->GetSectorTStripNbr(t)
            &&m_PreTreatedData->GetSectorEDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetSectorTDetectorNbr(t))
          Sector_T = m_PreTreatedData->GetSectorTTime(t);
      }

      DetectorNumber.push_back(N);
      StripRing_E.push_back(Ring_E);
      StripRing_T.push_back(Ring_T) ;
      StripSector_E.push_back(Sector_E) ;
      StripSector_T.push_back(Sector_T) ;

      if(m_Take_E_Ring)
        Strip_E.push_back(Ring_E) ;
      else
        Strip_E.push_back(Sector_E) ;

      if(m_Take_T_Sector)
        Strip_T.push_back(Sector_T) ;
      else
        Strip_T.push_back(Ring_T) ;

      Strip_Ring.push_back(Ring) ;
      Strip_Sector.push_back(Sector) ;

    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::PreTreat(){
  ClearPreTreatedData();

  //   Ring E
  unsigned int sizeRingE = m_EventData->GetRingEMult();
  for(unsigned int i = 0 ; i < sizeRingE ; ++i){
    if( m_EventData->GetRingEEnergy(i)>m_StripRing_E_RAW_Threshold && IsValidChannel("Ring", m_EventData->GetRingEDetectorNbr(i), m_EventData->GetRingEStripNbr(i)) ){
      double Ring_E = fStrip_Ring_E(m_EventData , i);
      if( Ring_E > m_StripRing_E_Threshold ){
        m_PreTreatedData->SetRingEDetectorNbr( m_EventData->GetRingEDetectorNbr(i) );
        m_PreTreatedData->SetRingEStripNbr( m_EventData->GetRingEStripNbr(i) );
        m_PreTreatedData->SetRingEEnergy( Ring_E );
      }
    }
  }

  //   Ring T
  unsigned int sizeRingT = m_EventData->GetRingTMult();
  for(unsigned int i = 0 ; i < sizeRingT ; ++i){
    if(IsValidChannel("Ring", m_EventData->GetRingTDetectorNbr(i), m_EventData->GetRingTStripNbr(i)) ){
      double Ring_T = fStrip_Ring_T(m_EventData , i);
      m_PreTreatedData->SetRingTDetectorNbr( m_EventData->GetRingTDetectorNbr(i) );
      m_PreTreatedData->SetRingTStripNbr( m_EventData->GetRingTStripNbr(i) );
      m_PreTreatedData->SetRingTTime( m_EventData->GetRingTTime(i) );
    }
  }

  //  Sector E
  unsigned int sizeSectorE = m_EventData->GetSectorEMult() ;
  for(unsigned int i = 0 ; i < sizeSectorE ; ++i){
    if( m_EventData->GetSectorEEnergy(i)>m_StripSector_E_RAW_Threshold && IsValidChannel("Sector", m_EventData->GetSectorEDetectorNbr(i), m_EventData->GetSectorEStripNbr(i)) ){
      double Sector_E = fStrip_Sector_E(m_EventData , i);
      if( Sector_E > m_StripSector_E_Threshold ){
        m_PreTreatedData->SetSectorEDetectorNbr( m_EventData->GetSectorEDetectorNbr(i) );
        m_PreTreatedData->SetSectorEStripNbr( m_EventData->GetSectorEStripNbr(i) );
        m_PreTreatedData->SetSectorEEnergy( Sector_E );
      }
    }
  }

  //  Sector T
  unsigned int sizeSectorT = m_EventData->GetSectorTMult() ;
  for(unsigned int i = 0 ; i < sizeSectorT ; ++i){
    if(IsValidChannel("Sector", m_EventData->GetSectorTDetectorNbr(i), m_EventData->GetSectorTStripNbr(i)) ){
      double Sector_T = fStrip_Sector_T(m_EventData , i);
      m_PreTreatedData->SetSectorTDetectorNbr( m_EventData->GetSectorTDetectorNbr(i) );
      m_PreTreatedData->SetSectorTStripNbr( m_EventData->GetSectorTStripNbr(i) );
      m_PreTreatedData->SetSectorTTime( m_EventData->GetSectorTTime(i) );
    }
  }
  return;
}


///////////////////////////////////////////////////////////////////////////
int TTiaraHyballPhysics :: CheckEvent(){
  // Check the size of the different elements
  if(m_PreTreatedData->GetSectorEMult() == m_PreTreatedData->GetRingEMult() )
    return 1 ; // Regular Event

  else
    return -1 ; // Rejected Event

}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TTiaraHyballPhysics :: Match_Ring_Sector(){
  vector < TVector2 > ArrayOfGoodCouple ;

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_PreTreatedData->GetRingEMult() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetSectorEMult() > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  for(unsigned int i = 0 ; i < m_PreTreatedData->GetRingEMult(); i++) {
    for(unsigned int j = 0 ; j < m_PreTreatedData->GetSectorEMult(); j++){
      //   if same detector check energy
      if ( m_PreTreatedData->GetRingEDetectorNbr(i) == m_PreTreatedData->GetSectorEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetRingEEnergy(i)-m_PreTreatedData->GetSectorEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
          ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
      }
    }
  }

  //   Prevent to treat event with ambiguous matchin beetween X and Y
  if( ArrayOfGoodCouple.size() > m_PreTreatedData->GetRingEMult() ) ArrayOfGoodCouple.clear() ;
  return ArrayOfGoodCouple;
}


////////////////////////////////////////////////////////////////////////////
bool TTiaraHyballPhysics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){

  if(DetectorType == "Ring")
    return *(m_RingChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Sector")
    return *(m_SectorChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigTiaraHyball.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigTiaraHyball.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigTiaraHyball.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigTiaraHyball.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigTiaraHyball") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {

      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo=="MAX_STRIP_MULTIPLICITY") {
        AnalysisConfigFile >> DataBuffer;
        m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
        cout << "MAXIMUN STRIP MULTIPLICITY " << m_MaximumStripMultiplicityAllowed << endl;
      }

      else if (whatToDo=="STRIP_ENERGY_MATCHING_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
        cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma <<endl;
      }

      else if (whatToDo=="STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str() );
        cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA " << m_StripEnergyMatchingNumberOfSigma << endl;
      }

      else if (whatToDo== "DISABLE_ALL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int Detector = atoi(DataBuffer.substr(2,1).c_str());
        vector< bool > ChannelStatus;
        ChannelStatus.resize(24,false);
        m_RingChannelStatus[Detector-1] = ChannelStatus;
        ChannelStatus.resize(48,false);
        m_SectorChannelStatus[Detector-1] = ChannelStatus;
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int Detector = atoi(DataBuffer.substr(2,1).c_str());
        int channel = -1;
        if (DataBuffer.compare(3,4,"STRF") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_RingChannelStatus[Detector-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(3,4,"STRB") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_SectorChannelStatus[Detector-1].begin()+channel-1) = false;
        }

        else cout << "Warning: detector type for TiaraHyball unknown!" << endl;

      }

      else if (whatToDo=="TAKE_E_RING") {
        m_Take_E_Ring = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_E_SECTOR") {
        m_Take_E_Ring = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_RING") {
        m_Take_T_Sector = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_SECTOR") {
        m_Take_T_Sector = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="STRIP_RING_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripRing_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripRing_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_SECTOR_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripSector_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripSector_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_RING_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripRing_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripRing_E_Threshold << endl;
      }

      else if (whatToDo=="STRIP_SECTOR_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripSector_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripSector_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::Clear(){
  EventMultiplicity = 0;

  //   Provide a Classification of Event
  EventType.clear() ;

  // Detector
  DetectorNumber.clear() ;

  //   DSSD
  Strip_E.clear() ;
  Strip_T.clear() ;
  StripRing_E.clear() ;
  StripRing_T.clear();
  StripSector_E.clear() ;
  StripSector_T.clear() ;
  Strip_Ring.clear() ;
  Strip_Sector.clear() ;

}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;

  double R,Phi,Z;
  R = 0 ; Phi = 0 ; Z = 0;
  TVector3 Pos;
  bool check_R   = false ;
  bool check_Phi = false ;
  bool check_Z   = false ;

  bool ReadingStatusWedge = false ;
  bool ReadingStatusBOX = false ;
  bool ReadingStatus    = false ;

  bool VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel(); ;

  while (!ConfigFile.eof()){

    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 5, "TiaraHyball") == 0)
      ReadingStatus = true;

    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      //   Hyball case
      if (DataBuffer=="TiaraHyballWedge"){
        if(VerboseLevel) cout << "///" << endl           ;
        if(VerboseLevel) cout << "Wedge found: " << endl   ;
        ReadingStatusWedge = true ;
      }

      //   Reading Block
      while(ReadingStatusWedge){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;

        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Z= " << Z << "mm" << endl;
        }

        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  R= " << R << "mm" << endl;
        }

        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Phi= " << Phi << "deg" << endl;
        }

        else if (DataBuffer == "ThicknessDector=") {
          /*ignore that*/
        }

        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusWedge = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }

        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out

        if (check_R && check_Phi && check_Z){

          ReadingStatusWedge = false;
          AddWedgeDetector(R,Phi,Z);
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
        }
      }
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitSpectra(){  
   m_Spectra = new TTiaraHyballSpectra();
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::FillSpectra(){  
   m_Spectra -> FillRawSpectra(m_EventData);
   m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
   m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::CheckSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    for( int j = 0 ; j < 24 ; ++j){
      Cal->AddParameter("TIARAHYBALL", "D"+itoa(i+1)+"_STRIP_RING"+itoa(j+1)+"_E","TIARAHYBALL_D"+itoa(i+1)+"_STRIP_RING"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARAHYBALL", "D"+itoa(i+1)+"_STRIP_RING"+itoa(j+1)+"_T","TIARAHYBALL_D"+itoa(i+1)+"_STRIP_RING"+itoa(j+1)+"_T")   ;
    }

    for( int j = 0 ; j < 48 ; ++j){
      Cal->AddParameter("TIARAHYBALL", "D"+itoa(i+1)+"_STRIP_SECTOR"+itoa(j+1)+"_E","TIARAHYBALL_D"+itoa(i+1)+"_STRIP_SECTOR"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARAHYBALL", "D"+itoa(i+1)+"_STRIP_SECTOR"+itoa(j+1)+"_T","TIARAHYBALL_D"+itoa(i+1)+"_STRIP_SECTOR"+itoa(j+1)+"_T")   ;
    }
  }
  return;

}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "TiaraHyball" , true )               ;
  inputChain->SetBranchStatus( "fTiaraHyball_*" , true )               ;
  inputChain->SetBranchAddress( "TiaraHyball" , &m_EventData )      ;

}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "EventMultiplicity" , true );
  inputChain->SetBranchStatus( "EventType" , true );
  inputChain->SetBranchStatus( "DetectorNumber" , true );
  inputChain->SetBranchStatus( "Strip_E" , true );
  inputChain->SetBranchStatus( "Strip_T" , true );
  inputChain->SetBranchStatus( "StripRing_E" , true );
  inputChain->SetBranchStatus( "StripSector_T" , true );
  inputChain->SetBranchStatus( "StripRing_E" , true );
  inputChain->SetBranchStatus( "StripSector_T" , true );
  inputChain->SetBranchStatus( "Strip_Ring" , true );
  inputChain->SetBranchStatus( "Strip_Sector" , true );
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "TiaraHyball" , "TTiaraHyballPhysics" , &m_EventPhysics );
}


/////   Specific to TiaraHyballArray   ////
void TTiaraHyballPhysics::AddWedgeDetector( double R,double Phi,double Z){

  double Wedge_R_Min = 9.+R;
  double Wedge_R_Max = 41.0+R;

  double Wedge_Phi_Min = 2.0*M_PI/180.  ;
  double Wedge_Phi_Max = 83.6*M_PI/180. ;
  Phi= Phi*M_PI/180.;

  int    Wedge_Ring_NumberOfStrip = 16 ;
  int    Wedge_Sector_NumberOfStrip = 24 ;

  double StripPitchSector = (Wedge_Phi_Max-Wedge_Phi_Min)/Wedge_Sector_NumberOfStrip ;
  double StripPitchRing = (Wedge_R_Max-Wedge_R_Min)/Wedge_Ring_NumberOfStrip  ; 

  TVector3 Strip_1_1;

  m_NumberOfDetector++;
  Strip_1_1=TVector3(0,0,Z);

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneWedgeStripPositionX   ;
  vector< vector< double > >   OneWedgeStripPositionY   ;
  vector< vector< double > >   OneWedgeStripPositionZ   ;

  TVector3 StripCenter = Strip_1_1;
  for(int f = 0 ; f < Wedge_Ring_NumberOfStrip ; f++){
    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for(int b = 0 ; b < Wedge_Sector_NumberOfStrip ; b++){
      StripCenter = Strip_1_1;
      StripCenter.SetY(Wedge_R_Max-f*StripPitchRing);
      StripCenter.SetZ(Z);
      StripCenter.RotateZ(Phi+Wedge_Phi_Min+b*StripPitchSector);
      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }
    OneWedgeStripPositionX.push_back(lineX);
    OneWedgeStripPositionY.push_back(lineY);
    OneWedgeStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back( OneWedgeStripPositionX ) ;
  m_StripPositionY.push_back( OneWedgeStripPositionY ) ;
  m_StripPositionZ.push_back( OneWedgeStripPositionZ ) ;

  return;
}

TVector3 TTiaraHyballPhysics::GetDetectorNormal( const int i) const{
  /*  TVector3 U =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) )

      -TVector3 ( GetStripPositionX( DetectorNumber[i] , 1 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 1 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 1 , 1 ) );

      TVector3 V =    TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 48 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 48 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 48 ) )

      -TVector3 ( GetStripPositionX( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionY( DetectorNumber[i] , 24 , 1 ) ,
      GetStripPositionZ( DetectorNumber[i] , 24 , 1 ) );

      TVector3 Normal = U.Cross(V);

      return(Normal.Unit()) ;*/

  return (TVector3(0,0,i));

}

TVector3 TTiaraHyballPhysics::GetPositionOfInteraction(const int i) const{
  TVector3 Position = TVector3 ( GetStripPositionX(DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] )    ,
      GetStripPositionY( DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] )    ,
      GetStripPositionZ( DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] )    ) ;

  return(Position) ;

}

void TTiaraHyballPhysics::InitializeStandardParameter(){
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_RingChannelStatus.clear()    ;
  m_SectorChannelStatus.clear()    ;

  ChannelStatus.resize(24,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_RingChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(48,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_SectorChannelStatus[i] = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfDetector   ;

  return;
}


///////////////////////////////////////////////////////////////////////////
namespace TiaraHyball_LOCAL{

  //   transform an integer to a string
  string itoa(unsigned int value){
    char buffer [33];
    sprintf(buffer,"%d",value);
    return buffer;
  }

  //   DSSD
  //   Ring
  double fStrip_Ring_E(const TTiaraHyballData* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "TIARAHYBALL/D" + itoa( m_EventData->GetRingEDetectorNbr(i) ) + "_STRIP_RING" + itoa( m_EventData->GetRingEStripNbr(i) ) + "_E",
        m_EventData->GetRingEEnergy(i) );
  }

  double fStrip_Ring_T(const TTiaraHyballData* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "TIARAHYBALL/D" + itoa( m_EventData->GetRingTDetectorNbr(i) ) + "_STRIP_RING" + itoa( m_EventData->GetRingTStripNbr(i) ) +"_T",
        m_EventData->GetRingTTime(i) );
  }

  //   Sector
  double fStrip_Sector_E(const TTiaraHyballData* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "TIARAHYBALL/D" + itoa( m_EventData->GetSectorTDetectorNbr(i) ) + "_STRIP_SECTOR" + itoa( m_EventData->GetSectorTStripNbr(i) ) +"_E",
        m_EventData->GetSectorEEnergy(i) );
  }

  double fStrip_Sector_T(const TTiaraHyballData* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "TIARAHYBALL/D" + itoa( m_EventData->GetSectorTDetectorNbr(i) ) + "_STRIP_SECTOR" + itoa( m_EventData->GetSectorTStripNbr(i) ) +"_T",
        m_EventData->GetRingTTime(i) );
  }

}

