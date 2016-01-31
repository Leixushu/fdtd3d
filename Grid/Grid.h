#ifndef FIELD_GRID_H
#define FIELD_GRID_H

#include <cstdint>
#include <vector>

#include "GridCoordinate.h"
#include "FieldPoint.h"


/*
 * Parallel grid buffer types.
 */
#if defined (PARALLEL_GRID)
enum BufferPosition
{
/*
 * One dimension buffers.
 */
#if defined (GRID_1D) && defined (PARALLEL_BUFFER_DIMENSION_1D) || \
    defined (GRID_2D) && defined (PARALLEL_BUFFER_DIMENSION_1D) || \
    defined (GRID_3D) && defined (PARALLEL_BUFFER_DIMENSION_1D)
  LEFT,
  RIGHT,
#endif
/*
 * Two dimension buffers.
 */
#if defined (GRID_2D) && defined (PARALLEL_BUFFER_DIMENSION_2D) || \
    defined (GRID_3D) && defined (PARALLEL_BUFFER_DIMENSION_2D)
  LEFT,
  RIGHT,
  UP,
  DOWN,
  LEFT_UP,
  LEFT_DOWN,
  RIGHT_UP,
  RIGHT_DOWN,
#endif
/*
 * Three dimension buffers.
 */
#if defined (GRID_3D) && defined (PARALLEL_BUFFER_DIMENSION_3D)
  LEFT,
  RIGHT,
  UP,
  DOWN,
  FRONT,
  BACK,
  LEFT_FRONT,
  LEFT_BACK,
  LEFT_UP,
  LEFT_DOWN,
  RIGHT_FRONT,
  RIGHT_BACK,
  RIGHT_UP,
  RIGHT_DOWN,
  UP_FRONT,
  UP_BACK,
  DOWN_FRONT,
  DOWN_BACK,
  LEFT_UP_FRONT,
  LEFT_UP_BACK,
  LEFT_DOWN_FRONT,
  LEFT_DOWN_BACK,
  RIGHT_UP_FRONT,
  RIGHT_UP_BACK,
  RIGHT_DOWN_FRONT,
  RIGHT_DOWN_BACK,
#endif

/*
 * Overall number of buffers for current dimension.
 */
  BUFFER_COUNT
};
#endif


// Vector of points in grid.
typedef std::vector<FieldPointValue*> VectorFieldPointValues;

// Type for buffer of values.
typedef std::vector<FieldValue> VectorBufferValues;

// Type for vector of buffer
typedef std::vector<VectorBufferValues> VectorBuffers;

// Type for vector of buffer
typedef std::vector<MPI_Request> VectorRequests;


// Grid itself.
class Grid
{
  // Size of the grid.
  // For parallel grid - size of current node plus size of buffers.
  GridCoordinate size;

  // Vector of points in grid.
  // Owns this. Deletes all FieldPointValue* itself.
  VectorFieldPointValues gridValues;

  // Current time step.
  uint32_t timeStep;

  // ======== Parallel parts. ========
#if defined (PARALLEL_GRID)
  // Current node (process) identificator.
  int processId;

  // Overall count of nodes (processes).
  int totalProcCount;

#if defined (GRID_2D)
  // Size of square grid.
  int nodeGridSizeX;
  int nodeGridSizeY;
#endif

  // Size of current node without buffers.
  GridCoordinate totalSize;

  // Size of current node without buffers.
  GridCoordinate currentSize;

  // Size of buffer zone.
  GridCoordinate bufferSizeLeft;
  GridCoordinate bufferSizeRight;

  // Send/Receive buffers for independent send and receive
  VectorBuffers buffersSend;
  VectorBuffers buffersReceive;

#endif /* PARALLEL_GRID */

private:

  // Check whether position is appropriate to get/set value from.
  bool isLegitIndexWithSize (const GridCoordinate& position, const GridCoordinate& sizeCoord) const;
  bool isLegitIndex (const GridCoordinate& position) const;

  // Calculate three-dimensional coordinate from position.
  grid_iter calculateIndexFromPositionWithSize (const GridCoordinate& position,
                                                const GridCoordinate& sizeCoord) const;
  grid_iter calculateIndexFromPosition (const GridCoordinate& position) const;

  // Get values in the grid.
  VectorFieldPointValues& getValues ();

#if defined (PARALLEL_GRID)
  // Raw send
  void SendRawBuffer (BufferPosition buffer, int processTo);

  // Raw receive
  void ReceiveRawBuffer (BufferPosition buffer, int processFrom);

  // Raw send/receive
  void SendReceiveRawBuffer (BufferPosition bufferSend, int processTo,
                             BufferPosition bufferReceive, int processFrom);

  void SendReceiveBuffer (BufferPosition bufferDirection);

  void SendReceive ();

  void ParallelGridConstructor (grid_iter numTimeStepsInBuild);

#endif /* PARALLEL_GRID */

public:

#if defined (PARALLEL_GRID)
  Grid (const GridCoordinate& totSize, const GridCoordinate& curSize,
        const GridCoordinate& bufSizeL, const GridCoordinate& bufSizeR,
        const int process, const int totalProc, uint32_t step);
#else /* PARALLEL_GRID */
  Grid (const GridCoordinate& s, uint32_t step);
#endif /* !PARALLEL_GRID */

  ~Grid ();

  // Get size of the grid.
  const GridCoordinate& getSize () const;


  // Calculate position from three-dimensional coordinate.
  GridCoordinate calculatePositionFromIndex (grid_iter index) const;

#if defined (GRID_1D) || defined (GRID_2D) || defined (GRID_3D)
  // Set field point at coordinate in grid.
  void setFieldPointValue (FieldPointValue* value, const GridCoordinate& position);

  void setFieldPointValueCurrent (const FieldValue& value,
                                  const GridCoordinate& position);

  // Get field point at coordinate in grid.
  FieldPointValue* getFieldPointValue (const GridCoordinate& position);
  FieldPointValue* getFieldPointValue (grid_iter coord);
/*#if defined (PARALLEL_GRID)
  FieldPointValue* getFieldPointValueGlobal (const GridCoordinate& position);
  FieldPointValue* getFieldPointValueGlobal (grid_iter coord);
#endif*/
#endif  /* GRID_1D || GRID_2D || GRID_3D*/

  // Replace previous layer with current and so on.
  void shiftInTime ();

  // Switch to next time step.
  //void nextTimeStep ();

#if defined (PARALLEL_GRID)
  void Share ();
#endif /* PARALLEL_GRID */
};


#endif /* FIELD_GRID_H */