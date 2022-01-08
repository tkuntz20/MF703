#include <cmath>
#include <vector>
#include <iostream>
#include <cstdio>
#include <iomanip>

// --------------------------part A------------------------------------
double ZeroCouponBond(int years, int facevalue, float YTM, int periods){
   return facevalue / std::pow((1 + (YTM / periods)), years * periods);
}

//---------------------------part B-----------------------------------
double FiniteDifferenceDuration1(int years, int facevalue, float YTM, int periods, float delta){
   float priceDelta_plus0 = ZeroCouponBond(years, facevalue, YTM + delta, periods);
   float priceDelta_minus0 = ZeroCouponBond(years, facevalue, YTM - delta, periods);
   float price0 = ZeroCouponBond(years, facevalue, YTM, periods);
   float duration = (priceDelta_minus0 - priceDelta_plus0) / (2 * price0 * delta);
   return duration;
}

//-----------------------------part C---------------------------------
float CouponBondPrice(int years, int facevalue, float YTM, int periods, float coupon){
   float couponvalue = (coupon * facevalue);
   float power = 1 / (std::pow((1 + (YTM / periods)), (years * periods)));
   float discount = (1 - power) / (YTM / periods);
   float discountedFV = (facevalue / std::pow((1 + (YTM / periods)), years * periods));
   float couponBond = couponvalue * discount + discountedFV;
   return couponBond;
}

//------------------------------Part D-------------------------------
float FiniteDifferenceDuration(int years, int facevalue, float YTM, int periods, float delta, float coupon){
   float priceDelta_plusCPN = CouponBondPrice(years, facevalue, YTM + delta, periods, coupon);
   float priceDelta_minusCPN = CouponBondPrice(years, facevalue, YTM - delta, periods, coupon);
   float priceCPN = CouponBondPrice(years, facevalue, YTM, periods, coupon);
   double duration = (priceDelta_minusCPN - priceDelta_plusCPN) / (2 * priceCPN * delta);

   return duration;
}

//---------------------------Part E---------------------------------
float EffectiveConvexity(int years, int facevalue, float YTM, int periods, float delta, float coupon){
   float priceDelta_plus = CouponBondPrice(years, facevalue, YTM + delta, periods, coupon);
   float priceDelta_minus = CouponBondPrice(years, facevalue, YTM - delta, periods, coupon);
   float price = CouponBondPrice(years, facevalue, YTM, periods, coupon);
   double convexity = (priceDelta_minus + priceDelta_minus - 2 * price) / (2 * price * (std::pow(delta, 2)));
   return convexity * (std::pow(delta, 2)) * 100;
}

float EffectiveConvexityZCPN(int years, int facevalue, float YTM, int periods, float delta){
   float priceDelta_plus = ZeroCouponBond(years, facevalue, YTM + delta, periods);
   float priceDelta_minus = ZeroCouponBond(years, facevalue, YTM - delta, periods);
   float price = ZeroCouponBond(years, facevalue, YTM, periods);
   double convexity = (priceDelta_minus + priceDelta_minus - (2 * price)) / (2 * price * (std::pow(delta, 2)));
   return convexity * (std::pow(delta, 2)) * 100;
}

//---------------------------Part F---------------------------------
float LongShortPortfolio(){
   double oneYear = ZeroCouponBond(1, 100, 0.025, 1);
   double twoYear = ZeroCouponBond(2, 100, 0.026, 1);
   double threeYear = ZeroCouponBond(3, 100, 0.027, 1);
   double portfolio = oneYear + threeYear - (2 * twoYear);
   return portfolio;
}

double PortfolioZeroDuration(double oneYr, double twoYr, double threeYr, double dur1, double dur2, double dur3, double price1, double price2, double price3){
   double portfolioAbsoluteWeight = ((oneYr * price1) + (threeYr * price3) - (twoYr * price2));
   return (((((oneYr * price1) / portfolioAbsoluteWeight) * dur1) + (((threeYr * price3) / portfolioAbsoluteWeight) * dur3)) / (((twoYr * price2) / portfolioAbsoluteWeight) / dur2));
}

