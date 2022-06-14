//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================
//
//  main();
//     heading();               display program heading
//     openfiles();             opens input & output image files
//     negative();              convert original image to negative
//     readhdr();               reads input image file header
//     closefiles();            closes input and processed output image files
//------------------------------------------------------------------------------
void     heading(void);
void     openfiles(void);
void     negative(void);
void     readhdr(void);
void     closefiles();
//------------------------------------------------------------------------------
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
//------------------------------------------------------------------------------
int      MRows, NCols;                     //indices for rows and cols
FILE     *infptr, *outfptr;                //input/output file pointers
char     infile[40];
char     static  outfile[40];              //names of input/output image files

//------------------------------------------------------------------------------
//-----------------------------MAIN---------------------------------------------
//------------------------------------------------------------------------------
main()
{
    //--------------( Display Heading with information )------------------------
    heading();
    //---------------(Open Input & Output Image Files)--------------------------
    openfiles();
    //---------------------(Obtain negative of image)---------------------------
    negative();
    //------------------(Close Any Open Image Files)----------------------------
    closefiles();
    //--------------------------------------------------------------------------
    printf("\n Bye! Bye!\n");
    system("PAUSE");
    return (1);
} //end main()

//------------------------------------------------------------------------------
//----------------------------HEADING-------------------------------------------
//------------------------------------------------------------------------------
void heading()
{ int      i;
  for (i=0;i<16;i++) printf("                                     +\n");
  printf("                       Digital Image Processing Program\n");
  printf("                         Updated: March 10, 2022\n");
  printf("                                 RickWare\n");
  for (i=0;i<4;i++)  printf("                                     +\n");
}//end heading()

//------------------------------------------------------------------------------
//--------------------------OPENFILES-------------------------------------------
//------------------------------------------------------------------------------
void openfiles(void)
{
  printf("\n                 OPEN an image file\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ? ");
  scanf("%s",&infile);
  printf(" Enter name of *.pgm OUTPUT image file (example: lenaconv.pgm ? ");
  scanf("%s",&outfile);
    
  if ((infptr = fopen(infile, "rb")) == NULL)
  { printf(" Can NOT open input image file: <%s>\n",infile);
    printf(" Exiting program..... "); system("PAUSE"); exit(1);
  }
  else  printf(" Input file <%s> opened sucessfully\n\n",infile);
  
  if ((outfptr = fopen(outfile,"wb")) == NULL)
  { printf(" Can NOT open output image file <%s>\n\n",outfile);
    printf(" Exiting program....."); system("PAUSE"); exit(1);
  }
  else printf(" Output file <%s> is opened sucessfully\n\n",outfile);
  
  readhdr();
  return;
}//end openfiles()

//------------------------------------------------------------------------------
//------------------------------READHDR-----------------------------------------
//------------------------------------------------------------------------------
void readhdr()
{
  int     i, k=0, MaxRGB;
  char    c, c1, buffer[128];
  //-----------------------[Read PPM File Header]-------------------------------
  printf("\n\n File <%s> Header Bytes:\n",infile);
  printf("------------------------------------------------------------\n");
    i = 0;
    do
    { c = fgetc(infptr);
      buffer[i] = c; i++;
    } while (c != '\n');
    
     c1 = buffer[1];
     
     if (c1 == '5')
     {
       printf("\nFile type is:  <P%c>\n",c1);
     }
     else
     { printf(" Image is in WRONG format!! Quitting.........\n\n");
       system("PAUSE");
       exit(0);
     }
    
  fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
  c = fgetc(infptr);
  printf("%d ",NCols);
  printf("%d     <-----(Width & Height)\n", MRows);
  printf("%d         <-----(Max. RGB Level)\n\n",MaxRGB);
}//end readhdr()

