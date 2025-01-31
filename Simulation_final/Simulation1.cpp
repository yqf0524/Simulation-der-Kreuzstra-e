//====================================================================
// Simulation dynamischer Systeme mit PLAN
//====================================================================
// Projektbeschreibung: Simulation einer Kreuzung
//
//        Autorin: Tifei Xu
//        Autoren: Qingfeng Yang, Xiao cui
//
//====================================================================
#include <vcl.h>
#pragma hdrstop
#include "Plan.h"
#include <math.h>


// ====================================================================
// Klasse Road
// ===================================================================
class Road {
    int *s, n, m, p;
public:
    Road(){s = 0; n = m = p = 0;}
    //Road(int i){ s = 0; n = m = p = 0; Size(i);}
    //Road(int i, int j){ s = 0; n = m = p = 0; Size(i,j);}

    Road(int i, int j, int k){ s = 0; n = m = p = 0; Size(i,j,k);}

    Road(const Road &t){ *this = t;}
    ~Road(){ if(s) delete [] s;   }

    __property int N = { read = n};
    __property int M = { read = m};
    __property int P = { read = p};

    int &operator () (int i, int j, int k ){
        static int z = 0; if(!s) return z;
        return s[i*n*p + j * p + k];}

    Road &operator = (int i) {
        int l = n*m*p;  while(l-->0) {s[l] = i;}
        return *this;}

    Road &operator = (Road &t){
        if((n!=t.n)||(m!=t.m)||(p!=t.p)) Size(t.m, t.n, t.p);
        if(n*m*p) for(int j=0;j<n*m*p;j++) s[j]=t.s[j];
        return *this;}

    Road &operator = (const Road &t){
        if((n!=t.n)||(m!=t.m)||(p!=t.p)) Size(t.m, t.n, t.p);
        if(n*m*p) for(int j=0;j<n*m*p;j++) s[j]=t.s[j];
        return *this;}

    int Adress() {return *s;}
    int Size() { return n*m*p;}
    int Size(int i, int j,int k){
        if(s) delete [] s; n = m = p = 0;
        if(i*j*k > 0) { s = new int [i*j*k];  m = i; n = j; p = k;}
        return n*m*p;}
};


// ====================================================================
// Klasse TUser
// ====================================================================

class TUser : public TPlan {
   // Road road;

    int FW, RW, GW, TW;     // fahrbahnWeite, radwegWeite, gehwegWeite, trambahnWeite;
    int RRUx, RRUy, RRDx, RRDy;     // Renbenring, upside and downside.
    int HSLx, HSLy, HSRx, HSRy;     // HamburgerStrasse, left side and right side.
    int WRUx, WRUy, WRDx, WRDy;     // Wendenring, upside and downside.
    int LSLx, LSLy, LSRx, LSRy;     // Lampestrasse, left side and right side.
    int r;      // arc radius
    int rLamp;   // Lamp Radius
    int sp;  // space between roads
    bool rad, passant;
    double ampelRad, ampelPass;
    bool tramNachOben, tramNachUnten;
    bool goCrossT1, goCrossT2;
    int countT1, countT2;
//============================== Lamp control ==================================
    int A1, A2, A3;
    int B1, B2, B3;
    int C1, C2, C3;
    int D1;
    int E1, E2;
    int randomTime;
//                 time interval
    int ratio;  // ratio between CallRunCount and times
    int T_B1B2, T_A1, T_C3, T_Gesamt;
    int stop1, stop2;
    int waitTimeT1, waitTimeT2;
    bool return_T1, return_T2; // Lamp signalrecovery
    bool tramAuto, SpurOn;
//==============================================================================
//                Fahrbahn Zone
    double rrx, rry1, rry2, rry3;   // Koordinate Renbenring
    double wrx, wry1;   // Koordinate Wendenring
    double kcar, b1, b12, b2, b23, b3;  // slope and intercept
    int y0, y1;

    int wry4;      //  Bus
    double kbus, b4, b45, b5;
    
    double hsy, hsx1, hsx2;       // Tram
    double lsy, lsx1, lsx2;
    double ktram, ktemp, bTL, bTR;
    int PR0, PR1, PR2, PL0, PL1, PL2;
//==============================================================================
    int time;
    int RadPo[2];
    int PassPo[2];
    int pose; // Control passant moving

//==============================================================================
//              AutoParam
    double AutoW, AutoL, AutoTyre, AutoAxle;

//==============================================================================
//              Turning Position Param

    int rt1x, rt1y, rt2x, rt2y, rt3x, rt3y, rt4x, rt4y,
        rt5x, rt5y, rt6x, rt6y, rt7x, rt7y;
    int R1, R2, R3, R4, R5, R6, R7;

//==============================================================================
//              TurningParam

    int VMax, PTroedel;

    Road road11,road12,road13, road21, road22,road31,road32, road4;
    Road road5, road61, road62, road7, road81,road82, road91,road92;
    int length1,length2,length3,length4,length5,length6,length7,length8,length9;

    int Zeile;
    int signal;
    int safeDistance;
    double PI;

    int acc;
    bool ein;
    double fre1,fre2,fre3,fre4,fre5,fre6,fre7,fre8,fre9;

    double Gleit;

//==============================================================================
//            statistic Parameter     
    bool isStopped;
    double numOfSettingCar;
    int countOfCar, sumOfSpeed;
    int sumOfCar1,sumOfCar3,sumOfCar6,sumOfCar8,sumOfCar9;
    int durch1,durch2,durch3,durch4,durch5,durch6,durch7,durch8,durch9;
    double vAvg1,vAvg3,vAvg6,vAvg8,vAvg9;
    float aveSpeed;

    int b1b2, a1, c3,tt;

//==============================================================================
//================================ Plot Parameter ==============================
    real tMeasure,a,xMax,xMin,yMax,yMin,unit;
    int pos,sleepTime;
    real halter9 [500],halter1 [500],halter3 [500],halter6 [500];
    bool linieEin,linieZwei;

    int startToMove;
    double winWidth;
    int periode;
    double volume1[10],volume3[10],volume6[10],volume8[10],volume9[10];

    int plot0bool, plot1bool;

//==============================================================================
//============================== =taste parameter ==============================
    bool plotVis;
    int choose;
    int test;
    bool AK_ampel;
    
//==============================================================================

public:
    void Init();
    void InitParam();
    void InitKreuzZone();
    void InitTrambahn();
    void InitFahrbahn();
    void InitRadweg();
    void InitGehweg();
    void InitArrowR1(int x, int y);  // Rebenring, Model 1; Arrow Fahren nach rechts
    void InitArrowG1(int x, int y);  // Rebenring, Model 1; Arrow Fahren nach Gerade
    void InitArrowL1(int x, int y);  // Rebenring, Model 1; Arrow Fahren nach Links
    void InitArrowR2(int x, int y);  // Hamburgerstrasse, Model 2; Arrow Fahren nach rechts
    void InitArrowG2(int x, int y);  // Hamburgerstrasse, Model 2; Arrow Fahren nach Gerade
    void InitArrowL2(int x, int y);  // Hamburgerstrasse, Model 2; Arrow Fahren nach Links
    void InitArrowR3(int x, int y);  // Wendenring, Model 3; Arrow Fahren nach rechts
    void InitArrowG3(int x, int y);  // Wendenring, Model 3; Arrow Fahren nach Gerade
    void InitArrowL3(int x, int y);  // Wendenring, Model 3; Arrow Fahren nach Links
    void InitArrowR4(int x, int y);  // Lampestrasse, Model 4; Arrow Fahren nach rechts

//==============================================================================
//      Strssen, abbiegen oder gerade fahren
//==============================================================================
    void HS2RR();  // Hamburgerstrasse nach Rebenring
    void HS2WR();  // Hamburgerstrasse nach Wendenring
    void HS2LS();  // Hamburgerstrasse nach Lampestrasse
    void WR2RR();  // Wendenring nach Rebenring
    void WR2HS();  // Wendenring nach Hamburgerstrasse
    void WR2LS();  // Wendenring nach Lampestrasse
    void LS2RR();  // Lampestrasse nach Rebenring
    void RR2HS();  // Rebenring nach Hamburgerstrasse
    void RR2WR();  // Rebenring nach Wendenring
    void RR2LS();  // Rebenring nach Lampestrasse

//==============================================================================
//      Radweg und Radweg, Fahrbereich
//==============================================================================
    void RadwegRR(int ampelRad, bool rad);    // Radweg auf der Rebenring
    void RadwegHS(int ampelRad, bool rad);    // Radweg auf der Hamburgerstrasse
    void RadwegWR(int ampelRad, bool rad);    // Radweg auf der Wendenring
    void RadwegLS(int ampelRad, bool rad);    // Radweg auf der Lampestrasse
    void GehwegRR(int ampelPass, bool passant);    // Gehweg auf der Rebenring
    void GehwegHS(int ampelPass, bool passant);    // Gehweg auf der Hamburgerstrasse
    void GehwegWR(int ampelPass, bool passant);    // Gehweg auf der Wendenring
    void GehwegLS(int ampelPass, bool passant);    // Gehweg auf der Lampestrasse
    
//==============================================================================
//      Ampel auf der Strasse
//==============================================================================
    void DrawLamp();
    void SetLampColor(int lamp);
    //void InitLampFarbe();
    void TextFarbe(int lamp);
    void DrawArrowH(int x1, int y1, int x2, int y2);
    void DrawArrowV(int x1, int y1, int x2, int y2);
    
//==============================================================================
//      Draw Tram
//==============================================================================
    void DrawTramNachOben(int y);
    void DrawTramNachUnten(int y);
    int DrawTramTeil1(double x, int y, double sink, double cosk);
    int DrawTramTeil2(double x, int y, double sink, double cosk);
    void DrawTram();
    
//==============================================================================
//      Draw cars, bikes and passants
//==============================================================================
    void DrawAutoHori(int x, int y);
    void DrawAutoVerti(int x, int y);
    void DrawAutoRR22WR1(int x, int y);
    void DrawAutoRR22WR2(int x, int y);

    void DrawAutoRR22HS(int R, int Rx, int Ry, double w);      
    void DrawAutoRR22LS(int R, int Rx, int Ry, double w);
    void DrawAutoWR22HS(int R, int Rx, int Ry, double w);
    void DrawAutoWR22LS(int R, int Rx, int Ry, double w);

    void DrawRad(int x, int y);
    void DrawPass(int x, int y);
    void DrawAutoGerade(int x, int y);
    void DrawAutoAbbiegen(int R, int Rx, int Ry, double w);
    void InitVerkehr();

//==============================================================================
//       Car running
//==============================================================================
    void InitRoad();

    void Road1();
    void Road2();
    void Road3();
    void Road4();
    void Road5();
    void Road6();
    void Road7();
    void Road8();
    void Road9();
    bool findCar(Road& road, int i);
    void firstCar(int AutoL, Road& road, int v);
    void calcAvg(Road& road, int& sumOfCar, double& vAvg, int i);
    void RunningRoad(Road& road, int lampSignal, int stopPosition );
    void RunningRoad2(Road& road,Road& roadS, int lampSignal, int stopPosition);
    void carGenerateCar();
    void GenerateCar();
    void ResetRoad();

    void adaptiveAmpel();

    void RunTaste0();
    void RunTaste1();
    void RunTaste4();
    void RunTaste5();
    void RunTaste6();
    void RunTaste7();
    void RunTaste8();
    void RunTaste9();
    void RunTaste12();
    void RunTaste13();
    void RunTaste14();
    void RunTaste15();
    void RunTaste16();
    void RunTaste17();
    void RunTaste18();
    void RunTaste19();


//==============================================================================
//       Plot beschreiben
//==============================================================================
    void plotInit();
    void plot0Run();
    void plot1Run(); 

    void histogram(double* vol);
    void runningline(int tpoint, real* saver, int pointer);
//==============================================================================
//       Run aufrufen
//==============================================================================
    void Run();
    void Reset();
    void RunAuto();
    void LampControl(int T_A1B1, int T_A1, int C3); // Lamp control
    void DrawCrossRoad();
    //===============================
};

void TUser::Init() {
    Clear(Grau);
    ProgrammName = "Kreuz Simulation";
    ProgrammInfo = "Dies ist eine Verkehrssimulation\r"
                   "einer Kreuzung 'Hamburgerstrasse' mit\r"
                   "der Simulationsumgebung PLAN";
    InitParam();
    InitFahrbahn();
    InitGehweg();
    InitRadweg();
    InitTrambahn();
    DrawLamp();

    InitRoad();
    plotInit();
    test = -1;
    sleepTime = 30;

    InsertTaste(0,"TramNachUnten", "draw Tram left side nach unten.");
    InsertTaste(1,"TramNachOben", "draw Tram right side nach oben.");
    InsertTaste(4," <  +  > ");
    InsertTaste(5," <  -  >  ");
    InsertTaste(6,"Beschleunigung");
    InsertTaste(7,"Vmax");
    InsertTaste(8,"Generation");
    InsertTaste(9,"auto(Tram)", "Ein Tram wird in jedem Zyklus erzeugt.");
    InsertTaste(12,"Plot");
    InsertTaste(13,"konst. Ample");
    InsertTaste(15,"Verkehrsvolume");
    InsertTaste(14,"Geschwindigkeit");
    InsertTaste(16,"zur Arbeit");
    InsertTaste(17,"nach Hause");
    InsertTaste(18,"schneller");
    InsertTaste(19,"langsamer");


}

void TUser::Run(){
    Clear(Grau);
    InitFahrbahn();
    InitGehweg();
    InitRadweg();
    LampControl(T_B1B2, T_A1, T_C3);
    DrawLamp();
    DrawCrossRoad();
    InitTrambahn();
    DrawTram();

    Road1();
    Road2();
    Road3();
    Road4();
    Road5();
    Road6();
    Road7();
    Road8();
    Road9();
    carGenerateCar();

    if(1){
        int basePoint = GetMaxW()-400;
        int TestLine =  basePoint + 200;
        int zeilPoint =  GetMaxH()-175;
        int zeilUnit = 25 ;
        SetTextColor(Gelb);
        SetBrush(Klar);
        
        Text(basePoint,zeilPoint,"Beschleunigung: ");
        Text(TestLine,zeilPoint, PlanString(acc) );
        Text(basePoint,zeilPoint + zeilUnit,"maximale Geschwindigkeit: ");
        Text(TestLine,zeilPoint + zeilUnit , PlanString(VMax) );
        Text(basePoint,zeilPoint + 2*zeilUnit,"Generation des Autos: ");
        Text(TestLine,zeilPoint + 2*zeilUnit , PlanString( numOfSettingCar ) );
        Text(TestLine+ 30,zeilPoint + 2*zeilUnit , " pro CallRunCount" );
    }
    

    sumOfCar1 = 0;
    sumOfCar3 = 0;
    sumOfCar6 = 0;
    sumOfCar9 = 0;
    
    adaptiveAmpel();
    plot0Run();
    plot1Run();

    Sleep(sleepTime);
    
    CallRun = true;
    
}

void TUser::Reset(){
    
    test = -1;
    AK_ampel = false;
    plotVis = false;
    InitParam();
    Clear(Grau);
    InitFahrbahn();
    InitGehweg();
    InitRadweg();
    InitTrambahn();
    DrawLamp();
    InitRoad();
    plotInit();
    InsertTaste(13,(AK_ampel)?"adpt. Ample":"Konst. Ampel", "Adaptive AmpelController");
    InsertTaste(9,(SpurOn)?"auto(Tram)":"manual(Tram)", "Ein Tram wird in jedem Zyklus erzeugt.");  
}

void TUser::adaptiveAmpel(){
    if(CallRunCount % 800 == 0 && CallRunCount > 0){
        //printf("    %2d %2d %2d \n",durch1,durch3,durch6);
        int rest_a = 80;
        int rest_b = 80;
        int rest_c = 80;
        int t_rest = T_Gesamt - 60 -  rest_a -  rest_b - rest_c; //550; //740
        if(AK_ampel){
            T_B1B2 = t_rest * ( (float)durch1/(float)(durch1+durch3+durch6)  );
            T_A1 = t_rest * ( (float)durch6/(float)(durch1+durch3+durch6)  );
            T_C3 = t_rest * ( (float)durch3/(float)(durch1+durch3+durch6)  );
            T_B1B2 += rest_b;
            T_A1   += rest_a;
            T_C3   += rest_c;    
        }
        printf("T_A1 = %3d,  T_B1B2 = %3d, T_C3 = %3d \n",T_A1,T_B1B2,T_C3);
    }
}


