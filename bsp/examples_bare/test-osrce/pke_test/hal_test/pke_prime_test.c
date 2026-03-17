#include <stdio.h>
//#include <string.h>

#include "../../../crypto_hal/pke.h"
#include "../../../crypto_hal/pke_prime.h"
#include "../../../crypto_include/crypto_common/utility.h"



#if 1


uint32_t bigint_div_table_high(uint32_t *a, uint32_t aWordLen, uint16_t *r, double_uint32_t *s,
        uint16_t *high_result, uint32_t PTL);
uint32_t bigint_div_table_low(uint32_t *a, uint16_t *r, double_uint32_t *s, uint16_t *high_result, uint32_t PTL);

#define PTL_TEST   (400)


extern const uint16_t PrimeTable[PTL_MAX];
extern const double_uint32_t PrimeTable_s[PTL_MAX];
extern const uint16_t PrimeTable_r[PTL_MAX];
extern void set_PTL(uint32_t n);


uint32_t big_mod_little_prime_test(uint32_t *a, uint32_t aWordLen, uint16_t std_out[PTL_TEST])
{
    uint16_t result[PTL_TEST];
    uint32_t ret;

    ret = bigint_div_table_high(a, aWordLen, (uint16_t *)PrimeTable_r, (double_uint32_t *)PrimeTable_s, result, PTL_TEST);
    if(PKE_SUCCESS != ret)
        return 1;

    if(memcmp_((uint8_t *)std_out, (uint8_t *)result, PTL_TEST*2))
    {
        return 1;
    }

    return 0;
}

uint32_t std_big_mod_little_a[128]={
    0x16C1EFB1,0xFFE775B8,0x44075A71,0x05A96BBF,0xA24B6052,0xA2F1AB50,0xE33BFD97,0x4F24F24B,
    0xA0297B5A,0x9102BFDF,0xA9C996E3,0xB6F69F10,0x1E0CA49A,0x6EAC25B0,0x38A51DA0,0x9932E098,
    0x0CBBE9E3,0xED35E780,0xE861BDA6,0xF7001232,0x06F5A695,0x2994EF6C,0xFEAA1A9A,0x743FD81E,
    0x62AC95D4,0xBFB745D2,0xCBEE72A2,0x2F6E3A77,0x8BB27CF9,0x0E987E71,0x50AACB88,0xC811D5F6,
    0xED0B24F1,0x624BAA99,0x954CC8CD,0xE29B7739,0xBA3D9876,0x7E19D3B4,0x8A1321FE,0xB6E85967,
    0x6D680E56,0x9CB1B444,0x2F241647,0xE05ADC49,0xB466D9AA,0x682C2E55,0x56AC84E8,0xE784C3E4,
    0xFFFD1943,0xCD0FE653,0xA1A3FCB1,0xC5889558,0xC29CFC60,0xE2F6005C,0xB8EAC503,0x11E8A96B,
    0x33DB8C5E,0x971B4936,0xD36D8286,0x9470B692,0x577A2B5F,0xCBE39117,0x7452A1D7,0xB1AC7500,
    0xF3BD0389,0x2501A07F,0x81FEB6D5,0xF345AB05,0xB4339D1A,0x38A8078E,0xC7F8912B,0x968B03E9,
    0x049B1564,0xA68ED1B0,0xE6010124,0xFF2E003E,0xBE6C3E60,0xAA17EE87,0xC458CE8B,0x642C5293,
    0xD3149570,0x8C3F78AE,0x18E7AE70,0x6DEDD2D1,0x200DA1AA,0x8F269642,0xE2A2B073,0xCBA8C5AE,
    0x278115DA,0x387DB3DF,0x2EB08D8E,0x1A04474A,0x1E7F7F95,0x355E82D1,0x0DCF1600,0xC8125317,
    0xE52B3719,0x149D465B,0xBB77CD90,0xAA6BAC27,0x06E8C600,0x80EDB047,0x7734F52B,0x5A219913,
    0x702932B8,0x2428161C,0x54032A4A,0x32FE891F,0xC6B3E386,0xF7D50512,0x5CC7B3C2,0xBE3C42D8,
    0x0B643D37,0x52FDA94B,0x47109829,0x703EDB05,0x44EB6213,0xA1E9AF69,0x803C2FEB,0xE7F5B4BC,
    0xCC2F1DC9,0xB590AEDA,0x2556286F,0xDC021A85,0x05061171,0xC1C03AC6,0xD7A855B2,0xBEE5CD69,
};

