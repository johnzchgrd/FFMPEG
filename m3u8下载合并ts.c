/** \file temp.c
  *
  *This is a simple IO batch processing C function that provide fast access to concatenate hundreds and thousands of ts somehow downloaded from the Internet. 
  *Have fun!
  *\author johnzchgrd@github.com
  *\date 2019/06/30
  *\version 1.0.0
  *
  */
#include<stdio.h>
#define TSNUM 1464 //ts片段数量
#define DELTA 61//可以用质因数分解方法找到合适的值，数量不宜过多
#define ENDNUM (TSNUM-DELTA)
void f(){
	int j;
	int i;
	char filename[15];
	FILE* fp;
	/*生成先合并小段的ts，合并数量不宜过多*/
	for(j=0;j<=ENDNUM;j+=DELTA){
		sprintf(filename,"runme%d.bat",j/DELTA);
		fp=fopen(filename,"w+");
		fprintf(fp,"ffmpeg -i concat:\"");
		
		for(i=j;i<j+DELTA;i++){
			fprintf(fp,"%d.ts",i);
			if(i!=DELTA-1)
				fprintf(fp,"|");
		}
		fprintf(fp,"\" -c copy out%d.ts\n",j/DELTA);
		fclose(fp);
	}
	/*批处理写在一起有些问题，暂时这样做*/
	fp=fopen("1_fewerTS.bat","w");
	fprintf(fp,"@echo off\nstart cmd /c \"");
	for(i=0;i<=ENDNUM/DELTA;i++){
		fprintf(fp,"runme%d.bat",i);
		if(i!=ENDNUM/DELTA)
			fprintf(fp,"&&");
	}
	fprintf(fp,"\"");
	fclose(fp);
	/*连接批处理生成的ts段*/
	fp=fopen("2_concatTS.bat","w");
	fprintf(fp,"ffmpeg -i concat:\"");
	for(i=0;i<=ENDNUM/DELTA;i++){
		fprintf(fp,"out%d.ts",i);
		if(i!=ENDNUM/DELTA)
			fprintf(fp,"|");
	}
	//fprintf(fp,"\" -c copy -bsf:a aac_adtstoasc -movflags +faststart output.mp4\n");
	fprintf(fp,"\" -c copy  output.mp4\n");
	fclose(fp);
	/*删除所有ts和bat*/
	fp=fopen("3_deleteTS&BAT.bat","w");
	fprintf(fp,"@echo off\ndel /a /f /q  .\\*.ts\ndel /a /f /q  .\\*.bat");
	fclose(fp);
	
}

int main()
{
	f();
	
	return 0;
	
}