//------------------------------------------------------------------------------
//--------------------------Obtain convolved image------------------------------
//---------------by taking the one's complement of every pixel------------------
//------------------------------------------------------------------------------
void negative()
{
  unsigned char Gray, GrayH, GrayV;
  unsigned char Mag;
  unsigned char mat [512][512];
  unsigned char convH[512][512];
  unsigned char convV[512][512];
  // Blur mask
  //         char mask[  3][  3] ={{ 1, 1, 1},
  //                               { 1, 1, 1},
  //								 { 1, 1, 1}};

  // Sobel horizontal mask (edge detection)
           int maskH[  3][  3] = {{ 1, 2, 1},
                                 { 0, 0, 0},
  								 {-1,-2,-1}};
								 
  // Sobel vertical mask (edge detection)
           int maskV[  3][  3] = {{-1, 0, 1},
                                 {-2, 0, 2},
  								 {-1, 0, 1}};
								 
  // Sharpening mask (second derivative)
  //          int mask[  3][  3] = {{ 0, 1, 0},
  //                                 { 1,-4, 1},
  //								 { 0, 1, 0}};
								 
  // Sharpening mask (second derivative)
  //         int mask[  3][  3] = {{ 1, 1, 1},
  //                               { 1,-8, 1},
  //		     					 { 1, 1, 1}};
								 
  int col, row, value;
  
  int dividefactor = 0;
  // Calcula dividefactor
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskH[col][row];
      }
      
  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despues) = %d \n",dividefactor);
    
  //Add *.pgm Header to output file
  fprintf(outfptr,"P5\n512 512\n255\n");
  
  //Read input image and store en matrix
  for (col = 0; col < MRows; col++)
    for (row = 0; row < NCols; row++) {
        Gray = fgetc(infptr);
        mat[col][row] = Gray;
    }
  
  // Clear convolution matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	convH[col][row] = 0;

  //Convolve image with mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskH[0][0] * mat[col-1][row-1]  +
      	                   maskH[0][1] * mat[col  ][row-1]  +
      	                   maskH[0][2] * mat[col+1][row-1]  +
      	                   maskH[1][0] * mat[col-1][row  ]  +
      	                   maskH[1][1] * mat[col  ][row  ]  +
      	                   maskH[1][2] * mat[col+1][row  ]  +
      	                   maskH[2][0] * mat[col-1][row+1]  +
      	                   maskH[2][1] * mat[col  ][row+1]  +
      	                   maskH[2][2] * mat[col+1][row+1]) / dividefactor);
    	convH[col][row] = value;
    	if      (value <   0) convH[col][row] = 0;
    	else if (value > 255) convH[col][row] = 255;
    	else                  convH[col][row] = value;
      }
  
  // printf("Fisrt convolved pixel = %d \n",conv[1][1]);






  // Calcula dividefactor
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskV[col][row];
      }
      
  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despues) = %d \n",dividefactor);
    
  
  // Clear convolution matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	convV[col][row] = 0;

  //Convolve image with mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskV[0][0] * mat[col-1][row-1]  +
      	                   maskV[0][1] * mat[col  ][row-1]  +
      	                   maskV[0][2] * mat[col+1][row-1]  +
      	                   maskV[1][0] * mat[col-1][row  ]  +
      	                   maskV[1][1] * mat[col  ][row  ]  +
      	                   maskV[1][2] * mat[col+1][row  ]  +
      	                   maskV[2][0] * mat[col-1][row+1]  +
      	                   maskV[2][1] * mat[col  ][row+1]  +
      	                   maskV[2][2] * mat[col+1][row+1]) / dividefactor);
    	convV[col][row] = value;
    	if      (value <   0) convV[col][row] = 0;
    	else if (value > 255) convV[col][row] = 255;
    	else                  convV[col][row] = value;
      }






  
  //Save countour image 
  for (col = 0; col < MRows; col++)
    for (row = 0; row < NCols; row++) {
    	GrayH = convH[col][row];
        GrayV = convV[col][row];
        Mag = sqrt(GrayH*GrayH + GrayV*GrayV);
        fputc(Mag,outfptr);
    }   
}

//------------------------------------------------------------------------------
//----------------------------CLOSEFILES----------------------------------------
//------------------------------------------------------------------------------
void closefiles()
{ //------------------------(Close Files)---------------------------------------
  fclose(infptr);
  fclose(outfptr);

  return;
} //end closefiles()
//------------------------------------------------------------------------------
