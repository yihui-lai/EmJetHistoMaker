#include "Fakerate.h"

//
// calculate the probability/weight of one event with fr[4] and nTags
//
double PnTag(double fr[], int nTag){
  double p_nTag = 0;
  if( nTag == 0){
    p_nTag = (1-fr[0]) * (1-fr[1]) * (1-fr[2]) * (1-fr[3]);
  }
  else if( nTag == 1 ){
    p_nTag  =   fr[0]   * (1-fr[1]) * (1-fr[2]) * (1-fr[3]); 
    p_nTag += (1-fr[0]) *   fr[1]   * (1-fr[2]) * (1-fr[3]);
    p_nTag += (1-fr[0]) * (1-fr[1]) *   fr[2]   * (1-fr[3]);
    p_nTag += (1-fr[0]) * (1-fr[1]) * (1-fr[2]) *   fr[3]  ;
  }
  else if( nTag == 2 ){
    p_nTag  = (1-fr[0]) * (1-fr[1]) *   fr[2]   *   fr[3]  ;
    p_nTag += (1-fr[0]) *   fr[1]   * (1-fr[2]) *   fr[3]  ;
    p_nTag +=   fr[0]   * (1-fr[1]) * (1-fr[2]) *   fr[3]  ;
    p_nTag += (1-fr[0]) *   fr[1]   *   fr[2]   * (1-fr[3]);
    p_nTag +=   fr[0]   * (1-fr[1]) *   fr[2]   * (1-fr[3]);
    p_nTag +=   fr[0]   *   fr[1]   * (1-fr[2]) * (1-fr[3]);
  }
  else {
    std::cout << " Error: events weight with nTag = " << nTag << " has not been implemented yet!!!" << std::endl;
    return 0;
  }
  return p_nTag;
}

//
// compute the probability/weight of ijet is tagged as emerging
// in the nTag case(nTag = 0, 1, 2)
//
double PEmergingnTag(double fr[], int nTag, int ijet){
  if( ijet >=4 ) {
    std::cout << " jet index out when calculating Emerging probability" << std::endl; 
    return 0;
  }

  double prob = 0.0;
  if ( nTag==1 ) {
    prob = fr[ijet];
    for(int ij = 0; ij<4; ij++){
      if(ij==ijet) continue;
      prob *= (1-fr[ij]);
    }
  }
  if( nTag==2 ){
    for(int ij1 = 0; ij1<4; ij1++){
      if( ij1== ijet ) continue;
      double prob_temp = fr[ijet] * fr[ij1];
      for(int ij2 = 0; ij2<4; ij2++){
        if( ij2==ij1 || ij2==ijet ) continue;
        prob_temp *= (1-fr[ij2]); 
      }
      prob += prob_temp;
    }
  }

  if( prob>1.0 || (prob<0 && nTag!=0 ) ){
    std::cout << " Error/DEBUG: probability calculation problem !!! " << std::endl;
  }
  return prob;
}

