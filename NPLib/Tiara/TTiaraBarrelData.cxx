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
 *  This class hold Tiara Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

#include "TTiaraBarrelData.h"

ClassImp(TTiaraBarrelData)
  /////////////////////////
  TTiaraBarrelData::TTiaraBarrelData(){

    // Reserve the space for the full array 
    fTiaraBarrel_FrontUpstreamE_DetectorNbr.reserve(8*4);         
    fTiaraBarrel_FrontUpstreamE_StripNbr.reserve(8*4);         
    fTiaraBarrel_FrontUpstreamE_Energy.reserve(8*4);        

    fTiaraBarrel_FrontDownstreamE_DetectorNbr.reserve(8*4);     
    fTiaraBarrel_FrontDownstreamE_StripNbr.reserve(8*4);           
    fTiaraBarrel_FrontDownstreamE_Energy.reserve(8*4);          

    fTiaraBarrel_BackE_DetectorNbr.reserve(8);                  
    fTiaraBarrel_BackE_Energy.reserve(8);                    

    fTiaraBarrel_OuterE_DetectorNbr.reserve(8*4);               
    fTiaraBarrel_OuterE_StripNbr.reserve(8*4);                    
    fTiaraBarrel_OuterE_Energy.reserve(8*4);                        
  
    fTiaraBarrel_FrontUpstreamT_DetectorNbr.reserve(8*4);         
    fTiaraBarrel_FrontUpstreamT_StripNbr.reserve(8*4);         
    fTiaraBarrel_FrontUpstreamT_Time.reserve(8*4);          

    fTiaraBarrel_FrontDownstreamT_DetectorNbr.reserve(8*4);     
    fTiaraBarrel_FrontDownstreamT_StripNbr.reserve(8*4);           
    fTiaraBarrel_FrontDownstreamT_Time.reserve(8*4);            

    fTiaraBarrel_BackT_DetectorNbr.reserve(8);                  
    fTiaraBarrel_BackT_Time.reserve(8);                      

    fTiaraBarrel_OuterT_DetectorNbr.reserve(8*4);               
    fTiaraBarrel_OuterT_StripNbr.reserve(8*4);                    
    fTiaraBarrel_OuterT_Time.reserve(8*4);


}

/////////////////////////
TTiaraBarrelData::~TTiaraBarrelData(){
  Clear();
}

/////////////////////////
void TTiaraBarrelData::Clear(){
  fTiaraBarrel_FrontUpstreamE_DetectorNbr.clear();         
  fTiaraBarrel_FrontUpstreamE_StripNbr.clear();         
  fTiaraBarrel_FrontUpstreamE_Energy.clear();        

  fTiaraBarrel_FrontDownstreamE_DetectorNbr.clear();     
  fTiaraBarrel_FrontDownstreamE_StripNbr.clear();           
  fTiaraBarrel_FrontDownstreamE_Energy.clear();          

  fTiaraBarrel_BackE_DetectorNbr.clear();                  
  fTiaraBarrel_BackE_Energy.clear();                    

  fTiaraBarrel_OuterE_DetectorNbr.clear();               
  fTiaraBarrel_OuterE_StripNbr.clear();                    
  fTiaraBarrel_OuterE_Energy.clear();                        
  
  fTiaraBarrel_FrontUpstreamT_DetectorNbr.clear();        
  fTiaraBarrel_FrontUpstreamT_StripNbr.clear();         
  fTiaraBarrel_FrontUpstreamT_Time.clear();          

  fTiaraBarrel_FrontDownstreamT_DetectorNbr.clear();     
  fTiaraBarrel_FrontDownstreamT_StripNbr.clear();           
  fTiaraBarrel_FrontDownstreamT_Time.clear();            

  fTiaraBarrel_BackT_DetectorNbr.clear();                  
  fTiaraBarrel_BackT_Time.clear();                      

  fTiaraBarrel_OuterT_DetectorNbr.clear();               
  fTiaraBarrel_OuterT_StripNbr.clear();                    
  fTiaraBarrel_OuterT_Time.clear();



}

/////////////////////////
void TTiaraBarrelData::Dump() const{
/*  cout << "************* TTiaraBarrelData Event ************"<< endl;
  cout << "*** Inner Barrel Front Upstream ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Front_Upstream_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Front_Upstream_DetectorNbr[i] << endl ;
    cout << "Strip Number = " 
      << fTiaraBarrel_Front_Upstream_StripNbr[i] << endl;
    cout << "Energy = " 
      << fTiaraBarrel_Front_Upstream_Energy[i]<< endl ;
    cout << "Time = " 
      << fTiaraBarrel_Front_Upstream_Time[i] << endl ;  
  }


  cout << "*** Inner Barrel Front Downstream ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Front_Downstream_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Front_Downstream_DetectorNbr[i] << endl;  
    cout << "Strip Number = " 
      << fTiaraBarrel_Front_Downstream_StripNbr[i] << endl;
    cout << "Energy = " 
      << fTiaraBarrel_Front_Downstream_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Front_Downstream_Time[i] << endl ;  
  }

  cout << "*** Inner Barrel Back ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Back_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Back_DetectorNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraBarrel_Back_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Back_Time[i] << endl ;  
  }
  cout << "*** Outer Barrel ***" << endl;
  for( unsigned int i = 0; i < fTiaraBarrel_Outer_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Outer_DetectorNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraBarrel_Outer_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Outer_Time[i] << endl ;  
  }*/
}

