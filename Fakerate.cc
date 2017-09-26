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
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 0.101664975286;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.0830161347985;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.0256090071052;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.023337462917;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.0124889658764;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.0113863414153;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.00899231806397;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.0098355030641;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.00631246017292;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.00731991883367;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.00562235759571;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.00516007607803;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.00459103193134;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.0045042000711;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.0039202561602;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.00343218748458;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.0032991275657;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.00308201019652;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.00250359415077;
    else if( jet_nTrack>=20 && jet_nTrack<21 ) fakerate = 0.00239912187681;
    else if( jet_nTrack>=21 && jet_nTrack<22 ) fakerate = 0.00251414859667;
    else if( jet_nTrack>=22 && jet_nTrack<23 ) fakerate = 0.0021813288331;
    else if( jet_nTrack>=23 && jet_nTrack<24 ) fakerate = 0.00190320878755;
    else if( jet_nTrack>=24 && jet_nTrack<25 ) fakerate = 0.00156756851356;
    else if( jet_nTrack>=25 && jet_nTrack<26 ) fakerate = 0.00156032282393;
    else if( jet_nTrack>=26 && jet_nTrack<27 ) fakerate = 0.00142234389205;
    else if( jet_nTrack>=27 && jet_nTrack<28 ) fakerate = 0.00138833827805;
    else if( jet_nTrack>=28 && jet_nTrack<29 ) fakerate = 0.0010712997755;
    else if( jet_nTrack>=29 && jet_nTrack<30 ) fakerate = 0.00111353571992;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000908015645109;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000590471900068;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000445289391791;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000271240394795;
    else if( jet_nTrack>=60 ) fakerate = 0.000243782022153;
  }

  else if( option== 1){ // light quark (u, d, s, c) or gluon jet
    if( jet_nTrack>=0 && jet_nTrack<1 ) fakerate = 0.0;
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 0.107380904257;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.0577206835151;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.0176952295005;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.0193438455462;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.00790952891111;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.00750857172534;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.00572516489774;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.00627888087183;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.00401736795902;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.00453061703593;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.00372729101218;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.00359169836156;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.00321470154449;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.0033162466716;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.00304777361453;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.00253053079359;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.00272737978958;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.00232243770733;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.0020687377546;
    else if( jet_nTrack>=20 && jet_nTrack<21 ) fakerate = 0.00191911798902;
    else if( jet_nTrack>=21 && jet_nTrack<22 ) fakerate = 0.00214445334859;
    else if( jet_nTrack>=22 && jet_nTrack<23 ) fakerate = 0.00199990044348;
    else if( jet_nTrack>=23 && jet_nTrack<24 ) fakerate = 0.00160041241907;
    else if( jet_nTrack>=24 && jet_nTrack<25 ) fakerate = 0.00130167009775;
    else if( jet_nTrack>=25 && jet_nTrack<26 ) fakerate = 0.00129874749109;
    else if( jet_nTrack>=26 && jet_nTrack<27 ) fakerate = 0.00119776744395;
    else if( jet_nTrack>=27 && jet_nTrack<28 ) fakerate = 0.00114099634811;
    else if( jet_nTrack>=28 && jet_nTrack<29 ) fakerate = 0.000955790281296;
    else if( jet_nTrack>=29 && jet_nTrack<30 ) fakerate = 0.00100709416438;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.000851855787914;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.000514374638442;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.00044416313176;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.000292580312816;
    else if( jet_nTrack>=60 ) fakerate = 0.00026265764609;
  }

  else if( option == 2 ){ // B jet fakerate
    if( jet_nTrack>=0 && jet_nTrack<1 ) fakerate = 0.0;
    else if( jet_nTrack>=1 && jet_nTrack<2 ) fakerate = 0.191868424416;
    else if( jet_nTrack>=2 && jet_nTrack<3 ) fakerate = 0.333265721798;
    else if( jet_nTrack>=3 && jet_nTrack<4 ) fakerate = 0.222120016813;
    else if( jet_nTrack>=4 && jet_nTrack<5 ) fakerate = 0.108483709395;
    else if( jet_nTrack>=5 && jet_nTrack<6 ) fakerate = 0.130471453071;
    else if( jet_nTrack>=6 && jet_nTrack<7 ) fakerate = 0.0867355689406;
    else if( jet_nTrack>=7 && jet_nTrack<8 ) fakerate = 0.0748661831021;
    else if( jet_nTrack>=8 && jet_nTrack<9 ) fakerate = 0.0772459581494;
    else if( jet_nTrack>=9 && jet_nTrack<10 ) fakerate = 0.0506395287812;
    else if( jet_nTrack>=10 && jet_nTrack<11 ) fakerate = 0.059877179563;
    else if( jet_nTrack>=11 && jet_nTrack<12 ) fakerate = 0.0428085550666;
    else if( jet_nTrack>=12 && jet_nTrack<13 ) fakerate = 0.0376959592104;
    else if( jet_nTrack>=13 && jet_nTrack<14 ) fakerate = 0.0318800993264;
    else if( jet_nTrack>=14 && jet_nTrack<15 ) fakerate = 0.0301347784698;
    else if( jet_nTrack>=15 && jet_nTrack<16 ) fakerate = 0.0230390951037;
    else if( jet_nTrack>=16 && jet_nTrack<17 ) fakerate = 0.0227218512446;
    else if( jet_nTrack>=17 && jet_nTrack<18 ) fakerate = 0.0158321391791;
    else if( jet_nTrack>=18 && jet_nTrack<19 ) fakerate = 0.0198232010007;
    else if( jet_nTrack>=19 && jet_nTrack<20 ) fakerate = 0.0119885224849;
    else if( jet_nTrack>=20 && jet_nTrack<21 ) fakerate = 0.012777495198;
    else if( jet_nTrack>=21 && jet_nTrack<22 ) fakerate = 0.0105151096359;
    else if( jet_nTrack>=22 && jet_nTrack<23 ) fakerate = 0.00613382365555;
    else if( jet_nTrack>=23 && jet_nTrack<24 ) fakerate = 0.00846456736326;
    else if( jet_nTrack>=24 && jet_nTrack<25 ) fakerate = 0.00703204562888;
    else if( jet_nTrack>=25 && jet_nTrack<26 ) fakerate = 0.0070224958472;
    else if( jet_nTrack>=26 && jet_nTrack<27 ) fakerate = 0.00610577268526;
    else if( jet_nTrack>=27 && jet_nTrack<28 ) fakerate = 0.00651229778305;
    else if( jet_nTrack>=28 && jet_nTrack<29 ) fakerate = 0.00328681175597;
    else if( jet_nTrack>=29 && jet_nTrack<30 ) fakerate = 0.00315665244125;
    else if( jet_nTrack>=30 && jet_nTrack<35 ) fakerate = 0.00194204156287;
    else if( jet_nTrack>=35 && jet_nTrack<40 ) fakerate = 0.00187294895295;
    else if( jet_nTrack>=40 && jet_nTrack<50 ) fakerate = 0.000462533032987;
    else if( jet_nTrack>=50 && jet_nTrack<60 ) fakerate = 0.0;
    else if( jet_nTrack>=60 ) fakerate = 0.0;
  }
  
  return fakerate;
}
