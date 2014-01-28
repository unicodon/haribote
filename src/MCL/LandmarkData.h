#ifndef LANDMARK_DATA_H_
#define LANDMARK_DATA_H_

const int width_step_num = 18;
const int width_step_wid = 3;
const int width_step_min = 3;
const int dist_step_num = 45;
const int dist_step_wid = 100;
const int dist_step_min = 450;
const double pwd[width_step_num][dist_step_num] = 
{
 {1e-10,1e-10,0.0044266932933932435,0.005028936340051885,1e-10,1e-10,1e-10,6.997667444185271E-4,0.0070,0.012833333333333332,0.012133333333333333,0.0063,7.000000000000001E-4,0.00105,0.00245,0.002333333333333333,0.0028000000000000004,1e-10,3.5000000000000005E-4,3.5000000000000005E-4,0.004199999999999999,0.023799999999999995,0.023799999999999995,0.021699999999999997,0.0476,0.0476,0.0896,0.1575,0.2233,0.3416,0.3416,0.5313,0.4879,0.6041,0.6103999999999999,0.6349,0.6849333333333333,0.7160666666666666,0.7911999999999999,0.8224999999999999,0.8597333333333332,0.8927999999999999,0.9333333333333333,0.9599,0.9985},
 {1e-10,1e-10,0.005275420203028791,0.006904809419277589,1e-10,1e-10,1e-10,5.331556147950684E-4,0.009066666666666669,0.019966666666666667,0.019666666666666666,0.011566666666666668,0.0010000000000000002,0.00115,0.00385,0.009399999999999999,0.013450000000000002,0.0623,0.30885,0.30885,0.4882999999999999,0.5757999999999999,0.5757999999999999,0.6211,0.6507499999999999,0.6507499999999999,0.6453,0.6079,0.5717,0.5048,0.5048,0.39640000000000003,0.4212,0.3548,0.35120000000000007,0.33720000000000006,0.29746666666666666,0.2685333333333334,0.20660000000000006,0.1753333333333334,0.1392000000000001,0.10640000000000009,0.06666666666666665,0.03986666666666677,0.0015},
 {1e-10,1e-10,0.004942586120818773,0.0066453801636399915,1e-10,1e-10,1e-10,0.005698100633122293,0.023600000000000003,0.034899999999999994,0.031166666666666665,0.03506666666666668,0.14216666666666666,0.2103,0.5559499999999999,0.5831999999999999,0.6153,0.6483,0.5222000000000001,0.5222000000000001,0.41779999999999995,0.353,0.353,0.32870000000000005,0.29220000000000007,0.29220000000000007,0.26360000000000006,0.23370000000000002,0.20470000000000005,0.15360000000000001,0.15360000000000001,0.0723,0.09090000000000001,0.04110000000000001,0.038400000000000004,0.027900000000000005,0.017600000000000005,0.015400000000000006,0.0022000000000000006,0.0021666666666666674,0.0010666666666666674,8.000000000000007E-4,1e-10,2.333333333333341E-4,1e-10},
 {1e-10,1e-10,0.0037111000166417042,0.005747355817202154,0.0016769814334198442,1e-10,1e-10,0.016161279573475507,0.09626666666666668,0.24913333333333335,0.42579999999999996,0.5512333333333334,0.5627,0.5548,0.37775000000000003,0.3580666666666667,0.3363500000000001,0.2825000000000001,0.16815000000000005,0.16815000000000005,0.0897,0.04739999999999999,0.04739999999999999,0.0285,0.009450000000000002,0.009450000000000002,0.0015000000000000002,9.000000000000002E-4,3.0000000000000003E-4,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,7.776049766718504E-4,0.00397736728240972,0.005248453402514468,0.003373926931523258,0.002564102564102564,0.052823588205897054,0.31076307897367544,0.4310333333333334,0.44730000000000003,0.40026666666666666,0.34623333333333345,0.2699333333333334,0.22265000000000004,0.06000000000000001,0.047,0.03210000000000001,0.006900000000000002,4.5000000000000015E-4,4.5000000000000015E-4,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,8.775827593868027E-4,0.0032784157097686807,0.004210736379964079,0.0212617288880016,0.32067932067932065,0.5207896051974014,0.4537154281906031,0.3420666666666667,0.20933333333333337,0.10626666666666668,0.04960000000000001,0.023500000000000007,0.010050000000000002,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,0.0011664074650077757,0.0027125977700116497,0.002394731590500898,0.2586344579756439,0.4619047619047619,0.36216891554222896,0.19023658780406533,0.08596666666666668,0.02643333333333334,0.0047,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,0.0014885580982003993,0.00289565651522716,0.1792057473558172,0.4382112198043522,0.1921744921744922,0.06406796601699152,0.02219260246584472,0.005000000000000002,1.0E-4,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,0.0022550544323483666,0.09316025961058415,0.4376771103572141,0.2343980834497904,0.02267732267732268,1.4992503748125941E-4,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,0.015618751388580312,0.3164420036611749,0.2811814009179805,0.041605110800559,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {1e-10,0.1359142412797156,0.3592278249292728,0.06228297744961087,8.384907167099222E-4,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {4.200840168033607E-4,0.26671850699844474,0.16658345814611422,0.0034723608062263026,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.08272654530906183,0.3207620528771384,0.031519387585288744,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.23484696939387878,0.19424572317262825,0.0018472291562656024,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.3094618923784757,0.05441013108198177,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.25079015803160637,0.005765385469895579,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.1033106621324265,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10},
 {0.018443688737747554,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10,1e-10}
};




const int pdw[width_step_num][dist_step_num+1] = 
{
 {0,0,0,59,115,115,115,115,119,166,252,333,373,377,381,390,405,416,416,416,416,425,531,637,684,899,1114,1316,1672,2178,3730,5282,6488,7594,8966,10352,11792,14762,17732,19304,20872,22449,24194,26125,28211,32767},
 {0,0,0,63,132,132,132,132,134,189,309,429,498,504,508,522,579,632,758,2021,3284,4283,6640,8997,10268,12951,15615,16936,18179,19348,21414,23480,24291,25153,25878,26596,27286,28621,29956,30578,31202,31822,32291,32598,32761,32767},
 {0,0,0,72,154,154,154,154,196,370,629,861,1123,2187,3236,6011,10402,13474,15092,17699,20306,21347,23110,24873,25692,27150,28608,29264,29846,30355,31121,31887,32066,32290,32392,32486,32553,32650,32747,32754,32761,32765,32767,32767,32767,32767},
 {0,0,0,67,153,177,177,177,324,1208,3503,7426,12504,17700,21107,23426,26725,28789,29655,30687,31719,31992,32280,32568,32654,32709,32764,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,30,129,241,310,340,794,4826,10420,16229,21421,25912,29412,31337,31856,32466,32742,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,40,148,268,874,6377,12337,20127,25996,29588,31407,32253,32653,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,75,195,285,10008,20425,25861,30140,32069,32662,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,108,250,7777,26181,31017,32089,32642,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,163,4726,22629,32212,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,1254,18283,30904,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,0,10839,29967,32732,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,38,23023,32605,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,6942,31181,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,18734,32680,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,28296,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,32114,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767},
 {0,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767}
};


const int angle_min = -21;
const int angle_max = 20;
const int angle_range = angle_max-angle_min+1;
const double angle_likelyhood[angle_range] = {
1.96313e-05,2.72506e-05,6.06638e-05,0.000129476,0.000264947,0.000519795,
0.000977718,0.001763202,0.003048582,0.005053603,0.008031781,0.01223856,0.017879552,0.025043261,0.033630485,0.043299561,
0.0534493,0.063257026,0.071776751,0.078084912,0.081443941,0.081443941,0.078084912,0.071776751,0.063257026,0.0534493,
0.043299561,0.033630485,0.025043261,0.017879552,0.01223856,0.008031781,0.005053603,0.003048582,0.001763202,0.000977718,
0.000519795,0.000264947,0.000129476,6.06638e-05,2.72506e-05,1.96313e-05
};
const int angle_like_sum[angle_range+1] = {0,1,2,4,8,16,33,66,123,223,389,652,1053,1639,2459,3561,4980,6732,8804,11156,13715,16384,19052,21611,23963,26035,27787,29206,30308,31128,31714,32115,32378,32544,32644,32701,32734,32751,32759,32763,32765,32766,32767};

#endif