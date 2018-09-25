#ifndef __WARP_RESOURCES_H__
#define __WARP_RESOURCES_H__

//Tile size = 16x8
constexpr unsigned int TILE_W_PWR2 = 4;
constexpr unsigned int TILE_H_PWR2 = 3;

constexpr unsigned int IMG_W = 64;
constexpr unsigned int IMG_H = 32;
constexpr unsigned int IN_W = IMG_W;

//typical output resolution
constexpr unsigned int OUT_W = IMG_W;
constexpr unsigned int OUT_H = IMG_H;

//typical mesh size
constexpr unsigned int MESH_W = 8;
constexpr unsigned int MESH_H = 4;

//clear color
constexpr unsigned int CCOL = 0x5A;

extern float   mat3x3I [];
extern float   mat3x3T0[]; //translate
extern float   mat3x3NN[]; //identity (+0.5 for NN sampling)
extern float   mat3x3Dx[]; //small shift on X

extern uint8_t oBuf [IMG_W*IMG_H*4]; //*4 to cover test's worst case output
extern uint8_t iBuf [IMG_W*IMG_H];
extern uint8_t iBufN[IMG_W*IMG_H];

extern float    meshA[(MESH_W*2)*MESH_H]; //pass through
extern float    meshB[(MESH_W*2)*MESH_H]; //flip V
extern float    meshC[(MESH_W*2)*MESH_H]; //flip H
extern float    meshD[(MESH_W*2)*MESH_H]; //flip H+V
extern float    meshF[(     4*2)*     8]; //rotate-90deg CW
extern float    meshG[(     4*2)*     8]; //rotate-90deg Counter-CW
extern float    meshH[(     4*2)*     2]; //zoom 0.5
extern float    meshK[(     4*2)*     2]; //zoom 0.5
extern uint16_t meshL[(IMG_W *2)* IMG_H]; //pre-expanded relative pass through
extern uint16_t meshM[(IMG_W *2)* IMG_H]; //pre-expanded relative integer-shift

extern uint8_t eBufB [IMG_W*IMG_H];
extern uint8_t eBufC [IMG_W*IMG_H];
extern uint8_t eBufD [IMG_W*IMG_H];
extern uint8_t eBufE [IMG_W*IMG_H];
extern uint8_t eBufE2[IMG_W*IMG_H];
extern uint8_t eBufF [32*64];
extern uint8_t eBufG [32*64];
extern uint8_t eBufH [IMG_W*IMG_H/4];
extern uint8_t eBufK [IMG_W*IMG_H/4];
extern uint8_t eBufJ [IMG_W*IMG_H*4];
extern uint8_t eBufN [IMG_W*IMG_H];

#endif