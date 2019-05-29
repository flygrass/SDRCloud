/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
 * Generated on: 28-May-2019 23:45:44
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 324
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * D:\MATLAB\extern\include\tmwtypes.h 
 */
const int FIR_2048K_TO_128K_LEN = 324;
const real64_T FIR_2048K_TO_128K[324] = {
  0.0002331713117823, 0.005909110665268, 0.001133544531306, 0.001454271681667,
   0.001564861117002, 0.001674310036821, 0.001771495736385, 0.001855197473065,
   0.001922689140737, 0.001972710744386, 0.002003008418818, 0.002012718737072,
   0.002000147240947, 0.001965026824597, 0.001906208940547, 0.001824403637312,
   0.001719231360746,  0.00159218406654, 0.001443534895053, 0.001275879750449,
   0.001090251521853,0.0008898270446198,0.0006763598205704,0.0004535974474663,
  0.0002241246715358,-8.284912890685e-06,-0.0002402536527941,-0.0004674223790094,
  -0.0006868876917276,-0.0008937674023519,-0.001085092783342,-0.001256758504183,
   -0.00140585370107,-0.001528819773884,-0.001624182877133,-0.001688792859582,
  -0.001721917024894,-0.001721842580378,-0.001688372921984,-0.001620429425351,
  -0.001519449896197,-0.001385435914823, -0.00122120757226,-0.001028364760688,
  -0.0008114246529234,-0.0005730747758181,-0.0003181573504889,-4.960599793222e-05,
  0.0002272255330856,0.0005085765970053,0.0007874461271234, 0.001058610517393,
    0.00131486774926, 0.001553614365528,  0.00176995804329, 0.001960816823846,
    0.00211617437497, 0.002233500990202, 0.002312117691407,  0.00235344615244,
   0.002342069992321, 0.002289570351872, 0.002192539076032, 0.002045333948685,
   0.001859315962532, 0.001627500678701, 0.001360076004247, 0.001055817014933,
  0.0007232378317998,0.0003648302716825,-1.084735955294e-05,-0.0003992275644469,
  -0.0007916092871739,-0.001181996331938,-0.001561506784222, -0.00192370915864,
   -0.00226014671792,-0.002564489784499,-0.002829013959429,-0.003048368221369,
  -0.003215731471424, -0.00332722585362,-0.003377445256933,-0.003364956420185,
  -0.003286274834997,-0.003141894414611,-0.002931665741432,-0.002657581747387,
  -0.002322239904772,-0.001930564220738,-0.001486822750587,-0.0009986500242702,
  -0.000473234789318,8.050783114032e-05,0.0006534144814008, 0.001234524962777,
    0.00181427769259, 0.002381055693405, 0.002923922194491, 0.003430986346463,
   0.003891768805491, 0.004295142282385, 0.004631853787874, 0.004892467339051,
   0.005069630990645, 0.005155524413225, 0.005145046928966, 0.005033987490215,
    0.00482125990184, 0.004505675603702, 0.004088516726408, 0.003572290515803,
    0.00296351002348, 0.002269090940775, 0.001497383079786,0.0006583780470531,
  -0.000233476321428,-0.001165479934851,-0.002123081447825, -0.00308856300883,
  -0.004044003728325,-0.004974378728123,-0.005856796994452,-0.006676377424469,
   -0.00741173455239,-0.008045980476137,-0.008561130442275,-0.008940691658649,
  -0.009169339401979,-0.009233113449655,-0.009120057308753,-0.008819989656629,
  -0.008325074815583,-0.007630038161278,-0.006731833672989,-0.005630388210253,
   -0.00432777730637,-0.002829776339715,-0.001143583682991,0.0007193138008989,
   0.002746648552113, 0.004921401338227, 0.007226501028543,  0.00964113303885,
    0.01214284548192,  0.01470892666641,  0.01731308114943,  0.01992961094995,
    0.02253196124614,  0.02509208929217,  0.02758302248659,  0.02997859186409,
    0.03225206746422,  0.03437813246478,  0.03633361164216,  0.03809634773672,
    0.03964618719374,  0.04096576652235,  0.04204030976228,  0.04285731488587,
    0.04340706875093,  0.04368333964403,  0.04368333964403,  0.04340706875093,
    0.04285731488587,  0.04204030976228,  0.04096576652235,  0.03964618719374,
    0.03809634773672,  0.03633361164216,  0.03437813246478,  0.03225206746422,
    0.02997859186409,  0.02758302248659,  0.02509208929217,  0.02253196124614,
    0.01992961094995,  0.01731308114943,  0.01470892666641,  0.01214284548192,
    0.00964113303885, 0.007226501028543, 0.004921401338227, 0.002746648552113,
  0.0007193138008989,-0.001143583682991,-0.002829776339715, -0.00432777730637,
  -0.005630388210253,-0.006731833672989,-0.007630038161278,-0.008325074815583,
  -0.008819989656629,-0.009120057308753,-0.009233113449655,-0.009169339401979,
  -0.008940691658649,-0.008561130442275,-0.008045980476137, -0.00741173455239,
  -0.006676377424469,-0.005856796994452,-0.004974378728123,-0.004044003728325,
   -0.00308856300883,-0.002123081447825,-0.001165479934851,-0.000233476321428,
  0.0006583780470531, 0.001497383079786, 0.002269090940775,  0.00296351002348,
   0.003572290515803, 0.004088516726408, 0.004505675603702,  0.00482125990184,
   0.005033987490215, 0.005145046928966, 0.005155524413225, 0.005069630990645,
   0.004892467339051, 0.004631853787874, 0.004295142282385, 0.003891768805491,
   0.003430986346463, 0.002923922194491, 0.002381055693405,  0.00181427769259,
   0.001234524962777,0.0006534144814008,8.050783114032e-05,-0.000473234789318,
  -0.0009986500242702,-0.001486822750587,-0.001930564220738,-0.002322239904772,
  -0.002657581747387,-0.002931665741432,-0.003141894414611,-0.003286274834997,
  -0.003364956420185,-0.003377445256933, -0.00332722585362,-0.003215731471424,
  -0.003048368221369,-0.002829013959429,-0.002564489784499, -0.00226014671792,
   -0.00192370915864,-0.001561506784222,-0.001181996331938,-0.0007916092871739,
  -0.0003992275644469,-1.084735955294e-05,0.0003648302716825,0.0007232378317998,
   0.001055817014933, 0.001360076004247, 0.001627500678701, 0.001859315962532,
   0.002045333948685, 0.002192539076032, 0.002289570351872, 0.002342069992321,
    0.00235344615244, 0.002312117691407, 0.002233500990202,  0.00211617437497,
   0.001960816823846,  0.00176995804329, 0.001553614365528,  0.00131486774926,
   0.001058610517393,0.0007874461271234,0.0005085765970053,0.0002272255330856,
  -4.960599793222e-05,-0.0003181573504889,-0.0005730747758181,-0.0008114246529234,
  -0.001028364760688, -0.00122120757226,-0.001385435914823,-0.001519449896197,
  -0.001620429425351,-0.001688372921984,-0.001721842580378,-0.001721917024894,
  -0.001688792859582,-0.001624182877133,-0.001528819773884, -0.00140585370107,
  -0.001256758504183,-0.001085092783342,-0.0008937674023519,-0.0006868876917276,
  -0.0004674223790094,-0.0002402536527941,-8.284912890685e-06,0.0002241246715358,
  0.0004535974474663,0.0006763598205704,0.0008898270446198, 0.001090251521853,
   0.001275879750449, 0.001443534895053,  0.00159218406654, 0.001719231360746,
   0.001824403637312, 0.001906208940547, 0.001965026824597, 0.002000147240947,
   0.002012718737072, 0.002003008418818, 0.001972710744386, 0.001922689140737,
   0.001855197473065, 0.001771495736385, 0.001674310036821, 0.001564861117002,
   0.001454271681667, 0.001133544531306, 0.005909110665268,0.0002331713117823
};
