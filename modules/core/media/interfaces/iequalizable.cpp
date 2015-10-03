#include "iequalizable.h"

IEqualizable::IEqualizable(QObject * parent) : ISpectrumable(parent), eqInUse(false) {

//    Октава →
//    Нота ↓ 	Суб-контр 	Контр 	Большая 	Малая 	1 	2 	3 	4 	5 	6
//    C 	16,352 (−48) 	32,703 (−36) 	65,406 (−24) 	130,81 (−12) 	261,63 (0) 	523,25 (+12) 	1046,5 (+24) 	2093,0 (+36) 	4186,0 (+48) 	8372,0 (+60)
//    C♯ / D♭ 	17,324 (−47) 	34,648 (−35) 	69,296 (−23) 	138,59 (−11) 	277,18 (+1) 	554,37 (+13) 	1108,7 (+25) 	2217,5 (+37) 	4434,9 (+49) 	8869,8 (+61)
//    D 	18,354 (−46) 	36,708 (−34) 	73,416 (−22) 	146,83 (−10) 	293,66 (+2) 	587,33 (+14) 	1174,7 (+26) 	2349,3 (+38) 	4698,6 (+50) 	9397,3 (+62)
//    D♯ / E♭ 	19,445 (−45) 	38,891 (−33) 	77,782 (−21) 	155,56 (−9) 	311,13 (+3) 	622,25 (+15) 	1244,5 (+27) 	2489,0 (+39) 	4978,0 (+51) 	9956,1 (+63)
//    E 	20,602 (−44) 	41,203 (−32) 	82,407 (−20) 	164,81 (−8) 	329,63 (+4) 	659,26 (+16) 	1318,5 (+28) 	2637,0 (+40) 	5274,0 (+52) 	10548 (+64)
//    F 	21,827 (−43) 	43,654 (−31) 	87,307 (−19) 	174,61 (−7) 	349,23 (+5) 	698,46 (+17) 	1396,9 (+29) 	2793,8 (+41) 	5587,7 (+53) 	11175 (+65)
//    F♯ / G♭ 	23,125 (−42) 	46,249 (−30) 	92,499 (−18) 	185,00 (−6) 	369,99 (+6) 	739,99 (+18) 	1480,0 (+30) 	2960,0 (+42) 	5919,9 (+54) 	11840 (+66)
//    G 	24,500 (−41) 	48,999 (−29) 	97,999 (−17) 	196,00 (−5) 	392,00 (+7) 	783,99 (+19) 	1568,0 (+31) 	3136,0 (+43) 	6271,9 (+55) 	12544 (+67)
//    G♯ / A♭ 	25,957 (−40) 	51,913 (−28) 	103,83 (−16) 	207,65 (−4) 	415,30 (+8) 	830,61 (+20) 	1661,2 (+32) 	3322,4 (+44) 	6644,9 (+56) 	13290 (+68)
//    A 	27,500 (−39) 	55,000 (−27) 	110,00 (−15) 	220,00 (−3) 	440,00 (+9) 	880,00 (+21) 	1760,0 (+33) 	3520,0 (+45) 	7040,0 (+57) 	14080 (+69)
//    A♯ / H♭ 	29,135 (−38) 	58,270 (−26) 	116,54 (−14) 	233,08 (−2) 	466,16 (+10) 	932,33 (+22) 	1864,7 (+34) 	3729,3 (+46) 	7458,6 (+58) 	14917 (+70)
//    H 	30,868 (−37) 	61,735 (−25) 	123,47 (−13) 	246,94 (−1) 	493,88 (+11) 	987,77 (+23) 	1975,5 (+35) 	3951,1 (+47) 	7902,1 (+59) 	15804 (+71)


    // 31 line equalizer // 20 25 31 40 50 63 80 100 125 160 200 250 315 400 500 630 800 1k 1.25k 1.6k 2k 2.5k 3.15k 4k 5k 6.3k 8k 10k 12.5k 16k 20k

    // octaves // 20 40 80

    // best fBandwidth is eq to 1/3 ~ 1.26
    // if begin from 100 * 1.26 = 126 * 1.26 = 159 * 1.26 = 200 and etc

//    eqBands.insert(20, QStringLiteral("20"));
//    eqBands.insert(32, QStringLiteral("32"));
//    eqBands.insert(64, QStringLiteral("64"));
//    eqBands.insert(90, QStringLiteral("90"));
//    eqBands.insert(125, QStringLiteral("125"));
//    eqBands.insert(160, QStringLiteral("160"));
//    eqBands.insert(200, QStringLiteral("200"));

//    eqBands.insert(250, QStringLiteral("250"));
//    eqBands.insert(375, QStringLiteral("375"));
//    eqBands.insert(500, QStringLiteral("500"));
//    eqBands.insert(750, QStringLiteral("750"));
//    eqBands.insert(1000, QStringLiteral("1k"));
//    eqBands.insert(1500, QStringLiteral("1.5k"));

//    eqBands.insert(2000, QStringLiteral("2k"));
//    eqBands.insert(3000, QStringLiteral("3k"));
//    eqBands.insert(4000, QStringLiteral("4k"));
//    eqBands.insert(8000, QStringLiteral("8k"));
//    eqBands.insert(12000, QStringLiteral("12k"));
//    eqBands.insert(16000, QStringLiteral("16k"));

    presets.insert((current_preset = QStringLiteral("4 octaves")), EqualizablePreset(4));
    presets.insert(QStringLiteral("3 octaves"), EqualizablePreset(3));
    presets.insert(QStringLiteral("2 octaves"), EqualizablePreset(2));
    presets.insert(QStringLiteral("1/3 octave"), EqualizablePreset(1.0/3));
}
