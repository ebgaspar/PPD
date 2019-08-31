#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<limits.h>
#include<unistd.h>
#include<mpi/mpi.h>

#define NUMBER_OF_TESTS 20

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
    char filename[PATH_MAX];
    int opt;
    static int size;
    int full_size;

    if ( argc == 1 )
    {

        fprintf( stderr , "ex01 -n <numero> -f <nome do arquivo> " );
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
            case '?':
                fprintf( stderr , "opcao desconhecida: %c\n" , optopt );
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

    for ( int i = 0 ; i < NUMBER_OF_TESTS ; ++i )
    {
        if ( pid == 0 )
        {
            auto *orig = ( double * ) malloc( full_size );

            fill( orig , size , -1.0 );
            fill( orig , size , 2.0 );
            MPI_Send( orig , size , MPI_DOUBLE , 1 , tag , MPI_COMM_WORLD );
            free( orig );
        } else
        {
            auto *dest = ( double * ) malloc( full_size );

            clock_t t;
            t = clock( );

            MPI_Recv( dest , size , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status );
            fill( dest , size , 4.0 );

            fprintf( out , "RUN: %d\tTime: %ld\n" , i + 1 , clock( ) - t );
            free( dest );
        }
    }
    MPI_Finalize( );


    return 0;
}

