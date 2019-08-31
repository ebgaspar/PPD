#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<limits.h>
#include<mpi/mpi.h>

int main( int argc , char **argv )
{
    FILE *out = stdout;
    char filename[PATH_MAX];
    int pid , p , dest , tag = 50 , soma = 12;
    int opt;

    if ( argc == 1 )
    {

        fprintf( stderr , "ex01 -n <numero> -f <nome do arquivo> " );
        exit( 1 );

    }

    while ( ( opt = getopt( argc , argv , "n:f:" ) ) != -1 )
    {
        switch ( opt )
        {
            case 'n':
                soma = atoi( optarg );
                break;
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

    if ( pid == 0 )
    {
        out = fopen( filename , "w" );

        soma += pid;
        dest = pid + 1;

        MPI_Send( &soma , 1 , MPI_INT , dest , tag , MPI_COMM_WORLD );

        fprintf( out , "Processo: %d , Soma: %d\n" , pid , soma );

        for ( int i = 1 ; i < p ; ++i )
        {
            MPI_Recv( &soma , 1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status );
            fprintf( out , "Processo: %d , Soma: %d\n" , i , soma );
        }

        fclose( out );
    } else
    {

        MPI_Recv( &soma , 1 , MPI_INT , ( pid - 1 ) , tag , MPI_COMM_WORLD , &status );

        soma = soma + pid;

        MPI_Send( &soma , 1 , MPI_INT , 0 , tag , MPI_COMM_WORLD );

        dest = pid + 1;

        if ( dest < p )
            MPI_Send( &soma , 1 , MPI_INT , dest , tag , MPI_COMM_WORLD );
    }

    MPI_Finalize( );

    return 0;

}
