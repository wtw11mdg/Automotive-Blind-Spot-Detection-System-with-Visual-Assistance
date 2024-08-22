// ROI_Binary_0119.cpp : ©w¸q¥D±±¥xÀ³¥Îµ{¦¡ªº¶i¤JÂI¡C
//


#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <math.h>

#define totalframe 1507

unsigned char input[138240];
unsigned char output[138240];
unsigned int label[138240];
unsigned char warn[6090];//change

bool ROI[138240] = { 0 };///change 384*240


int xmax, ymax = 0;
int xmin, ymin = 0;
int cxmax[1000], cymax[1000], cxmin[1000], cymin[1000] = { 0 };

int xmidpre = 0, ymidpre = 0, xmid2pre = 0, ymid2pre = 0, xmid2SECpre, ymid2SECpre;
int xmidex = 0, ymidex = 0, xmid2ex = 0, ymid2ex = 0;
int lockx = 0, locky = 0, lockx_ex = 0, locky_ex = 0;


int xmidpreSEC = 0, ymidpreSEC = 0, xmid2preSEC = 0, ymid2preSEC = 0;
int xmidexSEC = 0, ymidexSEC = 0, xmid2exSEC = 0, ymid2exSEC = 0;

int predict = 0, predict2 = 0, predictLock = 0, warnc = 0, warnc2 = 0, warnflag = 0;

bool lockon = false;

