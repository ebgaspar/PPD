#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<linux/limits.h>
#include<string.h>
#include<unistd.h>
#include<mpi/mpi.h>

#define NUMBER_OF_TESTS 10

void fill( double *arr , const int size , double val )
{
    for ( int i = 0 ; i < size ; ++i )
    {
        arr[ i ] = val;
    }
}

int main( int argc , char **argv )
{
    int pid , p , tag = 50;
    FILE *out = stdout;
    char filename[255];
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

    double *orig, *dest ;

    if ( pid == 0 )
    {
        orig = ( double * ) malloc( full_size );
        fill( orig , size , -1.0 );
    }
    else
    {
        dest = ( double * ) malloc( full_size );
        out = fopen( filename, "w" ) ;
        if ( out == NULL )
        {
            exit( 2 ) ;
        }
    }

    for ( int i = 0 ; i < NUMBER_OF_TESTS ; ++i )
    {
        if ( pid == 0 )
        {
            fill( orig , size , 2.0 );
            MPI_Send( orig , size , MPI_DOUBLE , 1 , tag , MPI_COMM_WORLD );
        } else
        {
            double t = MPI_Wtime( );

            MPI_Recv( dest , size , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status );
            t =  ( MPI_Wtime( ) - t ) ;

            fill( dest , size , 4.0 );

            fprintf( out , "RUN: %d\tTime: %f\n" , i + 1 , t );
        }
    }
    if ( pid == 0 )
    {
        free( orig ) ;
    }
    else
    {
        free( dest ) ;
        fclose( out ) ;
    }

    MPI_Finalize( );


    return 0;
}