void TUser::InitParam(){
    r = 80; // arc radius
    sp = 10;
    FW = 25; RW = 15; GW = 30; TW = 15; // fahrbahnWeite, radwegWeite, gehwegWeite, trambahnWeite;
    RRUx = GetMaxW(); RRUy = GetMaxH()/2-sp/2-3*FW;     // Renbenring, upside
    RRDx = GetMaxW(); RRDy = GetMaxH()/2+sp/2+5*FW;        // Renbenring, downside.
    HSLx = GetMaxW()/2-(2*sp+TW+4*FW); HSLy = 0;     //(2*sp+TW+4*FW) HamburgerStrasse, left side.
    HSRx = GetMaxW()/2+2*sp+TW+4*FW; HSRy = 0;     // HamburgerStrasse, right side.
    WRUx = 0; WRUy = GetMaxH()/2-sp/2-4*FW;      // Wendenring, upside.
    WRDx = 0; WRDy = GetMaxH()/2+sp/2+5*FW;     // Wendenring, downside.
    LSLx = HSLx; LSLy = GetMaxH();     // Lampestrasse, left side
    LSRx = HSRx; LSRy = GetMaxH();     // Lampestrasse, right side.
    rad = false;
    passant = false;
    ampelRad = 1;   // 0 Gruen,  1 rot
    ampelPass = 1;  // 0 Gruen,  1 rot
    rLamp = 20; // circle of Lamp
    pose = 0;
//==================== Lampefarbe ===========================
    A1=2; A2=2; A3=2;
    B1=B2=2; B3=2;
    C1=2; C2=2; C3=2;
    D1 = 2;
    E1 = 2; E2 = 2;
//==================== Lamp control =========================
    ratio = 10;
    time = 0;
    T_A1 = 26 * ratio;      // lamp A1 time
    T_B1B2 = 23 * ratio;    // lamp B1 B2 time
    T_C3 = 24 * ratio;      // lamp C3 time
    T_Gesamt = 80 * ratio;  // gesamte Zeit 

//==================== Turning Parameters ===================
    rt1x = HSRx+r-3*sp-2*TW+2*FW; rt1y = WRUy-r;    // HS2RR
    rt2x = HSLx-r; rt2y = WRUy-r;                   // HS2WR
    rt3x = HSLx-r+3*sp+2*TW+2*FW; rt3y = WRUy-r;    // WR2HS
    rt4x = HSLx-r; rt4y = WRDy+r;                   // WR2LS
    rt5x = HSRx+r; rt5y = RRDy+r;                   // LS2RR
    rt6x = HSRx+r-2; rt6y = RRUy-r;                 // RR2HS
    rt7x = HSRx+r-(4*FW+3*sp+2*TW); rt7y = WRDy+r;  // RR2LS
    R1 = r+sp+13*FW/2;
    R2 = r+FW/2;
    R3 = r+sp+9*FW/2;
    R4 = r+FW/2;
    R5 = r+FW/2;
    R6 = r+FW/2;
    R7 = r+sp+7*FW/2; 
    
//==================== RR2WR road ===========================
    rrx = HSRx+r; rry1 = RRUy+2*FW; rry2 = RRUy+3*FW; rry3 = RRUy+4*FW;
    wrx = HSLx-r; wry1 = WRUy;
    kcar=(wry1 - rry1)/(wrx - rrx);
    b1 = rry1 - kcar*rrx;           // function
    b12 = rry1+FW/2+2 - kcar*rrx;
    b2 = rry2 - kcar*rrx;
    b23 = rry2+FW/2+2 - kcar*rrx;
    b3 = rry3 - kcar*rrx;

    wry4 =  WRUy+3*FW;
    kbus =  (wry4-rry2)/(wrx-rrx);
    b4 = rry2 - kbus*rrx;
    b45 = rry2+FW/2+2 - kbus*rrx;
    b5 = rry3 - kbus*rrx;
//==================== Tram Parameters ======================    
    hsy=WRUy-r; hsx1=GetMaxW()/2+FW-sp-TW/2; hsx2=GetMaxW()/2+FW+sp+TW/2;         
    lsy=WRDy+r; lsx1=GetMaxW()/2-sp-TW/2; lsx2=GetMaxW()/2+sp+TW/2;
    ktram=(lsx1-hsx1)/(lsy-hsy); bTL=lsx1-ktram*lsy+2, bTR=lsx2-ktram*lsy+2;
    PR0 = lsy;
    PR1 = lsy-70*cos(atan(ktram));
    PR2 = hsy;
    PL0 = hsy;
    PL1 = hsy+70*cos(atan(ktram));
    PL2 = lsy;
    tramNachOben = false;
    tramNachUnten = false;
    goCrossT1 = false;
    goCrossT2 = false;
    stop1 = RRDy+r+5;
    stop2 = WRDy+r+235;
    countT1 = 0;
    countT2 = 0;
    waitTimeT1 = 10 * ratio;
    waitTimeT2 = 10 * ratio;
    return_T1 = false;
    return_T2 = false;
    tramAuto = true;
    SpurOn = true;
    randomTime = random(T_Gesamt);
//==================== Auto Parameter =======================
    AutoW = 12;
    AutoL = 30;
    AutoTyre = 6;
    AutoAxle = 12;
}

void TUser::InitRoad(){
        VMax = 15; PTroedel = 5;
        
        safeDistance=3;
        acc = 1;

        PI = GradToRad(180);

        numOfSettingCar = 0.5;
        countOfCar = 0;
        isStopped = false;
        
        length1 = (-HSLy + (WRUy - r)) + floor(0.5 * PI *(float)(R1 + 2 * FW)) + (RRUx-(HSRx+r))   + 100;
        length2 =  rt2x                + floor(0.5 * PI *(float)(R2))          + rt2y              + 20;
        length3 =  rt3x                + floor(0.5 * PI *(float)(R3 + 2 * FW)) + ((WRUy-r) - HSRy) + 20;
        length4 = (LSLy - rt4y)        + floor(0.5 * PI *(float)(R2))          + rt4x              + 20;
        length5 = (LSRy-(RRDy + r))    + 2 * R5                                + (RRDx-(HSRx+r))   + 20;
        length6 = (RRUx - (HSRx + r))  + floor(0.5 * PI *(float)(R6 + 2 * FW)) + ((WRUy-r) - HSRy) + 20;
        length7 =  rt7x                + floor(0.5 * PI *(float)(R7 ))         + ( LSLy - rt7y)    + 20;
        length8 = RRDx - WRDx;
        length9 = RRDx - WRDx;

        fre1 = 10.0;    // HS2RR
        fre2 = 1.0;     // HS2WR
        fre3 = 10.0;    // WR2HS
        fre4 = 1.0;     // WR2LS
        fre5 = 0.1;     // LS2RR
        fre6 = 10.0;    // RR2HS
        fre7 = 0.2;     // RR2LS
        fre8 = 6.0;     // WR2RR
        fre9 = 6.0;     // RR2WR

        Gleit = 0.0;

        durch1 = durch3 = durch6 = durch8 = durch9 = 0;

        road11.Size(length1,1,4);
        road12.Size(length1,1,4);
        road13.Size(length1,1,4);
        road21.Size(length2,1,4);
        road22.Size(length2,1,4);
        road31.Size(length3,1,4);
        road32.Size(length3,1,4);
        road4.Size(length4,1,4);
        road5.Size(length5,1,4);
        road61.Size(length6,1,4);
        road62.Size(length6,1,4);
        road7.Size(length7,1,4);
        road81.Size(length8,1,4);
        road82.Size(length8,1,4);
        road91.Size(length9,1,4);
        road92.Size(length9,1,4);
        ResetRoad();
}

void TUser::ResetRoad(){

        sumOfCar1 = 0; 
        sumOfCar3 = 0;
        sumOfCar6 = 0;
        sumOfCar8 = 0;
        sumOfCar9 = 0;
        
        vAvg1 = 0;
        vAvg3 = 0;
        vAvg6 = 0;
        vAvg8 = 0;
        vAvg9 = 0;

        road11 = -1;
        road12 = -1;
        road13 = -1;
        road21 = -1;
        road22 = -1;
        road31 = -1;
        road32 = -1;
        road4 = -1;
        road5 = -1;
        road61 = -1;
        road62 = -1;
        road7 = -1;
        road81 = -1;
        road82 = -1;
        road91 = -1;
        road92 = -1;
}
void TUser::carGenerateCar(){
    if(numOfSettingCar >=1) {
        int ganzZahl = floor(numOfSettingCar);
        for(int m = 0;m < ganzZahl; m++){
            GenerateCar();
        }
        Gleit += numOfSettingCar - ganzZahl;
        if(Gleit >= 1) {
            Gleit -= 1;
            GenerateCar();
        }
    }
    if(numOfSettingCar <1){
        Gleit += numOfSettingCar;
        if(Gleit >= 1) {
            Gleit -= 1;
            GenerateCar();
        }
    }
}

