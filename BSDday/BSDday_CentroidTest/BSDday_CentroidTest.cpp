// ROI_Binary_0119.cpp : ©w¸q¥D±±¥xÀ³¥Îµ{¦¡ªº¶i¤JÂI¡C
//
//二值化ROI控制在340

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#define ‪frame 1510

#define imagewidth 384
#define imageheight 240

unsigned char input[138240];
unsigned char output[138240];
unsigned int label[138240];
unsigned char warn[6090];//change

void YtoUV();
void CentroidR();
void CentroidL();

bool ROI[138240] = { 0 };///change 384*240


int xmax, ymax = 0;
int xmin, ymin = 0;
int cxmax[1000], cymax[1000], cxmin[1000], cymin[1000] = { 0 };

int xmidpre = 0, ymidpre = 0, xmid2pre = 0, ymid2pre = 0;
int xmidex = 0, ymidex = 0, xmid2ex = 0, ymid2ex = 0;

int xCENpre = 0, yCENpre = 0, xCEN2pre = 0, yCEN2pre = 0;
int xCENex = 0, yCENex = 0, xCEN2ex = 0, yCEN2ex = 0;

int predict = 0, predict2 = 0, warnc = 0, warnc2 = 0, warnflag2 = 0, warnflag = 0;
int predictCEN = 0, predictCEN2 = 0;
int predictYR = 0, predictYL = 0;

int RupPre, RdowmPre, RupEx, RdowmEx;
int LupPre, LdowmPre, LupEx, LdowmEx;

int select = 0, select2 = 0;

int censumx2 = 0, censumy2 = 0, dotsum = 0;
int censumx = 0, censumy = 0;

time_t c_start, t_start, c_end, t_end;

int Xy, Yy, Xu, Yu, Xv, Yv;

int cleanwarn2 = 0, cleanwarn = 0;