uint32_t pke_big_mod_little_prime_test(void)
{
    uint32_t *a = std_big_mod_little_a;
    uint16_t result[PTL_TEST];
    uint16_t std_out_2[PTL_TEST]={
        2,    3,    3,    4,    0,    7,    9,    5,    6,    24,
        17,   5,    0,    40,   2,    19,   57,   31,   4,    62,
        63,   52,   35,   89,   97,   32,   57,   84,   33,   89,
        27,   4,    34,   91,   91,   128,  21,   140,  17,   56,
        47,   186,  180,  96,   20,   54,   136,  121,  136,  61,    //50
        191,  97,   122,  152,  136,  25,   119,  192,  30,   224,
        48,   96,   222,  36,   58,   6,    128,  341,  124,  297,
        297,  245,  24,   278,  255,  324,  202,  275,  389,  214,
        127,  94,   126,  335,  394,  408,  294,  413,  122,  169,
        346,  424,  445,  23,   84,   201,  183,  437,  359,  456,   //100
#if (PTL_TEST == 400)
        344,  303,  121,  88,   554,  394,  392,  407,  363,  134,
        547,  613,  459,  11,   112,  55,   276,  639,  442,  14,
        195,  323,  362,  278,  541,  654,  561,  492,  565,  704,
        487,  657,  63,   411,  390,  679,  52,   683,  739,  210,
        461,  270,  231,  757,  396,  447,  698,  297,  414,  220,   //150
        539,  808,  231,  722,  160,  568,  834,  813,  495,  277,
        864,  400,  796,  115,  762,  772,  598,  445,  126,  437,
        947,  427,  549,  763,  777,  780,  524,  997,  831,  483,
        234,  299,  884,  43,   214,  740,  573,  466,  637,  6,
        1067, 439,  473,  513,  504,  437,  438,  1013, 1192, 458,   //200
        895,  993,  311,  845,  216,  665,  529,  104,  908,  279,
        1026, 967,  152,  1273, 1054, 91,   778,  354,  1274, 285,
        246,  456,  1305, 612,  21,   1239, 1360, 704,  326,  603,
        345,  873,  829,  316,  1480, 329,  1446, 630,  523,  1299,
        562,  1204, 338,  632,  544,  57,   1506, 552,  812,  1477,  //250
        1206, 279,  1381, 1180, 38,   347,  345,  1512, 1280, 267,
        545,  1156, 511,  188,  262,  1479, 596,  523,  300,  245,
        289,  199,  491,  1481, 783,  560,  1753, 448,  1127, 279,
        590,  1061, 683,  933,  1810, 339,  378,  1400, 1283, 1323,
        1166, 739,  377,  663,  197,  1822, 1813, 1903, 12,   601,   //300
        436,  893,  1033, 378,  397,  1038, 1243, 801,  740,  989,
        362,  657,  1441, 593,  45,   83,   1337, 1255, 82,   949,
        653,  938,  1448, 1340, 1954, 96,   703,  1372, 399,  1313,
        2228, 89,   1061, 1190, 622,  1133, 1368, 1718, 1966, 549,
        1771, 2291, 1871, 191,  1084, 924,  1914, 683,  59,   550,   //350
        566,  2248, 1244, 1213, 2042, 2015, 2266, 1115, 1992, 1142,
        223,  1426, 151,  1717, 110,  609,  961,  1301, 2430, 2107,
        571,  1329, 34,   565,  1302, 168,  1152, 1870, 2243, 2607,
        2179, 411,  1069, 659,  1087, 929,  2546, 1754, 1881, 133,
        2511, 2181, 1250, 1435, 624,  1805, 649,  2234, 2041, 2226,  //400  /**/
#endif
    };

    uint16_t std_out_7[PTL_TEST]={
        0,    2,    5,    9,    3,    0,    14,   2,    12,   11,
        18,   9,    41,   1,    30,   7,    44,   49,   18,   47,
        3,    59,   84,   58,   68,   79,   14,   63,   83,   112,
        127,  83,   6,    18,   85,   93,   154,  46,   72,   161,
        95,   17,   188,  52,   32,   134,  133,  181,  12,   20,    //50
        59,   148,  164,  236,  199,  106,  56,   248,  89,   189,
        52,   11,   4,    287,  156,  43,   173,  78,   16,   42,
        18,   296,  150,  130,  346,  38,   34,   87,   114,  386,
        309,  359,  421,  41,   25,   104,  369,  140,  191,  452,
        97,   289,  342,  164,  309,  306,  262,  359,  409,  483,   //100
#if (PTL_TEST == 400)
        160,  4,    166,  323,  176,  287,  37,   175,  230,  49,
        536,  600,  383,  205,  61,   482,  345,  278,  576,  11,
        139,  561,  304,  164,  95,   17,   131,  570,  292,  159,
        383,  301,  171,  246,  135,  572,  678,  38,   758,  321,
        636,  608,  571,  608,  515,  167,  407,  814,  308,  215,   //150
        327,  364,  651,  416,  217,  772,  564,  611,  550,  50,
        406,  493,  469,  761,  291,  216,  590,  513,  356,  975,
        462,  828,  55,   365,  334,  287,  169,  149,  605,  1029,
        886,  936,  1088, 425,  403,  345,  712,  480,  277,  1108,
        952,  1079, 1083, 1110, 184,  750,  654,  711,  628,  195,   //200
        1181, 802,  131,  1148, 16,   983,  1006, 212,  207,  717,
        840,  589,  592,  429,  721,  261,  708,  717,  919,  1263,
        6,    865,  1061, 1348, 478,  218,  826,  514,  738,  617,
        1066, 829,  113,  1452, 72,   398,  38,   1155, 253,  487,
        1414, 1021, 1205, 740,  1492, 1142, 888,  1221, 1296, 1033,  //250
        253,  1220, 329,  1278, 1288, 1086, 1412, 1335, 1144, 194,
        1518, 237,  1237, 1454, 1423, 645,  1553, 520,  1592, 926,
        628,  976,  215,  1219, 1179, 1738, 450,  488,  1783, 1561,
        1811, 725,  1683, 1857, 204,  303,  1558, 654,  684,  534,
        894,  1668, 1813, 1738, 1470, 1213, 86,   572,  1828, 1003,  //300
        1398, 442,  22,   1422, 1379, 1441, 1803, 1604, 1573, 1573,
        1083, 1160, 1249, 848,  131,  778,  1032, 1913, 325,  1010,
        330,  1798, 452,  928,  976,  1090, 1910, 670,  2059, 888,
        2118, 1316, 691,  1762, 956,  1969, 2199, 104,  170,  1888,
        1838, 569,  1708, 1240, 2016, 1627, 668,  2059, 681,  1939,  //350
        1735, 981,  1484, 1065, 1700, 2291, 2215, 1513, 1413, 2250,
        2082, 1144, 673,  1029, 88,   752,  1470, 348,  30,   2323,
        307,  391,  848,  2470, 936,  2157, 1680, 2297, 1384, 1817,
        2439, 2532, 1469, 252,  140,  44,   2616, 1979, 2250, 1458,
        666,  2465, 2615, 670,  517,  2261, 2099, 1034, 140,  1360,  //400  /**/
#endif
    };

    uint16_t std_out_19[PTL_TEST]={
        2,    0,    3,    2,    3,    14,   0,    12,   4,    3,
        25,   25,   14,   26,   47,   32,   5,    58,   17,   21,
        70,   19,   78,   88,   30,   40,   78,   21,   13,   99,
        54,   93,   46,   6,    29,   27,   137,  56,   37,   125,
        123,  100,  6,    105,  52,   34,   2,    151,  143,  141,   //50
        228,  176,  31,   12,   229,  143,  131,  35,   214,  36,
        158,  110,  86,   217,  0,    159,  109,  210,  225,  238,
        101,  157,  253,  159,  118,  323,  171,  389,  356,  234,
        93,   76,   169,  14,   16,   160,  346,  131,  53,   114,
        180,  121,  67,   440,  368,  404,  435,  16,   400,  255,   //100
#if (PTL_TEST == 400)
        5,    200,  51,   456,  6,    274,  185,  201,  436,  155,
        546,  604,  263,  302,  222,  531,  373,  423,  461,  465,
        643,  574,  38,   328,  238,  264,  703,  564,  219,  294,
        308,  621,  715,  135,  252,  341,  595,  447,  79,   294,
        740,  515,  215,  516,  132,  398,  346,  435,  827,  837,   //150
        555,  347,  3,    371,  393,  51,   735,  723,  718,  211,
        946,  118,  771,  454,  681,  397,  805,  115,  432,  347,
        737,  321,  1,    626,  820,  14,   272,  782,  624,  210,
        812,  15,   382,  817,  732,  123,  395,  988,  687,  127,
        1066, 508,  617,  316,  591,  921,  474,  1044, 908,  739,   //200
        611,  97,   959,  196,  988,  556,  200,  636,  599,  601,
        612,  1191, 425,  497,  206,  289,  111,  662,  1092, 1300,
        249,  74,   563,  100,  1177, 1125, 1426, 224,  222,  714,
        8,    1015, 729,  1028, 367,  598,  455,  480,  614,  1386,
        541,  145,  467,  624,  944,  1121, 439,  580,  185,  45,    //250
        1277, 129,  1005, 1607, 1435, 1196, 1286, 1523, 403,  1291,
        880,  443,  198,  776,  1227, 771,  204,  1639, 532,  648,
        479,  914,  1405, 1324, 518,  1626, 1106, 934,  1411, 777,
        1705, 387,  171,  887,  103,  1818, 514,  1692, 1298, 270,
        1860, 157,  1864, 1100, 1665, 944,  1848, 1725, 1332, 1949,  //300
        1627, 1712, 1289, 21,   531,  454,  673,  1886, 192,  1183,
        1571, 5,    528,  1419, 1235, 899,  582,  1914, 423,  1198,
        869,  690,  353,  726,  1779, 412,  279,  878,  1093, 2103,
        1473, 1897, 583,  1723, 160,  866,  1427, 2140, 2131, 1131,
        152,  1491, 690,  2073, 1719, 622,  1985, 2167, 1228, 1268,  //350
        369,  577,  1984, 347,  2039, 881,  1468, 2191, 1247, 1902,
        2431, 1960, 53,   2402, 923,  2325, 2207, 614,  2461, 977,
        1821, 305,  180,  1146, 808,  2549, 941,  1517, 1370, 1572,
        1732, 2404, 95,   1460, 1815, 637,  2570, 847,  1845, 2508,
        684,  433,  1059, 1382, 629,  35,   1902, 2151, 911,  1006,  //400  /**/
#endif
    };

    uint16_t std_out_43[PTL_TEST]={
        2,    3,    3,    3,    0,    13,   7,    6,    6,    22,
        23,   9,    14,   9,    24,   42,   41,   45,   58,   6,
        59,   74,   85,   65,   17,   34,   95,   66,   92,   121,
        13,   87,   83,   42,   70,   89,   96,   156,  48,   20,
        107,  39,   134,  105,  0,    171,  182,  222,  99,   100,   //50
        69,   216,  39,   66,   151,  84,   188,  72,   120,  193,
        2,    65,   50,   114,  86,   280,  266,  16,   110,  293,
        297,  329,  210,  258,  131,  383,  370,  55,   332,  8,
        348,  410,  215,  54,   379,  221,  58,   308,  109,  431,
        68,   196,  68,   262,  79,   340,  245,  397,  273,  382,   //100
#if (PTL_TEST == 400)
        269,  133,  360,  247,  115,  560,  570,  521,  267,  137,
        143,  442,  290,  189,  217,  176,  633,  580,  101,  76,
        125,  666,  667,  76,   533,  127,  488,  496,  154,  146,
        594,  487,  41,   736,  127,  90,   473,  364,  651,  591,
        639,  219,  333,  794,  613,  344,  539,  502,  323,  854,   //150
        43,   436,  129,  878,  227,  24,   149,  234,  276,  710,
        91,   31,   263,  0,    286,  664,  885,  783,  920,  294,
        250,  107,  115,  179,  782,  411,  185,  392,  528,  216,
        877,  827,  791,  355,  958,  650,  483,  483,  137,  1087,
        731,  871,  114,  478,  220,  108,  149,  658,  952,  232,   //200
        929,  433,  998,  97,   178,  1249, 942,  264,  683,  1017,
        361,  855,  350,  79,   197,  918,  776,  134,  342,  1033,
        547,  1317, 111,  25,   939,  1376, 239,  700,  272,  117,
        862,  298,  1316, 492,  294,  1407, 1270, 545,  107,  1418,
        1273, 84,   231,  1215, 463,  201,  641,  1319, 1316, 1299,  //250
        1351, 268,  263,  570,  626,  384,  1056, 603,  749,  316,
        182,  751,  1437, 934,  1636, 168,  1570, 1244, 775,  867,
        633,  1394, 593,  1740, 1099, 1372, 492,  98,   1595, 1731,
        1457, 1552, 1632, 1458, 883,  1122, 85,   1614, 1227, 731,
        1164, 1361, 133,  572,  1909, 1465, 1030, 273,  1801, 1510,  //300
        851,  5,    1307, 1526, 45,   1030, 1421, 1350, 875,  1889,
        1135, 1430, 873,  1300, 1257, 378,  126,  2075, 1933, 1957,
        793,  611,  1809, 2,    2010, 47,   454,  60,   1092, 1276,
        384,  473,  1788, 1547, 88,   1807, 719,  1353, 2216, 1356,
        1832, 1553, 384,  1163, 1496, 780,  1566, 1809, 1654, 889,   //350
        1456, 2345, 1469, 1619, 2006, 1115, 280,  692,  1805, 652,
        1148, 551,  1341, 190,  107,  727,  876,  737,  876,  1317,
        620,  1941, 768,  1069, 753,  2346, 1900, 21,   2234, 131,
        1585, 518,  818,  1790, 1701, 1622, 1097, 1542, 292,  1306,
        706,  847,  1230, 264,  1582, 1620, 257,  2726, 648,  1524,  //400  /**/
#endif
    };

    uint16_t std_out_64[PTL_TEST]={
        2,    3,    0,    1,    1,    5,    4,    5,    10,   22,
        15,   15,   25,   38,   6,    9,    0,    16,   41,   38,
        35,   13,   41,   62,   95,   70,   61,   61,   40,   122,
        62,   115,  29,   144,  143,  24,   86,   142,  106,  132,
        166,  158,  18,   92,   37,   66,   101,  54,   15,   206,   //50
        2,    24,   187,  175,  208,  24,   181,  221,  8,    280,
        164,  166,  174,  43,   148,  254,  242,  73,   255,  62,
        256,  31,   6,    257,  335,  132,  381,  234,  19,   179,
        296,  362,  10,   358,  173,  84,   364,  138,  37,   153,
        426,  115,  116,  239,  452,  17,   59,   252,  213,  173,   //100
#if (PTL_TEST == 400)
        504,  219,  171,  386,  117,  390,  357,  14,   441,  186,
        252,  109,  233,  462,  389,  128,  460,  18,   9,    550,
        26,   410,  45,   495,  13,   286,  65,   361,  507,  588,
        132,  503,  340,  514,  160,  757,  538,  699,  586,  761,
        403,  294,  653,  540,  194,  572,  806,  845,  47,   313,   //150
        760,  344,  736,  479,  428,  356,  431,  37,   909,  362,
        665,  21,   914,  579,  635,  471,  319,  572,  322,  523,
        23,   440,  718,  815,  280,  432,  450,  999,  551,  792,
        914,  715,  577,  552,  204,  917,  159,  1096, 193,  747,
        771,  214,  906,  282,  11,   550,  266,  456,  799,  608,   //200
        9,    574,  684,  839,  449,  414,  228,  313,  626,  139,
        945,  717,  631,  940,  1095, 949,  1203, 1362, 473,  190,
        442,  1290, 478,  1192, 375,  267,  4,    898,  75,   193,
        138,  1148, 797,  1215, 217,  1088, 129,  1263, 314,  23,
        135,  1514, 900,  682,  1261, 1110, 888,  244,  866,  738,   //250
        707,  112,  756,  1164, 219,  1466, 572,  1531, 1505, 1241,
        1323, 100,  390,  307,  1284, 113,  391,  1305, 367,  1707,
        1533, 958,  1413, 1703, 618,  663,  425,  686,  1768, 875,
        345,  774,  862,  1141, 535,  1274, 584,  955,  1478, 1024,
        207,  1098, 36,   79,   1663, 1518, 1198, 709,  1069, 683,   //300
        1371, 748,  331,  1367, 1786, 1609, 800,  288,  1618, 1631,
        698,  113,  1006, 1144, 1204, 1640, 486,  1350, 0,    1470,
        420,  351,  130,  1716, 1802, 2055, 654,  424,  822,  176,
        781,  2188, 1833, 849,  1863, 1988, 1054, 1458, 1124, 1458,
        1638, 192,  1744, 186,  1460, 1992, 475,  2107, 1761, 2316,  //350
        804,  1436, 225,  1373, 988,  1724, 145,  147,  1172, 1860,
        983,  1383, 1292, 1530, 1376, 621,  540,  1541, 1154, 1548,
        1710, 120,  2069, 1828, 705,  2522, 1199, 116,  1604, 791,
        2622, 2533, 2056, 1702, 1160, 2125, 1634, 1760, 459,  2179,
        1465, 965,  487,  2480, 1121, 101,  397,  244,  1044, 2421,  //400  /**/
#endif
    };

    uint16_t std_out_128[PTL_TEST]={
        1,    2,    4,    5,    4,    14,   3,    19,   18,   19,
        31,   4,    37,   41,   22,   37,   35,   38,   43,   40,
        22,   7,    37,   24,   13,   60,   49,   7,    9,    104,
        120,  63,   9,    94,   120,  19,   74,   86,   11,   68,
        4,    42,   16,   71,   98,   136,  155,  11,   34,   62,    //50
        159,  55,   165,  148,  136,  76,   265,  157,  199,  63,
        226,  85,   89,   195,  224,  218,  177,  101,  105,  285,
        299,  366,  364,  86,   212,  333,  138,  42,   377,  284,
        77,   352,  347,  288,  411,  363,  80,   109,  16,   444,
        437,  66,   256,  290,  8,    169,  199,  161,  8,    458,   //100
#if (PTL_TEST == 400)
        188,  324,  239,  31,   269,  269,  223,  17,   132,  179,
        76,   120,  497,  437,  471,  217,  317,  523,  516,  77,
        405,  411,  660,  352,  259,  425,  287,  378,  622,  165,
        460,  334,  202,  550,  277,  568,  105,  451,  706,  556,
        728,  323,  610,  0,    399,  618,  241,  530,  534,  513,   //150
        576,  825,  219,  278,  166,  719,  788,  233,  584,  216,
        917,  28,   826,  110,  702,  887,  490,  789,  554,  566,
        910,  219,  935,  820,  941,  736,  887,  507,  414,  167,
        807,  863,  228,  683,  190,  1013, 436,  1104, 809,  1058,
        377,  524,  859,  1065, 51,   82,   1209, 427,  646,  48,    //200
        349,  870,  1001, 596,  685,  27,   1027, 226,  1061, 1150,
        191,  985,  1008, 133,  19,   321,  1252, 614,  1357, 592,
        595,  1181, 1208, 1288, 421,  965,  96,   500,  398,  1144,
        1024, 28,   342,  14,   1417, 590,  564,  1112, 1205, 1414,
        931,  40,   593,  458,  1034, 888,  346,  1097, 69,   799,   //250
        94,   1023, 423,  1018, 260,  794,  499,  559,  913,  190,
        793,  67,   907,  1561, 948,  259,  365,  1675, 1583, 918,
        733,  1547, 1661, 102,  1351, 615,  1037, 842,  1291, 491,
        1619, 474,  1107, 8,    1829, 1326, 67,   698,  1518, 489,
        900,  1852, 1281, 190,  871,  1632, 851,  879,  162,  339,   //300
        1417, 1154, 1676, 1631, 1231, 323,  240,  1524, 31,   998,
        83,   240,  1257, 537,  1979, 1393, 186,  918,  231,  1211,
        948,  1344, 1048, 682,  679,  325,  1876, 1140, 173,  845,
        1493, 347,  567,  1791, 956,  1019, 1027, 1587, 1402, 1957,
        1653, 1443, 1971, 1369, 2321, 1266, 950,  944,  1323, 2145,  //350
        1311, 677,  2053, 1877, 660,  822,  924,  693,  1509, 391,
        2112, 2291, 1915, 1589, 1988, 1373, 166,  816,  1790, 1366,
        2060, 474,  1930, 1974, 2320, 2221, 1248, 591,  498,  1746,
        70,   1374, 377,  1307, 1525, 1274, 1200, 235,  1241, 1388,
        2384, 1345, 1930, 698,  1938, 2498, 2395, 1415, 2261, 1557,  //400  /**/
#endif
    };

    //aWordLen is from 1 to 128, and b is 269, thus PrimeTable[55]
    uint16_t std_out_269[128]={
        183, 25,  105, 199, 28,  179, 106, 223, 185, 18,
        90,  5,   69,  97,  154, 49,  222, 83,  143, 77,
        164, 192, 162, 95,  128, 119, 185, 125, 14,  151,
        29,  14,  47,  71,  119, 52,  225, 66,  184, 178,
        107, 7,   84,  70,  97,  221, 76,  51,  195, 98,
        99,  206, 232, 148, 145, 12,  232, 123, 73,  219,
        257, 83,  1,   24,  233, 248, 185, 38,  204, 198,
        101, 118, 130, 9,   20,  75,  71,  162, 18,  95,
        117, 46,  185, 255, 80,  26,  264, 13,  211, 131,
        194, 13,  117, 254, 190, 2,   220, 21,  89,  147,
        203, 159, 55,  52,  41,  238, 183, 57,  239, 17,
        113, 123, 250, 227, 84,  213, 185, 115, 165, 250,
        89,  246, 214, 14,  134, 120, 164, 76,
    };

    //check bigint_div_table_low function
#if (PTL_TEST == 100)
    uint8_t std_out_low[500]={
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff,
    };
#elif (PTL_TEST == 400)
    uint8_t std_out_low[500]={
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff,
    };
#endif

    uint32_t i;
    uint32_t ret;

    //aWordLen = 2
    ret = big_mod_little_prime_test(a, 2, std_out_2);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 2) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 2) success");
    }

    //aWordLen = 7
    ret = big_mod_little_prime_test(a, 7, std_out_7);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 7) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 7) success");
    }

    //aWordLen = 19
    ret = big_mod_little_prime_test(a, 19, std_out_19);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 19) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 19) success");
    }

    //aWordLen = 43
    ret = big_mod_little_prime_test(a, 43, std_out_43);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 43) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 43) success");
    }

    //aWordLen = 64
    ret = big_mod_little_prime_test(a, 64, std_out_64);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 64) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 64) success");
    }

    //aWordLen = 128
    ret = big_mod_little_prime_test(a, 128, std_out_128);
    if(ret)
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 128) failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n big_mod_little_prime_test(aWordLen = 128) success");
    }



    //a wordlen is from 1 to 128, check a mod PrimeTable[55]
    for(i=1;i<=128;i++)
    {
        ret = bigint_div_table_high(a, i, (uint16_t *)PrimeTable_r, (double_uint32_t *)PrimeTable_s, result, PTL_TEST);
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n big_mod_little_prime_test(aWordLen = %u) failure, ret=%x", i, ret);
            return 1;
        }


        if(result[55] != std_out_269[i-1])
        {
            printf("\r\n big_mod_little_prime_test(aWordLen = %u) failure, result=%x", i, result[55]);
            return 1;
        }
    }
    printf("\r\n big_mod_little_prime_test(aWordLen is from 1 to 128) success");


    //check bigint_div_table_low
    ret = bigint_div_table_high(a+1, 128-1, (uint16_t *)PrimeTable_r, (double_uint32_t *)PrimeTable_s, result, PTL_TEST);
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n big_mod_little_prime_test(hign part, i=%u) failure, ret=%x", i+1, ret);
        return 1;
    }

    for(i=0; i<500; i++)
    {
        ret = bigint_div_table_low(a, (uint16_t *)PrimeTable_r, (double_uint32_t *)PrimeTable_s, result, PTL_TEST);
        if(std_out_low[i] != (ret & 0xFF))
        {
            printf("\r\n big_mod_little_prime_test(low part, i=%u) failure, ret=%x", i+1, ret);
            return 1;
        }

        a[0] += 2;
    }

    a[0] -= i*2;
    printf("\r\n big_mod_little_prime_test(check bigint_div_table_low) success");

    return 0;
}