void TUser::GenerateCar(){
    int i = 0,v = 0.5 * VMax;
    int numOfSubroad;
    double   numOfRoad ;
    int ganz = floor(fre1 + fre2 + fre3 + fre4 + fre5 + fre6 + fre7 + fre8 + fre9);
    
   while(true){
      numOfRoad = random(ganz) + 0.1 * random(9); 
      if( numOfRoad > 0 && numOfRoad < (fre1 + fre2 + fre3 + fre4 + fre5 + fre6 + fre7 + fre8 + fre9) ) break;
   }
    
    //printf("%2.2f\n",numOfRoad);
    v = 10 + random(4);
    if(0 < numOfRoad && numOfRoad < fre1) {
        numOfSubroad = ceil( random(3) + 1);
        if (numOfSubroad == 1) firstCar(AutoL, road11, v);
        if (numOfSubroad == 2) firstCar(AutoL, road12, v);
        if (numOfSubroad == 3) firstCar(AutoL, road13, v);
    } else if (numOfRoad < fre1 + fre2) {
        firstCar(AutoL, road21, v);
    } else if (numOfRoad < fre1 + fre2 + fre3)  {  
        numOfSubroad = ceil( random(2) + 1);
        if (numOfSubroad == 1) firstCar(AutoL, road31, v);
        if (numOfSubroad == 2) firstCar(AutoL, road32, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4)  { 
        firstCar(AutoL, road4, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4 + fre5)  {
        firstCar(AutoL, road5, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4 + fre5 + fre6)  {
        numOfSubroad = ceil( random(2) + 1);
        if (numOfSubroad == 1) firstCar(AutoL, road61, v);
        if (numOfSubroad == 2) firstCar(AutoL, road62, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4 + fre5 + fre6 + fre7)  {
        firstCar(AutoL, road7, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4 + fre5 + fre6 + fre7 + fre8)  {
        numOfSubroad = ceil( random(2) + 1);
        if (numOfSubroad == 1) firstCar(AutoL, road81, v);
        if (numOfSubroad == 2) firstCar(AutoL, road82, v);
    } else if (numOfRoad < fre1 + fre2 + fre3 + fre4 + fre5 + fre6 + fre7 + fre8 + fre9)  {
        numOfSubroad = ceil( random(2) + 1);
        if (numOfSubroad == 1) firstCar(AutoL, road91, v);
        if (numOfSubroad == 2) firstCar(AutoL, road92, v);
    }     
}

void TUser::plotInit(){

        tMeasure = 0;
        a = 9.0;
        
        winWidth = 50;
        startToMove = ceil(winWidth);

        xMin = 0.0;
        xMax = xMin + winWidth;
        
        yMin = 0;
        yMax = yMin + VMax + 5;
        unit = 0.1;
        pos = 0;
        
        linieEin = true;
        linieZwei = true;

        plotVis = false;
        AK_ampel = false;
        
        choose = -1;

        plot0bool = 1;
        plot1bool = 0;


        int i;
        for(i = 0;i < 500; i++) {
            halter1[i] = 0;
            halter3[i] = 0;
            halter6[i] = 0;
            halter9[i] = 0;
        }

        for(i = 0;i < 10; i++) {
           
            volume1[i] = 0;
            volume3[i] = 0;
            volume6[i] = 0;
            volume9[i] = 0;
        }

        Plot0->Size(25,25,300,200);
        Plot0->AchsenTyp = 2;
        Plot0->Links = Plot0->Rechts;
        Plot0->Titel = "Momentane Durchschnittsgeschwindigkeit";
        Plot0->Achse(0,xMin,xMax);
        Plot0->Achse(0,"Time");
        Plot0->Achse(1,yMin,yMax);
        Plot0->Achse(1,"Geschwindigkeit");
        Plot0->Kurve0->SetPen(Gelb,3);
        Plot0->Kurve1->SetPen(Blau,2);
        Plot0->Visible=false;

        Plot1->Size(GetMaxW()-25-350,25,350,220);
        Plot1->Titel = "Verkehrsaufkommen in eine Signalperiode";
        Plot1->Achse(0,0,10);
        Plot1->Achse(0,"Signalperiode");
        Plot1->Achse(1,0,80);
        Plot1->Achse(1,"Verkehrsaufkommen");
        Plot1->Kurve0->SetPen(Gelb,5);
        Plot1->Visible=false;

}

void TUser::plot0Run() {

        pos = pos%500;
        halter1[pos] = vAvg1;
        halter3[pos] = vAvg3;
        halter6[pos] = vAvg6;
        halter9[pos] = vAvg9;

        switch (plot0bool) {
        
        case 0:
            Plot0->Titel = "Momentane Geschwindigkeit in road 1 (B3)";
            runningline(CallRunCount,halter1,pos);  
            break;
        case 1:
            Plot0->Titel = "Momentane Geschwindigkeit in road 3 (C3)";
            runningline(CallRunCount,halter3,pos); 
            break;
        case 2:
            Plot0->Titel = "Momentane Geschwindigkeit in road 6 (A1)";
            runningline(CallRunCount,halter6,pos);
            break;
        case 3:
            Plot0->Titel = "Momentane Geschwindigkeit in road 9 (A2)";
            runningline(CallRunCount,halter9,pos); 
            break;
        }
        pos++;

        /*if(plot0bool == 0){
        Plot0->Titel = "Momentane Geschwindigkeit in road 1 (B3)";
        runningline(CallRunCount,halter1,pos);     
        }
        if(plot0bool == 1){
        Plot0->Titel = "Momentane Geschwindigkeit in road 3 (C3)";
        runningline(CallRunCount,halter3,pos);     
        }
        if(plot0bool == 2){
        Plot0->Titel = "Momentane Geschwindigkeit in road 6 (A1)";
        runningline(CallRunCount,halter6,pos);     
        }
        if(plot0bool == 3){
        Plot0->Titel = "Momentane Geschwindigkeit in road 9 (A2)";
        runningline(CallRunCount,halter9,pos);     
        }*/
}

void TUser::runningline(int tpoint, real* saver, int pointer) {
        Plot0->Achse(1,0,1.1*VMax);
        if(tpoint * unit > startToMove) {
            xMin = tpoint * unit - 50;
            xMax = tpoint * unit;
            Plot0->Achse(0,xMin,xMax);
        }
        Plot0->Kurve0->MoveTo(tpoint * unit,saver[pointer]);
        int j,m;
        for(j = 0; j< 500 ;j++) {
            m = pos - j;
            if (m < 0) m = m +500;
            m = m % 500;
                
            Plot0->Kurve0->LineTo((tpoint  - j) * unit ,saver[m]);               
        }

}

void TUser::plot1Run() { 

    switch (plot1bool) {
        case 0:
            Plot1->Titel = "Verkehrsaufkommen in Road1 (B3)";
            histogram(volume1);  
            break;
        case 1:
            Plot1->Titel = "Verkehrsaufkommen Road3 (C3)";
            histogram(volume3);  
            break;
        case 2:
            Plot1->Titel = "Verkehrsaufkommen in Road6 (A1)";
            histogram(volume6); 
            break;
        case 3:
            Plot1->Titel = "Verkehrsaufkommen in Road9 (A2)";
            histogram(volume9);  
            break;
     }
    /*if(plot1bool == 0){
        Plot1->Titel = "Verkehrsaufkommen in Road1 (B3)";
        histogram(volume1);   
        }
        if(plot1bool == 1){
        Plot1->Titel = "Verkehrsaufkommen Road3 (C3)";
        histogram(volume3);    
        }
        if(plot1bool == 2){
        Plot1->Titel = "Verkehrsaufkommen in Road6 (A1)";
        histogram(volume6); 
        }
        if(plot1bool == 3){
        Plot1->Titel = "Verkehrsaufkommen in Road9 (A2)";
        histogram(volume9);  
    }*/

    if(CallRunCount % T_Gesamt == 0) {
        test = test % 10;
        volume1[test] = durch1;
        volume3[test] = durch3;
        volume6[test] = durch6;
        volume9[test] = durch9;
        durch1 = durch3 = durch6  = durch9 = 0;
        test++;
    }
}

void TUser::histogram(double* vol){
    Plot1->Reset();
    for(int i = 0; i < 10; i++){
        //printf("%2.2f\n",vol[i]);
        for(int j = 0;j < 10; j++){
            Plot1->Kurve0->MoveTo(i+0.1*j,0);
            Plot1->Kurve0->LineTo(i+0.1*j,vol[i] - 0.08);                        
            if( j == 9) {
                Plot1->Kurve0->SetPen(Schwarz,2);
                Plot1->Kurve0->MoveTo(i,0);
                Plot1->Kurve0->LineTo(i,vol[i]);
                Plot1->Kurve0->LineTo(i+1,vol[i]);
                Plot1->Kurve0->LineTo(i+1,0);
                Plot1->Kurve0->SetPen(Gelb,5);
            }

        }    
    }   
}

void TUser::firstCar(int AutoL, Road& road, int v){
    int i = 0;
    while(i< AutoL && road(i,0,1)<0) i++;
    if(i >= AutoL) {
        road(0,0,1) = v ;
        road(0,0,2) = AutoL;

    }
}

//==============================================================================
//      Lamp control. using callRun. 10 times equivalent one second
void TUser::LampControl(int T_B1B2, int T_A1, int T_C3){

    time = CallRunCount % T_Gesamt; // Lamp control
    // regelm??ig tram erzeugen
    if (tramAuto) {
        if (CallRunCount % (T_Gesamt*4) == T_Gesamt + randomTime && tramNachOben != true) {
            randomTime = random(T_Gesamt);
            tramNachOben = true;
        } 
        if (CallRunCount % (T_Gesamt*4) == T_Gesamt * 3 + randomTime && tramNachUnten != true) {
            randomTime = random(T_Gesamt);
            tramNachUnten = true;
        }
    }
//=============================== lamp control ================================= 
    if(time < T_B1B2){     // default T_B1B2 = 23 seconds;
        if (time == 0) {
            B1 = 0;
            B2 = 0;
            A2 = 2;
            A3 = 2;
            C2 = 2;
            C3 = 2;
        }
        if (time == 2*ratio) {
            if (B3 == 2) B3 = 0;
        }
        if (time == T_B1B2-3*ratio) {
            B1 = 1;
            B2 = 1;
            if (B3 == 0) B3 = 1;
        }
        if (time == T_B1B2-1.5*ratio) {
            B1 = 2;
            B2 = 2;
            if (B3==1) B3 = 2;
        }
    } else if(time < T_B1B2+T_A1){       // default T_A1 = 26 seconds
        if (time == T_B1B2) {
            A1 = 0;
            A3 = 2;
            B3 = 2;
            C3 = 2;
        }
        if (time == T_B1B2+3*ratio) {
            if(A2 == 2) A2 = 0;
            C1 = 0;
            C2 = 0;
            
        }
        if (time == T_B1B2+T_A1-3*ratio) {
            A1 = 1;
            if(A2 == 0) A2 = 1;
            C1 = 1;
            C2 = 1;
        }
        if (time == T_B1B2+T_A1-1.5*ratio) {
            A1 = 2;
            if(A2 == 1) A2 = 2;
            C1 = 2;
            C2 = 2;
        }
    } else if(time < T_B1B2+T_A1+T_C3){
        if (time == T_B1B2+T_A1) {
            if (C3 == 2) C3 = 0;
            A2 = 2;
            A3 = 2;
            B3 = 2;
            C2 = 2; 
        }
        if (time == T_B1B2+T_A1+T_C3-3*ratio) {
            if (C3 == 0) C3 = 1;
        }
        if (time == T_B1B2+T_A1+T_C3-1.5*ratio) {
            if (C3 == 1) C3 = 2;
        }
    } else {
        if (time == T_B1B2+T_A1+T_C3) {
            if (A3 == 2) A3 = 0;
            A2 = 2;
            B3 = 2;
            C2 = 2;
            C3 = 2;
        }
        if (time == T_B1B2+T_A1+T_C3+3*ratio) {
            D1 = 0;
        }
        if (time == T_Gesamt-3*ratio) {
            if (A3 == 0) A3 = 1;
            if (D1 = 0) D1 = 1;
        }
        if (time == T_Gesamt-1.5*ratio) {
            if (A3 == 1)A3 = 2;
            if (D1 = 1) D1 = 2;
        }
    }
//============================ lamp signal recovery ============================
    if (return_T1 || return_T2) {        
        if (T_B1B2+3*ratio <= time && time < T_B1B2+T_A1-3*ratio)   A2 = 0, C2 = 0;
        else if (T_B1B2+T_A1+T_C3 <= time && time < 80*ratio-3*ratio)    A3 = 0;
        else if (2*ratio <= time && time < T_B1B2-3*ratio)               B3 = 0;
        else if (T_B1B2+T_A1 <= time && time < T_B1B2+T_A1+T_C3-3*ratio) C3 = 0;
        if (return_T1) return_T1 = !return_T1;
        if (return_T2) return_T2 = !return_T2;
    }
//============= tram go through the cross, other lamps will become red ==============
    if (waitTimeT1 == 20 || E1 == 0) {
        A2 = 2;
        A3 = 2;
        B3 = 2;
        C2 = 2;
        C3 = 2;
    }
    if (E2 == 0) {
        A2 = 2;
        A3 = 2;
        B3 = 2;
        C2 = 2;
        C3 = 2;
    }
}

void TUser::DrawCrossRoad(){
    if (A1!=2) {
        RR2HS();
    }
    if (A2!=2) {
        GehwegHS(A2, passant);
        RadwegHS(A2, rad);
        RR2WR();
    }
    if (A3!=2) RR2LS();
    if (B1!=2) HS2WR();
    if (B2!=2) HS2LS(); 
    if (B3!=2) {
        HS2RR();
        GehwegWR(B1, passant);
        RadwegWR(B1, rad);
    }
    if (C1!=2) WR2LS();
    if (C2!=2) {
        GehwegLS(B1, passant);
        RadwegLS(B1, rad);
        WR2RR();
    }
    if (C3!=2) {
        GehwegLS(C3, passant);
        RadwegLS(C3, rad);
        GehwegRR(C3, passant);
        RadwegRR(C3, rad);
        WR2HS();
    }
    if (D1!=2) LS2RR();
}
//==============================================================================

void TUser::RunningRoad(Road& road, int lampSignal, int stopPosition) { 
//road-- the way to run/ lampSignal-- control signal/ stopPosion-- the potision for car to stop

    Road hRoad;
    hRoad.Size(road.M,road.N,road.P);
    
    int a, b, c, SizeI = road.M, SizeJ = road.N, SizeK = road.P, mark = 0;
   
    hRoad = road;
    road = -1;
    int j = 0;
    // erste Fahrzeug
    int i = 0;
    while(i< SizeI && hRoad(i,j,1)<0) i++;
    if (i >= SizeI) {
        mark = 1;
    }
    int v,k;
    while(i< SizeI && mark != 1){
        v = hRoad(i,j,1);
        // 1. Regel   accelerate
        if(v<VMax) v+=acc;
        // 2. Regel   break against carsh
        k=0;
        do{
            k++;
            if (i+k >= SizeI) {
                k= SizeI ;
                break;
            }

        } while (hRoad(i+k, j, 1)< 0);
        if (i + k < SizeI ) {
            if(v>(k-safeDistance)- hRoad(i+k,j,2)) {
                v = (k - safeDistance) - hRoad(i+k,j,2);
            }
        }
        // 3. Regel   random braking
        if(v>0 && (random(10) < PTroedel)) v--;
        if(v<0) v = 0;
        // 4. Regel  stop for red light

        int vNext;
        vNext = i + v;
        if (lampSignal == 2){
            if(vNext>stopPosition && vNext < stopPosition + VMax + 5) {
                v=0;
                vNext = stopPosition;
            }
        }
        // 5. Regel   go forward
        if(vNext < SizeI) {
            road(vNext,j,2) = hRoad(i,j,2);
            road(vNext,j,1) = v;
        }
        i+=k;
    }              
}

void TUser::RunningRoad2(Road& road,Road& roadS, int lampSignal, int stopPosition) { 
//road-- the way to run/ lampSignal-- control signal/ stopPosion-- the potision for car to stop

    Road hRoad;
    hRoad.Size(road.M,road.N,road.P);
    //printf("road.M %d \n", road.M);
    int a, b, c, SizeI = road.M, SizeJ = road.N, SizeK = road.P, mark = 0;
    //printf("SizeI %d \n", SizeI);
    hRoad = road;
    road = -1;
    int j = 0;
    // erste Fahrzeug
    int i = 0;
    while(i< SizeI && hRoad(i,j,1)<0) i++;
    if (i >= SizeI) {
        mark = 1;
    }
    int v,k;
    while(i< SizeI && mark != 1){
        v = hRoad(i,j,1);
        // 1. Regel   accelerate
        if(v<VMax) v+=acc;
        // 2. Regel   break against carsh
        k=0;
        do{
            k++;
            if (i+k >= SizeI) {
                k= SizeI ;
                break;
            }

        } while (hRoad(i+k, j, 1)< 0);
        if (i + k < SizeI ) {
            if(v>(k-safeDistance)- hRoad(i+k,j,2)) {
                v = (k - safeDistance) - hRoad(i+k,j,2);
            }
        }
        // 3. Regel   random braking
        if(v>0 && (random(10) < PTroedel)) v--;
        if(v<0) v = 0;
        // 4. Regel  stop for red light

        int vNext;
        vNext = i + v;
        if (lampSignal == 2){
            if(vNext>stopPosition && vNext < stopPosition + VMax) {
                v=0;
                vNext = stopPosition;
            }
        }
        if( vNext > stopPosition    && vNext < stopPosition+VMax +3) {
            if ( random(30) < 1) {
                roadS(vNext - stopPosition,1,1) = hRoad(i,0,1);
                roadS(vNext - stopPosition,1,2) = hRoad(i,0,2);
                v = -1;
                hRoad(i,0,2) = -1;
            }
        } 
        // 5. Regel   go forward
        if(vNext < SizeI) {
            road(vNext,j,2) = hRoad(i,j,2);
            road(vNext,j,1) = v;
        }
        i+=k;
    }              
}



//==============================================================================
void TUser::Road1() {
    int stopPosition = (LSRy-(RRDy + r)) - AutoL/2 + 50;
    int turningPosition, turningEnd;
    turningPosition = (LSRy-(RRDy + r)) + 50;
    turningEnd = (LSRy-(RRDy + r)) + 2 * (R5) +50;
    RunningRoad(road11, B3,stopPosition);
    RunningRoad(road12, B3,stopPosition);
    RunningRoad(road13, B3,stopPosition);
    for(int j = 0; j < 3; j++){           
        for(int i = 0; i < road11.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road11, i);
            if(j == 1) hasCar = findCar(road12, i);
            if(j == 2) hasCar = findCar(road13, i);
            if(hasCar == true){
                calcAvg(road11, sumOfCar1, vAvg1, i);
                calcAvg(road12, sumOfCar1, vAvg1, i);
                calcAvg(road13, sumOfCar1, vAvg1, i);

                if( (length1 - VMax) < i ) durch1++;

                int xTemp,yTemp;
                double wTemp;
                if (i <turningPosition  ) {
                    xTemp = LSLx + 3.5 * FW - j * FW;
                    yTemp = HSLy + i + 16 - 50;
                    DrawAutoVerti(xTemp, yTemp);                       
                } else if (i< (turningPosition + floor(0.5 * PI *(float)(R1) + j * FW) )  ) {
                    wTemp =  90 -( i - turningPosition )/ (double)(floor(0.5 * PI *(float)(R1 + j * FW))) * 90 ;
                    DrawAutoRR22HS(R1 + j * FW, rt1x, rt1y, wTemp);     
                } else {
                    xTemp = (i - (turningPosition + floor(0.5 * PI *(float)(R1 + j * FW))) ) + HSRx +r;
                    yTemp = RRDy - 2.5 * FW + j * FW;
                    DrawAutoHori(xTemp, yTemp);
                }
            }
        }
    }

}

void TUser:: calcAvg(Road& road, int& sumOfCar, double& vAvg, int i){
    if(findCar(road, i) == true) {
        sumOfCar++;
        if(sumOfCar == 0) {
            vAvg = road(i,0,1);
        } else{
            vAvg =(1.0 - 1/(double)(sumOfCar)) * vAvg + 1.0 /(double)(sumOfCar) * road(i,0,1);
        }
        
    }
}

bool TUser:: findCar(Road& road, int i){
    if(road(i,0,1) > -1) {
        return true;
    } else {
        return false;
    }
}


void  TUser:: Road2() {
    int turningPosition, turningEnd;
    int stopPosition = WRUy - r - AutoL/2 - 8;
    turningPosition = rt2y;
    turningEnd = rt2y + floor(0.5 * PI *(float)(R2));
    RunningRoad2(road21,road22, B1, stopPosition);
    RunningRoad(road22, 1,stopPosition);
    for(int j = 0; j < 3; j++){           
        for(int i = 0; i < road21.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road21, i);
            if(j == 1) hasCar = findCar(road22, i);
            
            if(hasCar == true){
                int xTemp,yTemp;
                double wTemp;
                if (j == 0 ){
                    if (i <= turningPosition   ) {
                        xTemp = rt2x + R2;
                        yTemp = i ;
                        DrawAutoVerti(xTemp, yTemp);
                        //printf("s %d %d j %d\n",i,turningPosition,j);
                    } else if (i< (turningPosition + floor(0.5 * PI *(float)(R2)) )  ) {
                        wTemp =  ( i - turningPosition )/ (double)(floor(0.5 * PI *(float)(R2))) * 90 - 90;
                        DrawAutoRR22HS(R2, rt2x, rt2y, wTemp);
                        //printf("r %d %d %3.2f j %d\n",i,turningPosition,wTemp,j);
                    } else {
                        xTemp = -((i - (turningPosition + floor(0.5 * PI *(float)(R2)) ))) + rt2x;
                        yTemp = rt2y + R2;
                        DrawAutoHori(xTemp, yTemp);
                    }
                }
                if (j == 1) {
                    xTemp = rt2x + R2;
                    yTemp = rt2y + i - 24;
                DrawAutoVerti(xTemp, yTemp);
                }    
            }
        }
    }
}



void TUser::Road3() {
    int stopPosition = HSLx - r - AutoL / 2 - 8;
    int turningPosition, turningEnd;
    turningPosition = rt3x;
    RunningRoad(road31, C3,stopPosition);
    RunningRoad(road32, C3,stopPosition);

    for(int j = 0; j < 2; j++){           
        for(int i = 0; i < road31.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road31, i);
            if(j == 1) hasCar = findCar(road32, i);
            
            if(hasCar == true){
                calcAvg(road31, sumOfCar3, vAvg3, i);
                calcAvg(road32, sumOfCar3, vAvg3, i);
                if( (length3 - VMax) < i ) durch3++;
                int xTemp,yTemp;
                double wTemp;
                if (i <=turningPosition -5  ) {
                    xTemp = (i);
                    yTemp = WRDy  - 4.5 * FW + j * FW;
                    DrawAutoHori(xTemp, yTemp);
                } else if (i< (turningPosition + floor(0.5 * PI *(float)(R3 + j * FW)) )  - 5  ) {
                    wTemp =  -( i - turningPosition )/ (double)(floor(0.5 * PI *(float)(R3+ j * FW))) * 90 ;
                    DrawAutoRR22HS(R3 + j * FW, rt3x, rt3y, wTemp);   
                } else {
                yTemp = -( i -(turningPosition + floor(0.5 * PI *(float)(R3 + j * FW))) ) + rt3y  ;
                xTemp = HSRx + j * FW - 1.5 * FW;
                DrawAutoVerti(xTemp, yTemp); 
                }
            }
        }
    }
}

void TUser::Road4() {
    int stopPosition = rt4x - AutoL / 2 - 8;
    int turningPosition, turningEnd;
    turningPosition = rt4x;
    RunningRoad(road4, C1,stopPosition);
    for(int j = 0; j < 1; j++){           
        for(int i = 0; i < road4.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road4, i);
            if(hasCar == true){
                int xTemp,yTemp;
                double wTemp;
                if (i<turningPosition - 5  ) {
                    xTemp = i;
                    yTemp = WRDy - 0.5 * FW ;
                    DrawAutoHori(xTemp, yTemp);
                } else if ( i< turningPosition + 2 * R4) {
                    wTemp =  ( i - turningPosition )/ (double)(2 * R4) * 90 ;
                    DrawAutoWR22LS(R4, rt4x, rt4y, wTemp);
                } else {
                    xTemp = rt4x + R4;
                    yTemp = (i - (turningPosition + 2 * R4)) + rt4y;
                    DrawAutoVerti(xTemp, yTemp);
                }
            }
        }
    }    
}


void TUser::Road5() {

    int stopPosition = LSRy-(RRDy + r) - AutoL / 2 - 8;
    int turningPosition, turningEnd;
    turningPosition = LSRy-(RRDy + r);
    turningEnd = LSRy-(RRDy + r) + 2 * R5;
    RunningRoad(road5, D1,stopPosition);
    for(int j = 0; j < 1; j++){           
        for(int i = 0; i < road5.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road5, i);
            if(hasCar == true){
                int xTemp,yTemp;
                double wTemp;
                if (i<turningPosition - 5  ) {
                    xTemp = LSRx - FW/2;
                    yTemp = LSRy - i ;
                    DrawAutoVerti(xTemp, yTemp);
                } else if ( i<turningEnd) {
                    wTemp =  ( i - turningPosition )/ (double)(2 * R5) * 90 - 90;
                    DrawAutoWR22LS(R5, rt5x, rt5y, wTemp);
                } else {
                    xTemp = (i - turningEnd) + HSRx +r;
                    yTemp = RRDy - FW / 2;
                    DrawAutoHori(xTemp, yTemp);
                }
            }
        }
    }  
}

                


void TUser::Road6() {
    int stopPosition = (RRUx - (HSRx + r)) - AutoL / 2 - 8;
    int turningPosition, turningEnd;
    turningPosition = (RRUx - (HSRx + r)) ;
    turningEnd = (RRUx - (HSRx + r))+ floor(0.5 * PI *(float)(R6 + 2 * FW));
    RunningRoad(road61, A1,stopPosition);
    RunningRoad(road62, A1,stopPosition);
    for(int j = 0; j < 2; j++){           
        for(int i = 0; i < road61.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road61, i);
            if(j == 1) hasCar = findCar(road62, i);
            if(hasCar == true){
                calcAvg(road61, sumOfCar6, vAvg6, i);
                calcAvg(road62, sumOfCar6, vAvg6, i);
                if( (length6 - VMax) < i ) durch6++;
                int xTemp,yTemp;
                double wTemp;
                if (i <=turningPosition  ) {
                    xTemp = (RRUx - i);
                    yTemp = RRUy  + 0.5 * FW +j * FW;
                    DrawAutoHori(xTemp, yTemp);
                } else if (i< (turningPosition + floor(0.5 * 3.1415926 *(float)(R6+ j * FW)) )  ) {
                    wTemp =  ( i - turningPosition )/ (double)(floor(0.5 * PI *(float)(R6+ j * FW))) * 90 ;
                    DrawAutoRR22HS(R6 + j * FW, rt6x, rt6y, wTemp);   
                } else {
                    yTemp = -(i - (turningPosition + floor(0.5 * 3.1415926 *(float)(R6+ j * FW)) )) + WRUy - r + 15 ;
                    xTemp = HSRx - j * FW - 0.5 * FW;
                    DrawAutoVerti(xTemp, yTemp);
                }
            }
        }
    } 
}