int main()
{

	

	int x;
	int y = 0, Yavg = 0, YavgFIN = 0, countavg = 0;

	int Yavgc[9] = { 0 };
	//FILE *inputfile = fopen("D:\\BSD素材\\BSD輸出影片存檔\\d8_12_384x240.yuv", "rb");
	//FILE *inputfile = fopen("D:\\BSD素材\\BSD輸出影片存檔\\d7_12_384x240.yuv", "rb");
	FILE *inputfile = fopen("D:\\BSD素材\\BSD輸出影片存檔\\d5_12.yuv", "rb");
	if (inputfile == NULL){
		printf("failinput");
		system("pause");
		return -1;

	}
	else{
		printf("SUCCE111\n");
	}

	FILE *outputfile = fopen("afterBACK11_384x240_NN.yuv", "wb");
	if (inputfile == NULL){
		printf("fail");
		system("pause");
		return -1;

	}
	else{
		printf("SUCCE\n");
	}


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




	clock_t t1, t2, t3, t4, t5, t6; // typedef time_t long;


	for (int i = imagewidth * 55; i < imagewidth *imageheight; i++)//change
	{
		x = (i) % (imagewidth);
		y = ((i) / (imagewidth));
		if ((y >= 56) && ((63 * x - 97 * y) <= 9852) && ((60 * x + 28 * y) <= 24912) && ((123 * x - 106 * y) >= 20187))//change(RROI)
		{
			ROI[i] = 1;
		}
		else if ((y >= 56) && ((73 * x + 135 * y) >= 19240) &&/* (x >= 35)*/ ((84 * x - 64 * y) >= -6156) && ((157 * x + 101 * y) <= 35486))//change(LROI)
		{
			ROI[i] = 1;
		}

		//printf("%d ROI=", ROI[i]);
	}

	/*for (int i = 0; i <85; i++)
	{
	fread(input, 1, 1382400, inputfile);			//quick to frame85
	}
	fread(input, 1, 1382400, inputfile);*/

	t_start = time(NULL);

	for (int a = 0; a < ‪frame; a++)
	{
		t1 = clock();

		printf("frame%d\n", a + 1);
		Yavg = 0, YavgFIN = 0, countavg = 0;
		Yavgc[8] = { 0 };

		fread(input, 1, 138240, inputfile);//change


		/*for (int i = ( 230 * imagewidth); i < imagewidth *imageheight; i++)
		{
		x = (i) % (imagewidth);
		y = ((i) / (imagewidth));*/
		for (int y = 56; y <imageheight; y++)//change
		{
			for (int x = 0; x <= imagewidth; x++)
			{

				int i;
				i = imagewidth*y + x;

				if (ROI[i] == 1)
				{
					if (((y & 15) == 0) && ((x & 15) == 0))
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
		YavgFIN = (YavgFIN / countavg) * 0.45;

		////////上面是統計閥值，下面二值化
		memcpy(output, input, sizeof(output));


		//for (int i = ( 230 * imagewidth); i < imagewidth *imageheight; i++)
		for (int y = 56; y <imageheight; y++)//change
		{
			for (int x = 0; x < imagewidth; x++)/////////////////////////////////////////?????????<=????
			{

				int i;
				i = imagewidth*y + x;


				if (ROI[i] == 1)
				{

					//二值化
					if (input[i] < YavgFIN)
					{
						output[i] = 255;
					}
					else
					{
						output[i] = 0;
					}
					//固定閥值
				}

			}
		}
		t2 = clock();

		///////////////////////////////以下標籤化

		int c = 0, bound = 0;
		memset(label, 0, sizeof(label));












		/////////////////////////////////////////////////////////////


		for (int y = 220; y >= 55; y--)
		{
			for (int x = 20; x <= 200; x++) /////由左往右，下到上(LROI)
			{
				int i = y*imagewidth + x;
				//x = (i) % (imagewidth);
				//y = ((i) / (imagewidth));


				if (ROI[i] == 1)
				{
					if ((label[i] == 0) && (output[i] == 255))
					{
						if ((label[i - imagewidth - 1] == 0) && (label[i - imagewidth] == 0) && (label[i - imagewidth + 1] == 0) && (label[i - 1] == 0) && (label[i + 1] == 0) && (label[i + imagewidth - 1] == 0) && (label[i + imagewidth] == 0) && (label[i + imagewidth + 1] == 0))
						{
							c++;
							//label[i] = c;
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

							////¥H¤W¬°3x3®Ø¤º³£0¡Aµ¹·s¼ÐÅÒ
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
						}		////¥H¤W¬°¦pªG¦³¼ÐÅÒ¤£¬O0¡Aµ¹³Ì¤pªº¼ÐÅÒµ¹¤j®a
					}
				}
			}
		}



		/////////////////////////////////----------------------/////////////////////////////////////














		bound = c; ////標記左右畫框的c界線

		for (int y = 56; y <= 200; y++)//change
		{
			for (int x = 200; x <= 360; x++) /////由左往右，下到上(RROI)//change
			{
				int i = y*imagewidth + x;



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
		t3 = clock();

		////////////////////以上給標籤


		///////////////////////////////////////////////////////////////////
	/*	for (int i = (imagewidth); i < imagewidth *imageheight; i++)
		{
		output[i] = input[i];
		}*/
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

		/*for (int i = ( 230 * imagewidth); i < imagewidth *imageheight; i++)
		{
		x = (i) % (imagewidth);
		y = ((i) / (imagewidth));*/
		for (int y = 56; y <imageheight; y++)  //change
		{
			for (int x = 0; x < imagewidth; x++)
			{
				int i = y*imagewidth + x;
				//for (int y = 719; y >= 225; y--)
				//{
				//	for (int x = 0; x <= 620; x++) /////由左往右，下到上(LROI)
				//	{
				//		int i = y*imagewidth + x;

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

		int size = 0, FNsize = 0, FNsize2 = 0;

		select = 0;
		select2 = 0;

		for (int ctag = 1; ctag <= c; ctag++)
		{												//vvvvvvvvvvvv 中心點y除2再除2.5(原始20)
			if (((cxmax[ctag] - cxmin[ctag]) >= ((cymax[ctag] + cymin[ctag])/5) ) && ((cymax[ctag] - cymin[ctag]) >= 10))  //change
			{
				//if ((cxmax[ctag] - cxmin[ctag]) > (cymax[ctag] - cymin[ctag]))
				{
					size = (cxmax[ctag] - cxmin[ctag]) + (cymax[ctag] - cymin[ctag]);

					if ((size > FNsize) && (ctag <= bound))
					{
						select = ctag;
						FNsize = size;
					}
					if ((size > FNsize2) && (ctag > bound))
					{
						select2 = ctag;
						FNsize2 = size;
					}
				}
			}
		}


		if (select2 != 0)/////去除上下緣浮動R，計算浮動距離
		{
			RdowmEx = RdowmPre;
			RdowmPre = cymax[select2];

			RupEx = RupPre;
			RupPre = cymin[select2];

			predictYR = RdowmPre - RdowmEx;

			if ((RdowmPre - RdowmEx > 8) || (RdowmPre - RdowmEx < -8))
			{
				select2 = 0;
			}
			else if (((RupPre - RupEx > 8) || (RupPre - RupEx < -8)))
			{
				select2 = 0;
			}
		}
		

		if (select != 0)/////去除上下緣浮動L，計算浮動距離
		{
			LdowmEx = LdowmPre;
			LdowmPre = cymax[select];

			LupEx = LupPre;
			LupPre = cymin[select];

			predictYL = LdowmPre - LdowmEx;

			if ((LdowmPre - LdowmEx > 8) || (LdowmPre - LdowmEx < -8))
			{
				select = 0;
			}
			else if (((LupPre - LupEx > 8) || (LupPre - LupEx < -8)))
			{
				select = 0;
			}
		}

		





		t4 = clock();


		xmidex = xmidpre;
		ymidex = ymidpre;
		xmid2ex = xmid2pre;
		ymid2ex = ymid2pre;

		xmidpre = (cxmax[select] + cxmin[select]) / 2;
		ymidpre = (cymax[select] + cymin[select]) / 2;
		xmid2pre = (cxmax[select2] + cxmin[select2]) / 2;
		ymid2pre = (cymax[select2] + cymin[select2]) / 2;

		

		
		


		if (select2 != 0)    //Right 中心、重心操作
		{
			Xy = xmid2pre;
			Yy = ymid2pre;

			YtoUV();

			output[Yy*imagewidth + Xy] = 76;     //Y
			output[384 * 240 + Yu*imagewidth + Xu] = 84;   //U
			output[384 * 240 + Yu*imagewidth + Xu + 1] = 255;    //V

			/////////////////////上面顯示中心，下面顯示重心
						
			xCEN2ex = xCEN2pre;//紀錄上一frame重心物位置
			yCEN2ex = yCEN2pre;

			CentroidR();

			xCEN2pre = censumx2;//紀錄當前重心
			yCEN2pre = censumy2;
			
			Xy = xCEN2pre;  //計算當前重心UV位置 著色
			Yy = yCEN2pre;
			YtoUV();

			output[Yy*imagewidth + Xy] = 129;     //Y
			output[384 * 240 + Yu*imagewidth + Xu] = 55;   //U
			output[384 * 240 + Yu*imagewidth + Xu + 1] = 35;    //V

		}



		if (select != 0)   //Left 中心、重心操作
		{
			Xy = xmidpre;
			Yy = ymidpre;
			YtoUV();

			output[Yy*imagewidth + Xy] = 76;     //Y
			output[384 * 240 + Yu*imagewidth + Xu] = 84;   //U
			output[384 * 240 + Yu*imagewidth + Xu + 1] = 255;    //V

			/////////////////////上面顯示中心，下面顯示重心

			xCENex = xCENpre;//紀錄上一frame重心物位置
			yCENex = yCENpre;

			CentroidL();

			xCENpre = censumx;//紀錄當前重心
			yCENpre = censumy;

			Xy = xCENpre;	//計算當前重心UV位置 著色
			Yy = yCENpre;
			YtoUV();

			output[Yy*imagewidth + Xy] = 129;     //Y
			output[384 * 240 + Yu*imagewidth + Xu] = 55;   //U
			output[384 * 240 + Yu*imagewidth + Xu + 1] = 35;    //V

		}
		
		predict = xmidpre - xmidex;
		predict2 = xmid2pre - xmid2ex;//計算 中心 移動距離
		
		predictCEN = yCENpre - yCENex;//計算 重心 移動距離
		predictCEN2 = yCEN2pre - yCEN2ex;

		//if ((predict > 2))
		//{
		//	warnc += 1;
		//}
		//else if (predict<-10)
		//{
		//	warnc = 0;
		//	//warnflag = 0;
		//}             /////////left
		

		if ((predictYR >= 1) && (predictYR < 10))   ///RROI警告////////////////改了這個變成下緣
		{
			warnc2 += 1;
		}
		else if ((predictYR<-20) || (predictYR > 10))
		{
			warnc2 = 0;
			//warnflag = 0;
		}
		else
		{			
			if (cleanwarn2 >= 3)
			{
				//if (warnc2 > 0)
				{
					warnc2--;
				}
				cleanwarn2 = 0;
			}
			cleanwarn2++;
		}
		


		if ((predictYL >= 1) && (predictYL < 10))   ///LROI警告////////////////改了這個變成下緣
		{
			warnc += 1;
		}
		else if ((predictYL < -20) || (predictYL > 10))
		{
			warnc = 0;
			//warnflag = 0;
		}
		else
		{
			if (cleanwarn >= 2)
			{
				//if (warnc > 0)
				{
					warnc--;
				}
				cleanwarn = 0;
			}
			cleanwarn++;
		}

		///////////////////////////////////////////////////////////////////
		for (int i = (imagewidth); i < imagewidth *imageheight; i++)
		{
			output[i] = input[i];
		}
		//////////////////////////////上面是輸出沒有二值化黑白畫面的迴圈

				
		if ((select2 == 0))  //////////right
		{
			warnc2 = 0;
		}
		if ((select == 0))  ////left
		{
			warnc = 0;
		}

		if (warnc > 3)
		{
			warnflag += 1;
			warnc = 0;
		}
		if (warnc2 > 3)
		{
			warnflag2 += 1;
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
		}

		if (warnflag2>0)
		{
			for (int y = 0; y < 58; y++)
			{
				for (int x = 0; x < 70; x++)
				{
					int i = y*imagewidth + x;
					output[i+313] = warn[x + y * 70];
				}
			}
			warnflag2 += 1;
		}

		if (select2 == 0)  ///或是select2 == 0
		{
			if (warnflag2>45)
			{
				warnflag2 = 0;
			}
		}

		if (select == 0)  ///或是select2 == 0
		{
			if (warnflag>45)
			{
				warnflag = 0;
			}
		}

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


		

		for (int i = 0; i < imagewidth *imageheight; i++)
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
		}
		////////////////// close write line func.


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
		t5 = clock();

		fwrite(output, 1, imagewidth *imageheight*1.5, outputfile);
		//t2 = clock();
		//printf("fps=%lf\n", (double)(CLOCKS_PER_SEC) / (t2 - t1));
		//t3= clock();
		//printf("delay fps=%lf\n", (double)(t3- t2));


	}

	t6 = clock();
	t_end = time(NULL);
	printf("The pause used %f s by time()\n", difftime(t_end, t_start));





	fclose(inputfile);
	fclose(outputfile);
	printf("DONE\n");
	system("pause");
	return 0;
}

void YtoUV()
{
	if (Xy % 2 == 0)
	{
		Xu = Xy;
		Yu = Yy / 2;

		Xv = Xu + 1;
		Yv = Yu;
	}
	else
	{
		Xu = Xy - 1;
		Yu = Yy / 2;

		Xv = Xu + 1;
		Yv = Yu;
	}
}

void CentroidR()
{
	censumx2 = 0;//重心x //Right
	censumy2 = 0;//重心x
	dotsum =0;//重心計算用點數量

	for (int y = cymin[select2]; y <= cymax[select2];y++)
	{
		for (int x = cxmin[select2]; x <= cxmax[select2]; x++)
		{
			int i;
			i = imagewidth*y + x;

			if (output[i] == 255)
			{
				censumx2 = censumx2 + x;
				censumy2 = censumy2 + y;
				dotsum++;
			}
			
		}
	}

	censumx2 = censumx2 / dotsum;
	censumy2 = censumy2 / dotsum;
}

void CentroidL()
{
	censumx = 0;//重心x //Left
	censumy = 0;//重心x
	dotsum = 0;//重心計算用點數量

	for (int y = cymin[select]; y <= cymax[select]; y++)
	{
		for (int x = cxmin[select]; x <= cxmax[select]; x++)
		{
			int i;
			i = imagewidth*y + x;

			if (output[i] == 255)
			{
				censumx = censumx + x;
				censumy = censumy + y;
				dotsum++;
			}

		}
	}

	censumx = censumx / dotsum;
	censumy = censumy / dotsum;
}