#ifdef PKE_HP
/************** test for little integer of 64 bits ***************/
extern void pke_set_microcode(uint32_t addr);
extern void pke_clear_interrupt(void);
extern void pke_start(void);
extern void pke_wait_till_done(void);
extern uint32_t pke_check_rt_code(void);

uint32_t bigint_mod_uint64(uint32_t *a, uint32_t aWordLen, uint32_t b[2], uint32_t r[2], uint32_t s[2],
        uint32_t result[2])
{
    uint32_t tmp_step, ret;
    uint32_t *p, *q;

    tmp_step = pke_set_operand_width(aWordLen<<5);

    p = (uint32_t *)(rPKE_B(1,tmp_step));
    q = (uint32_t *)(rPKE_A(2,tmp_step));

    pke_load_operand((uint32_t *)(rPKE_A(1,tmp_step)), a, aWordLen);

    //clear the high part(this action can not be deleted)
    if(tmp_step > aWordLen)
    {
        uint32_clear((uint32_t *)(rPKE_A(1,tmp_step))+aWordLen, (tmp_step/4)-aWordLen);
    }

    p[0] = b[0];
    p[1] = b[1];
    p[2] = r[0];
    p[3] = r[1];
    p[4] = s[0];
    p[5] = s[1];

    pke_set_microcode(MICROCODE_MODRES);    //must be called every time

    pke_clear_interrupt();

    pke_start();

    pke_wait_till_done();

    ret = pke_check_rt_code();
    if(PKE_SUCCESS != ret)
    {
        return ret;
    }

    result[0] = q[0];
    result[1] = q[1];

    return PKE_SUCCESS;
}