void TUser::Road7() {
    int stopPosition = RRUx - (HSRx + r) - 8 - AutoL/2;
    int turningPosition, turningEnd;
    turningPosition = RRUx - rt7x;
    RunningRoad(road7, A3,stopPosition);
    for(int j = 0; j < 1; j++){           
        for(int i = 0; i < road7.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road7, i);
            if(hasCar == true){
                int xTemp,yTemp;
                double wTemp;
                if (i<turningPosition - 5  ) {
                    xTemp = RRUx - i;
                    yTemp = RRUy + 4.5 * FW ;
                    DrawAutoHori(xTemp, yTemp);
                } else if ( i< turningPosition + floor(0.5 * PI *(float)(R7)) ) {
                    wTemp =   -( i - ( turningPosition  ) )/ (double)( floor(0.5 * PI *(float)(R7)) ) * 90 ;
                    DrawAutoWR22LS(R7, rt7x, rt7y, wTemp);
                } else {
                    xTemp = LSLx + FW / 2;
                    yTemp = rt7y + i - ( turningPosition + floor(0.5 * PI *(float)(R7)) );
                    DrawAutoVerti(xTemp, yTemp);
                }
            }
        }
    }
}

void TUser::Road8() {
    int stopPosition = HSLx - r -AutoL/ 2 -8;
    int turningPosition, turningEnd;
    turningPosition = LSRy-(RRDy + r);
    RunningRoad(road81, C2,stopPosition);
    RunningRoad(road82, C2,stopPosition);
    for(int j = 0; j < 2; j++){           
        for(int i = 0; i < road81.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road81, i);
            if(j == 1) hasCar = findCar(road82, i);
            if(hasCar == true){
                calcAvg(road81, sumOfCar8, vAvg8, i);
                calcAvg(road82, sumOfCar8, vAvg8, i);
                int xTemp,yTemp;
                double wTemp;
                xTemp = i;
                yTemp = WRDy - 1.5 * FW  - j * FW;
                DrawAutoHori(xTemp, yTemp);  
            }
        }
    }                
}

void TUser::Road9() {
    int stopPosition = RRUx - (HSRx + r)  -AutoL/ 2 -8 ;
    int turningPosition, turningEnd;
    turningPosition = RRUx - (HSRx + r);
    turningEnd = RRUx - (HSLx - r) ;
    RunningRoad(road91, A2,stopPosition);
    RunningRoad(road92, A2,stopPosition);
    for(int j = 0; j < 2; j++){           
        for(int i = 0; i < road91.M; i++) {
            bool hasCar;
            if(j == 0) hasCar = findCar(road91, i);
            if(j == 1) hasCar = findCar(road92, i);
            if(hasCar == true) {
                calcAvg(road91, sumOfCar9, vAvg9, i);
                calcAvg(road92, sumOfCar9, vAvg9, i);
                if( (length9 - VMax) < i ) durch9++;
                int xTemp,yTemp;
                double wTemp;
                if (i< turningPosition - 5  ) {
                    xTemp = RRUx - i;
                    yTemp = RRUy + 2.5 * FW + j * FW ;
                    DrawAutoHori(xTemp, yTemp);
                } else if ( i < turningEnd) {
                    xTemp = HSRx + r -(i - (turningPosition - 5));
                    
                    if(j == 0) {
                        DrawAutoRR22WR1(xTemp, kcar*xTemp + b12);
                    } else if (j == 1) {
                        
                        DrawAutoRR22WR1(xTemp, kcar*xTemp + b23);
                        
                        //DrawAutoRR22WR2(xTemp, kbus*xTemp + b45);
                    }
                } else {
                    xTemp = - (i - turningEnd) + HSLx - r;
                    yTemp = WRUy + j * FW + 0.5 * FW;
                    DrawAutoHori(xTemp, yTemp);
                }     
            }
        }
    } 

                            
}

//=====================================================================//
//---------------- Alle Methoden bezueglich der Oberflaeche ----------//
//====================================================================//

//==============================================================================
//      Draw cars, bikes and passants
//==============================================================================
void TUser::DrawAutoHori(int x, int y){     // Horizontale upside
    ClearPoints();
    SetPen(Schwarz,2);
    MoveTo(x-12, y-8);
    LineTo(x-6, y-8);
    MoveTo(x-12, y+9);
    LineTo(x-6, y+9);
    
    MoveTo(x+12, y-8);
    LineTo(x+6, y-8);
    MoveTo(x+12, y+9);
    LineTo(x+6, y+9);

    SetPen(Schwarz,1);
    MoveTo(x-9, y-8);
    LineTo(x-9, y+8);
    MoveTo(x+9, y-8);
    LineTo(x+9, y+8);
    
    SetPoint(x-15, y-6);
    SetPoint(x-15, y+6);
    SetPoint(x+15, y+6);
    SetPoint(x+15, y-6);
    SetPoint(x-15, y-6);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}

void TUser::DrawAutoVerti(int x, int y){      // vertikale leftside
    ClearPoints();
    SetPen(Schwarz,2);                              
    MoveTo(x-8, y-12); // qian che lun
    LineTo(x-8, y-6);
    MoveTo(x+9, y-12);
    LineTo(x+9, y-6);
    MoveTo(x-8, y+12); // hou che lun
    LineTo(x-8, y+6);
    MoveTo(x+9, y+12);
    LineTo(x+9, y+6);

    SetPen(Schwarz,1);
    MoveTo(x-8, y-9);   // qian zhou
    LineTo(x+8, y-9);
    MoveTo(x-8, y+9);   // hou zhou
    LineTo(x+8, y+9);
    
    SetPoint(x-6, y-15);
    SetPoint(x-6, y+15);    // che shen
    SetPoint(x+6, y+15);
    SetPoint(x+6, y-15);
    SetPoint(x-6, y-15);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}

void TUser::DrawAutoRR22HS(int R, int Rx, int Ry, double grad){

    double ww1, ww2, rr1, rr2, w;
    int xx0, yy0, xx1, yy1, xx2, yy2, xx3, yy3;

    w = GradToRad(grad);
    ww1 = atan((AutoL/4)/(R-AutoW/2));
    ww2 = atan(AutoL/4/(R+AutoW/2));
    rr1 = sqrt((AutoL*AutoL/16)+(R-AutoW/2)*(R-AutoW/2));
    rr2 = sqrt((AutoL*AutoL/16)+(R+AutoW/2)*(R+AutoW/2));

    xx0 = Rx-rr1*sin(w+ww1);
    yy0 = Ry+rr1*cos(w+ww1);
    xx1 = Rx-rr1*sin(w-ww1);
    yy1 = Ry+rr1*cos(w-ww1);
    xx2 = Rx-rr2*sin(w+ww2);
    yy2 = Ry+rr2*cos(w+ww2);
    xx3 = Rx-rr2*sin(w-ww2);
    yy3 = Ry+rr2*cos(w-ww2);

    SetPen(Schwarz,1);
    SetBrush(Schwarz);
    Circle(xx0, yy0, 3);
    Circle(xx1, yy1, 3);
    Circle(xx2, yy2, 3);
    Circle(xx3, yy3, 3);

    double w1, w2, r1, r2;
    int x0, y0, x1, y1, x2, y2, x3, y3;

    w = GradToRad(grad);
    w1 = atan((AutoL/2)/(R-AutoW/2));
    w2 = atan(AutoL/2/(R+AutoW/2));
    r1 = sqrt((AutoL*AutoL/4)+(R-AutoW/2)*(R-AutoW/2));
    r2 = sqrt((AutoL*AutoL/4)+(R+AutoW/2)*(R+AutoW/2));

    x0 = Rx-r1*sin(w+w1);
    y0 = Ry+r1*cos(w+w1);
    x1 = Rx-r1*sin(w-w1);
    y1 = Ry+r1*cos(w-w1);
    x2 = Rx-r2*sin(w+w2);
    y2 = Ry+r2*cos(w+w2);
    x3 = Rx-r2*sin(w-w2);
    y3 = Ry+r2*cos(w-w2);

    ClearPoints();
    SetPen(Schwarz,1);
    SetPoint(x0, y0);
    SetPoint(x1, y1);
    SetPoint(x3, y3);
    SetPoint(x2, y2);
    SetPoint(x0, y0);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}

void TUser::DrawAutoRR22WR1(int x, int y){
    
    int xx0, yy0, xx1, yy1, xx2, yy2, xx3, yy3;
    double sinkk, coskk;
    sinkk = sin(atan(kcar));
    coskk = cos(atan(kcar));
    xx0=x+AutoL/4*coskk-6*sinkk; yy0=y+AutoL/4*sinkk+6*coskk;
    xx1=xx0-AutoL/2*coskk; yy1=yy0-AutoL/2*sinkk;
    xx2=xx1+AutoW*sinkk; yy2=yy1-AutoW*coskk;
    xx3=xx2+1+AutoL/2*coskk; yy3=yy2+AutoL/2*sinkk;
    SetPen(Schwarz, 1);
    SetBrush(Schwarz);
    Circle(xx0, yy0, 3);
    Circle(xx1, yy1, 3);
    Circle(xx2, yy2, 3);
    Circle(xx3, yy3, 3);

    int x0, y0;
    double sink, cosk;
    sink = sin(atan(kcar));
    cosk = cos(atan(kcar));
    
    ClearPoints();
    SetPen(Schwarz,1);
    x0 = x+15*cosk-6*sink; y0 = y+15*sink+6*cosk;
    SetPoint(x0, y0);
    x0=x0-30*cosk; y0=y0-30*sink;
    SetPoint(x0, y0);
    x0=x0+12*sink; y0=y0-12*cosk;
    SetPoint(x0, y0);
    x0=x0+1+30*cosk; y0=y0+30*sink;
    SetPoint(x0, y0);

    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();        
}

void TUser::DrawAutoRR22WR2(int x, int y){
    
    int x0, y0;
    double sink, cosk;
    sink = sin(atan(kbus));
    cosk = cos(atan(kbus));
    
    ClearPoints();
    SetPen(Schwarz,1);
    x0 = x+15*cosk-6*sink; y0 = y+15*sink+6*cosk;
    SetPoint(x0, y0);
    x0=x0-30*cosk; y0=y0-30*sink;
    SetPoint(x0, y0);
    x0=x0+12*sink; y0=y0-12*cosk;
    SetPoint(x0, y0);
    x0=x0+1+30*cosk; y0=y0+30*sink;
    SetPoint(x0, y0);
    
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();        
}

void TUser::DrawAutoRR22LS(int R, int Rx, int Ry, double grad){

    double ww1, ww2, rr1, rr2, w;
    int xx0, yy0, xx1, yy1, xx2, yy2, xx3, yy3;

    w = GradToRad(grad);
    ww1 = atan((AutoL/4)/(R-AutoW/2));
    ww2 = atan(AutoL/4/(R+AutoW/2));
    rr1 = sqrt((AutoL*AutoL/16)+(R-AutoW/2)*(R-AutoW/2));
    rr2 = sqrt((AutoL*AutoL/16)+(R+AutoW/2)*(R+AutoW/2));

    xx0 = Rx-rr1*sin(w+ww1);
    yy0 = Ry-rr1*cos(w+ww1);
    xx1 = Rx-rr1*sin(w-ww1);
    yy1 = Ry-rr1*cos(w-ww1);
    xx2 = Rx-rr2*sin(w+ww2);
    yy2 = Ry-rr2*cos(w+ww2);
    xx3 = Rx-rr2*sin(w-ww2);
    yy3 = Ry-rr2*cos(w-ww2);

    SetPen(Schwarz,1);
    SetBrush(Schwarz);
    Circle(xx0, yy0, 3);
    Circle(xx1, yy1, 3);
    Circle(xx2, yy2, 3);
    Circle(xx3, yy3, 3);

    double w1, w2, r1, r2;
    int x0, y0, x1, y1, x2, y2, x3, y3;
    
    w = GradToRad(grad);
    w1 = atan((AutoL/2)/(R-AutoW/2));
    w2 = atan(AutoL/2/(R+AutoW/2));
    r1 = sqrt((AutoL*AutoL/4)+(R-AutoW/2)*(R-AutoW/2));
    r2 = sqrt((AutoL*AutoL/4)+(R+AutoW/2)*(R+AutoW/2));

    x0 = Rx-r1*sin(w+w1);
    y0 = Ry-r1*cos(w+w1);
    x1 = Rx-r1*sin(w-w1);
    y1 = Ry-r1*cos(w-w1);
    x2 = Rx-r2*sin(w+w2);
    y2 = Ry-r2*cos(w+w2);
    x3 = Rx-r2*sin(w-w2);
    y3 = Ry-r2*cos(w-w2);

    ClearPoints();
    SetPoint(x0, y0);
    SetPoint(x1, y1);
    SetPoint(x3, y3);
    SetPoint(x2, y2);
    SetPoint(x0, y0);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}

void TUser::DrawAutoWR22HS(int R, int Rx, int Ry, double grad){

    double ww1, ww2, rr1, rr2, w;
    int xx0, yy0, xx1, yy1, xx2, yy2, xx3, yy3;

    w = GradToRad(grad);
    ww1 = atan((AutoL/4)/(R-AutoW/2));
    ww2 = atan(AutoL/4/(R+AutoW/2));
    rr1 = sqrt((AutoL*AutoL/16)+(R-AutoW/2)*(R-AutoW/2));
    rr2 = sqrt((AutoL*AutoL/16)+(R+AutoW/2)*(R+AutoW/2));

    xx0 = Rx+rr1*sin(w+ww1);
    yy0 = Ry+rr1*cos(w+ww1);
    xx1 = Rx+rr1*sin(w-ww1);
    yy1 = Ry+rr1*cos(w-ww1);
    xx2 = Rx+rr2*sin(w+ww2);
    yy2 = Ry+rr2*cos(w+ww2);
    xx3 = Rx+rr2*sin(w-ww2);
    yy3 = Ry+rr2*cos(w-ww2);

    SetPen(Schwarz,1);
    SetBrush(Schwarz);
    Circle(xx0, yy0, 3);
    Circle(xx1, yy1, 3);
    Circle(xx2, yy2, 4);
    Circle(xx3, yy3, 4);

    double w1, w2, r1, r2;
    int x0, y0, x1, y1, x2, y2, x3, y3;

    w = GradToRad(grad);
    w1 = atan((AutoL/2)/(R-AutoW/2));
    w2 = atan(AutoL/2/(R+AutoW/2));
    r1 = sqrt((AutoL*AutoL/4)+(R-AutoW/2)*(R-AutoW/2));
    r2 = sqrt((AutoL*AutoL/4)+(R+AutoW/2)*(R+AutoW/2));

    x0 = Rx+r1*sin(w+w1);
    y0 = Ry+r1*cos(w+w1);
    x1 = Rx+r1*sin(w-w1);
    y1 = Ry+r1*cos(w-w1);
    x2 = Rx+r2*sin(w+w2);
    y2 = Ry+r2*cos(w+w2);
    x3 = Rx+r2*sin(w-w2);
    y3 = Ry+r2*cos(w-w2);

    ClearPoints();
    SetPoint(x0, y0);
    SetPoint(x1, y1);
    SetPoint(x3, y3);
    SetPoint(x2, y2);
    SetPoint(x0, y0);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}

void TUser::DrawAutoWR22LS(int R, int Rx, int Ry, double grad){

    double ww1, ww2, rr1, rr2, w;
    int xx0, yy0, xx1, yy1, xx2, yy2, xx3, yy3;

    w = GradToRad(grad);
    ww1 = atan((AutoL/4)/(R-AutoW/2));
    ww2 = atan(AutoL/4/(R+AutoW/2));
    rr1 = sqrt((AutoL*AutoL/16)+(R-AutoW/2)*(R-AutoW/2));
    rr2 = sqrt((AutoL*AutoL/16)+(R+AutoW/2)*(R+AutoW/2));

    xx0 = Rx+rr1*sin(w+ww1);
    yy0 = Ry-rr1*cos(w+ww1);
    xx1 = Rx+rr1*sin(w-ww1);
    yy1 = Ry-rr1*cos(w-ww1);
    xx2 = Rx+rr2*sin(w+ww2);
    yy2 = Ry-rr2*cos(w+ww2);
    xx3 = Rx+rr2*sin(w-ww2);
    yy3 = Ry-rr2*cos(w-ww2);

    SetPen(Schwarz,1);
    SetBrush(Schwarz);
    Circle(xx0, yy0, 3);
    Circle(xx1, yy1, 3);
    Circle(xx2, yy2, 3);
    Circle(xx3, yy3, 3);

    double w1, w2, r1, r2;
    int x0, y0, x1, y1, x2, y2, x3, y3;

    w = GradToRad(grad);
    w1 = atan((AutoL/2)/(R-AutoW/2));
    w2 = atan(AutoL/2/(R+AutoW/2));
    r1 = sqrt((AutoL*AutoL/4)+(R-AutoW/2)*(R-AutoW/2));
    r2 = sqrt((AutoL*AutoL/4)+(R+AutoW/2)*(R+AutoW/2));

    x0 = Rx+r1*sin(w+w1);
    y0 = Ry-r1*cos(w+w1);
    x1 = Rx+r1*sin(w-w1);
    y1 = Ry-r1*cos(w-w1);
    x2 = Rx+r2*sin(w+w2);
    y2 = Ry-r2*cos(w+w2);
    x3 = Rx+r2*sin(w-w2);
    y3 = Ry-r2*cos(w-w2);

    ClearPoints();
    SetPoint(x0, y0);
    SetPoint(x1, y1);
    SetPoint(x3, y3);
    SetPoint(x2, y2);
    SetPoint(x0, y0);
    SetPenColor(Schwarz);
    SetBrushColor(Gelb);
    Poly();
}



