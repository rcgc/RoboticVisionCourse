//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//        Median Filter to remove salt n pepper noise
//
//------------------------------------------------------------------------------
//                  			March 17, 2022
//                               Robotic Vision
//------------------------------------------------------------------------------

//==============================================================================
//------------------------P R O G R A M-----------------------------------------
//==============================================================================

//----------------------------------------------------------------------------//
//         Prototype declarations                                             //
//----------------------------------------------------------------------------//

void heading (void);      /* Displays program heading */
void openfiles (void);    /* Opens input and output files in binary */
void userdefined (void);  /* Here, the user will implement his routine */
void readhdr (void);      /* Read header from input image file */
void addhdr (void);       /* Add header to output image file */
void closefiles (void);   /* Close all files used in program */

//----------------------------------------------------------------------------//
//         Include declarations                                               //
//----------------------------------------------------------------------------//

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>

//----------------------------------------------------------------------------//
//         Variable declarations                                              //
//----------------------------------------------------------------------------//

int    MRows, NCols;                 /* Number of Rows and columns in image */
FILE   *infptr, *outfptr, *outfptrh; /* Input and output file pointers */
char   infile[40];                   /* Name of input file */
char   outfile[40];                  /* Name of output file */

//----------------------------------------------------------------------------//
//         Main program                                                       //
//----------------------------------------------------------------------------//

main ()
{
     // Display heading
     heading ();
     // Open Input and Output Image Files
     openfiles ();
     // Read header from image file
     readhdr();
     // Add header to output file
     addhdr();
     // Execute userdefined procedure
     userdefined ();
     // Close all image files
     closefiles ();
     
     printf("\n Bye! Bye!\n");
     system("PAUSE");
     return(1);
} // end main ();

//----------------------------------------------------------------------------//
//         Headings                                                           //
//----------------------------------------------------------------------------//

void heading ()
{
     int i;
     for (i=0; i<16; i++)
       printf("                                    +                       \n");
       printf("                      Digital Image Processing in C         \n");
       printf("                         Date: March 17, 2022             \n");
       printf("                            Author: Roberto Guzm?n          \n");
     for (i=0; i<3; i++)
       printf("                                    +                       \n");
       return;
} // end heading ()

//----------------------------------------------------------------------------//
//         Openfiles                                                          //
//----------------------------------------------------------------------------//

void openfiles ()
{
     printf("\n Opening Input and Output image files\n");
     printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ");
     scanf("%s",&infile);
     
     //Check if input file exists
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n",infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s",&outfile);
     
     //Check if output file was created succesfully
     if ((outfptr = fopen(outfile, "wb")) == NULL)
     {
       printf(" Cannot create output image file: %s\n",outfile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
       }
       
     // If this point is reached, file are OK
     printf(" File opened and created OK! \n\n");
     
}  //end openfiles ()

//----------------------------------------------------------------------------//
//         Read Header                                                        //
//----------------------------------------------------------------------------//

void readhdr ()
{
     int   i=0, MaxRGB;
     char  c, buffer[128];
     
     //Read header of PGM file
     printf("\n\n File <%s> Header: \n",infile);
     
     do
     {
       c = fgetc(infptr);
       buffer[i] = c;
       i++; 
     } while (c != '\n');
     
     //Check if file is P5 (pgm) format
     if (buffer[1] == '5')
       printf("\n Input file is pgm, OK\n");
     else
     {
       printf("\n Input file is NOT pgm, Exiting program...\n");
       system("PAUSE");
       exit(0);
     }
     
     //File is pgm, read rest of header
     fscanf(infptr,"%d %d %d",&NCols, &MRows, &MaxRGB);
     c = fgetc(infptr);
     printf("%d ",NCols);
     printf("%d   <---- Width x Height) \n",MRows);
     printf("%d   <---- Max. Grayscale level) \n\n",MaxRGB);
}  // end readhdr ()

//----------------------------------------------------------------------------//
//         Add Header                                                         //
//----------------------------------------------------------------------------//

void addhdr ()
{
     fprintf(outfptr, "P5\n%d %d\n%d\n",NCols,MRows,255);
} //addhdr ()

//----------------------------------------------------------------------------//
//         MEDIAN FILTER PROGRAM                                              //
//----------------------------------------------------------------------------//

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void userdefined ()
{
	// Leer cada pixel
	unsigned char Pixel;
	// Matriz para la imagen origen
	unsigned char mat[512][512];
	// Matriz para la imagen convulucion
	unsigned char conv[512][512];
	
	// Variables para las matrices
	int col, row, value;
	
	// array con vecinos y actual
	int arr[9];
	
	// Leer imagen de entrada a matriz mat
	for (row=0; row< MRows; row++)
		for (col=0; col<NCols; col++) {
			Pixel= fgetc(infptr);
			mat[col][row]= Pixel;
		}
		
	// Agregar borde a imagen de salida
	for (row=0; row< MRows;row++)
		for (col=0; col<NCols;col++) 
			conv[col][row]=0;
			
	for (row = 1; row < MRows-1; row++)
      for (col = 1; col < NCols-1; col++) {
      	// Lleno array con valores de vecino y acutal
      	arr[0] = mat[col-1][row-1];
      	arr[1] = mat[col  ][row-1];
      	arr[2] = mat[col+1][row-1];
      	arr[3] = mat[col-1][row  ];
      	arr[4] = mat[col  ][row  ];
      	arr[5] = mat[col+1][row  ];
      	arr[6] = mat[col-1][row+1];
      	arr[7] = mat[col  ][row+1];
      	arr[8] = mat[col+1][row+1];
      	
      	// ordeno array
      	qsort(arr, 9, sizeof(int), cmpfunc);
      	
      	// asigno al pixel actual el valor de la media, array[4]
      	conv[col][row] = arr[4];  	
	}
	
	// Guardar imagen de salida
	for (row=0; row< MRows;row++)
		for (col=0; col<NCols;col++) {
			Pixel = conv[col][row];
			fputc(Pixel,outfptr);
		}
	
}  // end userdefined ()

//----------------------------------------------------------------------------//
//         Close Files                                                        //
//----------------------------------------------------------------------------//

void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // end closefiles ()
