#ifndef MESH_H_
#define MESH_H_

#include "mv_types.h"
#include "imageWarpDefines.h"

#define WINDOW_WIDTH             1920
#define WINDOW_HEIGHT            1080

#define MESH_WIDTH (WINDOW_WIDTH / MESH_CELL_SIZE + 1)
#define MESH_HEIGHT (WINDOW_HEIGHT / MESH_CELL_SIZE + 2)

#include "identity_mesh.h"
#include "fisheye_mesh.h"
#include "perspective_mesh.h"
#include "wave_mesh.h"


#endif /* MESH_H_ */