void TUser::DrawRad(int x, int y){

}

void TUser::DrawPass(int x, int y){     // Passant
    pose = pose % 3;
    if(x<HSLx-sp-RW || x>HSRx+sp+RW && y>WRDy+sp+RW &&y<RRDy+FW+RW+GW){
        SetPen(Schwarz,2);            // RRD
        switch(pose){
            case 0:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                Circle(x,y,5);
                pose++;
                break;
            case 1:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                MoveTo(x,y+5);
                LineTo(x+8, y+5);
                MoveTo(x,y-5);
                LineTo(x-8, y-5);
                Circle(x,y,5);
                pose++;
                break;
            default:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                MoveTo(x,y-5);
                LineTo(x+8, y-5);
                MoveTo(x,y+5);
                LineTo(x-8, y+5);
                Circle(x,y,5);
                pose++;
        }
    }
    else if(x<HSLx-sp-RW || x>HSRx+sp+RW && y<RRUy-sp-RW && y>WRUy-sp-RW-GW){
        SetPen(Schwarz,2);            //WRu & RRU
        switch(pose){
            case 0:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                Circle(x,y,5);
                pose++;
                break;
            case 1:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                MoveTo(x,y-5);
                LineTo(x+8, y-5);
                MoveTo(x,y+5);
                LineTo(x-8, y+5);
                Circle(x,y,5);
                pose++;  
                break;
            default:
                MoveTo(x,y-10);
                LineTo(x,y+10);
                MoveTo(x,y+5);
                LineTo(x+8, y+5);
                MoveTo(x,y-5);
                LineTo(x-8, y-5);
                Circle(x,y,5);
                pose++;
        }
    }
//    else if(x>HSLx)
}
//================ Draw Tram ==================
void TUser::DrawTram(){
    
    if (tramNachOben)
    {
        DrawTramNachOben(LSRy-countT1);
        if (LSRy-countT1 > stop1) {  // not arrived at stop position
            countT1+=5;
        } else {
            if (waitTimeT1 != 0) {
                waitTimeT1--;
            } else {
                if (E1 != 0) {
                    E1 = 0;     // lamp E1 green
                }
                countT1+=5;
            }
            if (E1 != 2 && LSRy-countT1 < -20) {    // // lamp E1 red
                E1 = 2;
                goCrossT1 = false;
                return_T1 = true;       // recover lamp signal
            }
        }
        if ((LSRy-countT1) <= -250) {
            tramNachOben = false;
            countT1 = 0;
            waitTimeT1 = 10 * ratio;
        }
    }
    
    if (tramNachUnten)
    {
        DrawTramNachUnten(countT2);
        if (countT2 < stop2) {        // not arrived at stop position
            countT2+=5;
            if (countT2 > 100) {
                E2 = 0;                 // lamp E2 green
                goCrossT2 = true;       // go through cross
            }
        } else {
            if (E2 != 2) {
                E2 = 2;
                goCrossT2 = false;      // not allow go through the cross
                return_T2 = true;       // recover lamp signal
            }
            if (waitTimeT2 != 0) {
                waitTimeT2--;           // wait 
            } else {
                countT2+=5;              // continue driving
            }            
        }
        if ((countT2) >= LSLy+250) {
            tramNachUnten = false;
            countT2 = 0;
            waitTimeT2 = 10 * ratio;
        }
    }
    
}

void TUser::DrawTramNachOben (int y) {
    if (y >= PR0) {
        int y1 = DrawTramTeil1(lsx2, y, 0, 1);        // 1
        //SetBrush(Schwarz);
        Circle ((int)lsx2, y1+5, 6);
        int y2 = DrawTramTeil1(lsx2, y1+10, 0, 1);    // 2
        Circle ((int)lsx2, y2+5, 6);
        int y3 = DrawTramTeil1(lsx2, y2+10, 0, 1);    // 3
        // Circle ((int)lsx2, y3+5, 6);
    } else if (y >= PR1) {
        double x0 = ktram*y+bTR;
        double sink = (x0-lsx2)/70;
        double cosk = sqrt(1-sink*sink);
        int y1 = DrawTramTeil1(x0, y, sink, cosk);    // 1
        Circle ((int)lsx2, y1+5, 6);
        int y2 = DrawTramTeil1(lsx2, y1+10, 0, 1);    // 2
        Circle ((int)lsx2, y2+5, 6);
        int y3 = DrawTramTeil1(lsx2, y2+10, 0, 1);    // 3
        // Circle ((int)lsx2, y3+5, 6);
    } else if (y >= PR2) {
        double x0 = ktram*y+bTR;
        double sink = sin(atan(-ktram));
        double cosk = sqrt(1-sink*sink);
        int y1 = DrawTramTeil1(x0, y, sink, cosk);  // 1
        Circle ((int)(ktram*(y1+5*cosk)+bTR), (int)(y1+5*cosk), 6);
        if (y1+10*cosk >= PR1) {
            double x1 = ktram*(y1+10*cosk)+bTR;
            double sink1 = (x1-lsx2)/70;
            double cosk1 = sqrt(1-sink1*sink1);
            int y2 = DrawTramTeil1(x1, y1+10*cosk1, sink1, cosk1);     // 2
            Circle ((int)lsx2, (int)(y2+5*cosk1), 6);                  
            int y3 = DrawTramTeil1(lsx2, y2+10*cosk1, 0, 1);           // 3
            //Circle ((int)lsx2, y3+5, 6);
        } else {
            double x1 = ktram*(y1+10*cosk)+bTR;
            int y2 = DrawTramTeil1(x1, y1+10*cosk, sink, cosk);        // 2
            Circle ((int)(ktram*(y1+5*cosk)+bTR), (int)(y1+5*cosk), 6);
            if (y2+10*cosk >= PR1) {
                double x3 = ktram*(y2+10*cosk)+bTR;
                double sink3 = (x3-lsx2)/70;
                double cosk3 = sqrt(1-sink3*sink3);
                int y3 = DrawTramTeil1(x3, y2+10*cosk3, sink3, cosk3);   // 3
                Circle ((int)(ktram*(y2+5*cosk3)+bTR), (int)(y2+5*cosk3), 6);
            } else {
                double x3 = ktram*(y2+10*cosk)+bTR;
                DrawTramTeil1(x3, y2+10*cosk, sink, cosk);             // 3
                Circle((int)(ktram*(y2+5*cosk)+bTR), (int)(y2+5*cosk), 6);    
            }
        }
    } else {
        double sink = sin(atan(-ktram));
        double cosk = sqrt(1-sink*sink);
        double y0 = y+70*cosk;
        if (y0 > PR2) {
            double x0 = ktram*y0+bTR;
            double sink0 = (hsx2-x0)/70;
            double cosk0 = sqrt(1-sink0*sink0);
            int y1 = DrawTramTeil2(x0, y0, sink0, cosk0)+70*cosk;    // 1
            Circle((int)(ktram*(y1+5*cosk)+bTR), (int)(y1+5*cosk), 6);
            y1 += 10*cosk;
            double x1 = ktram*y1+bTR;
            int y2 = DrawTramTeil1(x1, y1, sink, cosk);
            Circle((int)(ktram*(y2+5*cosk)+bTR), (int)(y2+5*cosk), 6);
            y2 += 10*cosk;
            double x2 = ktram*y2+bTR;
            DrawTramTeil1(x2, y2, sink, cosk);
        } else {
            int y1 = DrawTramTeil1(hsx2, y, 0, 1);        // 1
            Circle ((int)hsx2, y1+5, 6);
            int y0 = y1+10 + 70*cosk;
            if (y0 > PR2) {
                double x0 = ktram*y0+bTR;
                double sink0 = (hsx2-x0)/70;
                double cosk0 = sqrt(1-sink0*sink0);
                int y2 =DrawTramTeil2(x0, y0, sink0, cosk0)+70*cosk;
                Circle((int)(ktram*(y2+5*cosk)+bTR), (int)(y2+5*cosk), 6);
                y2 += 10*cosk;
                double x2 = ktram*y2+bTR;
                DrawTramTeil1(x2, y2, sink, cosk);
            } else {
                y1 += 10;
                int y2 = DrawTramTeil1(hsx2, y1, 0, 1);        // 2
                Circle ((int)hsx2, y2+5, 6);
                y0 = y2+10 + 70*cosk;
                if (y0 > PR2) {
                    double x0 = ktram*y0+bTR;
                    double sink0 = (hsx2-x0)/70;
                    double cosk0 = sqrt(1-sink0*sink0);
                    DrawTramTeil2(x0, y0, sink0, cosk0);
                } else {
                    y2 += 10;
                    DrawTramTeil1(hsx2, y2, 0, 1);
                } 
            }            
        }        
    }
}

void TUser::DrawTramNachUnten (int y) {

    if (y <= PL0) {
        int y1 = DrawTramTeil2(hsx1, y, 0, 1);        // 1
        Circle ((int)hsx1, y1-5, 6);
        int y2 = DrawTramTeil2(hsx1, y1-10, 0, 1);    // 2
        Circle ((int)hsx1, y2-5, 6);
        int y3 = DrawTramTeil2(hsx1, y2-10, 0, 1);    // 3
        // Circle ((int)lsx2, y3+5, 6);
    } else if (y <= PL1) {
        double x0 = ktram*y+bTL;
        double sink = (hsx1-x0)/70;
        double cosk = sqrt(1-sink*sink);
        int y1 = DrawTramTeil2(x0, y, sink, cosk);    // 1
        Circle ((int)hsx1, y1-5, 6);
        int y2 = DrawTramTeil2(hsx1, y1-10, 0, 1);    // 2
        Circle ((int)hsx1, y2-5, 6);
        int y3 = DrawTramTeil2(hsx1, y2-10, 0, 1);    // 3
    } else if (y <= PL2) {
        double x0 = ktram*y+bTL;
        double sink = sin(atan(-ktram));
        double cosk = sqrt(1-sink*sink);
        int y1 = DrawTramTeil2(x0, y, sink, cosk);  // 1
        Circle ((int)(ktram*(y1-5*cosk)+bTL)-1, (int)(y1-5*cosk), 6);
        if (y1-10*cosk <= PL1) {
            double x1 = ktram*(y1-10*cosk)+bTL;
            double sink1 = (hsx1-x1)/70;
            double cosk1 = sqrt(1-sink1*sink1);
            int y2 = DrawTramTeil2(x1, y1-10*cosk1, sink1, cosk1);     // 2
            Circle ((int)hsx1, (int)(y2-5*cosk1), 6);                  
            int y3 = DrawTramTeil2(hsx1, y2-10*cosk1, 0, 1);           // 3
            //Circle ((int)hsx1, y3-5, 6);
        } else {
            double x1 = ktram*(y1-10*cosk)+bTL;
            int y2 = DrawTramTeil2(x1, y1-10*cosk, sink, cosk);        // 2
            Circle ((int)(ktram*(y1-5*cosk)+bTL)-1, (int)(y1-5*cosk), 6);
            if (y2-10*cosk <= PL1) {
                double x3 = ktram*(y2-10*cosk)+bTL;
                double sink3 = (hsx1-x3)/70;
                double cosk3 = sqrt(1-sink3*sink3);
                int y3 = DrawTramTeil2(x3, y2-10*cosk3, sink3, cosk3);   // 3
                Circle ((int)(ktram*(y2-5*cosk3)+bTL)-1, (int)(y2-5*cosk3), 6);
            } else {
                double x3 = ktram*(y2-10*cosk)+bTL;
                DrawTramTeil2(x3, y2-10*cosk, sink, cosk);             // 3
                Circle((int)(ktram*(y2-5*cosk)+bTL)-1, (int)(y2-5*cosk), 6);    
            }
        }
     } else {
        double sink = sin(atan(-ktram));
        double cosk = sqrt(1-sink*sink);
        double y0 = y-70*cosk;
        if (y0 < PL2) {
            double x0 = ktram*y0+bTL;
            double sink0 = (x0-lsx1)/70;
            double cosk0 = sqrt(1-sink0*sink0);
            int y1 = DrawTramTeil1(x0, y0, sink0, cosk0)-70*cosk;    // 1
            Circle((int)(ktram*(y1-5*cosk)+bTL), (int)(y1-5*cosk), 6);
            y1 -= 10*cosk;
            double x1 = ktram*y1+bTL;                      //2
            int y2 = DrawTramTeil2(x1, y1, sink, cosk);
            Circle((int)(ktram*(y2-5*cosk)+bTL), (int)(y2-5*cosk), 6);
            y2 -= 10*cosk;
            double x2 = ktram*y2+bTL;
            DrawTramTeil2(x2, y2, sink, cosk);            //3
        } else {
            int y1 = DrawTramTeil2(lsx1, y, 0, 1);        // 1
            Circle ((int)lsx1, y1-5, 6);
            int y0 = y1-10 - 70*cosk;
            if (y0 < PL2) {
                double x0 = ktram*y0+bTL;
                double sink0 = (x0-lsx1)/70;
                double cosk0 = sqrt(1-sink0*sink0);
                int y2 =DrawTramTeil1(x0, y0, sink0, cosk0)-70*cosk;    //2
                Circle((int)(ktram*(y2-5*cosk)+bTL), (int)(y2-5*cosk), 6);
                y2 -= 10*cosk;
                double x2 = ktram*y2+bTL;
                DrawTramTeil2(x2, y2, sink, cosk);             // 3
            } else {
                y1 -= 10;
                int y2 = DrawTramTeil2(lsx1, y1, 0, 1);        // 2
                Circle ((int)lsx1, y2-5, 6);
                y0 = y2-10 -70*cosk;
                if (y0 < PL2) {
                    double x0 = ktram*y0+bTL-1;
                    double sink0 = (x0-lsx1)/70;
                    double cosk0 = sqrt(1-sink0*sink0);
                    DrawTramTeil1(x0, y0, sink0, cosk0);       // 3
                } else {
                    y2 -= 10;
                    DrawTramTeil2(lsx1, y2, 0, 1);             // 3
                } 
            }            
        }  
     }
}

int TUser::DrawTramTeil1(double x, int y, double sink, double cosk){
    double xedge, yedge;
    ClearPoints();
    SetPen(Schwarz,1);

    xedge = x-12*cosk; yedge = y-12*sink;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge-70*sink; yedge = y+70*cosk;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge+25*cosk; yedge = yedge+25*sink;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge+70*sink; yedge = yedge-70*cosk;
    SetPoint((int)xedge, (int)yedge);
    SetPenColor(Schwarz);
    SetBrushColor(Hellrot);
    Poly();
    return (int)(y+70*cosk);
}

int TUser::DrawTramTeil2(double x, int y, double sink, double cosk){
    double xedge, yedge;
    ClearPoints();
    SetPen(Schwarz,1);

    xedge = x+12*cosk; yedge = y+12*sink;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge+70*sink; yedge = y-70*cosk;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge-25*cosk; yedge = yedge-25*sink;
    SetPoint((int)xedge, (int)yedge);
    xedge = xedge-70*sink; yedge = yedge+70*cosk;
    SetPoint((int)xedge, (int)yedge);
    SetPenColor(Schwarz);
    SetBrushColor(Hellrot);
    Poly();
    return (int)(y-70*cosk);
}