uint32_t bigint_mod_uint64_std_test(void)
{
    uint32_t *a = std_big_mod_little_a;   //4096 bits
    uint32_t b[2]={0xB2E983D6, 0xB8CE9016};
    uint32_t r[2]={0x4D167C2A, 0x47316FE9};
    uint32_t s[2]={1,0};
    uint32_t result[2];

    uint32_t std_out[]={
        0x16c1efb1,0x00000000,
        0x63d86bdb,0x4718e5a1,
        0x32a2e9a9,0x5ceefffe,
        0xed95ecb7,0x4def3bf3,
        0xb7dd1db3,0xb5acb043,
        0x239b25a7,0x75f970d7,
        0x3b254235,0x8b4fe65a,
        0x21fcdd27,0x0ae21edf,
        0x147d1885,0x3af67401,
        0x3e733113,0x226eb3d9,
        0x61651117,0x41006bfa,
        0x65b67c07,0x480265f3,
        0x3dd7e71d,0x629c581f,
        0x814aa129,0x8ea0b3bb,
        0xedfe8ae9,0xb55984d6,
        0x838e557d,0x128f148f,
        0x6c57eb2f,0x63d79331,
        0x7f71f45b,0xa0cd0964,
        0xc7f75f09,0x25f756c1,
        0x3f58a36b,0x41904202,
        0xc2f0ea85,0x789d2190,
        0x2f740fd7,0xaa7b138f,
        0x312d0d2d,0x0c737a13,
        0xa154134f,0x9eb04836,
        0x619ca033,0x11facbe9,
        0x4d68fa27,0x9413a755,
        0xd08bfc81,0x1a86fc6d,
        0x41a69887,0x665dcf94,
        0x6fe4ca8d,0x01c56fd4,
        0xb8d550cb,0x387cb804,
        0x2a2f48a7,0x6a7efadd,
        0x16628fed,0x572f677f,
        0x76aac95d,0x2133694f,
        0x4cf41691,0x9688f16e,
        0xf77a77a1,0x60dc5fa1,
        0xac0126d7,0x7413445c,
        0x11b8c10b,0x5d0b6e4c,
        0x473f87cd,0x6a09aab9,
        0x0f9fde09,0x3e12b432,
        0xeb3b38b9,0x28bbd07c,
        0x9ec3f0c7,0x71521638,
        0x11b557f1,0x29bd21ca,
        0xa98caa5f,0x2d667725,
        0x96d8c54f,0x2da86a9e,
        0x83226cfb,0x5c10f211,
        0x1da9d4a1,0x0f1130fd,
        0xdbb5308b,0x9b0875c5,
        0xc7ba00d7,0x0061b520,
        0xf9cb59d9,0x362913f3,
        0x30b2d903,0xb32267e1,
        0xd4cfd975,0xa84141ac,
        0xcecd3e1f,0x20d11dc0,
        0xa0c2b6a5,0x35720d8d,
        0xe584c9bf,0x98a1dea4,
        0x956a2249,0x26791afa,
        0xbc16814d,0x2650fb9b,
        0x8961b6e9,0x25b7e86f,
        0x888d8fd7,0xa2b01d9c,
        0xbb2098bb,0x2b8504ee,
        0x6cd06249,0x2135cac2,
        0x641a278f,0x92d33c86,
        0xf67ac1e5,0x2ff9c521,
        0x683c36d1,0x5a8841b7,
        0xb51454af,0x679cd2f9,
        0x69ee59e7,0x88e10c6b,
        0x6bafab67,0x0e1e549e,
        0xb418b22d,0x6c4df855,
        0x4285e26d,0x008e30c4,
        0x1428102d,0x980242d0,
        0x696845d3,0x4a33a4db,
        0x144862dd,0x7b9c2937,
        0xbeebabad,0x076c0771,
        0xb021c9d5,0x796fbd4b,
        0xdd5edd79,0xb1f10b2e,
        0x4cadf1b5,0x2c5d9655,
        0xb0a252ff,0x3f04605e,
        0x8c0c3b87,0x6d4c56d8,
        0x9bbdcdb5,0xa55a27f8,
        0x50f894a3,0x0c0d1948,
        0x4be95a67,0x14df6bb6,
        0x1a809033,0x824089b1,
        0xde1c2347,0xae83b575,
        0x44b90a3f,0x3c6c61c4,
        0x3f5073a7,0x3845853d,
        0xc4367f13,0x9f476edb,
        0x3d9c3ddf,0x0f69a714,
        0x3fc34b0d,0x6b9935ca,
        0x17184c2f,0x5244338c,
        0x117a40cd,0x34949167,
        0x6a2c7a1f,0x08cb37e4,
        0xb12994ab,0x27ee1419,
        0x97538061,0x10696d68,
        0xad402ab3,0x41e258f2,
        0x2e625f1d,0x363e76c7,
        0x174ae54d,0x47266455,
        0x07168871,0x7446b640,
        0x691713a1,0x73542acf,
        0xcc43553d,0xa8054cc3,
        0x80c1e541,0xa7591ae7,
        0xbfaa77e9,0x70fe847a,
        0x0c85ffad,0x867b110e,
        0x73248059,0xa15a8c26,
        0x5f9de529,0x1496097c,
        0x34fc68d5,0x880e2c7d,
        0xdf17fccd,0x5aeeb256,
        0x6c0238f5,0x097c6ec6,
        0xa5329c4f,0x2448e4f9,
        0x54dc2be3,0x78076033,
        0xef94eb19,0x2b47307e,
        0xd1070ed5,0x1d115320,
        0xa19ea5fd,0x3cbe3ed9,
        0x05d882c7,0x44a1c344,
        0xff78a00b,0x7c500ca2,
        0xe97acc05,0x4ee2396a,
        0xf87798c7,0x727c7d71,
        0x2933118b,0x89ec27ab,
        0x652755d3,0x5d7a7f76,
        0x8922ce51,0x6a26514a,
        0xc9e7dcf9,0x27f43710,
        0x0a012691,0x6c32a8d9,
        0xca7d60a3,0x24d977bc,
        0xf0d0d1e7,0xacc83345,
        0x34a4af01,0x7fc25b92,
        0xcfd8c869,0x90d658ca,
        0xb6ed570f,0x9ae4ffef,
        0xda416dcb,0x330c8077,
        0x760dfba3,0x3977c31e,
        0x1db583bd,0x8b7fab84,
    };

    uint32_t i;
    uint32_t ret;

    for(i=1;i<=128;i++)
    {
        ret = bigint_mod_uint64(a, i, b, r, s, result);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(result, 2, std_out+(i-1)*2, 2))
        {
            printf("\r\n bigint_mod_uint64_std_test(aWordLen = %u) failure, ret=%x", i, ret);
            print_BN_buf_U32(result, 2, "result");
            return 1;
        }
    }
    printf("\r\n bigint_mod_uint64_std_test success");

    return 0;
}