double frCal(int jet_nTrack, int option){
  // option 0 : overall fakerate
  // option 1 : tagged as light quark or gluon
  // option 2 : tagged as b quark
  double fakerate = -10.0;
  if (option == 0) { // Overall fakerate
    if( jet_nTrack>=0 && jet_nTrack<1 ) fakerate = 0.0;
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 0.100166484714;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.0247390083969;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.025094570592;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.0132361305878;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.00686093093827;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.00804169103503;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.00540071493015;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.00506141874939;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.00349486316554;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.00471088476479;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.00412426237017;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.0043537649326;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.00337082473561;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.00357077131048;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.0034410469234;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.0036155579146;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.00243874266744;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.00232653692365;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.00158669997472;
    else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00201801001094;
    else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00199463148601;
    else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00125382083934;
    else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00152524199802;
    else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000854579207953;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000498356763273;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000146453472553;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
    else if( jet_nTrack>=60 ) fakerate = 0.0;
  }    
  else if( option== 1){ // light quark (u, d, s, c) or gluon jet
    if( jet_nTrack>=0 && jet_nTrack<1 ) fakerate = 0.0;
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 0.0959333628416;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.0207948312163;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.0174008868635;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.0117111718282;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.00526357349008;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.00674029812217;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.00410299515352;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.0036684286315;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.00278205564246;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.00308293313719;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.00290068984032;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.00310750422068;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.00272023258731;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.00235459092073;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.00231097382493;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.00303528620861;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.00167094310746;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.00190366036259;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.00113670190331;
    else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00137563748285;
    else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00177181069739;
    else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00105632282794;
    else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00123635912314;
    else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000830400211271;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000331299612299;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.0;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
    else if( jet_nTrack>=60 ) fakerate = 0.0;
  }
  else if( option == 2 ){ // B jet fakerate
    if( jet_nTrack>=0 && jet_nTrack<1 ) fakerate = 0.0;
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 1.0;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.414214074612;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.432087123394;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.182697549462;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.0988489612937;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.0754975825548;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.0700974613428;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.0662532374263;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.0303324125707;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.0717951059341;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.0503005683422;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.048179320991;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.0268137957901;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.0422920063138;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.0404816456139;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.0218178126961;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.0276208892465;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.0161321684718;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.01694730483;
    else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.0213858038187;
    else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00875690113753;
    else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00663258507848;
    else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00860296469182;
    else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00152849056758;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00412552850321;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00303404405713;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
    else if( jet_nTrack>=60 ) fakerate = 0.0;
  }
  return fakerate;
}