void TUser::InitTrambahn() {
    // Draw Trambahn Bereich
    // upside
    SetPen(Hellgrau,2);
    SetBrush(Hellgrau);
    Rectangle(HSLx+4*FW+sp+10, HSLy, 4*sp+2*TW+2*FW-2*(sp+10), WRUy-r);
    // Isolation Belt
    SetPen(Schwarz,1);
    SetBrush(Gruen);
    Rectangle(HSLx+4*FW+5, HSLy, sp+5, WRUy-r);
    Rectangle(HSRx-2*FW-5, HSRy, -sp-5, WRUy-r);
    // downside
    SetPen(Schwarz,2);  
    SetBrush(Hellgrau);
    Rectangle(HSLx+4*FW, WRDy+r, 4*sp+2*TW+1, LSLy-WRDy);
    // stop line
    SetPen(Hellblau,2);
    MoveTo(HSLx+4*FW, WRUy-r);      // up
    LineTo(HSLx+4*FW+2*TW+4*sp+2*FW, WRUy-r);
    MoveTo(HSLx+4*FW, WRDy+r);           // down
    LineTo(HSLx+4*FW+2*TW+4*sp, WRDy+r);
    //Draw boundary
    SetPen(Schwarz, 2);
    MoveTo(GetMaxW()/2-2*sp-TW, 0);
    LineTo(GetMaxW()/2-2*sp-TW, WRUy-r);
    MoveTo(GetMaxW()/2+2*sp+TW+2*FW, 0);
    LineTo(GetMaxW()/2+2*sp+TW+2*FW, WRUy-r);
    //Linkenseite
    // upside
    SetPen(Schwarz, 1);
    MoveTo(GetMaxW()/2+FW-sp, 0);    // nach rechts um FW Pixels verschieben.
    LineTo(GetMaxW()/2+FW-sp, WRUy-r);
    MoveTo(GetMaxW()/2+FW-sp-TW, 0);
    LineTo(GetMaxW()/2+FW-sp-TW, WRUy-r);
    for(int j=-5; j<WRUy-r; j+=5) {
        SetPen(Schwarz, 1);
        MoveTo(GetMaxW()/2+FW-sp-TW, j);
        LineTo(GetMaxW()/2+FW-sp, j);
    }
    // in der mittel
    //SetPen(Hellgrau, 1);
    MoveTo(GetMaxW()/2+FW-sp, WRUy-r);
    LineTo(GetMaxW()/2-sp, WRDy+r);
    MoveTo(GetMaxW()/2+FW-sp-TW, WRUy-r);
    LineTo(GetMaxW()/2-sp-TW, WRDy+r);
    // downside
    SetPen(Schwarz, 1);
    MoveTo(GetMaxW()/2-sp, WRDy+r);
    LineTo(GetMaxW()/2-sp, GetMaxH()+5);
    MoveTo(GetMaxW()/2-sp-TW, WRDy+r);
    LineTo(GetMaxW()/2-sp-TW, GetMaxH()+5);
    for(int j=WRDy+r; j<GetMaxH()+5; j+=5) {
        SetPen(Schwarz, 1);
        MoveTo(GetMaxW()/2-sp-TW, j);
        LineTo(GetMaxW()/2-sp, j);
    }
    // Rechtseite
    // upside
    SetPen(Schwarz, 1);
    MoveTo(GetMaxW()/2+FW+sp, -5);
    LineTo(GetMaxW()/2+FW+sp, WRUy-r);
    MoveTo(GetMaxW()/2+FW+sp+TW, -5);
    LineTo(GetMaxW()/2+FW+sp+TW, WRUy-r);
    for(int j=-5; j<WRUy-r; j+=5) {
        SetPen(Schwarz, 1);
        MoveTo(GetMaxW()/2+FW+sp, j);
        LineTo(GetMaxW()/2+FW+sp+TW, j);
    }
    // in der mittel
    //SetPen(Hellgrau, 1);
    MoveTo(GetMaxW()/2+FW+sp, WRUy-r);
    LineTo(GetMaxW()/2+sp, WRDy+r);
    MoveTo(GetMaxW()/2+FW+sp+TW, WRUy-r);
    LineTo(GetMaxW()/2+sp+TW, WRDy+r);
    // downside
    SetPen(Schwarz, 1);
    MoveTo(GetMaxW()/2+sp, WRDy+r);
    LineTo(GetMaxW()/2+sp, GetMaxH()+5);
    MoveTo(GetMaxW()/2+sp+TW, WRDy+r);
    LineTo(GetMaxW()/2+sp+TW, GetMaxH()+5);
    for(int j=WRDy+r; j<GetMaxH()+5; j+=5) {
        SetPen(Schwarz, 1);
        MoveTo(GetMaxW()/2+sp, j);
        LineTo(GetMaxW()/2+sp+TW, j);
    }
}
//=======================Set Lamp color=============================
void TUser::SetLampColor(int lamp) {
    switch(lamp){
        case 0:
            SetBrush(Hellgruen);
            break;
        case 1:
            SetBrush(Hellgelb);
            break;
        default:
            SetBrush(Hellrot);}
}

void TUser::TextFarbe(int lamp) {
    switch(lamp){
        case 0:
            SetTextColor(Hellgruen);
            break;
        case 1:
            SetTextColor(Hellgelb);
            break;
        default:
            SetTextColor(Hellrot);}
}

//========================== Draw Arrow of Lamps ===============================
// Horizontale Achse
void TUser::DrawArrowH(int x1, int y1, int x2, int y2) {
	SetPen(Schwarz, 2);
	if (x1 > x2) {
    	MoveTo(x1, y1);
	    LineTo(x2, y1);
        if(y1 > y2){      // A1   go right
            LineTo(x2, y2);
            LineTo(x2-5, y2+10);
		    MoveTo(x2, y2);
		    LineTo(x2+5, y2+10);
        }
        else if(y1 < y2) {    // A3, go left
            LineTo(x2, y2);
            LineTo(x2-5, y2-10);
		    MoveTo(x2, y2);
		    LineTo(x2+5, y2-10);
        }
        else{                 // A2, go straight
            LineTo(x2+10, y2-5);
		    MoveTo(x2, y2);
		    LineTo(x2+10, y2+5);
        }
	}
	else {   //(x1 < x2)
        MoveTo(x1, y1);
	    LineTo(x2, y1);
        if(y1 > y2){     // C3    go left
            LineTo(x2, y2);
            LineTo(x2-5, y2+10);
		    MoveTo(x2, y2);
		    LineTo(x2+5, y2+10);
        }
        else if(y1 < y2){            // C1 go right
            LineTo(x2, y2);
            LineTo(x2+5, y2-10);
		    MoveTo(x2, y2);
		    LineTo(x2-5, y2-10);
        }
        else {                   // C2 Go straight
            LineTo(x2-10, y2-5);
		    MoveTo(x2, y2);
		    LineTo(x2-10, y2+5);
        }
	}
}

// Vertikale Achse
void TUser::DrawArrowV(int x1, int y1, int x2, int y2) {
	SetPen(Schwarz, 2);
	if (y1 < y2) {
    	MoveTo(x1, y1);
	    LineTo(x1, y2);
        if(x1 > x2){      // B1   go right
            LineTo(x2, y2);
            LineTo(x2+10, y2-5);
		    MoveTo(x2, y2);
		    LineTo(x2+10, y2+5);
        }
        else if(x1 < x2) {    // B3, go left
            LineTo(x2, y2);
            LineTo(x2-10, y2-5);
		    MoveTo(x2, y2);
		    LineTo(x2-10, y2+5);
        }
        else{   // x1 = x2    // B2, go straight
		    LineTo(x2-5, y2-10);
            MoveTo(x2, y2);
		    LineTo(x2+5, y2-10);
        }
	}
	else{      //(y1 < y2)
        MoveTo(x1, y1);
	    LineTo(x1, y2);
        LineTo(x2, y2);    // D1    go right
        LineTo(x2-10, y2+5);
        MoveTo(x2, y2);
        LineTo(x2-10, y2-5);
	}
}

//=======================Draw Lamp=============================
void TUser::DrawLamp(){
    // Draw Lamp A;
    SetPen(Schwarz, 2);
	SetBrush(Klar);
	SetTextSize(20);
	// Rectangle for Lamp A
	Rectangle(HSRx+r+rLamp, RRUy-r, 2*rLamp, -6*rLamp);
    SetLampColor(A3);         // LampA3
    Circle(HSRx+r+2*rLamp, RRUy-r-rLamp, rLamp);
    DrawArrowH(HSRx+r+3*rLamp, RRUy-r-rLamp, HSRx+r+2*rLamp, RRUy-r);
    SetBrush(Klar);
    TextFarbe(A3);
    Text(HSRx+r-rLamp/2, RRUy-r-rLamp-10, "A3");   // numerieren
    SetLampColor(A2);         // LampA2
    Circle(HSRx+r+2*rLamp, RRUy-r-3*rLamp, rLamp);
    DrawArrowH(HSRx+r+3*rLamp, RRUy-r-3*rLamp, HSRx+r+rLamp, RRUy-r-3*rLamp);
    SetBrush(Klar);
    TextFarbe(A2);
    Text(HSRx+r-rLamp/2, RRUy-r-3*rLamp-10, "A2");  // numerieren
    SetLampColor(A1);         // LampA1
    Circle(HSRx+r+2*rLamp, RRUy-r-5*rLamp, rLamp);
    DrawArrowH(HSRx+r+3*rLamp, RRUy-r-5*rLamp, HSRx+r+2*rLamp, RRUy-r-6*rLamp);
    SetBrush(Klar);
    TextFarbe(A1);
    Text(HSRx+r-rLamp/2, RRUy-r-5*rLamp-10, "A1");  // numerieren

    // Draw Lamp B;
    SetPen(Schwarz, 2);
	SetBrush(Klar);
	// Rectangle for Lamp B
	Rectangle(HSLx-r, WRUy-r-rLamp, -6*rLamp, -2*rLamp);
    SetLampColor(B3);         // LampB3
    Circle(HSLx-r-rLamp, WRUy-r-2*rLamp, rLamp);
    DrawArrowV(HSLx-r-rLamp, WRUy-r-3*rLamp, HSLx-r, WRUy-r-2*rLamp);
    SetBrush(Klar);
    TextFarbe(B3);
    Text(HSLx-r-rLamp-10, WRUy-r-rLamp+rLamp/2, "B3");  // numerieren
    SetLampColor(B2);         // LampB2
    Circle(HSLx-r-3*rLamp, WRUy-r-2*rLamp, rLamp);
    DrawArrowV(HSLx-r-3*rLamp, WRUy-r-3*rLamp, HSLx-r-3*rLamp, WRUy-r-rLamp);
    SetBrush(Klar);
    TextFarbe(B2);
    Text(HSLx-r-3*rLamp-10, WRUy-r-rLamp+rLamp/2, "B2");  // numerieren
    SetLampColor(B1);         // LampB1
    Circle(HSLx-r-5*rLamp, WRUy-r-2*rLamp, rLamp);
    DrawArrowV(HSLx-r-5*rLamp, WRUy-r-3*rLamp, HSLx-r-6*rLamp, WRUy-r-2*rLamp);
    SetBrush(Klar);
    TextFarbe(B1);
    Text(HSLx-r-5*rLamp-10, WRUy-r-rLamp+rLamp/2, "B1");  // numerieren

    // Draw Lamp C;
    SetPen(Schwarz, 2);
	SetBrush(Klar);
	// Rectangle for Lamp C
	Rectangle(LSLx-r-rLamp, WRDy+r, -2*rLamp, 6*rLamp);
    /*SetPen(Schwarz, 1);
	SetBrush(Weiss);
    Rectangle(LSLx-r+rLamp, WRDy+r, -2*rLamp, 6*rLamp);*/
    SetLampColor(C3);         // LampC3
    Circle(LSLx-r-2*rLamp, WRDy+r+rLamp, rLamp);
    DrawArrowH(LSLx-r-3*rLamp, WRDy+r+rLamp, LSLx-r-2*rLamp, WRDy+r);
    SetBrush(Klar);
    TextFarbe(C3);
    Text(LSLx-r-rLamp/2, WRDy+r+10, "C3");  // numerieren
    SetLampColor(C2);         // LampC2
    Circle(LSLx-r-2*rLamp, WRDy+r+3*rLamp, rLamp);
    DrawArrowH(LSLx-r-3*rLamp, WRDy+r+3*rLamp, LSLx-r-rLamp, WRDy+r+3*rLamp);
    SetBrush(Klar);
    TextFarbe(C2);
    Text(LSLx-r-rLamp/2, WRDy+r+2*rLamp+10, "C2");  // numerieren
    SetLampColor(C1);         // LampC1
    Circle(LSLx-r-2*rLamp, WRDy+r+5*rLamp, rLamp);
    DrawArrowH(LSLx-r-3*rLamp, WRDy+r+5*rLamp, LSLx-r-2*rLamp, WRDy+r+6*rLamp);
    SetBrush(Klar);
    TextFarbe(C1);
    Text(LSLx-r-rLamp/2, WRDy+r+4*rLamp+10, "C1");  // numerieren

    // Draw Lamp D;
    SetPen(Schwarz, 2);
	SetBrush(Klar);
	// Rectangle for Lamp D
	Rectangle(LSRx+r, RRDy+r+FW+rLamp, 2*rLamp, 2*rLamp);
    SetLampColor(D1);         // LampD1
    Circle(LSRx+r+rLamp, RRDy+r+FW+2*rLamp, rLamp);
    DrawArrowV(LSRx+r+rLamp, RRDy+r+FW+3*rLamp, LSRx+r+2*rLamp, RRDy+r+FW+2*rLamp);
    SetBrush(Klar);
    TextFarbe(D1);
    Text(LSRx+r+rLamp/2, RRDy+r+FW-rLamp/2, "D1");  // numerieren

    // Draw Lamp of Tram E
    SetPen(Schwarz, 1);
    SetLampColor(E1);
    Circle(HSLx+4*FW+2*TW+4*sp+10, WRDy+r+10, 10);  // Lamp E1

    SetPen(Schwarz, 1);
    SetLampColor(E2);
    Circle(HSLx+4*FW+15, WRUy-r+10, 10);  // Lamp E2

    // Connect line
    // A
    SetPen(Schwarz,1);
    MoveTo(HSRx+r+2*rLamp, RRUy-r);
    LineTo(HSRx+r+2*rLamp, RRUy-r+25);
    // B
    MoveTo(HSLx-r, WRUy-r-2*rLamp);
    LineTo(HSLx-r+25, WRUy-r-2*rLamp);
    // C
    MoveTo(LSLx-r-2*rLamp, WRDy+r);
    LineTo(LSLx-r-2*rLamp, WRDy+r-25);
    // D
    MoveTo(LSRx+r, RRDy+r+FW+2*rLamp);
    LineTo(LSRx+r-25, RRDy+r+FW+2*rLamp);
}

//=============================================================
//    Strssen, abbiegen oder gerade fahren
//=============================================================
// Hamburgerstrasse nach Rebenring
void TUser::HS2RR(){
    // HSL -> RRD, Hambugerstrasse nach Rebenring abbiegen
    SetPen(Weiss,2);
    for(int i=180; i<271; i+=6){
        Arc(HSRx+r-3*sp-2*TW+2*FW, WRUy-r, i, i+4, r+sp+6*FW);
    }
    for(int i=180; i<275; i+=5){
        Arc(HSRx+r-3*sp-2*TW+2*FW+1, WRUy-r, i, i+3, r+sp+7*FW+1);
    }
    for(int i=180; i<275; i+=5){
        Arc(HSRx+r-3*sp-2*TW+2*FW+1, WRUy-r, i, i+3, r+sp+8*FW+1);
    }
    for(int i=180; i<273; i+=5){
        Arc(HSRx+r-3*sp-2*TW+2*FW, WRUy-r, i, i+3, r+sp+9*FW);
    }
}