int main()
{

	int imagewidth = 384;//change
	int imageheight = 240;

	int x;
	int y = 0, Yavg = 0, YavgFIN = 0, countavg = 0;

	int Yavgc[9] = { 0 };

	FILE *inputfile = fopen("n_Rs.yuv", "rb");
	if (inputfile == NULL){
		printf("fail");
		system("pause");
		return -1;

	}
	else{
		printf("SUCCE\n");
	}
	FILE *outputfile = fopen("after7NN.yuv", "wb");



	FILE *warning = fopen("C:\\Users\\XX\\Documents\\Visual Studio 2013\\Projects\\BSDfor67_ver.2\\BSDfor67_ver.2\\warning.yuv", "rb");
	if (warning == NULL){
		printf("warning open fail");
		system("pause");
		return -1;

	}
	else{
		printf("warning SUCCE\n");
	}
	fread(warn, 1, 70 * 58, warning);


	//fread(input, 1, 1382400, inputfile);

	clock_t t1, t2; // typedef time_t long;

	for (int i = (imagewidth * 76); i < imagewidth *imageheight; i++)//change
	{
		x = (i) % (imagewidth);
		y = ((i) / (imagewidth));
		if ((x >= 55) && (x <= 216) && ((10 * x + 151 * y) <= 18770) && ((8 * x + 151 * y) >= 12600))//change
		{
			ROI[i] = 1;
		}
		//printf("%d ROI=", ROI[i]);
	}


	for (int a = 0; a < totalframe; a++)
	{
		t1 = clock();
		printf("frame%d\n", a + 1);
		Yavg = 0, YavgFIN = 0, countavg = 0;
		Yavgc[8] = { 0 };

		fread(input, 1, 138240, inputfile);//change


		for (int y = 70; y <imageheight; y++)//change
		{
			for (int x = 0; x <= imagewidth; x++)
			{
				//if (((((50 * x / 96) + y) >500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x>140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x<1190)))
				int i;
				i = imagewidth*y + x;

				if (ROI[i] == 1)
				{
					if ((y % 20 == 0) && (x % 20 == 0))
					{
						Yavgc[0] = input[i];
						Yavgc[1] = input[i + 1];
						Yavgc[2] = input[i + 2];
						Yavgc[3] = input[i + imagewidth];
						//Yavgc[4] = input[i + 1 + imagewidth];
						//Yavgc[5] = input[i + 2 + imagewidth];
						//Yavgc[6] = input[i + 2 * imagewidth];
						//Yavgc[7] = input[i + 1 + 2 * imagewidth];
						//Yavgc[8] = input[i + 2 + 2 * imagewidth];

						for (int j = 0; j <= 3; j++)
						{
							Yavg = Yavg + Yavgc[j];
						}
						Yavg = Yavg / 4;

						YavgFIN = YavgFIN + Yavg;
						countavg++;

						//統計亮度參考點著色
						i += 19;
					}
				}
			}
		}
		YavgFIN = (YavgFIN / countavg) * 1 / 2;

		////////上面是統計閥值，下面二值化
		memcpy(output, input, sizeof(output));


		for (int y = 70; y <imageheight; y++)//change
		{
			for (int x = 0; x <= imagewidth; x++)
			{
				//if (((((50 * x / 96) + y) >500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x>140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x<1190)))
				int i;
				i = imagewidth*y + x;


				//if (((((50 * x / 96) + y) >500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x>140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x < 1190)))
				if (ROI[i] == 1)
				{

					//統計亮度參考點著色
					if (input[i] > 210)   //設定二值化閥值
					{
						output[i] = 255;
					}
					else
					{
						output[i] = 0;
					}
					//固定閥值
				}
				/*else
				{
				output[i] = input[i];
				}
				}
				else
				{
				output[i] = input[i];

				//roi著色
				}*/
			}
		}


		///////////////////////////////以下標籤化

		int c = 0, bound = 0;
		memset(label, 0, sizeof(label));


		//////////////     for (int i = (imagewidth *imageheight * 1 / 3); i < imagewidth *imageheight; i++)
		//for (int y = 719; y >= 225; y--)
		//{
		//	for (int x = 0; x <= 620; x++) /////由左往右，下到上(LROI)
		//	{
		//		int i = y*imagewidth + x;
		//		//x = (i) % (imagewidth);
		//		//y = ((i) / (imagewidth));

		//		//if (((((50 * x / 96) + y) > 500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x > 140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x < 1190)))
		//		if (ROI[i] == 1)
		//		{
		//			if ((label[i] == 0) && (output[i] == 255))
		//			{
		//				if ((label[i - imagewidth - 1] == 0) && (label[i - imagewidth] == 0) && (label[i - imagewidth + 1] == 0) && (label[i - 1] == 0) && (label[i + 1] == 0) && (label[i + imagewidth - 1] == 0) && (label[i + imagewidth] == 0) && (label[i + imagewidth + 1] == 0))
		//				{
		//					c++;
		//					//label[i] = c;
		//					for (int p = -1; p <= 1; p++)
		//					{
		//						for (int q = -1; q <= 1; q++)
		//						{
		//							if (output[i + imagewidth*p + q] == 255)
		//							{
		//								label[i + imagewidth*p + q] = c;
		//							}
		//						}
		//					}

		//					////¥H¤W¬°3x3®Ø¤º³£0¡Aµ¹·s¼ÐÅÒ
		//				}
		//				else
		//				{
		//					int min = 100000;
		//					for (int p = -1; p <= 1; p++)
		//					{
		//						for (int q = -1; q <= 1; q++)
		//						{
		//							if ((label[i + imagewidth*p + q] < min) && (label[i + imagewidth*p + q] > 0))
		//							{
		//								min = label[i + imagewidth*p + q];
		//							}
		//						}
		//					}

		//					for (int p = -1; p <= 1; p++)
		//					{
		//						for (int q = -1; q <= 1; q++)
		//						{
		//							if (output[i + imagewidth*p + q] == 255)
		//							{
		//								label[i + imagewidth*p + q] = min;

		//							}
		//						}
		//					}
		//				}		////¥H¤W¬°¦pªG¦³¼ÐÅÒ¤£¬O0¡Aµ¹³Ì¤pªº¼ÐÅÒµ¹¤j®a
		//			}
		//		}
		//	}
		//}
		/////////////////////////////////----------------------/////////////////////////////////////

		bound = c; ////標記左右畫框的c界線

		for (int y =70 ; y <130; y++)
		{
			for (int x = 60; x <= 240; x++) /////由左往右，下到上(RROI)
			{
				int i = y*imagewidth + x;


				//if (((((50 * x / 96) + y) > 500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x > 140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x < 1190)))
				if (ROI[i] == 1)
				{
					if ((label[i] == 0) && (output[i] == 255))
					{
						if ((label[i - imagewidth - 1] == 0) && (label[i - imagewidth] == 0) && (label[i - imagewidth + 1] == 0) && (label[i - 1] == 0) && (label[i + 1] == 0) && (label[i + imagewidth - 1] == 0) && (label[i + imagewidth] == 0) && (label[i + imagewidth + 1] == 0))
						{
							c++;

							for (int p = -1; p <= 1; p++)
							{
								for (int q = -1; q <= 1; q++)
								{
									if (output[i + imagewidth*p + q] == 255)
									{
										label[i + imagewidth*p + q] = c;
									}
								}
							}

							////以上為3x3框內都0，給新標籤
						}
						else
						{
							int min = 100000;
							for (int p = -1; p <= 1; p++)
							{
								for (int q = -1; q <= 1; q++)
								{
									if ((label[i + imagewidth*p + q] < min) && (label[i + imagewidth*p + q] > 0))
									{
										min = label[i + imagewidth*p + q];
									}
								}
							}

							for (int p = -1; p <= 1; p++)
							{
								for (int q = -1; q <= 1; q++)
								{
									if (output[i + imagewidth*p + q] == 255)
									{
										label[i + imagewidth*p + q] = min;
									}
								}
							}
						}		////以上為如果有標籤不是0，給最小的標籤給大家
					}
				}
			}
		}

		//////////////////------------------------------------////////////////////////////////////


		////////////////////以上給標籤


		///////////////////////////////////////////////////////////////////
		for (int i = (imagewidth); i < imagewidth *imageheight; i++)
		{
		output[i] = input[i];
		}
		//////////////////////////////上面是輸出沒有二值化黑白畫面的迴圈



		//for (int csmall = 1; csmall <= c; csmall++)
		//{

		for (int b = 0; b < 1000; b++)
		{
			cxmax[b] = -1;
			cymax[b] = -1;
			cxmin[b] = 10000;
			cymin[b] = 10000;
		}


		//xmax = -1;
		//ymax = -1;
		//xmin = 10000;
		//ymin = 10000;

		for (int y = 76; y <imageheight; y++)  //change
		{
			for (int x = 0; x <= imagewidth; x++)
			{
				int i = y*imagewidth + x;
				//for (int y = 719; y >= 225; y--)
				//{
				//	for (int x = 0; x <= 620; x++) /////由左往右，下到上(LROI)
				//	{
				//		int i = y*imagewidth + x;
				//if (((((50 * x / 96) + y) > 500) && (((719 * x / 750) + y) < 21570 / 25) && (y>220) && (x > 140)) || ((((719 * x / 720) - y) > 2876 / 6) && (((490 * x / 829) - y) <(220500 / 829)) && (y>220) && (x < 1190)))
				if (ROI[i] == 1)
				{


					{
						if (x < cxmin[label[i]])
						{
							cxmin[label[i]] = x;
						}
						if (x > cxmax[label[i]])
						{
							cxmax[label[i]] = x;
						}
						if (y < cymin[label[i]])
						{
							cymin[label[i]] = y;
						}
						if (y > cymax[label[i]])
						{
							cymax[label[i]] = y;
						}
					}
				}

			}
		}



		int size = 0, FNsize = 0, FNsize2 = 0, FNsizeSEC = 0, FNsize2SEC = 0, select = 0, select2 = 0, selectSEC = 0, select2SEC = 0;

		for (int ctag = 1; ctag <= c; ctag++)
		{
			if (((cxmax[ctag] - cxmin[ctag]) >= 5) && ((cymax[ctag] - cymin[ctag]) >= 5))
			{
				//if ((cxmax[ctag] - cxmin[ctag]) > (cymax[ctag] - cymin[ctag]))
				{
					size = ((cxmax[ctag] + cxmin[ctag]) / 2);

					if ((size > FNsize) && (ctag <= bound))
					{
						select = ctag;
						FNsize = size;
					}
					if ((size > FNsize2) && (ctag > bound))  //(right ROI)
					{
						select2 = ctag;
						FNsize2SEC = FNsize2;
						FNsize2 = size;
					}
					else if (size > FNsize2SEC)
					{
						select2SEC = ctag;
						FNsize2SEC = size;
					}
				}
			}
		}




		xmidex = xmidpre;
		ymidex = ymidpre;
		xmid2ex = xmid2pre;
		ymid2ex = ymid2pre;

		xmidpre = (cxmax[select] + cxmin[select]) / 2;
		ymidpre = (cymax[select] + cymin[select]) / 2;

		xmid2pre = (cxmax[select2] + cxmin[select2]) / 2;  //(目前 RROI First x position 中心點)
		ymid2pre = (cymax[select2] + cymin[select2]) / 2;  //(目前 RROI First y position 中心點)
		xmid2SECpre = (cxmax[select2SEC] + cxmin[select2SEC]) / 2;  //(目前 RROI Secound x position 中心點)
		ymid2SECpre = (cymax[select2SEC] + cymin[select2SEC]) / 2;  //(目前 RROI Secound y position 中心點)


		if ((abs(xmid2SECpre - xmid2pre)>15) && (abs(ymid2SECpre - ymid2pre)>9) && (abs(xmid2SECpre - xmid2pre) <= 50) && (abs(ymid2SECpre - ymid2pre) <=30))  //鎖定找出兩框中心
		{
			lockx_ex = lockx;
			locky_ex = lockx;//////////有錯?
			lockx = (xmid2SECpre + xmid2pre) / 2;
			locky = (ymid2SECpre + ymid2pre) / 2;
			lockon = true;
		}
		else
		{
			lockon = false;
		}



		//predict = xmidpre - xmidex;
		predict2 = xmid2pre - xmid2ex;

		if ((lockx_ex != 0) && (locky_ex != 0) && (lockon == true))
		{
			predictLock = lockx - lockx_ex;
		}
		else
		{
			predictLock = 0;
		}


		//if ((predict > 2))
		//{
		//	warnc += 1;
		//}
		//else if (predict<-10)
		//{
		//	warnc = 0;
		//	//warnflag = 0;
		//}             /////////left

		//if (predict2 > 2)
		//{
		//	warnc2 += 1;
		//}
		//else if (predict<-10)
		//{
		//	warnc2 = 0;
		//	//warnflag = 0;
		//}          ////////////////right
		//if ((select2 == 0))
		//{
		//	warnc2 = 0;
		//}
		//if (/*(warnc > 5) || */ (warnc2 > 3))
		//{
		//	warnflag += 1;
		//	warnc = 0;
		//	warnc2 = 0;
		//}


		//if (warnflag>0)
		//{
		//	for (int y = 0; y < 58; y++)
		//	{
		//		for (int x = 0; x < 70; x++)
		//		{
		//			int i = y*imagewidth + x;
		//			output[i] = warn[x + y * 70];
		//		}
		//	}
		//	warnflag += 1;
		//	//warnflag = 0;
		//}

		//if (warnflag>45)
		//{
		//	if ((predict2 < 1))
		//	{
		//		warnflag = 0;
		//	}

		//}

		//////////////////////////////////////  lock warn  /////////////////////

		if (predictLock >= 2)
		{

			warnc2 += 1;
			printf("warn2=%d\n", warnc2);
		}
		else if (predictLock<-10)
		{
			warnc2 = 0;
			//warnflag = 0;
		}          ////////////////right
		if ((select2 == 0) || (select2SEC == 0))
		{
			warnc2 = 0;
		}
		if (/*(warnc > 5) || */ (warnc2 > 3))
		{
			warnflag += 1;
			warnc = 0;
			warnc2 = 0;
		}


		if (warnflag>0)
		{
			for (int y = 0; y < 58; y++)
			{
				for (int x = 0; x < 70; x++)
				{
					int i = y*imagewidth + x;
					output[i] = warn[x + y * 70];
				}
			}
			warnflag += 1;
			//warnflag = 0;
		}

		if (warnflag>45)
		{
			if ((predictLock < 1))
			{
				warnflag = 0;
			}

		}
		/////////////////lock warn/////////////////////

		///////////////以上尋找每個標籤xy最大最小
		//if (((xmax - xmin) >= 50) && ((ymax - ymin) >= 30))
		//{
		//	if ((xmax - xmin) >= (ymax - ymin))
		//	{
		//		for (int i = 0; i < imagewidth *imageheight; i++)
		//		{
		//			x = (i) % (imagewidth);
		//			y = ((i) / (imagewidth));
		//			if (((xmin <= x) && (x <= xmax)) && ((y == ymax) || (y == ymin)))
		//			{
		//				output[i] = 200;
		//			}
		//			if (((ymin <= y) && (y <= ymax)) && ((x == xmax) || (x == xmin)))
		//			{
		//				output[i] = 200;
		//			}
		//		}
		//		///////////////以上依照XY最大最小畫偵測框
		//	}
		//}

		if (lockon == true)    //////////把有成對的標記
		{
			for (int lockxprint = lockx - 3; lockxprint < lockx + 3; lockxprint++)
			{
				for (int lockyprint = locky - 3; lockyprint < locky + 3; lockyprint++)
				{
					int i = lockyprint*imagewidth + lockxprint;
					output[i] = 200;
					//printf("lock status=%d\n\n",lockon);
				}

			}
		}

		
		/*for (int i = 0; i < imagewidth *imageheight; i++)
		{
			x = (i) % (imagewidth);
			y = ((i) / (imagewidth));
			if (((select != 0) && (((cxmin[select] <= x) && (x <= cxmax[select])) && ((y == cymax[select]) || (y == cymin[select])))) || ((select2 != 0) && (((cxmin[select2] <= x) && (x <= cxmax[select2])) && ((y == cymax[select2]) || (y == cymin[select2])))))
			{
				output[i] = 200;
			}
			if (((select != 0) && (((cymin[select] <= y) && (y <= cymax[select])) && ((x == cxmax[select]) || (x == cxmin[select])))) || ((select2 != 0) && (((cymin[select2] <= y) && (y <= cymax[select2])) && ((x == cxmax[select2]) || (x == cxmin[select2])))))
			{
				output[i] = 200;

			}
			if (((selectSEC != 0) && (((cxmin[selectSEC] <= x) && (x <= cxmax[selectSEC])) && ((y == cymax[selectSEC]) || (y == cymin[selectSEC])))) || ((select2SEC != 0) && (((cxmin[select2SEC] <= x) && (x <= cxmax[select2SEC])) && ((y == cymax[select2SEC]) || (y == cymin[select2SEC])))))
			{
				output[i] = 200;
			}
			if (((selectSEC != 0) && (((cymin[selectSEC] <= y) && (y <= cymax[selectSEC])) && ((x == cxmax[selectSEC]) || (x == cxmin[selectSEC])))) || ((select2SEC != 0) && (((cymin[select2SEC] <= y) && (y <= cymax[select2SEC])) && ((x == cxmax[select2SEC]) || (x == cxmin[select2SEC])))))
			{
				output[i] = 200;

			}
		}
*/




		///////////////以上依照XY最大最小畫偵測框


		/*if ((select != 0) || (select2 != 0))
		{
		for (int y = 0; y < 58; y++)
		{
		for (int x = 0; x < 70; x++)
		{
		int i = y*imagewidth + x;
		output[i] = warn[x+y*70];
		}
		}

		}*/
		/////////上面，貼上warning

		//////////////-------------------///////////////////------------////////////////
		//}
		//////////////////////以上畫框
		t2 = clock();
		//printf("fps=%lf\n", (double)(CLOCKS_PER_SEC) / (t2 - t1));
		fwrite(output, 1, imagewidth *imageheight*1.5, outputfile);

	}

	fclose(inputfile);
	fclose(outputfile);
	printf("DONE\n");
	system("pause");
	return 0;
}

/////////////找兩個相近的燈，比對間距