double PortDur(double oneYr, double twoYr, double threeYr, double dur1, double dur2, double dur3, double price1, double price2, double price3){
   double portfolioAbsoluteWeight = ((oneYr * price1) + (threeYr * price3) - (twoYr * price2));
   return ((((oneYr * price1) / portfolioAbsoluteWeight) * dur1) + (((threeYr * price3) / portfolioAbsoluteWeight) * dur3) - (((twoYr * price2) / portfolioAbsoluteWeight) / dur2));
}

float PortfolioConvexity(double oneYr, double twoYr, double threeYr, float delta){
   double oneYearCvx = EffectiveConvexityZCPN(1, 100, 0.025, 1, delta);
   double twoYearCvx = EffectiveConvexityZCPN(2, 100, 0.026, 1, delta);
   double threeYearCvx = EffectiveConvexityZCPN(3, 100, 0.027, 1, delta);
   double oneYear = ZeroCouponBond(1, 100, 0.025, 1);
   double twoYear = ZeroCouponBond(2, 100, 0.026, 1);
   double threeYear = ZeroCouponBond(3, 100, 0.027, 1);
   double PortfolioCvx = ((((oneYr * oneYear) / LongShortPortfolio()) * oneYearCvx) + (((threeYr * threeYear) / LongShortPortfolio()) * threeYearCvx) - (((twoYr * twoYear) / LongShortPortfolio()) * twoYearCvx));
   return PortfolioCvx * (std::pow(delta, 2)) * 100;
}

float PortfolioShockPlus100(double oneYr, double twoYr, double threeYr, double delta){
   double oneYear = ZeroCouponBond(1, 100, (0.025 + delta), 1);
   //std::cout << oneYear << std::endl;
   double twoYear = ZeroCouponBond(2, 100, (0.026 + delta), 1);
   //std::cout << twoYear << std::endl;
   double threeYear = ZeroCouponBond(3, 100, (0.027 + delta), 1);
   //std::cout << threeYear << std::endl;
   double portfolio = oneYear + threeYear - (twoYr * twoYear);
   //std::cout << - (twoYr * twoYear) << std::endl;
   return portfolio;
}

//------------------------------------------------- part K --------------------------------------------------
double FiveYrCashFLows(int years, double facevalue, float YTM, int periods, double coupon, double principal){
   double PrincipalPayment = principal * facevalue;
   double CouponPayment = coupon * facevalue;
   double BondPrice = 0.0;
   for (int n = 1; n <= years * periods; ++n){
      std::cout << std::fixed;
      std::cout << std::setprecision(2);
      BondPrice += (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n);
      //std::cout << BondPrice << std::endl;
      if (n < 10)
         std::cout << "  " << n << "    |     " << CouponPayment + PrincipalPayment << "       |    " << (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n) << "         |      " << facevalue - ((PrincipalPayment / periods) * n) << std::endl;
      else
         std::cout << "  " << n << "    |     " << CouponPayment + PrincipalPayment << "      |    " << (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n) << "         |      " << facevalue - ((PrincipalPayment / periods) * n) << std::endl;
      CouponPayment = coupon * (facevalue - ((PrincipalPayment / (periods) * n)));  //take this out if the calcuation is messed up
   }
   return BondPrice;
}

double FiveYrDelta(int years, double facevalue, float YTM, int periods, double coupon, double principal){
   double PrincipalPayment = principal * facevalue;
   double CouponPayment = coupon * facevalue;
   double BondPrice = 0.0;
   for (int n = 1; n <= years * periods; ++n){
      BondPrice += (PrincipalPayment + (CouponPayment / periods)) / std::pow((1 + YTM / periods), n);
   }

   return BondPrice;
}

float FiveYrDuration(int years, double facevalue, float YTM, int periods, double coupon, double principal, double delta){
   float priceDelta_plusCPN = FiveYrDelta(years, facevalue, YTM + delta, periods, coupon, principal);
   float priceDelta_minusCPN = FiveYrDelta(years, facevalue, YTM - delta, periods, coupon, principal);
   float priceCPN = FiveYrDelta(years, facevalue, YTM, periods, coupon, principal);
   double duration = (priceDelta_minusCPN - priceDelta_plusCPN) / (2 * priceCPN * delta);

   return duration;
}