// Hamburgerstrasse nach Wendenring
void TUser::HS2WR(){
    // Hamburgerstrasse nach Wendenring abbiegen
    SetPen(Weiss, 2);
    for(int i=270; i<360; i+=18){
        Arc(HSLx-r, WRUy-r, i, i+10, r+FW);
    }
}
// Hamburgerstrasse nach Lampestrasse
void TUser::HS2LS(){
    // Hambugerstrasse nach Lampestrasse.
    SetPen(Weiss,2);
    for(int y=WRUy-r; y<WRDy+r; y+=20){
        MoveTo(HSLx, y);
        LineTo(LSLx, y+15);
        MoveTo(HSLx+FW, y);
        LineTo(LSLx+FW, y+15);        
    }
}
// Wendenring nach Rebenring
void TUser::WR2RR(){
    // Wendenring nach Rebenring fahren
    // Fahrbahnbereich
    SetPen(Weiss, 2);
    for(int x = HSRx+r; x > HSLx-r; x-=20){
        MoveTo(x, WRDy-3*FW);
        LineTo(x-15, WRDy-3*FW);
    }
    for(int x = HSRx+r; x > HSLx-r; x-=20){
        MoveTo(x, WRDy-2*FW);
        LineTo(x-15, WRDy-2*FW);
    }
    for(int x = HSRx+r; x > HSLx-r; x-=20){
        MoveTo(x, WRDy-FW);
        LineTo(x-15, WRDy-FW);
    }
    // Bus linie
    SetPen(Gelb, 2);
    for(int x = HSRx+r; x > HSLx-r; x-=20){
        MoveTo(x, WRDy);
        LineTo(x-15, WRDy);
    }
}
// Wendenring nach Hamburgerstrasse
void TUser::WR2HS(){
    // Wendenring nach Hamburgerstrasse fahren
    // Fahrbahn Bereich
    SetPen(Weiss, 2);
    for(int i = 270; i<360; i+=8){
        Arc(HSLx-r+3*sp+2*TW+2*FW, WRUy-r, i, i+4, r+4*FW+sp+1);
    }
    for(int i = HSLx-r; i<HSLx-r+3*sp+2*TW+2*FW; i+=25){
        MoveTo(i,WRDy-5*FW);
        LineTo(i+15, WRDy-5*FW);
    }
    for(int i = 270; i<360; i+=8){
        Arc(HSLx-r+3*sp+2*TW+2*FW, WRUy-r, i, i+4, r+5*FW+sp+1);
    }
    for(int i = HSLx-r; i<HSLx-r+3*sp+2*TW+2*FW; i+=25){
        MoveTo(i,WRDy-4*FW);
        LineTo(i+15, WRDy-4*FW);
    }
    for(int i = 270; i<360; i+=8){
        Arc(HSLx-r+3*sp+2*TW+2*FW, WRUy-r, i, i+4, r+6*FW+sp);
    }
    for(int i = HSLx-r; i<HSLx-r+3*sp+2*TW+2*FW; i+=25){
        MoveTo(i,WRDy-3*FW);
        LineTo(i+15, WRDy-3*FW);
    }
}
// Wendenring nach Lampestrasse
void TUser::WR2LS(){
    // Wendenring nach Lampestrasse abbiegen
    SetPen(Weiss, 2);
    for(int i=0; i< 90; i+=18){
        Arc(HSLx-r, WRDy+r, i, i+10, r+FW);
    }
}
// Lampestrasse nach Rebenring
void TUser::LS2RR(){
    // Lampestrasse nach Renbenring abbiegen
    SetPen(Weiss, 2);
    for(int i=90; i<180; i+=18){
    Arc(HSRx+r, RRDy+r, i, i+10, r+FW);
    }
}
// Rebenring nach Wendenring
void TUser::RR2WR(){
// Renbenring nach Wendenring fahren

    int y0, y1; 
    SetPen(Weiss,2);
    for(int x = rrx; x>wrx; x-=20){
        // line 1
        y0 = kcar*x+b1;
        MoveTo(x, y0);
        y1 = kcar*(x-15)+b1;
        LineTo(x-15, y1);
        // Line 2
        y0 = kcar*x+b2;
        MoveTo(x, y0);
        y1 = kcar*(x-15)+b2;
        LineTo(x-15, y1);
        // Line 3
        y0 = kcar*x+b3;
        MoveTo(x, y0);
        y1 = kcar*(x-15)+b3;
        LineTo(x-15, y1);
    }
    
    // Bus Fahrbahn
    SetPen(Gelb, 2);
    for(int x=rrx; x>wrx; x-=20){
        // line 1
        y0 = kbus * x + b4;
        MoveTo(x,y0);
        y1 = kbus*(x-15)+b4;
        LineTo(x-15, y1);
        // line 2
        y0 = kbus * x + b5;
        MoveTo(x,y0);
        y1 = kbus*(x-15)+b5;
        LineTo(x-15, y1);
    }
}
// Rebenring nach Hamburgerstrasse
void TUser::RR2HS(){
    //Rebenring nach Hambugerstrasse abbiegen
    SetPen(Weiss, 2);
    for(int i=180; i<270; i+=14){
        Arc(HSRx+r-2, RRUy-r, i, i+9, r+FW);
    }
    for(int i=180; i<270; i+=9){
        Arc(HSRx+r, RRUy-r, i, i+6, r+2*FW+2);
    }
    MoveTo(HSRx-2*FW, WRUy-r+3);
    LineTo(HSRx-2*FW, WRUy-r+18);
    MoveTo(HSRx-FW, WRUy-r+3);
    LineTo(HSRx-FW, WRUy-r+18);
}
// Rebenring nach Lampestrasse
void TUser::RR2LS(){
    // Renbenring nach Lampestrasse fahren
    SetPen(Weiss, 2);
    for(int i=90; i<180; i+=6){
        Arc(HSRx+r-(4*FW+3*sp+2*TW), WRDy+r, i, i+4, r+4*FW+sp);
        //Arc(HSRx, RRDy+r+12*FW, i, i+4, r+12*FW+sp+3*FW);
    }
    for(int i=90; i<180; i+=6){
        Arc(HSRx+r-(4*FW+3*sp+2*TW), WRDy+r, i, i+4, r+3*FW+sp);
    }
    for(int x = HSRx+r; x > HSRx-(4*FW+3*sp+2*TW-r); x-=15){
        MoveTo(x,RRUy+4*FW);
        LineTo(x-=15, RRUy+4*FW);
    }
    for(int x = HSRx+r; x > HSRx-(4*FW+3*sp+2*TW-r); x-=15){
        MoveTo(x,RRUy+5*FW);
        LineTo(x-=14, RRUy+5*FW);
    }
}

