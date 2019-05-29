/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
 * Generated on: 28-May-2019 23:48:42
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 496
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * D:\MATLAB\extern\include\tmwtypes.h 
 */
const int FIR_128K_TO_48K_LEN = 496;
const real64_T FIR_128K_TO_48K[496] = {
   0.005712266620067,0.0006490310592918,0.0006810937775803,0.0007098650438889,
  0.0007364074306815,0.0007589890388079,0.0007786095819475,0.0007934905952298,
  0.0008045983439158,0.0008102272426186,0.0008115134852683,0.0008070435420704,
   0.000798408110493,0.0007845004790813,0.0007668275444964,0.0007435232672736,
  0.0007145463549052, 0.000676703241474,0.0006322081181253,0.0005924850798114,
  0.0005376483748893,0.0004812608003831,0.0004199320682936,0.0003531360934203,
   0.000282435306814,0.0002068155908006,0.0001280343505055, 4.52872369166e-05,
  -3.95532326267e-05,-0.0001273461980358,-0.0002165201934255,-0.0003080748366695,
  -0.0004003188162104,-0.0004936101138319,-0.0005855466833054,-0.0006767329417336,
  -0.0007665964895488,-0.0008565837690359,-0.0009404831906847,-0.001024152426006,
   -0.00110263077636,-0.001177585055453,-0.001247009769331,-0.001311487782015,
  -0.001369388475467,-0.001421442338645,-0.001466184570768,-0.001504270508735,
  -0.001534081904957,-0.001556249792927,-0.001569409584918,-0.001574688014775,
   -0.00157102172143,-0.001559079754583,-0.001537332779613,-0.001507331636537,
  -0.001469035495449,-0.001421335760559,-0.001365581579084,-0.001301833723067,
  -0.001229967832053,-0.001151284059302,-0.001065286751793,-0.0009733479372017,
  -0.0008750492089277,-0.0007719727370044,-0.0006639856471935,-0.000552729020052,
  -0.0004380501271615,-0.0003214117592011,-0.0002028815705644,-8.432484106291e-05,
  3.419952936806e-05, 0.000151470266845,0.0002672284954582,0.0003790274601025,
   0.000487978693307,0.0005914244794936,0.0006898582366093,0.0007815834670741,
  0.0008664941582635,0.0009431324700845,  0.00101139197347, 0.001070070587099,
   0.001119198755584, 0.001157671765414, 0.001185535344647, 0.001201802038182,
    0.00120686059242, 0.001200016393013, 0.001181482890797, 0.001150555918271,
   0.001108077112719, 0.001053828023397, 0.000987897113697, 0.000910960082435,
  0.0008231857380522,0.0007249944996342,0.0006172828556995,0.0005003996637809,
  0.0003754666649471,0.0002429154152014,0.0001040210613275,-4.055194961692e-05,
  -0.000189400718749,-0.0003417621354773,-0.0004963398990143,-0.0006521406260607,
  -0.0008075734844882,-0.0009616592292211,-0.001113131722446,-0.001260792082737,
  -0.001403050252224,-0.001539295310998,-0.001667592601952,-0.001787312583685,
  -0.001897009229631, -0.00199578020006,-0.002082466962288, -0.00215617775209,
  -0.002216022905024,-0.002261280507006,-0.002291249939435,-0.002305295124307,
  -0.002302871934261,-0.002283744354426,-0.002247704299212,-0.002194537847206,
  -0.002124233294479,-0.002037049936441,-0.001933246401916,-0.001813009321527,
  -0.001677324583627, -0.00152638183643,-0.001361337725688,-0.001182915365126,
  -0.0009923493462647,-0.0007906814306501,-0.0005792753043931,-0.0003593898027835,
  -0.0001326481981856,9.945265714226e-05,0.0003351194975184,0.0005729065645162,
  0.0008107971940672, 0.001047130724676, 0.001279884906596, 0.001507489789814,
   0.001727736671787, 0.001939024275066, 0.002139431467452, 0.002327097530943,
   0.002500301700325, 0.002657473053949, 0.002796824398582, 0.002917080895661,
   0.003016510093855, 0.003094142848824, 0.003148507222711, 0.003178956845708,
   0.003184231799047, 0.003163921710461, 0.003117246898535, 0.003044162714961,
   0.002944128599553, 0.002817494867202, 0.002664207577104, 0.002484941839695,
   0.002279975812198, 0.002050654112518, 0.001797396259982, 0.001521971322156,
    0.00122532584356,0.0009095092881696,0.0005759666603728,0.0002270118239217,
  -0.0001355532997557,-0.0005090907252331,-0.0008914090032286,-0.001279589718175,
  -0.001671327229248,-0.002063372103077,-0.002453170119284,-0.002837364758673,
  -0.003213277788797,-0.003577380255779,-0.003927001945165,-0.004258686721961,
  -0.004569528941296,-0.004856280870971,-0.005116155039935,-0.005345952400238,
  -0.005543160060832,-0.005704672240771,-0.005828276744604,-0.005911161605807,
   -0.00595149958405,-0.005946767446922,-0.005895450667623,-0.005795590290999,
  -0.005646072918353,-0.005445396091751,-0.005192986011296,-0.004887870386667,
   -0.00452992383385,-0.004118784560627,-0.003655023120476,-0.003138666985302,
  -0.002570941831358,-0.001952498282358,-0.001285067771615,-0.0005700103011496,
  0.0001904745010757,0.0009944458580958,  0.00183920967841, 0.002722183195815,
   0.003640190455129,  0.00459026073253, 0.005568677415601, 0.006572079603424,
    0.00759639846428, 0.008637849956873, 0.009692053785335,  0.01075503340107,
    0.01182219409516,  0.01288921311888,  0.01395151325249,  0.01500461886968,
    0.01604391300054,  0.01706500058648,  0.01806322056414,  0.01903430156118,
    0.01997373366919,  0.02087752614468,   0.0217413558838,  0.02256147278675,
    0.02333400875099,  0.02405547937342,   0.0247225098018,  0.02533210123095,
    0.02588132548108,  0.02636765198471,  0.02678880969712,  0.02714288289534,
    0.02742810303071,  0.02764326842303,    0.027787224464,  0.02785937912825,
    0.02785937912825,    0.027787224464,  0.02764326842303,  0.02742810303071,
    0.02714288289534,  0.02678880969712,  0.02636765198471,  0.02588132548108,
    0.02533210123095,   0.0247225098018,  0.02405547937342,  0.02333400875099,
    0.02256147278675,   0.0217413558838,  0.02087752614468,  0.01997373366919,
    0.01903430156118,  0.01806322056414,  0.01706500058648,  0.01604391300054,
    0.01500461886968,  0.01395151325249,  0.01288921311888,  0.01182219409516,
    0.01075503340107, 0.009692053785335, 0.008637849956873,  0.00759639846428,
   0.006572079603424, 0.005568677415601,  0.00459026073253, 0.003640190455129,
   0.002722183195815,  0.00183920967841,0.0009944458580958,0.0001904745010757,
  -0.0005700103011496,-0.001285067771615,-0.001952498282358,-0.002570941831358,
  -0.003138666985302,-0.003655023120476,-0.004118784560627, -0.00452992383385,
  -0.004887870386667,-0.005192986011296,-0.005445396091751,-0.005646072918353,
  -0.005795590290999,-0.005895450667623,-0.005946767446922, -0.00595149958405,
  -0.005911161605807,-0.005828276744604,-0.005704672240771,-0.005543160060832,
  -0.005345952400238,-0.005116155039935,-0.004856280870971,-0.004569528941296,
  -0.004258686721961,-0.003927001945165,-0.003577380255779,-0.003213277788797,
  -0.002837364758673,-0.002453170119284,-0.002063372103077,-0.001671327229248,
  -0.001279589718175,-0.0008914090032286,-0.0005090907252331,-0.0001355532997557,
  0.0002270118239217,0.0005759666603728,0.0009095092881696,  0.00122532584356,
   0.001521971322156, 0.001797396259982, 0.002050654112518, 0.002279975812198,
   0.002484941839695, 0.002664207577104, 0.002817494867202, 0.002944128599553,
   0.003044162714961, 0.003117246898535, 0.003163921710461, 0.003184231799047,
   0.003178956845708, 0.003148507222711, 0.003094142848824, 0.003016510093855,
   0.002917080895661, 0.002796824398582, 0.002657473053949, 0.002500301700325,
   0.002327097530943, 0.002139431467452, 0.001939024275066, 0.001727736671787,
   0.001507489789814, 0.001279884906596, 0.001047130724676,0.0008107971940672,
  0.0005729065645162,0.0003351194975184,9.945265714226e-05,-0.0001326481981856,
  -0.0003593898027835,-0.0005792753043931,-0.0007906814306501,-0.0009923493462647,
  -0.001182915365126,-0.001361337725688, -0.00152638183643,-0.001677324583627,
  -0.001813009321527,-0.001933246401916,-0.002037049936441,-0.002124233294479,
  -0.002194537847206,-0.002247704299212,-0.002283744354426,-0.002302871934261,
  -0.002305295124307,-0.002291249939435,-0.002261280507006,-0.002216022905024,
   -0.00215617775209,-0.002082466962288, -0.00199578020006,-0.001897009229631,
  -0.001787312583685,-0.001667592601952,-0.001539295310998,-0.001403050252224,
  -0.001260792082737,-0.001113131722446,-0.0009616592292211,-0.0008075734844882,
  -0.0006521406260607,-0.0004963398990143,-0.0003417621354773,-0.000189400718749,
  -4.055194961692e-05,0.0001040210613275,0.0002429154152014,0.0003754666649471,
  0.0005003996637809,0.0006172828556995,0.0007249944996342,0.0008231857380522,
   0.000910960082435, 0.000987897113697, 0.001053828023397, 0.001108077112719,
   0.001150555918271, 0.001181482890797, 0.001200016393013,  0.00120686059242,
   0.001201802038182, 0.001185535344647, 0.001157671765414, 0.001119198755584,
   0.001070070587099,  0.00101139197347,0.0009431324700845,0.0008664941582635,
  0.0007815834670741,0.0006898582366093,0.0005914244794936, 0.000487978693307,
  0.0003790274601025,0.0002672284954582, 0.000151470266845,3.419952936806e-05,
  -8.432484106291e-05,-0.0002028815705644,-0.0003214117592011,-0.0004380501271615,
  -0.000552729020052,-0.0006639856471935,-0.0007719727370044,-0.0008750492089277,
  -0.0009733479372017,-0.001065286751793,-0.001151284059302,-0.001229967832053,
  -0.001301833723067,-0.001365581579084,-0.001421335760559,-0.001469035495449,
  -0.001507331636537,-0.001537332779613,-0.001559079754583, -0.00157102172143,
  -0.001574688014775,-0.001569409584918,-0.001556249792927,-0.001534081904957,
  -0.001504270508735,-0.001466184570768,-0.001421442338645,-0.001369388475467,
  -0.001311487782015,-0.001247009769331,-0.001177585055453, -0.00110263077636,
  -0.001024152426006,-0.0009404831906847,-0.0008565837690359,-0.0007665964895488,
  -0.0006767329417336,-0.0005855466833054,-0.0004936101138319,-0.0004003188162104,
  -0.0003080748366695,-0.0002165201934255,-0.0001273461980358,-3.95532326267e-05,
   4.52872369166e-05,0.0001280343505055,0.0002068155908006, 0.000282435306814,
  0.0003531360934203,0.0004199320682936,0.0004812608003831,0.0005376483748893,
  0.0005924850798114,0.0006322081181253, 0.000676703241474,0.0007145463549052,
  0.0007435232672736,0.0007668275444964,0.0007845004790813, 0.000798408110493,
  0.0008070435420704,0.0008115134852683,0.0008102272426186,0.0008045983439158,
  0.0007934905952298,0.0007786095819475,0.0007589890388079,0.0007364074306815,
  0.0007098650438889,0.0006810937775803,0.0006490310592918, 0.005712266620067
};
