#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<linux/limits.h>
#include<string.h>
#include<mpi/mpi.h>

int main( int argc , char** argv )
{
    FILE *out = stdout;
    char filename[255];
    int pid , p , tag = 50 ;
    int opt;

    if ( argc == 1 )
    {

        fprintf( stderr , "ex01 -f <nome do arquivo> " );
        exit( 1 );

    }

    while ( ( opt = getopt( argc , argv , "f:" ) ) != -1 )
    {
        switch ( opt )
        {
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

    MPI_Status status ;
    MPI_Init( &argc , &argv ) ;
    MPI_Comm_rank( MPI_COMM_WORLD , &pid ) ;
    MPI_Comm_size( MPI_COMM_WORLD , &p ) ;

    if ( pid == 0 )
    {
        out = fopen( filename, "a" ) ;
        for ( int j = 0 , total_p = 100 * ( p - 1 ) ; j < total_p ; j++ )
        {
            int msg ;

            MPI_Irecv( &msg , 1 , MPI_INT , MPI_ANY_SOURCE , MPI_ANY_TAG , MPI_COMM_WORLD , &status ) ;
            fprintf( out, "MPI_SOURCE = %d "
                          "\t msg = %d \t"
                          " j = %d\n" , status.MPI_SOURCE , msg , j + 1 ) ;
        }

        fclose( out ) ;

    }
    else
    {
        for ( int j = 0 ; j < 100 ; j++ )
        {
            MPI_Send( &j , 1 , MPI_INT , 0 , tag , MPI_COMM_WORLD ) ;

        }
    }

    MPI_Finalize( ) ;

    return 0 ;
}

