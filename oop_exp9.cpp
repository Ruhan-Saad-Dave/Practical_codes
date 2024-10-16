#include<iostream>
#include <stdlib.h>
#include<math.h>
#include <graphics.h>
using namespace std;

void translation(float x1, float y1, float x2, float y2, float x3, float y3){
	float tx, ty;
	int buffer;
	cout << "Enter translation factor of x:";
	cin >> tx;
	cout << "Enter translation factor of y:";
	cin >> ty;

	x1 = x1 + tx;
	x2 = x2 + tx;
	x3 = x3 + tx;
	y1 = y1 + ty;
	y2 = y2 + ty;
	y3 = y3 + ty;
	
	line(x1, y1, x2, y2);
	line(x2, y2, x3, y3);
	line(x1, y1, x3, y3);
	cin >> buffer;
}

void rotation(float x1, float y1, float x2, float y2, float x3, float y3){
	float theta, nx1, nx2, nx3, ny1, ny2, ny3, xr, yr;
	int buffer;
	cout << "Enter degree of rotation:";
	cin >> theta;
	theta = theta * (M_PI/180);

	xr = x3;
	yr = y3;
	
	nx1 = xr + ((x1 - xr)*cos(theta)) - ((y1-yr)*sin(theta));
	nx2 = xr + ((x2 - xr)*cos(theta)) - ((y2-yr)*sin(theta));
	nx3 = xr + ((x3 - xr)*cos(theta)) - ((y3-yr)*sin(theta));
	ny1 = yr + ((x1 - xr)*sin(theta)) - ((y1-yr)*cos(theta));
	ny2 = yr + ((x2 - xr)*sin(theta)) - ((y2-yr)*cos(theta));
	ny3 = yr + ((x3 - xr)*sin(theta)) - ((y3-yr)*cos(theta));

	line(nx1, ny1, nx2, ny2);
	line(nx2, ny2, nx3, ny3);
	line(nx1, ny1, nx3, ny3);
	cin >> buffer;
}

void scaling(float x1, float y1, float x2, float y2, float x3, float y3){
	float tx, ty;
	int buffer;
	cout << "Enter translation factor of x:";
	cin >> tx;
	cout << "Enter translation factor of y:";
	cin >> ty;

	x1 = x1 * tx;
	x2 = x2 * tx;
	x3 = x3 * tx;
	y1 = y1 * ty;
	y2 = y2 * ty;
	y3 = y3 * ty;
	
	line(x1, y1, x2, y2);
	line(x2, y2, x3, y3);
	line(x1, y1, x3, y3);
	cin >> buffer;
}

int main(){
	int choice = 0;
	float x1, y1, x2, y2, x3, y3;
	int i, gd = DETECT, gm;
	
	initgraph(&gd, &gm, NULL);
	
	cout << "Enter x cordinate of 1st point:";
	cin >> x1;
	cout << "ENter y coordinate of 1st point:";
	cin >> y1;
	cout << "Enter x cordinate of 2nd point:";
	cin >> x2;
	cout << "ENter y coordinate of 2nd point:";
	cin >> y2;
	cout << "Enter x cordinate of 3rd point:";
	cin >> x3;
	cout << "ENter y coordinate of 3rd point:";
	cin >> y3;

	while(choice!=4){
		line(x1, y1, x2, y2);
		line(x2, y2, x3, y3);
		line(x1, y1, x3, y3);

		cout << "-------MENU-------" << endl;
		cout << "1.ENter 1 for translation" << endl;
		cout << "2.Enter 2 for rotation" << endl;
		cout << "3.Enter 3 for scaling" <<endl;
		cout << "4.Enter 4 for exit" << endl;
		cout << "Enter choice:";
		cin >> choice;
		
		if(choice==1){
			translation(x1, y1, x2, y2, x3, y3);
		}
		else if(choice==2){
			rotation(x1, y1, x2, y2, x3, y3);
		}
		else if(choice==3){
			scaling(x1, y1, x2, y2, x3, y3);
		}

		cleardevice();
	}
	return 0;
}