//=============================================================
//    Strssen, Arrow Richtung, Model 1,  Rechts, Gerade, Links
//=============================================================
void TUser::InitArrowR1(int x, int y){
    ClearPoints();
    SetPoint(x, y-1);
    SetPoint(x-49, y-1);
    SetPoint(x-49, y-4);
    SetPoint(x-35, y-4);
    SetPoint(x-49, y-7);
    SetPoint(x-65, y-4);
    SetPoint(x-51, y-4);
    SetPoint(x-51, y+1);
    SetPoint(x, y+1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowG1(int x, int y){
    ClearPoints();
    SetPoint(x, y-1);
    SetPoint(x-50, y-1);
    SetPoint(x-50, y-4);
    SetPoint(x-65, y);
    SetPoint(x-50, y+4);
    SetPoint(x-50, y+1);
    SetPoint(x, y+1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowL1(int x, int y){
    ClearPoints();
    SetPoint(x, y-1);
    SetPoint(x-51, y-1);
    SetPoint(x-51, y+4);
    SetPoint(x-65, y+4);
    SetPoint(x-50, y+7);
    SetPoint(x-35, y+4);
    SetPoint(x-49, y+4);
    SetPoint(x-49, y+1);
    SetPoint(x, y+1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
//=============================================================
//    Strssen, Arrow Richtung, Model 2,  Rechts, Gerade, Links
//=============================================================
void TUser::InitArrowR2(int x, int y){
    ClearPoints();
    SetPoint(x-1, y);
    SetPoint(x-1, y+24);
    SetPoint(x-4, y+24);
    SetPoint(x-4, y+10);
    SetPoint(x-7, y+24);
    SetPoint(x-4, y+40);
    SetPoint(x-4, y+26);
    SetPoint(x+1, y+26);
    SetPoint(x+1, y);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowG2(int x, int y){
    ClearPoints();
    SetPoint(x-1, y);
    SetPoint(x-1, y+25);
    SetPoint(x-4, y+25);
    SetPoint(x, y+40);
    SetPoint(x+4, y+25);
    SetPoint(x+1, y+25);
    SetPoint(x+1, y);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowL2(int x, int y){
    ClearPoints();
    SetPoint(x-1, y);
    SetPoint(x-1, y+51);
    SetPoint(x+4, y+51);
    SetPoint(x+4, y+65);
    SetPoint(x+7, y+50);
    SetPoint(x+4, y+35);
    SetPoint(x+4, y+49);
    SetPoint(x+1, y+49);
    SetPoint(x+1, y);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}

//=============================================================
//    Strssen, Arrow Richtung, Model 3,  Rechts, Gerade, Links
//=============================================================
void TUser::InitArrowR3(int x, int y){
    ClearPoints();
    SetPoint(x, y+1);
    SetPoint(x+29, y+1);
    SetPoint(x+29, y+4);
    SetPoint(x+20, y+4);
    SetPoint(x+30, y+7);
    SetPoint(x+40, y+4);
    SetPoint(x+31, y+4);
    SetPoint(x+31, y-1);
    SetPoint(x, y-1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowG3(int x, int y){
    ClearPoints();
    SetPoint(x, y-1);
    SetPoint(x+50, y-1);
    SetPoint(x+50, y-4);
    SetPoint(x+65, y);
    SetPoint(x+50, y+4);
    SetPoint(x+50, y+1);
    SetPoint(x, y+1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}
void TUser::InitArrowL3(int x, int y){
    ClearPoints();
    SetPoint(x, y+1);
    SetPoint(x+51, y+1);
    SetPoint(x+51, y-4);
    SetPoint(x+65, y-4);
    SetPoint(x+50, y-7);
    SetPoint(x+35, y-4);
    SetPoint(x+49, y-4);
    SetPoint(x+49, y-1);
    SetPoint(x, y-1);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}

//=============================================================
//    Strssen, Arrow Richtung, Model 4,  Rechts, Gerade, Links
//=============================================================
void TUser::InitArrowR4(int x, int y){
    ClearPoints();
    SetPoint(x+1, y);
    SetPoint(x+1, y-49);
    SetPoint(x+4, y-49);
    SetPoint(x+4, y-35);
    SetPoint(x+7, y-49);
    SetPoint(x+4, y-65);
    SetPoint(x+4, y-51);
    SetPoint(x-1, y-51);
    SetPoint(x-1, y);
    SetPenColor(Weiss);
    SetBrushColor(Weiss);
    Poly();
}

//==============================================================================
//   Init Fahrbaehne
//==============================================================================
void TUser::InitFahrbahn() {

//==============================================================================
//   Hamburgerstrasse
//==============================================================================
    //  Hamburgerstrasse left side
    //  left Boundary
    SetPen(Schwarz, 2);
    MoveTo(HSLx, HSLy);
    LineTo(HSLx, WRUy-r);
    // Stop line
    SetPen(Weiss, 2);
    MoveTo(HSLx, WRUy-r);
    LineTo(HSLx+4*FW, WRUy-r);
    // bai se biao shi xian, white color line
    for(int i = (HSLx+FW); i < (HSLx+4*FW); i += FW) {
        for(int j = HSLy; j < (WRUy-r); j += 25) {
            SetPen(Weiss, 2);
            MoveTo(i, j);
            if(WRUy-r-j >= 15){
                LineTo(i, j+15);}
            else{
                LineTo(i, WRUy-r);}
        }
    }
//=================================Bahn arrow===================================
    // Arrow 1
    InitArrowR2(HSLx+13, WRUy-r-2*FW-sp-FW);
    InitArrowG2(HSLx+13, WRUy-r-2*FW-sp);
    // Arrow 2
    InitArrowL2(HSLx+13+FW, WRUy-r-3*FW-sp);
    // Arrow 3
    InitArrowL2(HSLx+13+2*FW, WRUy-r-3*FW-sp);
    // Arrow 4
    InitArrowL2(HSLx+13+3*FW, WRUy-r-3*FW-sp);
//=============================Bahn arrow End===================================
    // Hamburgerstrasse right side 
    // right Boundary
    SetPen(Schwarz, 2);
    MoveTo(HSRx, HSRy);
    LineTo(HSRx, RRUy-r);
    // bai se biao shi xian, white color line
    SetPen(Weiss, 2);
    for(int i = HSRy; i < (WRUy-r); i += 20) {
        MoveTo(HSRx-FW, i);
        if(WRUy-r-i >= 15)
            LineTo(HSRx-FW, i+15);
        else
            LineTo(HSRx-FW, WRUy-r);
    }

//==============================================================================
//   Wendenring 
//==============================================================================
    //upside Boundary
    SetPen(Schwarz, 2);
    MoveTo(WRUx, WRUy);
    LineTo(HSLx-r, WRUy);
    // arc  Boundary
    Arc(HSLx-r, WRUy-r, 270, 0, r);
    // bai se biao shi xian, white color line
    for(int i = WRUx; i <= (HSLx-r); i += 25) {
        SetPen(Weiss, 2);
        MoveTo(i, WRUy+FW);
        if(HSLx-r-i >= 15)
            LineTo(i+15, WRUy+FW);
        else
            LineTo(HSLx-r-FW, WRUy+FW);
        }
    for(int j = WRUy+2*FW; j<WRUy+4*FW; j+=FW){
        for(int i = WRUx; i <= HSLx-r-150; i += 25) {
            SetPen(Weiss, 2);
            MoveTo(i, j);
            if(HSLx-r-150-i >= 15)
                LineTo(i+15, j);
            else
                LineTo(HSLx-r-150, j);
        }
    }
    // Busstation
    SetPen(Schwarz);
    SetBrush(Gelb);
    Rectangle(HSLx-r, WRUy+2*FW, -150, FW);
    //isolation belt
    SetPen(Schwarz,1);
    SetBrush(Gruen);
    Rectangle(0, GetMaxH()/2-5, HSLx-r, 10);
    
    // Wendenring downside
    // downside Boundary
    SetPen(Schwarz, 2);
    MoveTo(WRDx, WRDy);
    LineTo(HSLx-r, WRDy);
    // arc downside
    Arc(HSLx-r, WRDy+r, 0, 90, r);
    // Stop line
    SetPen(Weiss, 2);
    MoveTo(HSLx-r, WRDy);
    LineTo(HSLx-r, WRDy-5*FW);
    // bai se biao shi xian, white color line
    for(int i = WRDy-FW; i > WRDy-5*FW; i-=FW) {
        for(int j = WRDx; j < (HSLx-r); j += 25) {
            SetPen(Weiss, 2);
            MoveTo(j, i);
            if(HSLx-r-j>=15)
                LineTo(j+15, i);
            else
                LineTo(HSLx-r, i);
        }
    }
//=================================Bahn arrow===================================
    // Arrow 1
    InitArrowR3(LSLx-r-3*FW-sp, WRDy-13);
    InitArrowR3(LSLx-r-2*FW-sp, WRDy-13);
    // Arrow 2
    InitArrowG3(LSLx-r-3*FW-sp, WRDy-13-FW);
    // Arrow 3
    InitArrowG3(LSLx-r-3*FW-sp, WRDy-13-2*FW);
    // Arrow 4
    InitArrowL3(LSLx-r-3*FW-sp, WRDy-13-3*FW);
    // Arrow 5
    InitArrowL3(LSLx-r-3*FW-sp, WRDy-13-4*FW);

//=============================Bahn arrow End===================================

//==============================================================================
//   Lampestrasse 
//==============================================================================
    //left side left boundary
    SetPen(Schwarz, 2);
    MoveTo(LSLx, LSLy);
    LineTo(LSLx, WRDy+r);
    // Fahrbahn right boundary
    MoveTo(LSLx+FW, LSLy);
    LineTo(LSLx+FW, WRDy+r);
    LineTo(LSLx+FW+sp, WRDy+r);
    // TramStation
    SetPen(Schwarz,2);
    SetBrush(Gelb);
    Rectangle(LSLx+FW+sp, WRDy+r, 3*FW-sp+1, 8*FW);
    MoveTo(LSLx+4*FW, GetMaxH());
    LineTo(LSLx+4*FW, GetMaxH()-5*FW-sp);

    // right boundary
    SetPen(Schwarz, 2);
    MoveTo(LSRx, LSRy);
    LineTo(LSRx, WRDy+r);
    // Fahrbahn left boundary
    MoveTo(LSRx-FW, LSRy);
    LineTo(LSRx-FW, WRDy+r);
    LineTo(LSRx-FW-sp, WRDy+r);
    // Stop line
    SetPen(Weiss,2);
    MoveTo(LSRx, WRDy+r);
    LineTo(HSRx-FW, WRDy+r);
//=================================Bahn arrow===================================
    // Arrow 1
    InitArrowR4(LSRx-13, RRDy+r+3*FW+sp);
//=================================Bahn arrow===================================
    // TramStation
    SetPen(Schwarz,2);
    SetBrush(Gelb);
    Rectangle(LSRx-FW-sp, RRDy+r, -(3*FW-sp), 8*FW);
    MoveTo(LSRx-4*FW, GetMaxH());
    LineTo(LSRx-4*FW, GetMaxH()-5*FW-sp);    
//==============================================================================

//      Rebenring
//==============================================================================
    // upside boundary
    SetPen(Schwarz, 2);
    MoveTo(RRUx, RRUy);
    LineTo(HSRx+r, RRUy);
    // arc boundary
    Arc(HSRx+r, RRUy-r, 180, 270, r);
    // Stop line
    SetPen(Weiss, 2);
    MoveTo(HSRx+r, RRUy);
    LineTo(HSRx+r, RRUy+5*FW);
    // bai se biao shi xian, white color line
    for(int i = RRUy+FW; i<RRUy+5*FW; i+=FW) {
        for(int j = RRUx; j > (HSRx+r); j -= 20) {
            SetPen(Weiss, 2);
            MoveTo(j, i);
            if(j-(HSRx+r) >= 15)
                LineTo(j-15, i);
            else
                LineTo(HSRx+r, i);
        }
    }
//=================================Bahn arrow===================================
    // Arrow 1
    InitArrowR1(HSRx+r+3*FW+sp, RRUy+13);
    //Arrow 2
    InitArrowR1(HSRx+r+3*FW+sp, RRUy+13+FW);
    //Arrow 3
    InitArrowG1(HSRx+r+3*FW+sp, RRUy+13+2*FW);
    //Arrow 4
    InitArrowG1(HSRx+r+3*FW+sp, RRUy+13+3*FW);
    //Arrow 5
    InitArrowL1(HSRx+r+3*FW+sp, RRUy+13+4*FW);
//=================================Bahn arrow===================================
    // Isolation belt, Renbenring
    SetPen(Schwarz,1);
    SetBrush(Gruen);
    Rectangle(RRUx, RRUy+5*FW, -(RRUx-r-HSRx), sp);

    // Rebenring downside
    // downside boundary
    SetPen(Schwarz, 2);
    MoveTo(RRDx, RRDy);
    LineTo(HSRx+r, RRDy);
    // arc boundary
    Arc(HSRx+r, RRDy+r, 90, 180, r);
    
    // Busstation
    SetPen(Schwarz, 1);
    SetBrush(Gelb);
    Rectangle(LSRx+r, RRDy, 150, FW);
    // bai se biao shi xian, white color line
    for(int i = RRDy-FW; i > RRDy-3*FW; i-=FW) {
        for(int j = RRDx; j >= (HSRx+r); j -= 25) {
            SetPen(Weiss, 2);
            MoveTo(j, i);
            if(j-(HSRx+r) >= 15)
                LineTo(j-15, i);
            else
                LineTo(HSRx+r, i);
        }
    }     
}   

void TUser::InitRadweg() {
    // Hamburger left
    SetPen(Schwarz, 1);
    MoveTo(HSLx-sp, HSLy);
    LineTo(HSLx-sp, WRUy-sp-RW-GW);     //WRUy-sp-RW-GW
    MoveTo(HSLx-sp-RW, HSLy);
    LineTo(HSLx-sp-RW, WRUy-sp-RW-GW); //WRUy-sp-RW
    // Hamburger right
    MoveTo(HSRx+sp, HSRy);
    LineTo(HSRx+sp, RRUy-sp-RW-GW);   //RRUy-sp-RW-GW
    MoveTo(HSRx+sp+RW, HSRy);
    LineTo(HSRx+sp+RW, RRUy-sp-RW-GW);  //RRUy-sp-RW

    // Rebenring upside
    SetPen(Schwarz,1);
    MoveTo(HSRx+sp+RW+GW, RRUy-sp);   // HSRx+sp+RW+GW
    LineTo(GetMaxW(), RRUy-sp);
    MoveTo(HSRx+sp+RW+GW, RRUy-sp-RW); //HSRx+sp+RW
    LineTo(GetMaxW(), RRUy-sp-RW);
    // Downside
    MoveTo(HSRx+sp+RW+GW, RRDy+FW);  //HSRx+sp+RW +GW
    LineTo(GetMaxW(), RRDy+FW);
    MoveTo(HSRx+sp+RW+GW, RRDy+FW+RW); //HSRx+sp+RW
    LineTo(GetMaxW(), RRDy+FW+RW);

    // Wendenring upside
    SetPen(Schwarz,1);
    MoveTo(WRUx, WRUy-sp);
    LineTo(HSLx-sp-RW-GW, WRUy-sp);   // HSLx-sp-RW-GW
    MoveTo(WRUx, WRUy-sp-RW);
    LineTo(HSLx-sp-RW-GW, WRUy-sp-RW); // HSLx-sp-RW
    // Wendenring downside
    MoveTo(WRDx, WRDy+sp);
    LineTo(LSLx-sp-RW-GW, WRDy+sp);  // LSLx-sp-RW-GW
    MoveTo(WRDx, WRDy+sp+RW);
    LineTo(LSLx-sp-RW-GW, WRDy+sp+RW); //LSLx-sp-RW
    
    //Lampestrasse
    // left
    SetPen(Schwarz,1);
    MoveTo(LSLx-sp, WRDy+sp+RW+GW);  //+RW+GW
    LineTo(LSLx-sp, GetMaxH());
    MoveTo(LSLx-sp-RW, WRDy+sp+RW+GW);//WRDy+sp+RW
    LineTo(LSLx-sp-RW, GetMaxH());
    //right
    MoveTo(LSRx+sp, RRDy+FW+RW+GW); //RRDy+FW+RW+GW
    LineTo(LSRx+sp, GetMaxH());
    MoveTo(LSRx+sp+RW, RRDy+FW+RW+GW); //RRDy+FW+RW
    LineTo(LSRx+sp+RW, GetMaxH());
}

// Radweg auf der Rebenring
void TUser::RadwegRR(int ampel, bool rad){
    // verbinden
    switch(ampel){  // Rot-0; Gruen  als default
        case 0:
            SetPen(Schwarz,1);
            MoveTo(HSRx+sp+RW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(HSRx+sp, RRUy-sp-RW-GW);
            LineTo(LSRx+sp, RRDy+FW+RW+GW);
            break;
        default:
            if(rad){   // Im Ragweg gibt es noch Fahrad.
                SetPen(Rot,2);
            }
            else {
                SetPen(Schwarz,1);
            }
            MoveTo(HSRx+sp+RW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(HSRx+sp, RRUy-sp-RW-GW);
            LineTo(LSRx+sp, RRDy+FW+RW+GW);
    }

}
// Radweg auf der Hamburgerstrasse
void TUser::RadwegHS(int Ampel, bool rad){
    // verbinden
    switch(Ampel){
        case 0:
            SetPen(Schwarz,1);
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);
            LineTo(HSRx+sp+RW, RRUy-sp-RW);
            MoveTo(HSLx-sp-RW-GW, WRUy-sp);
            LineTo(HSRx+sp+RW+GW, RRUy-sp);
            break;
        default:
            if(rad){
                SetPen(Rot,2);
            }
            else {
                SetPen(Schwarz,1);
            }
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);
            LineTo(HSRx+sp+RW, RRUy-sp-RW);
            MoveTo(HSLx-sp-RW-GW, WRUy-sp);
            LineTo(HSRx+sp+RW+GW, RRUy-sp);
    }
}
// Radweg auf der Wendenring
void TUser::RadwegWR(int ampel, bool rad){
    // verbinden
    switch(ampel){
        case 0:
            SetPen(Schwarz,1);
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);
            LineTo(LSLx-sp-RW, WRDy+sp+RW);
            MoveTo(HSLx-sp, WRUy-sp-RW-GW);
            LineTo(LSLx-sp, WRDy+sp+RW+GW);
            break;
        default:
            if(rad){
                SetPen(Rot,2);
            }
            else {
                SetPen(Schwarz,1);
            }
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);
            LineTo(LSLx-sp-RW, WRDy+sp+RW);
            MoveTo(HSLx-sp, WRUy-sp-RW-GW);
            LineTo(LSLx-sp, WRDy+sp+RW+GW);
    }
}
// Radweg auf der Lampestrasse
void TUser::RadwegLS(int Ampel, bool rad){
    // verbinden
    switch(Ampel){
        case 0:
            SetPen(Schwarz,1);
            MoveTo(LSLx-sp-RW, WRDy+sp+RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(LSLx-sp-RW-GW, WRDy+sp);
            LineTo(LSRx+sp+RW+GW, RRDy+FW);
            break;
        default:
            if(rad){
                SetPen(Rot,2);
            }
            else {
                SetPen(Schwarz,1);
            }
            MoveTo(LSLx-sp-RW, WRDy+sp+RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(LSLx-sp-RW-GW, WRDy+sp);
            LineTo(LSRx+sp+RW+GW, RRDy+FW);
    }
}

void TUser::InitGehweg() {
    // Hamburger left
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(HSLx-sp-RW-GW, HSLy, GW, WRUy-sp-RW);
    SetPen(Schwarz, 2);
    MoveTo(HSLx-sp-RW-GW, HSLy);
    LineTo(HSLx-sp-RW-GW, WRUy-sp-RW-GW);
    // Hamburger right
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(HSRx+sp+RW, HSLy, GW, RRUy-sp-RW);
    SetPen(Schwarz, 2);
    MoveTo(HSRx+sp+RW+GW, HSLy);
    LineTo(HSRx+sp+RW+GW, RRUy-sp-RW-GW);

    // Wendenring upside
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(WRUx, WRUy-sp-RW-GW, HSLx-sp-RW, GW);
    SetPen(Schwarz, 2);
    MoveTo(WRUx, WRUy-sp-RW-GW);
    LineTo(HSLx-sp-RW-GW, WRUy-sp-RW-GW);
    // Wendenring downside
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(WRDx, WRDy+sp+RW, LSLx-sp-RW, GW);
    SetPen(Schwarz, 2);
    MoveTo(WRDx, WRDy+sp+RW+GW);
    LineTo(LSLx-sp-RW-GW, WRDy+sp+RW+GW);
    
    // Lampestrasse left
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(LSLx-sp-RW, LSLy, -GW, -(LSLy-(WRDy+sp+RW)));
    SetPen(Schwarz, 2);
    MoveTo(LSLx-sp-RW-GW, LSLy);
    LineTo(LSLx-sp-RW-GW, WRDy+sp+RW+GW);
    // Lampestrasse right
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(LSRx+sp+RW, RRDy+FW+RW, GW, LSRy-(RRDy+FW+RW));
    SetPen(Schwarz, 2);
    MoveTo(LSRx+sp+RW+GW, RRDy+FW+RW+GW);
    LineTo(LSRx+sp+RW+GW, LSRy);

    // Rebenring upside
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(HSRx+sp+RW, RRUy-sp-RW-GW, RRUx-(HSRx+sp+RW), GW);
    SetPen(Schwarz, 2);
    MoveTo(HSRx+sp+RW+GW, RRUy-sp-RW-GW);
    LineTo(RRUx, RRUy-sp-RW-GW);
    // Rebenring downside
    SetPen(Hellgrau,1);
    SetBrush(Hellgrau);
    Rectangle(LSRx+sp+RW, RRDy+FW+RW, RRDx-(LSRx+sp), GW);
    SetPen(Schwarz, 2);
    MoveTo(LSRx+sp+RW+GW, RRDy+FW+RW+GW);
    LineTo(RRDx, RRDy+FW+RW+GW);
}

// Gehweg auf der Rebenring
void TUser::GehwegRR(int ampelPass, bool passant){
    ClearPoints();
    SetPoint(HSRx+sp+RW, RRUy-sp-RW);
    SetPoint(HSRx+sp+RW+GW, RRUy-sp-RW);
    SetPoint(LSRx+sp+RW+GW, RRDy+FW+RW);
    SetPoint(LSRx+sp+RW, RRDy+FW+RW);
    SetPenColor(Hellgrau);
    SetBrushColor(Hellgrau);
    Poly();
    // verbinden
    switch(ampelPass) {
        case 0:
            SetPen(Weiss,1);
            MoveTo(HSRx+sp+RW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(HSRx+sp+RW+GW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW+GW, RRDy+FW+RW);
            break;
        default:
            if(passant){
                SetPen(Rot,2);
            }
            else {
                SetPen(Weiss,1);
            }
            MoveTo(HSRx+sp+RW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW, RRDy+FW+RW);
            MoveTo(HSRx+sp+RW+GW, RRUy-sp-RW);
            LineTo(LSRx+sp+RW+GW, RRDy+FW+RW);
    }
}
// Gehweg auf der Hamburgerstrasse
void TUser::GehwegHS(int ampelPass, bool passant){
    ClearPoints();
    SetPoint(HSLx-sp-RW, WRUy-sp-RW-GW);
    SetPoint(HSRx+sp+RW, RRUy-sp-RW-GW);
    SetPoint(HSRx+sp+RW, RRUy-sp-RW);
    SetPoint(HSLx-sp-RW, WRUy-sp-RW);
    SetPenColor(Hellgrau);
    SetBrushColor(Hellgrau);
    Poly();
    // verbinden
    switch(ampelPass) {
        case 0:
            SetPen(Weiss,1);
            MoveTo(HSLx-sp-RW, WRUy-sp-RW-GW);   // up
            LineTo(HSRx+sp+RW, RRUy-sp-RW-GW);
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);   // down
            LineTo(HSRx+sp+RW, RRUy-sp-RW);
            break;
        default:              
            if(passant){
                SetPen(Rot,2);
            }
            else {
                SetPen(Weiss,1);
            }
            MoveTo(HSLx-sp-RW, WRUy-sp-RW-GW);   // up
            LineTo(HSRx+sp+RW, RRUy-sp-RW-GW);
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);   // down
            LineTo(HSRx+sp+RW, RRUy-sp-RW);
    }                                        
}
// Gehweg auf der Wendenring
void TUser::GehwegWR(int ampelPass, bool passant){
    ClearPoints();
    SetPoint(HSLx-sp-RW-GW, WRUy-sp-RW);
    SetPoint(LSLx-sp-RW-GW, WRDy+sp+RW);
    SetPoint(LSLx-sp-RW, WRDy+sp+RW);
    SetPoint(HSLx-sp-RW, WRUy-sp-RW);
    SetPenColor(Hellgrau);
    SetBrushColor(Hellgrau);
    Poly();
    // verbinden
    switch(ampelPass) {
        case 0:
            SetPen(Weiss,1);
            MoveTo(HSLx-sp-RW-GW, WRUy-sp-RW);    //left  down
            LineTo(LSLx-sp-RW-GW, WRDy+sp+RW);    //left up
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);    //right  down
            LineTo(LSLx-sp-RW, WRDy+sp+RW);    //right up    
            break;
        default:
            if(passant){
                SetPen(Rot,2);
            }
            else {
                SetPen(Weiss,1);
            }
            MoveTo(HSLx-sp-RW-GW, WRUy-sp-RW);    //left  down
            LineTo(LSLx-sp-RW-GW, WRDy+sp+RW);    //left up
            MoveTo(HSLx-sp-RW, WRUy-sp-RW);    //right  down
            LineTo(LSLx-sp-RW, WRDy+sp+RW);     //right up
    }
}
// Gehweg auf der Lampestrasse
void TUser::GehwegLS(int ampelPass, bool passant){
    ClearPoints();
    SetPoint(LSLx-sp-RW, WRDy+sp+RW+GW);
    SetPoint(LSRx+sp+RW, RRDy+FW+RW+GW);
    SetPoint(LSRx+sp+RW, RRDy+FW+RW);
    SetPoint(LSLx-sp-RW, WRDy+sp+RW);
    SetPenColor(Hellgrau);
    SetBrushColor(Hellgrau);
    Poly();
    // verbinden
    switch(ampelPass) {
        case 0:
            SetPen(Weiss,1);
            MoveTo(LSLx-sp-RW, WRDy+sp+RW+GW);   // down left
            LineTo(LSRx+sp+RW, RRDy+FW+RW+GW);   // down right
            MoveTo(LSLx-sp-RW, WRDy+sp+RW);   // up left
            LineTo(LSRx+sp+RW, RRDy+FW+RW);   // up right
            break;
        default:
            if(passant){
                SetPen(Rot, 2);
            }
            else {
                SetPen(Weiss,1);
            }
            MoveTo(LSLx-sp-RW, WRDy+sp+RW+GW);   // down left
            LineTo(LSRx+sp+RW, RRDy+FW+RW+GW);   // down right
            MoveTo(LSLx-sp-RW, WRDy+sp+RW);   // up left
            LineTo(LSRx+sp+RW, RRDy+FW+RW);   // up right
    }
}

void TUser::RunTaste0() {
    if (!tramNachUnten && !tramAuto) {
        tramNachUnten = true;
    }
}

void TUser::RunTaste1() {
    if (!tramNachOben && !tramAuto) {
        tramNachOben = true;
    }
}

void TUser::RunTaste4() {
    switch (choose){
        case 1: acc++; break;
        case 2: VMax += 2; break;
        case 3: numOfSettingCar += 0.1; break;
        case 4: plot0bool += 1; plot0bool %= 4; break;
        case 5: plot1bool += 1; plot1bool %= 4; break;
    }
}

void TUser::RunTaste5() {
    switch (choose){
        case 1: acc--; if(acc<1) acc = 1; break;
        case 2: VMax -= 2; if(VMax<5) VMax = 5; break;
        case 3: numOfSettingCar -= 0.1; if(numOfSettingCar<0.1) numOfSettingCar = 0.1; break;
        case 4: plot0bool -= 1; if(plot0bool<0) plot0bool += 4; break;
        case 5: plot1bool -= 1; if(plot1bool<0) plot1bool += 4; break;
    }
}

void TUser::RunTaste6() {

        choose = 1;   
}

void TUser::RunTaste7() {

        choose = 2;
}
void TUser::RunTaste8() {
        choose = 3;         
}

void TUser::RunTaste9() {
    SpurOn = !SpurOn;
    InsertTaste(9,(SpurOn)?"auto(Tram)":"manual(Tram)", "Ein Tram wird in jedem Zyklus erzeugt.");   
    tramAuto = !tramAuto;      
}

void TUser::RunTaste12() {
    plotVis = !plotVis;
    Plot0->Visible=plotVis;
    Plot1->Visible=plotVis;
    
}
void TUser::RunTaste13() {
    AK_ampel = !AK_ampel;
    InsertTaste(13,(AK_ampel)?"adpt. Ample":"Konst. Ampel", "Adaptive AmpelController");
}

void TUser::RunTaste14() {
    choose = 4;
    
}
void TUser::RunTaste15() {
    choose = 5;
}   

void TUser::RunTaste16() {
    Reset();
    fre1 = 3.0;    // HS2RR
    fre2 = 0.5;     // HS2WR
    fre3 = 20.0;    // WR2HS
    fre4 = 0.5;     // WR2LS
    fre5 = 0.1;     // LS2RR
    fre6 = 20.0;    // RR2HS
    fre7 = 0.2;     // RR2LS
    fre8 = 3.0;     // WR2RR
    fre9 = 3.0;     // RR2WR
}

void TUser::RunTaste17() {
    Reset();
    fre1 = 20.0;    // HS2RR
    fre2 = 0.5;     // HS2WR
    fre3 = 4.0;    // WR2HS
    fre4 = 0.5;     // WR2LS
    fre5 = 0.1;     // LS2RR
    fre6 = 4.0;    // RR2HS
    fre7 = 0.2;     // RR2LS
    fre8 = 3.0;     // WR2RR
    fre9 = 3.0;     // RR2WR
}


void TUser::RunTaste18() {
    sleepTime -= 5;
    if(sleepTime <8) sleepTime = 0;
}  

void TUser::RunTaste19() {
    sleepTime += 5;
}  


//====================================================================
#pragma argsused
int main(int argc, char* argv[]){ TUser a; a.Execute(); return 0; }
//__________________________________________________________Ost08_____








