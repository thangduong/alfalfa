#include <memory>
#include <queue>
#include <functional>
#include <unordered_map>
#include <cassert>

#include "exception.hh"
#include "raster_handle.hh"

using namespace std;

/* helper to dequeue front element from a queue */
template <typename T>
static T dequeue( queue<T> & q )
{
  T ret { move( q.front() ) };
  q.pop();
  return ret;
}

class RasterPool
{
private:
  queue<RasterHolder> unused_rasters_ {};

public:
  RasterHolder make_raster( const unsigned int display_width,
			    const unsigned int display_height )
  {
    RasterHolder ret;

    if ( unused_rasters_.empty() ) {
      ret.reset( new HashCachedRaster( display_width, display_height ) );
    } else {
      if ( (unused_rasters_.front()->display_width() != display_width)
	   or (unused_rasters_.front()->display_height() != display_height) ) {
	throw Unsupported( "raster size has changed" );
      }

      ret = dequeue( unused_rasters_ );
    }

    ret.get_deleter().set_raster_pool( this );
    assert( not ret->has_cache() );
    return ret;
  }

  void free_raster( HashCachedRaster * raster )
  {
    assert( raster );
    assert( not raster->has_cache() );
    unused_rasters_.emplace( raster );
  }
};

void RasterDeleter::operator()( HashCachedRaster * raster ) const
{
  if ( raster_pool_ ) {
    raster->reset_cache();
    raster_pool_->free_raster( raster );
  } else {
    delete raster;
  }
}

RasterPool * RasterDeleter::get_raster_pool( void ) const
{
  return raster_pool_;
}

void RasterDeleter::set_raster_pool( RasterPool * pool )
{
  assert( not raster_pool_ );
  raster_pool_ = pool;
}

static RasterPool & global_raster_pool( void )
{
  static RasterPool pool;
  return pool;
}

RasterHolder & MutableRasterHandle::get_holder( void )
{
  return raster_;
}

MutableRasterHandle::MutableRasterHandle( const unsigned int display_width, const unsigned int display_height )
  : MutableRasterHandle( display_width, display_height, global_raster_pool() )
{}

MutableRasterHandle::MutableRasterHandle( const unsigned int display_width, const unsigned int display_height, RasterPool & raster_pool )
  : raster_( raster_pool.make_raster( display_width, display_height ) )
{
  assert( not raster_->has_cache() );
}

RasterHandle::RasterHandle( MutableRasterHandle && mutable_raster )
  : raster_( move( mutable_raster.get_holder() ) )
{
  assert( not raster_->has_cache() );
}

size_t RasterHandle::hash( void ) const
{
  return raster_->hash();
}

bool RasterHandle::operator==( const RasterHandle & other ) const
{
  return hash() == other.hash();
}

bool RasterHandle::operator!=( const RasterHandle & other ) const
{
  return not operator==( other );
}

size_t HashCachedRaster::hash() const
{
  if ( not frozen_hash_.initialized() ) {
    frozen_hash_.initialize( Raster::raw_hash() );
  }

  return frozen_hash_.get();
}

void HashCachedRaster::reset_cache()
{
  frozen_hash_.clear();
}

bool HashCachedRaster::has_cache() const
{
  return frozen_hash_.initialized();
}
