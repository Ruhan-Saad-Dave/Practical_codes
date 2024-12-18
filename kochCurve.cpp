#include <iostream>
#include <graphics.h>
#include <cmath>
using namespace std;

class KochCurve{
  public:
  void drawKochCurve(int x1, int y1, int x2, int y2, int iteration){
    if (iteration == 0){
      line(x1, y1, x2, y2);
    } else {
      int dx = x2 - x1;
      int dy = y2 - y1;
      int x3 = x1 + dx / 3;
      int y3 = y1 + dy / 3;
      int x4 = x1 + 2*dx/3;
      int y4 = y1 + 2*dy/3;

      int x5 = x3 + (x4 - x3)/2 - (y4 - y3) * sqrt(3)/2;
      int y5 = y3 + (y4 - y3)/2 + (x4 - x3) * sqrt(3)/2;

      drawKochCurve(x1, y1, x3, y3, iteration - 1);
      drawKochCurve(x3, y3, x5, y5, iteration - 1);
      drawKochCurve(x5, y5, x4, y4, iteration - 1);
      drawKochCurve(x4, y4, x2, y2, iteration - 1);
    }
  }

  void drawKochSnowflake(int x1,int y1,int x2,int y2,int iteration){
    drawKochCurve(x1, y1, x2, y2, iteration);

    int x3 = (x1 + x2)/2 + (y1 - y2) * sqrt(3) / 2;
    int y3 = (y1 + y2)/2 - (x2 - x1) * sqrt(3) / 2;
    drawKochCurve(x2, y2, x3, y3, iteration);

    x3 = (x1 + x2)/2 + (y1 - y2) * sqrt(3) / 2;
    y3 = (y1 + y2)/2 + (x1 - x2) * sqrt(3) / 2;
    drawKochCurve(x3, y3, x1, y1, iteration);
  }

  void centerKochSnowflake(int &x1, int &y1, int &x2, int &y2, int &x3, int &y3){
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int sideLength = 300;
    x1 = centerX - sideLength / 2;
    y1 = centerY + sideLength / 2;
    x2 = centerX + sideLength / 2;
    y2 = y1;
    x3 = centerX;
    y3 = centerY - static_cast<int>(sideLength * sqrt(3) / 2);
  }
};

int main(){
  int gd = DETECT, gm;
  initgraph(&gd, &gm, NULL);
  int x1, y1, x2, y2, x3, y3;
  int iterations;

  kochCurve k;
  k.centerKochSnowflake(x1, y1, x2, y2, x3, y3);
  cout << "Enter the number of iterations for Koch Curve:";
  cin >> iterations;
  k.drawKochSnowflake(x1, y1, x2, y2, iterations);

  getch();
  closegraph();
  return 0;
}
