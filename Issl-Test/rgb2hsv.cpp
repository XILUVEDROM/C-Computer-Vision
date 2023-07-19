#include "rgb2hsv.h"
#include "mainwindow.h"

using namespace std;

RGB2HSV::RGB2HSV()
{

}


void RGB2HSV::RGB2HSVv (unsigned int &hue, unsigned char &sat, unsigned char &val,

unsigned char r, unsigned char g, unsigned char b)

/*

* 0 <= r,g,b <= 255 - значения красного, зеленого, синего

*

*

* Выход:

* 0 <= hue <= 360 градусов - оттенок.

* 0 <= sat <= 255 - Saturation - насыщенность

* 0 <= val <= 255 - Value - яркость

*/

  {

    double h, rc, gc, bc, dmax;

    unsigned char s, v;

    unsigned char minc, maxc;
    unsigned int Nhue;

    int abs1=0,abs2=0,abs3=0;

    maxc = (r > g) ? ((r > b) ? r : b) : ((g > b)? g : b);

    minc = (r < g) ? ((r < b) ? r : b) : ((g < b)? g : b);


    s = 0; // Насыщенность



    if (maxc)

      {

         s = (maxc - minc) * 255 / maxc;

      }


    sat = s;

    val = maxc;


    if (!s)

      { // Ахроматический цвет

        hue = Nhue;

      }

    else

      { // Хроматический цвет

        dmax = maxc - minc;

        rc = (maxc - r) / dmax; /* rc - удаленность */

        gc = (maxc - g) / dmax; /* цвета от красного */

        bc = (maxc - b) / dmax;

        if (r == maxc)

          { // Цвет между желтым и пурпурным

            h = bc - gc;

            abs1++;
          }

        else if (g == maxc)

          { // Цвет между голубым и желтым

            h = 2 + rc - bc;
            abs2++;
          }

        else

          { // Цвет между пурпурным и голубым

            h = 4 + gc - rc;
            abs3++;
          }

        h *= 60.0;

        if (h < 0.0)

          {

             h += 360.0;

          }

        hue = h;

        if (hue == 360)

          {

              hue = 0;

          }

      } // if (!s)

    if(abs1>abs2+abs3/2 || abs1>abs2/2+abs3)
    {
        cout<<"Признык бело голубых структур отциателен"<<endl;
    }
    else {
        cout<<"Призанк бело голубых структур положителен"<<endl;
    }

  } // RGB 2 HSV