double frCal(int jet_nTrack, double jet_pt, int option){
  // option 0 : overall fakerate
  // option 1 : tagged as light quark or gluon
  // option 2 : tagged as b quark
  double fakerate = -10.0;
  if (option == 0) { // Overall fakerate
    if( jet_pt>100.0 && jet_pt<200.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.104624725878;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0336811430752;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0185735039413;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0107710435987;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.0089674051851;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00579061172903;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00353524740785;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00237612891942;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00186278251931;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00130201829597;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00118419725914;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.000834538484924;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.000814502127469;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.000935943680815;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000221588867134;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000888386217412;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.0;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>200.0 && jet_pt<400.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0994125157595;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0485784523189;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0170706380159;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0105884103104;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00826636888087;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00699687376618;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00520219001919;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00480104703456;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00300946296193;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00232997490093;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00183064106386;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00133575173095;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.000943261256907;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.000828003976494;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000576545018703;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000408401305322;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000359799101716;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>400.0 && jet_pt<600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0431056134403;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0341437794268;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.00986595451832;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00754955969751;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00596704287454;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00666033988819;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00592957343906;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.0048815519549;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00494266394526;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.0037634675391;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00342831155285;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.0028348385822;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00202087592334;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00160522689112;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00123563501984;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000727681617718;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000372641632566;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000270172167802;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000100766017567;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0583058670163;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0267733912915;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0104341953993;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00821194984019;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00555273750797;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00525155570358;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00558680156246;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00559000205249;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00586880929768;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00629967357963;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00529074762017;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00467286817729;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00344432797283;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00301956594922;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00231192493811;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00212585390545;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00112336536404;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000730399857275;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000343736348441;
      else if( jet_nTrack>=60 ) fakerate = 0.000274066725979;
    }
  }

  else if( option== 1){ // light quark (u, d, s, c) or gluon jet
    if( jet_pt>100.0 && jet_pt<200.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.118093810976;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0263213478029;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0125157283619;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00586608238518;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00453890301287;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00300107430667;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.0017334963195;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00152390473522;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00102888501715;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.000678213487845;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00060539302649;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00054164766334;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.000545933435205;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.000278130872175;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000233356782701;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000105339364382;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.0;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>200.0 && jet_pt<400.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.103690862656;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0290060099214;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0127701712772;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00694444077089;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00546867633238;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00413556396961;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00349979428574;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00331296236254;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00209198449738;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00159302575048;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00130255729891;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00104617304169;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00067440525163;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.000569661089685;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.000349781883415;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000392878049752;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000200739610591;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>400.0 && jet_pt<600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0433036349714;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.025025434792;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.00749427033588;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00589420134202;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00480091478676;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.0054226461798;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00504020415246;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00427843118086;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00448017287999;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00325492792763;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00322008179501;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00264195422642;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.0016616868088;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00137686519884;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00118152715731;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000667242216878;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000286371912807;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000267928553512;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000108611668111;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0603301264346;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0256872717291;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0095713250339;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.00743044447154;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.00495054619387;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.00508152646944;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.00516261672601;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.00544065190479;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.00569420913234;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00621439749375;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00503201643005;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00456226523966;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00341718341224;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00298336963169;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00232610758394;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00206807581708;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.0011208698852;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000729362247512;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000370703870431;
      else if( jet_nTrack>=60 ) fakerate = 0.000296966347378;
    }
  }

  else if( option == 2 ){ // B jet fakerate
    if( jet_pt>100.0 && jet_pt<200.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.333333343267;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0588471964002;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.137997329235;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0878701359034;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.0811599344015;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.0568427406251;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.04003219679;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.0218352451921;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.0199393071234;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.0153419589624;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.014224470593;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00715398974717;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.0062225535512;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.0134717756882;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.0;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.0140187963843;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.0;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>200.0 && jet_pt<400.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.405821681023;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.113858900964;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0798508524895;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.0557395704091;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.0531061738729;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.0345000736415;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.0327710136771;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.0212967507541;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.0171563327312;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.0124597707763;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00712510943413;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00627397326753;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.0059852944687;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00471364706755;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000682444311678;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00275990157388;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.0;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>400.0 && jet_pt<600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.485910713673;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.11949570477;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0579301342368;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.0387420468032;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.0380209796131;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.0276529341936;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.0194320622832;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.0153928231448;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.0153243467212;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.00802886951715;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.0073999511078;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00937055796385;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00630882615224;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00225425488316;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00183408951852;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00182414695155;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000303781445837;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
    else if( jet_pt>600.0 ){
      if( jet_nTrack>=0 && jet_nTrack<2 ) fakerate = 0.0;
      else if( jet_nTrack>=2 && jet_nTrack<4 ) fakerate = 0.0;
      else if( jet_nTrack>=4 && jet_nTrack<6 ) fakerate = 0.0508281700313;
      else if( jet_nTrack>=6 && jet_nTrack<8 ) fakerate = 0.0458525121212;
      else if( jet_nTrack>=8 && jet_nTrack<10 ) fakerate = 0.03476318717;
      else if( jet_nTrack>=10 && jet_nTrack<12 ) fakerate = 0.0132932476699;
      else if( jet_nTrack>=12 && jet_nTrack<14 ) fakerate = 0.0228286609054;
      else if( jet_nTrack>=14 && jet_nTrack<16 ) fakerate = 0.0115064382553;
      else if( jet_nTrack>=16 && jet_nTrack<18 ) fakerate = 0.0119919022545;
      else if( jet_nTrack>=18 && jet_nTrack<20 ) fakerate = 0.00919837504625;
      else if( jet_nTrack>=20 && jet_nTrack<22 ) fakerate = 0.0130496984348;
      else if( jet_nTrack>=22 && jet_nTrack<24 ) fakerate = 0.00770313898101;
      else if( jet_nTrack>=24 && jet_nTrack<26 ) fakerate = 0.00417155399919;
      else if( jet_nTrack>=26 && jet_nTrack<28 ) fakerate = 0.00395212881267;
      else if( jet_nTrack>=28 && jet_nTrack<30 ) fakerate = 0.00197921553627;
      else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00329894502647;
      else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00116969249211;
      else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000746676174458;
      else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
      else if( jet_nTrack>=60 ) fakerate = 0.0;
    }
  }
  
  return fakerate;
}