uint32_t bigint_mod_uint64_corner_test(void)
{
    uint32_t a[128];   //4096 bits
    uint32_t b[2]={0xFFFFFFFF, 0xFFFFFFFF};
    uint32_t r[2]={1, 0};
    uint32_t s[2]={1, 0};
    uint32_t result[2];

    uint32_t std_out[]={
        0xFFFFFFFF,0,
        0,0,
    };

    uint32_t i;
    uint8_t ret;

    uint32_set(a, 0xFFFFFFFF, 128);

    for(i=1;i<=128;i++)
    {
        ret = bigint_mod_uint64(a, i, b, r, s, result);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(result, 2, std_out+((i-1)%2)*2, 2))
        {
            printf("\r\n bigint_mod_uint64_corner_test(aWordLen = %u) failure, ret=%x", i, ret);
            print_BN_buf_U32(result, 2, "result");
            return 1;
        }
    }
    printf("\r\n bigint_mod_uint64_corner_test success");

    return 0;
}
#endif

uint32_t pke_big_mod_little_test(void)
{
    if(pke_big_mod_little_prime_test())
        return 1;

#ifdef PKE_HP
    if(bigint_mod_uint64_std_test())
        return 1;

    if(bigint_mod_uint64_corner_test())
        return 1;
#endif

    return 0;
}

#endif
