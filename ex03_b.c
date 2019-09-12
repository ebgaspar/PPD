#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<linux/limits.h>
#include<string.h>
#include<unistd.h>
#include<mpi/mpi.h>

#define NUMBER_OF_TESTS 10

void fill( double *arr , const int size , const double val )
{
    for ( int i = 0 ; i < size ; ++i )
    {
        arr[ i ] = val;
    }
}

int main( int argc , char **argv )
{
    int pid , p , tag = 50;
    char filename[ 255 ];
    int opt;
    static int size;
    int full_size;

    if ( argc == 1 )
    {

        fprintf( stderr , "ex01 -s <numero> -f <nome do arquivo> " );
        exit( 1 );

    }

    while ( ( opt = getopt( argc , argv , "s:f:" ) ) != -1 )
    {
        switch ( opt )
        {
            case 's':
                size = atoi( optarg );
            case 'f':
                strcpy( filename , optarg );
                break;
            case ':':
                fprintf( stdout , "parametro precisa de um valor\n" );
                break;
            default:
                break;
        }
    }

    MPI_Status status;
    MPI_Init( &argc , &argv );
    MPI_Comm_rank( MPI_COMM_WORLD , &pid );
    MPI_Comm_size( MPI_COMM_WORLD , &p );
    full_size = size * sizeof( double );
    MPI_File file ;

    int isOpen = MPI_File_open( MPI_COMM_WORLD , filename,
                              MPI_MODE_WRONLY | MPI_MODE_CREATE , MPI_INFO_NULL,
                                &file ) ;

    for ( int i = 0 ; i < NUMBER_OF_TESTS ; ++i )
    {
        if ( pid == 0 )
        {
            double *orig = ( double * ) malloc( full_size );

            fill( orig , size , -1.0 );
            fill( orig , size , 2.0 );
            MPI_Send( orig , size , MPI_DOUBLE , 1 , tag , MPI_COMM_WORLD );
            free( orig );
        } else
        {
            double *dest = ( double * ) malloc( full_size );

            double t = MPI_Wtime( );

            MPI_Recv( dest , size , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status );
            fill( dest , size , 4.0 );

            t =  ( MPI_Wtime( ) - t ) ;
            char buf[ 255 ] ;
            // sprintf( buf, "%f", t ) ;
            sprintf( buf, "RUN: %d\tTime: %f\n", i + 1, t ) ;
            
            MPI_File_write_shared( file , buf , strlen( buf ) , MPI_CHAR , &status );
            // MPI_File_write_shared( file , "\n" , 1, MPI_CHAR , &status );
            
            free( dest );
        }
    }

    MPI_File_close( &file ) ;
    MPI_Finalize( );


    return 0;
}

