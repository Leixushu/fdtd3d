#ifndef GRID_COORDINATE_3D_H
#define GRID_COORDINATE_3D_H

#include "GridCoordinate2D.h"

template<class TcoordType>
class GridCoordinate3DTemplate;

template<class TcoordType>
GridCoordinate3DTemplate<TcoordType> CUDA_DEVICE CUDA_HOST operator* (TcoordType lhs, const GridCoordinate3DTemplate<TcoordType>& rhs);

/**
 * 3-dimensional coordinate in the grid.
 */
template<class TcoordType>
class GridCoordinate3DTemplate: public GridCoordinate2DTemplate<TcoordType>
{
  TcoordType z;

public:

  CUDA_DEVICE CUDA_HOST GridCoordinate3DTemplate ()
    : GridCoordinate2DTemplate<TcoordType> (), z (0) {}

  explicit CUDA_DEVICE CUDA_HOST GridCoordinate3DTemplate (const TcoordType& cx, const TcoordType& cy, const TcoordType& cz)
    : GridCoordinate2DTemplate<TcoordType> (cx, cy), z (cz) {}

  CUDA_DEVICE CUDA_HOST GridCoordinate3DTemplate (const GridCoordinate3DTemplate& pos)
    : GridCoordinate2DTemplate<TcoordType> (pos.getX (), pos.getY ()), z (pos.getZ ()) {}

  CUDA_DEVICE CUDA_HOST ~GridCoordinate3DTemplate () {}

  // Calculate three-dimensional coordinate.
  grid_iter CUDA_DEVICE CUDA_HOST calculateTotalCoord () const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    return x * y * z;
  }

  // Get one-dimensional coordinates.
  const TcoordType& CUDA_DEVICE CUDA_HOST getZ () const
  {
    return z;
  }

  // Set one-dimensional coordinates.
  void CUDA_DEVICE CUDA_HOST setZ (const TcoordType& new_z)
  {
    z = new_z;
  }

  TcoordType CUDA_DEVICE CUDA_HOST getMax () const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();

    if (x > y && x > z)
    {
      return x;
    }
    else if (y > z)
    {
      return y;
    }
    else
    {
      return z;
    }
  }

  GridCoordinate3DTemplate CUDA_DEVICE CUDA_HOST operator+ (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return GridCoordinate3DTemplate (x + rhs_x, y + rhs_y, getZ () + rhs.getZ ());
  }

  GridCoordinate3DTemplate CUDA_DEVICE CUDA_HOST operator- (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return GridCoordinate3DTemplate (x - rhs_x, y - rhs_y, getZ () - rhs.getZ ());
  }

  bool CUDA_DEVICE CUDA_HOST operator== (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x == rhs_x && y == rhs_y && getZ () == rhs.getZ ();
  }

  bool CUDA_DEVICE CUDA_HOST operator!= (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x != rhs_x || y != rhs_y || getZ () == rhs.getZ ();
  }

  bool CUDA_DEVICE CUDA_HOST operator> (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x > rhs_x && y > rhs_y && getZ () > rhs.getZ ();
  }

  bool CUDA_DEVICE CUDA_HOST operator< (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x < rhs_x && y < rhs_y && getZ () < rhs.getZ ();
  }

  bool CUDA_DEVICE CUDA_HOST CUDA_DEVICE CUDA_HOST operator>= (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x >= rhs_x && y >= rhs_y && getZ () >= rhs.getZ ();
  }

  bool CUDA_DEVICE CUDA_HOST operator<= (const GridCoordinate3DTemplate& rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType rhs_x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    TcoordType rhs_y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();

    return x <= rhs_x && y <= rhs_y && getZ () <= rhs.getZ ();
  }

  GridCoordinate3DTemplate CUDA_DEVICE CUDA_HOST operator- () const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    return GridCoordinate3DTemplate (- x, - y, - getZ ());
  }

  GridCoordinate3DTemplate CUDA_DEVICE CUDA_HOST operator* (TcoordType rhs) const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    return GridCoordinate3DTemplate (x * rhs, y * rhs, getZ () * rhs);
  }

  friend GridCoordinate3DTemplate<TcoordType> CUDA_DEVICE CUDA_HOST (::operator* <TcoordType>) (TcoordType lhs, const GridCoordinate3DTemplate<TcoordType>& rhs);

  GridCoordinate2DTemplate<TcoordType> CUDA_DEVICE CUDA_HOST shrink () const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    return GridCoordinate2DTemplate<TcoordType> (x, y);
  }

  void print () const
  {
    TcoordType x = GridCoordinate1DTemplate<TcoordType>::getX ();
    TcoordType y = GridCoordinate2DTemplate<TcoordType>::getY ();
    printf ("Coord (%lu,%lu,%lu).\n", x, y, getZ ());
  }
};

template<class TcoordType>
GridCoordinate3DTemplate<TcoordType> CUDA_DEVICE CUDA_HOST operator* (TcoordType lhs, const GridCoordinate3DTemplate<TcoordType>& rhs)
{
  TcoordType x = rhs.GridCoordinate1DTemplate<TcoordType>::getX ();
  TcoordType y = rhs.GridCoordinate2DTemplate<TcoordType>::getY ();
  return GridCoordinate3DTemplate<TcoordType> (lhs * x, lhs * y, lhs * rhs.getZ ());
}

template<class TcoordType>
GridCoordinate3DTemplate<TcoordType> CUDA_DEVICE CUDA_HOST expand (const GridCoordinate2DTemplate<TcoordType> &coord)
{
  TcoordType x = coord.GridCoordinate1DTemplate<TcoordType>::getX ();
  TcoordType y = coord.GridCoordinate2DTemplate<TcoordType>::getY ();
  return GridCoordinate3DTemplate<TcoordType> (x, y, 0);
}

typedef GridCoordinate3DTemplate<grid_iter> GridCoordinate3D;
typedef GridCoordinate3DTemplate<FPValue> GridCoordinateFP3D;

GridCoordinate3D CUDA_DEVICE CUDA_HOST convertCoord (GridCoordinateFP3D coord);
GridCoordinateFP3D CUDA_DEVICE CUDA_HOST convertCoord (GridCoordinate3D coord);

#endif /* GRID_COORDINATE_3D_H */
