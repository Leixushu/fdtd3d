#include <iostream>

#include <ctime>
#include <sys/time.h>

#if defined (PARALLEL_GRID)
#include "ParallelGrid.h"
#include <mpi.h>
#else /* PARALLEL_GRID */
#include "Grid.h"
#endif /* !PARALLEL_GRID */

#ifdef CUDA_ENABLED
#include "CudaInterface.h"
#endif

#include "BMPDumper.h"
#include "BMPLoader.h"
#include "DATDumper.h"
#include "DATLoader.h"

#include "Settings.h"
#include "SchemeTMz.h"
#include "SchemeTEz.h"
#include "Scheme3D.h"

#include "PhysicsConst.h"


int cudaThreadsX = 8;
int cudaThreadsY = 8;
int cudaThreadsZ = 8;

int main (int argc, char** argv)
{
  solverSettings.SetupFromCmd (argc, argv);

#ifdef GRID_2D
  GridCoordinate3D overallSize (solverSettings.getSizeX (), solverSettings.getSizeY (), 0);
  GridCoordinate3D pmlSize (solverSettings.getPMLSizeX (), solverSettings.getPMLSizeY (), 0);
  GridCoordinate3D tfsfSize (solverSettings.getTFSFSizeX (), solverSettings.getTFSFSizeY (), 0);
#endif
#ifdef GRID_3D
  GridCoordinate3D overallSize (solverSettings.getSizeX (), solverSettings.getSizeY (), solverSettings.getSizeZ ());
  GridCoordinate3D pmlSize (solverSettings.getPMLSizeX (), solverSettings.getPMLSizeY (), solverSettings.getPMLSizeZ ());
  GridCoordinate3D tfsfSize (solverSettings.getTFSFSizeX (), solverSettings.getTFSFSizeY (), solverSettings.getTFSFSizeZ ());
#endif

  int rank = 0;
  int numProcs = 1;

  struct timeval tv1, tv2;

  YeeGridLayout *yeeLayout = NULLPTR;

#ifdef PARALLEL_GRID
  ParallelGridCore *parallelGridCore = NULLPTR;
  bool skipProcess = false;
#endif

  if (solverSettings.getDoUseParallelGrid ())
  {
#if defined (PARALLEL_GRID)
    GridCoordinate3D topology (solverSettings.getTopologySizeX (),
                               solverSettings.getTopologySizeY (),
                               solverSettings.getTopologySizeZ ());

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    parallelGridCore = new ParallelGridCore (rank, numProcs, overallSize,
                                             solverSettings.getDoUseManualVirtualTopology (),
                                             topology);
    ParallelGrid::initializeParallelCore (parallelGridCore);

    if (rank >= parallelGridCore->getTotalProcCount ())
    {
      skipProcess = true;
    }

    if (!skipProcess)
    {
      DPRINTF (LOG_LEVEL_STAGES, "Start process %d of %d (using %d)\n", rank, numProcs, parallelGridCore->getTotalProcCount ());

      yeeLayout = new ParallelYeeGridLayout (overallSize,
                                             pmlSize,
                                             tfsfSize,
                                             solverSettings.getIncidentWaveAngle1 () * PhysicsConst::Pi / 180.0,
                                             solverSettings.getIncidentWaveAngle2 () * PhysicsConst::Pi / 180.0,
                                             solverSettings.getIncidentWaveAngle3 () * PhysicsConst::Pi / 180.0,
                                             solverSettings.getDoUseDoubleMaterialPrecision ());
      ((ParallelYeeGridLayout *) yeeLayout)->Initialize (parallelGridCore);
    }
#else
    ASSERT_MESSAGE ("Solver is not compiled with support of parallel grid. Recompile it with -DPARALLEL_GRID=ON.");
#endif
  }
  else
  {
    yeeLayout = new YeeGridLayout (overallSize,
                                   pmlSize,
                                   tfsfSize,
                                   solverSettings.getIncidentWaveAngle1 () * PhysicsConst::Pi / 180.0,
                                   solverSettings.getIncidentWaveAngle2 () * PhysicsConst::Pi / 180.0,
                                   solverSettings.getIncidentWaveAngle3 () * PhysicsConst::Pi / 180.0,
                                   solverSettings.getDoUseDoubleMaterialPrecision ());
  }

#if defined (PARALLEL_GRID)
  if (!skipProcess)
#endif
  {
#ifdef CUDA_ENABLED
    cudaInfo ();

    if (solverSettings.getDoUseParallelGrid ())
    {
#if defined (PARALLEL_GRID)
      cudaInit (rank % solverSettings.getNumCudaGPUs ());
#else
      ASSERT_MESSAGE ("Solver is not compiled with support of parallel grid. Recompile it with -DPARALLEL_GRID=ON.");
#endif
    }
    else
    {
      cudaInit (solverSettings.getNumCudaGPUs ());
    }

    cudaThreadsX = solverSettings.getNumCudaThreadsX ();
    cudaThreadsY = solverSettings.getNumCudaThreadsY ();
    cudaThreadsZ = solverSettings.getNumCudaThreadsZ ();
#endif

#ifdef GRID_3D
    Scheme3D scheme (yeeLayout, overallSize, solverSettings.getNumTimeSteps ());

    scheme.initScheme (solverSettings.getGridStep (), /* dx */
                       PhysicsConst::SpeedOfLight / solverSettings.getSourceWaveLength ()); /* source frequency */

    scheme.initGrids ();

    gettimeofday(&tv1, NULL);

    scheme.performSteps ();

    gettimeofday(&tv2, NULL);
#endif
  }

  delete yeeLayout;

  if (solverSettings.getDoUseParallelGrid ())
  {
#if defined (PARALLEL_GRID)
    delete parallelGridCore;

    MPI_Barrier (MPI_COMM_WORLD);
    MPI_Finalize ();
#else
    ASSERT_MESSAGE ("Solver is not compiled with support of parallel grid. Recompile it with -DPARALLEL_GRID=ON.");
#endif
  }

  if (rank == 0)
  {
    printf ("Total time = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

    printf ("Dimension: %d\n", solverSettings.getDimension ());
#ifdef GRID_2D
    printf ("Grid size: %dx%d\n", solverSettings.getSizeX (), solverSettings.getSizeY ());
#endif
#ifdef GRID_3D
    printf ("Grid size: %dx%dx%d\n", solverSettings.getSizeX (), solverSettings.getSizeY (), solverSettings.getSizeZ ());
#endif
    printf ("Number of time steps: %d\n", solverSettings.getNumTimeSteps ());

    printf ("\n");

#ifdef FLOAT_VALUES
    printf ("Value type: float\n");
#endif
#ifdef DOUBLE_VALUES
    printf ("Value type: double\n");
#endif
#ifdef LONG_DOUBLE_VALUES
    printf ("Value type: long double\n");
#endif

#ifdef TWO_TIME_STEPS
    printf ("Number of time steps: 2\n");
#endif
#ifdef ONE_TIME_STEP
    printf ("Number of time steps: 1\n");
#endif

    printf ("\n-------- Details --------\n");
    printf ("Parallel grid: %d\n", solverSettings.getDoUseParallelGrid ());

#if defined (PARALLEL_GRID)
    printf ("Number of processes: %d\n", numProcs);

#ifdef PARALLEL_BUFFER_DIMENSION_1D_X
    printf ("Parallel grid scheme: X\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_1D_Y
    printf ("Parallel grid scheme: Y\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_1D_Z
    printf ("Parallel grid scheme: Z\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_2D_XY
    printf ("Parallel grid scheme: XY\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_2D_YZ
    printf ("Parallel grid scheme: YZ\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_2D_XZ
    printf ("Parallel grid scheme: XZ\n");
#endif
#ifdef PARALLEL_BUFFER_DIMENSION_3D_XYZ
    printf ("Parallel grid scheme: XYZ\n");
#endif

    printf ("Buffer size: %d\n", solverSettings.getBufferSize ());
#endif
  }

  return EXIT_OK;
}