int main(){
   //----------------------------------------beginning part A-----------------------------------------------
   std::cout << "---------------------------- Zero coupon bond values (A) ---------------------------------- \n"
             << std::endl;

   float OneYrZcpn = ZeroCouponBond(1, 100, 0.025, 1);
   float TwoYrZcpn = ZeroCouponBond(2, 100, 0.026, 1);
   float ThreeYrZcpn = ZeroCouponBond(3, 100, 0.027, 1);
   float FiveYrZcpn = ZeroCouponBond(5, 100, 0.03, 1);
   float TenYrZcpn = ZeroCouponBond(10, 100, 0.035, 1);
   float ThrityYrZcpn = ZeroCouponBond(30, 100, 0.04, 1);
   std::cout << " Maturity |  Price " << std::endl;
   std::cout << "-------------------" << std::endl;
   std::cout << "   1yr    | " << OneYrZcpn << std::endl;
   std::cout << "   2yr    | " << TwoYrZcpn << std::endl;
   std::cout << "   3yr    | " << ThreeYrZcpn << std::endl;
   std::cout << "   5yr    | " << FiveYrZcpn << std::endl;
   std::cout << "   10yr   | " << TenYrZcpn << std::endl;
   std::cout << "   30yr   | " << ThrityYrZcpn << std::endl;
   std::cout << std::endl;
   //-----------------------------------------end part A---------------------------------------------------

   //---------------------------------------beginning part B-----------------------------------------------
   std::cout << "------------------------ Zero coupon bond durations for a 100bps delta (B) ----------------------- \n"
             << std::endl;
   float Duration1 = FiniteDifferenceDuration1(1, 100, 0.025, 1, 0.01);
   float Duration2 = FiniteDifferenceDuration1(2, 100, 0.026, 1, 0.01);
   float Duration3 = FiniteDifferenceDuration1(3, 100, 0.027, 1, 0.01);
   float Duration5 = FiniteDifferenceDuration1(5, 100, 0.03, 1, 0.01);
   float Duration10 = FiniteDifferenceDuration1(10, 100, 0.035, 1, 0.01);
   float Duration30 = FiniteDifferenceDuration1(30, 100, 0.04, 1, 0.01);
   std::cout << " Maturity |  Duration " << std::endl;
   std::cout << "----------------------" << std::endl;
   std::cout << "   1yr    |  " << Duration1 << std::endl;
   std::cout << "   2yr    |  " << Duration2 << std::endl;
   std::cout << "   3yr    |  " << Duration3 << std::endl;
   std::cout << "   5yr    |  " << Duration5 << std::endl;
   std::cout << "   10yr   |  " << Duration10 << std::endl;
   std::cout << "   30yr   |  " << Duration30 << std::endl;
   std::cout << std::endl;
   //----------------------------------------------end part B---------------------------------------------

   //--------------------------------------------beginnning part C----------------------------------------
   std::cout << "------------------------- Coupon bond values (C) ------------------------------ \n"
             << std::endl;
   float CpnBond1 = CouponBondPrice(1, 100, 0.025, 1, 0.03);
   float CpnBond2 = CouponBondPrice(2, 100, 0.026, 1, 0.03);
   float CpnBond3 = CouponBondPrice(3, 100, 0.027, 1, 0.03);
   float CpnBond5 = CouponBondPrice(5, 100, 0.03, 1, 0.03);
   float CpnBond10 = CouponBondPrice(10, 100, 0.035, 1, 0.03);
   float CpnBond30 = CouponBondPrice(30, 100, 0.04, 1, 0.03);
   std::cout << " Maturity |  Coupon Bond Price " << std::endl;
   std::cout << "-------------------------------" << std::endl;
   std::cout << "   1yr    |     " << CpnBond1 << std::endl;
   std::cout << "   2yr    |     " << CpnBond2 << std::endl;
   std::cout << "   3yr    |     " << CpnBond3 << std::endl;
   std::cout << "   5yr    |     " << CpnBond5 << std::endl;
   std::cout << "   10yr   |     " << CpnBond10 << std::endl;
   std::cout << "   30yr   |     " << CpnBond30 << std::endl;
   std::cout << std::endl;
   //-------------------------------------------end part C----------------------------------------------

   //-----------------------------------------beginning part D ------------------------------------------
   std::cout << "-------------------------- Coupon bond durations for a 100bps delta (D) -------------------------- \n"
             << std::endl;
   float Duration11 = FiniteDifferenceDuration(1, 100, 0.025, 1, 0.01, 0.03);
   float Duration22 = FiniteDifferenceDuration(2, 100, 0.026, 1, 0.01, 0.03);
   float Duration33 = FiniteDifferenceDuration(3, 100, 0.027, 1, 0.01, 0.03);
   float Duration55 = FiniteDifferenceDuration(5, 100, 0.03, 1, 0.01, 0.03);
   float Duration1010 = FiniteDifferenceDuration(10, 100, 0.035, 1, 0.01, 0.03);
   float Duration3030 = FiniteDifferenceDuration(30, 100, 0.04, 1, 0.01, 0.03);
   std::cout << " Maturity |  Coupon Bond Duration " << std::endl;
   std::cout << "---------------------------------" << std::endl;
   std::cout << "   1yr    |     " << Duration11 << std::endl;
   std::cout << "   2yr    |     " << Duration22 << std::endl;
   std::cout << "   3yr    |     " << Duration33 << std::endl;
   std::cout << "   5yr    |     " << Duration55 << std::endl;
   std::cout << "   10yr   |     " << Duration1010 << std::endl;
   std::cout << "   30yr   |     " << Duration3030 << std::endl;
   std::cout << std::endl;
   //-----------------------------------------end part D -----------------------------------------------

   //---------------------------------------beginning part E -------------------------------------------
   std::cout << "-------------------------Coupon bond convexity for a 100bps delta (E) ------------------------ \n"
             << std::endl;
   float convexity1 = EffectiveConvexity(1, 100, 0.025, 1, 0.01, 0.03);
   float convexity2 = EffectiveConvexity(2, 100, 0.026, 1, 0.01, 0.03);
   float convexity3 = EffectiveConvexity(3, 100, 0.027, 1, 0.01, 0.03);
   float convexity5 = EffectiveConvexity(5, 100, 0.03, 1, 0.01, 0.03);
   float convexity10 = EffectiveConvexity(10, 100, 0.035, 1, 0.01, 0.03);
   float convexity30 = EffectiveConvexity(30, 100, 0.04, 1, 0.01, 0.03);
   float ZCPNconvexity1 = EffectiveConvexityZCPN(1, 100, 0.025, 1, 0.01);
   float ZCPNconvexity2 = EffectiveConvexityZCPN(2, 100, 0.026, 1, 0.01);
   float ZCPNconvexity3 = EffectiveConvexityZCPN(3, 100, 0.027, 1, 0.01);
   float ZCPNconvexity5 = EffectiveConvexityZCPN(5, 100, 0.03, 1, 0.01);
   float ZCPNconvexity10 = EffectiveConvexityZCPN(10, 100, 0.035, 1, 0.01);
   float ZCPNconvexity30 = EffectiveConvexityZCPN(30, 100, 0.04, 1, 0.01);
   std::cout << " Maturity |  coupon Convexity | ZCPN convexity " << std::endl;
   std::cout << "------------------------------------------------" << std::endl;
   std::cout << "   1yr    |  " << convexity1 << "       " << ZCPNconvexity1 << std::endl;
   std::cout << "   2yr    |  " << convexity2 << "       " << ZCPNconvexity2 << std::endl;
   std::cout << "   3yr    |  " << convexity3 << "       " << ZCPNconvexity3 << std::endl;
   std::cout << "   5yr    |  " << convexity5 << "       " << ZCPNconvexity5 << std::endl;
   std::cout << "   10yr   |  " << convexity10 << "        " << ZCPNconvexity10 << std::endl;
   std::cout << "   30yr   |  " << convexity30 << "       " << ZCPNconvexity30 << std::endl;
   std::cout << std::endl;
   //---------------------------------------end part E ----------------------------------------------

   //---------------------------------------Beginning part F ----------------------------------------
   std::cout << "-------------------------------Long-Short Portfolio (F) ------------------------------- \n" << std::endl;
   float initailPortfolio = LongShortPortfolio();
   std::cout << "Portfolio Inital Value :  " << initailPortfolio << std::endl;
   std::cout << std::endl;
   //-----------------------------------------end part F --------------------------------------------

   //----------------------------------------Beginning part G ---------------------------------------
   std::cout << "--------------------------------Long-Short Portfolio duration (G) ------------------------------- \n" << std::endl;
   double portfolioDur = PortDur(1, 2, 1, Duration1, Duration2, Duration3, OneYrZcpn, TwoYrZcpn, ThreeYrZcpn);
   std::cout << "Correct Portfolio Duration :  " << portfolioDur << std::endl;
   std::cout << std::endl;

   std::cout << "---------------------------------Long-Short Portfolio convexity (G) ----------------------------------- \n" << std::endl;
   float PortConvexity = PortfolioConvexity(1, 2, 1, 0.01);
   std::cout << "Portfolio Convexity :  " << PortConvexity << std::endl;
   std::cout << std::endl;
   //-----------------------------------------------end part G ---------------------------------------

   //---------------------------------------------Beginning part H -----------------------------------
   std::cout << "------------------------Long-Short Portfolio rebalnace to Zero Duration (H) -------------------------\n" << std::endl;
   double PortfolioZeroDurRebalance = PortfolioZeroDuration(1, 2, 1, Duration1, Duration2, Duration3, OneYrZcpn, TwoYrZcpn, ThreeYrZcpn);
   std::cout << "Rebalanced Portfolio to Zero Duration Adjustmant : " << PortfolioZeroDurRebalance << std::endl;
   std::cout << std::endl;

   std::cout << "------------------Long-Short Portfolio adjusted duration------------------- \n" << std::endl;
   float AdjPortDuration = PortDur(1, (2 * PortfolioZeroDurRebalance), 1, Duration1, Duration2, Duration3, OneYrZcpn, TwoYrZcpn, ThreeYrZcpn);
   std::cout << "Adjusted Portfolio Duration :  " << AdjPortDuration << std::endl;
   //std::cout << "portfolio adjustment :  " << ((OneYrZcpn + ThreeYrZcpn) - (2 * TwoYrZcpn)) << "  " << (OneYrZcpn + ThreeYrZcpn) / 2 << "  " << ThreeYrZcpn << "  " << TwoYrZcpn << std::endl;
   std::cout << std::endl;
   //---------------------------------------------------end part H ------------------------------------

   //------------------------------------------------Beginning part I ---------------------------------
   std::cout << "------------------Long-Short Portfolio------------------- \n" << std::endl;
   float shockValuePlus = PortfolioShockPlus100(1, (2 * PortfolioZeroDurRebalance), 1, 0.01);
   std::cout << "Portfolio Value After Shock Plus :  " << shockValuePlus << std::endl;
   std::cout << std::endl;
   //---------------------------------------------------end part I ------------------------------------

   //------------------------------------------------Beginning part J ---------------------------------
   std::cout << "------------------Long-Short Portfolio------------------- \n" << std::endl;
   float shockValueMinus = PortfolioShockPlus100(1, (2 * PortfolioZeroDurRebalance), 1, -0.01);
   std::cout << "Portfolio Value After Shock Minus :  " << shockValueMinus << std::endl;
   std::cout << std::endl;
   //---------------------------------------------------end part J ------------------------------------

   //------------------------------------------------Beginning part K ---------------------------------
   std::cout << "-------------------------------------5 Year Bond cash flows (K) ---------------------------------------" << std::endl;
   std::cout << std::endl;
   std::cout << " Year  | Scheduled Value | Discounted Value | Remaining Pricipal " << std::endl;
   std::cout << " --------------------------------------------------------------- " << std::endl;
   double cashFlows = FiveYrCashFLows(5, 100, 0.03, 1, 0.03, 0.2);
   std::cout << std::endl;
   std::cout << "Price is : " << cashFlows << std::endl;
   printf("%0.4lf", cashFlows);
   std::cout << std::endl;
   //---------------------------------------------------end part K -------------------------------------

   //------------------------------------------------Beginning part I ----------------------------------
   std::cout << "-------------------------------------5 Year Bond Duration (L) ---------------------------------------\n" << std::endl;
   double amortizationDuration = FiveYrDuration(5, 100, 0.03, 1, 0.03, 0.2, 0.01);
   std::cout << "Duration is : " << amortizationDuration << std::endl;
   std::cout << std::endl;
   //---------------------------------------------------end part K -------------------------------------
}