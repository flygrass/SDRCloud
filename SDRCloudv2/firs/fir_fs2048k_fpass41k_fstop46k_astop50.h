/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
 * Generated on: 28-May-2019 23:10:15
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 696
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * D:\MATLAB\extern\include\tmwtypes.h 
 */
const int FIR_2048K_TO_128K_LEN = 696;
const real64_T FIR_2048K_TO_128K[696] = {
  0.0005741249693943,-0.001592741381732,-0.0008400576242546,-0.0006653588111729,
  -0.0006659903201151,-0.0007178918292535,-0.0007871923658909,-0.0008595899748707,
  -0.0009338348688494,-0.001005285947876,-0.001075547232064,-0.001140810877537,
  -0.001202931622219,-0.001258195653384,-0.001308536878119,-0.001350329020337,
  -0.001385621853107,-0.001410909029915,-0.001428403709599,-0.001434771793512,
  -0.001432446484295,-0.001418275952265,-0.001394952520582,-0.001359554701879,
  -0.001315058574491,-0.001258796039325,-0.001194038882493,-0.001118372423867,
  -0.001035393560679,-0.0009429123425767,-0.0008448533589965,-0.0007391940573398,
  -0.0006301622737906,-0.0005158000249735,-0.0004006575797265,-0.0002826779084952,
  -0.0001667840124227,-5.059527235717e-05,6.062473369071e-05,0.0001692611995601,
  0.0002710230791752,0.0003659499686304,0.0004537744916045,0.0005308956158971,
   0.000598183896134,0.0006558019369679,0.0006993732029138,0.0007318676241329,
  0.0007516968529406,0.0007593914222797,0.0007525050284858,0.0007344554786558,
  0.0007038349229868,0.0006627218380577,0.0006089630063118,0.0005458702251482,
  0.0004727252847294, 0.000393289746764, 0.000306460557257,0.0002157605903339,
  0.0001202371783612,2.383784478141e-05,-7.412460880775e-05,-0.0001695851625326,
  -0.0002633300255437,-0.0003514149427882,-0.0004347631648144,-0.0005095590599107,
  -0.0005769257695929,-0.0006332656522209,-0.0006800147896725,-0.0007138635581594,
  -0.0007366509642011,-0.0007454097881369,-0.0007424735287754,-0.0007251789290639,
  -0.000696461504136,-0.000653882354048,-0.0006010322006342,-0.0005357377140722,
  -0.000461929721936,-0.0003782047641968,-0.0002880099134671,-0.0001912700400666,
  -9.088354691873e-05,1.294268575081e-05,0.0001163788755498,0.0002202385122844,
  0.0003201136674612, 0.000416366707017,0.0005055301669128,  0.00058792466617,
  0.0006596311342663,0.0007217983166186, 0.000771127362394,0.0008085336503825,
  0.0008310249625412,0.0008404833359776,0.0008343861771705, 0.000814624219748,
  0.0007791500437419,0.0007307624938073,0.0006679962224736,0.0005938324929092,
  0.0005071048262514, 0.000411464501088,0.0003063932682323,0.0001958015681166,
  7.923469080022e-05,-3.92628603472e-05,-0.0001598388700308,-0.0002781524358441,
  -0.000394245404878,-0.0005038710672257,-0.0006073011502979,-0.0007004424339435,
  -0.0007838430266947,-0.0008535118440536,-0.0009104975308916,-0.0009510899989203,
  -0.0009769103985836,-0.0009848224479847,-0.0009767546372707,-0.0009505426294374,
  -0.000908254139019,-0.0008485998487462,-0.0007742600125794,-0.0006841990169739,
  -0.0005820361216572,-0.0004671203107291,-0.0003433118810309,-0.0002107538201877,
  -7.353224970178e-05, 6.81368140984e-05,0.0002096368528675,0.0003505981289408,
  0.0004866475420979,0.0006172532208464,0.0007379965555403,0.0008487458189868,
  0.0009454384189541, 0.001028062649966, 0.001093004646486, 0.001140942041175,
   0.001168765409988, 0.001177578996232, 0.001164984622072, 0.001132910355176,
   0.001079648395966, 0.001007660938126,0.0009159915942146,0.0008079219886816,
  0.0006831833339176,0.0005455240960421,0.0003952125128087,0.0002366018773932,
  7.043970861442e-05,-9.855126103018e-05,-0.0002695520387287,-0.0004375441058076,
  -0.0006016594602442,-0.0007567990760992,-0.0009021998080356,-0.001033234081392,
  -0.001149287418242,-0.001246343030767,-0.001324111918495,-0.001379099462226,
  -0.001411881741674,-0.001419588727448,-0.001403613137476,-0.001362044305141,
  -0.001296894812327, -0.00120717382051,-0.001095859968727,-0.0009626411042737,
  -0.0008113134369929,-0.000642487958377,-0.0004604843997374,-0.0002665203408425,
  -6.556702005429e-05,0.0001407615010818,0.0003472989572774,0.0005520929652139,
  0.0007498385041394,0.0009387304618152, 0.001113638964889, 0.001272913040843,
   0.001411867349146, 0.001529417519234, 0.001621503137462,  0.00168765827251,
   0.001724708321344, 0.001733111852088, 0.001710640485984, 0.001658651586984,
   0.001576018130863, 0.001465229626428, 0.001326110885323, 0.001162178483316,
  0.0009741722889755,0.0007666683986191,0.0005412259393431,0.0003030649116605,
  5.450674388028e-05,-0.0001987756728309,-0.0004539151009979,-0.0007050505050298,
  -0.0009492704340403,-0.001180646512397,-0.001396469916927,-0.001591150637141,
  -0.001762378413486, -0.00190534702914,-0.002018474687651,-0.002097727739349,
  -0.002142580218279,-0.002150103763679,-0.002120881933133,-0.002053379314063,
  -0.001949403930202,-0.001808612937474,-0.001634139056852,-0.001426994245657,
  -0.001191453270445,-0.0009297204923936,-0.0006472080868439,-0.0003470271320294,
  -3.535570010745e-05,0.0002839068860769,0.0006040725585533,0.0009209068586969,
   0.001227565736115, 0.001519724886643, 0.001790798495211, 0.002036848568471,
   0.002251896531144, 0.002432682189664, 0.002574178459937, 0.002674287642982,
   0.002729134799578,  0.00273801438631, 0.002698523490763, 0.002611530571523,
   0.002476264341775, 0.002295129120472, 0.002069139678711, 0.001802303168032,
   0.001497195278266, 0.001159322435491,0.0007926604216449,0.0004040386445966,
  -1.421057663905e-06,-0.0004160569164279,-0.0008339210029843,-0.001246803002033,
  -0.001648492976443,-0.002030672157626,-0.002387306492201,-0.002710572041264,
  -0.002994970085939,-0.003233617978008,-0.003422201763139,-0.003555173050216,
  -0.003629724992179,-0.003642092491345,-0.003591294213619,-0.003475553696463,
   -0.00329608218357,-0.003053288902512,-0.002750469472183,-0.002390266142184,
  -0.001978149952402,-0.001518861283141,-0.001019798013206,-0.0004875901316406,
   6.88065367416e-05,0.0006413231220955, 0.001219843277693, 0.001795330158159,
   0.002357014446702, 0.002895610564137, 0.003400299682514, 0.003862094100899,
   0.004270873206989, 0.004618605547114, 0.004896520022873,  0.00509811891972,
    0.00521659573926, 0.005247653780648, 0.005186891755562, 0.005032652778057,
   0.004783416384182, 0.004440458333166, 0.004005364405955, 0.003482505187204,
   0.002876655730131, 0.002195343960947, 0.001446320089923,0.0006399974433438,
  -0.0002130243268272,-0.001099893577473,-0.002007721031377,-0.002921702482267,
  -0.003827334540501,-0.004708566535418,-0.005549962007388,-0.006335079899104,
  -0.007048489189254,-0.007674119541203,-0.008197416889288,-0.008603695903212,
   -0.00888009389096,-0.009014123619718,-0.008995514795705,-0.008814734016933,
  -0.008464725748625,-0.007939663818045,-0.007236328835047,-0.006352968637564,
  -0.005290564432731,-0.004051704372062,-0.002641832104081,-0.001067922411497,
  0.0006602642715227,  0.00253149517758,  0.00453202978979, 0.006646791773645,
   0.008858510640548,  0.01114887160587,  0.01349781813996,   0.0158846727224,
    0.01828745158245,  0.02068395879884,  0.02305132953788,  0.02536693915046,
    0.02760804410186,  0.02975268666565,  0.03177932507053,  0.03366775488905,
    0.03539868085398,  0.03695455180203,  0.03831935190506,  0.03947905045114,
    0.04042158026312,  0.04113722078295,  0.04161850653027,  0.04186040687043,
    0.04186040687043,  0.04161850653027,  0.04113722078295,  0.04042158026312,
    0.03947905045114,  0.03831935190506,  0.03695455180203,  0.03539868085398,
    0.03366775488905,  0.03177932507053,  0.02975268666565,  0.02760804410186,
    0.02536693915046,  0.02305132953788,  0.02068395879884,  0.01828745158245,
     0.0158846727224,  0.01349781813996,  0.01114887160587, 0.008858510640548,
   0.006646791773645,  0.00453202978979,  0.00253149517758,0.0006602642715227,
  -0.001067922411497,-0.002641832104081,-0.004051704372062,-0.005290564432731,
  -0.006352968637564,-0.007236328835047,-0.007939663818045,-0.008464725748625,
  -0.008814734016933,-0.008995514795705,-0.009014123619718, -0.00888009389096,
  -0.008603695903212,-0.008197416889288,-0.007674119541203,-0.007048489189254,
  -0.006335079899104,-0.005549962007388,-0.004708566535418,-0.003827334540501,
  -0.002921702482267,-0.002007721031377,-0.001099893577473,-0.0002130243268272,
  0.0006399974433438, 0.001446320089923, 0.002195343960947, 0.002876655730131,
   0.003482505187204, 0.004005364405955, 0.004440458333166, 0.004783416384182,
   0.005032652778057, 0.005186891755562, 0.005247653780648,  0.00521659573926,
    0.00509811891972, 0.004896520022873, 0.004618605547114, 0.004270873206989,
   0.003862094100899, 0.003400299682514, 0.002895610564137, 0.002357014446702,
   0.001795330158159, 0.001219843277693,0.0006413231220955, 6.88065367416e-05,
  -0.0004875901316406,-0.001019798013206,-0.001518861283141,-0.001978149952402,
  -0.002390266142184,-0.002750469472183,-0.003053288902512, -0.00329608218357,
  -0.003475553696463,-0.003591294213619,-0.003642092491345,-0.003629724992179,
  -0.003555173050216,-0.003422201763139,-0.003233617978008,-0.002994970085939,
  -0.002710572041264,-0.002387306492201,-0.002030672157626,-0.001648492976443,
  -0.001246803002033,-0.0008339210029843,-0.0004160569164279,-1.421057663905e-06,
  0.0004040386445966,0.0007926604216449, 0.001159322435491, 0.001497195278266,
   0.001802303168032, 0.002069139678711, 0.002295129120472, 0.002476264341775,
   0.002611530571523, 0.002698523490763,  0.00273801438631, 0.002729134799578,
   0.002674287642982, 0.002574178459937, 0.002432682189664, 0.002251896531144,
   0.002036848568471, 0.001790798495211, 0.001519724886643, 0.001227565736115,
  0.0009209068586969,0.0006040725585533,0.0002839068860769,-3.535570010745e-05,
  -0.0003470271320294,-0.0006472080868439,-0.0009297204923936,-0.001191453270445,
  -0.001426994245657,-0.001634139056852,-0.001808612937474,-0.001949403930202,
  -0.002053379314063,-0.002120881933133,-0.002150103763679,-0.002142580218279,
  -0.002097727739349,-0.002018474687651, -0.00190534702914,-0.001762378413486,
  -0.001591150637141,-0.001396469916927,-0.001180646512397,-0.0009492704340403,
  -0.0007050505050298,-0.0004539151009979,-0.0001987756728309,5.450674388028e-05,
  0.0003030649116605,0.0005412259393431,0.0007666683986191,0.0009741722889755,
   0.001162178483316, 0.001326110885323, 0.001465229626428, 0.001576018130863,
   0.001658651586984, 0.001710640485984, 0.001733111852088, 0.001724708321344,
    0.00168765827251, 0.001621503137462, 0.001529417519234, 0.001411867349146,
   0.001272913040843, 0.001113638964889,0.0009387304618152,0.0007498385041394,
  0.0005520929652139,0.0003472989572774,0.0001407615010818,-6.556702005429e-05,
  -0.0002665203408425,-0.0004604843997374,-0.000642487958377,-0.0008113134369929,
  -0.0009626411042737,-0.001095859968727, -0.00120717382051,-0.001296894812327,
  -0.001362044305141,-0.001403613137476,-0.001419588727448,-0.001411881741674,
  -0.001379099462226,-0.001324111918495,-0.001246343030767,-0.001149287418242,
  -0.001033234081392,-0.0009021998080356,-0.0007567990760992,-0.0006016594602442,
  -0.0004375441058076,-0.0002695520387287,-9.855126103018e-05,7.043970861442e-05,
  0.0002366018773932,0.0003952125128087,0.0005455240960421,0.0006831833339176,
  0.0008079219886816,0.0009159915942146, 0.001007660938126, 0.001079648395966,
   0.001132910355176, 0.001164984622072, 0.001177578996232, 0.001168765409988,
   0.001140942041175, 0.001093004646486, 0.001028062649966,0.0009454384189541,
  0.0008487458189868,0.0007379965555403,0.0006172532208464,0.0004866475420979,
  0.0003505981289408,0.0002096368528675, 6.81368140984e-05,-7.353224970178e-05,
  -0.0002107538201877,-0.0003433118810309,-0.0004671203107291,-0.0005820361216572,
  -0.0006841990169739,-0.0007742600125794,-0.0008485998487462,-0.000908254139019,
  -0.0009505426294374,-0.0009767546372707,-0.0009848224479847,-0.0009769103985836,
  -0.0009510899989203,-0.0009104975308916,-0.0008535118440536,-0.0007838430266947,
  -0.0007004424339435,-0.0006073011502979,-0.0005038710672257,-0.000394245404878,
  -0.0002781524358441,-0.0001598388700308,-3.92628603472e-05,7.923469080022e-05,
  0.0001958015681166,0.0003063932682323, 0.000411464501088,0.0005071048262514,
  0.0005938324929092,0.0006679962224736,0.0007307624938073,0.0007791500437419,
   0.000814624219748,0.0008343861771705,0.0008404833359776,0.0008310249625412,
  0.0008085336503825, 0.000771127362394,0.0007217983166186,0.0006596311342663,
    0.00058792466617,0.0005055301669128, 0.000416366707017,0.0003201136674612,
  0.0002202385122844,0.0001163788755498,1.294268575081e-05,-9.088354691873e-05,
  -0.0001912700400666,-0.0002880099134671,-0.0003782047641968,-0.000461929721936,
  -0.0005357377140722,-0.0006010322006342,-0.000653882354048,-0.000696461504136,
  -0.0007251789290639,-0.0007424735287754,-0.0007454097881369,-0.0007366509642011,
  -0.0007138635581594,-0.0006800147896725,-0.0006332656522209,-0.0005769257695929,
  -0.0005095590599107,-0.0004347631648144,-0.0003514149427882,-0.0002633300255437,
  -0.0001695851625326,-7.412460880775e-05,2.383784478141e-05,0.0001202371783612,
  0.0002157605903339, 0.000306460557257, 0.000393289746764,0.0004727252847294,
  0.0005458702251482,0.0006089630063118,0.0006627218380577,0.0007038349229868,
  0.0007344554786558,0.0007525050284858,0.0007593914222797,0.0007516968529406,
  0.0007318676241329,0.0006993732029138,0.0006558019369679, 0.000598183896134,
  0.0005308956158971,0.0004537744916045,0.0003659499686304,0.0002710230791752,
  0.0001692611995601,6.062473369071e-05,-5.059527235717e-05,-0.0001667840124227,
  -0.0002826779084952,-0.0004006575797265,-0.0005158000249735,-0.0006301622737906,
  -0.0007391940573398,-0.0008448533589965,-0.0009429123425767,-0.001035393560679,
  -0.001118372423867,-0.001194038882493,-0.001258796039325,-0.001315058574491,
  -0.001359554701879,-0.001394952520582,-0.001418275952265,-0.001432446484295,
  -0.001434771793512,-0.001428403709599,-0.001410909029915,-0.001385621853107,
  -0.001350329020337,-0.001308536878119,-0.001258195653384,-0.001202931622219,
  -0.001140810877537,-0.001075547232064,-0.001005285947876,-0.0009338348688494,
  -0.0008595899748707,-0.0007871923658909,-0.0007178918292535,-0.0006659903201151,
  -0.0006653588111729,-0.0008400576242546,-0.001592741381732,0.0005741249693943
};
