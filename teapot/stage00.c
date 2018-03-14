/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/
#include <nusys.h>
#include <assert.h>
#include <malloc.h>

#include "main.h"
#include "graphic.h"
#include "vector3.h"
#include "Texture.h"
#include "model.h"
#include "segment.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif


static float theta;  /* The rotational angle of the square */
static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */
static float triPos_z; /* The display position, Y */

static vector3 camPos;
static vector3 camForward;
static vector3 camRight;

static Light sun;

#define MSG_QUEUE_SIZE 256

static OSMesgQueue msgQueue;
static OSMesg msgBuffer[MSG_QUEUE_SIZE];
static OSPiHandle* cartHandle;

#define HEAP_SIZE 1024 * 1024
static Model teapotModel;
static u8 modelHeap[HEAP_SIZE]; //allocate 1 megabyte for the heap

static u16 perspNorm;
/* Declaration of the prototype */
int shadetri(Dynamic* dynamicp);

//update camera
void updateCamera(void){
  vector3 tempUp;
  
  camForward = sub(&camForward, &camPos);
  tempUp.x = 0; tempUp.y = 1; tempUp.z = 0;
  camForward = normalize(&camForward);
  camRight = cross(&camForward, &tempUp);
  camRight = normalize(&camRight);
}

/* The initialization of stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  triPos_z = 0.0;
  theta = 0.0;
  perspNorm = 0;
  camPos.x = -37.0f;
  camPos.y = 40.0F;
  camPos.z = 37.0F;
  camForward.x = 0; camForward.y = 0; camForward.z = 0;
  updateCamera();

  //init cartdridge
  osCreateMesgQueue(&msgQueue, msgBuffer, MSG_QUEUE_SIZE);
  osCreatePiManager(OS_MESG_PRI_NORMAL, &msgQueue, msgBuffer, MSG_QUEUE_SIZE);

  cartHandle = osCartRomInit();

  InitHeap(modelHeap, HEAP_SIZE);

  LoadModel(_teapotSegmentRomStart, &teapotModel);
  sun.l.col[0] = 255;
  sun.l.col[1] = 45;
  sun.l.col[2] = 180;
  sun.l.colc[0] = 255;
  sun.l.colc[1] = 32;
  sun.l.colc[2] = 32;
  sun.l.dir[0] = 25;
  sun.l.dir[1] = 127;
  sun.l.dir[2] = 25;
}

/* Make the display list and activate the task */
void makeDL00(void)
{
  int shadedTris = 0;
  vector3 tempPoint;
  Dynamic* dynamicp;
  char conbuf[32]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* The initialization of RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  guPerspective(&dynamicp->projection,& perspNorm, 45.0F, 1.3F, 0.1F, 512.0F, 1.0F);
  //guRotate(&dynamicp->modeling, theta, 1.0F, 0.0F, 0.0F);
  guMtxIdent(&dynamicp->modeling);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, triPos_z);

  tempPoint = add(&camPos, &camForward);
  guLookAt(&dynamicp->camera, camPos.x, camPos.y, camPos.z, tempPoint.x,tempPoint.y,tempPoint.z, 0.0F, 1.0F, 0.0F);

  /*  Draw a square */
  shadedTris = shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  //print debug text
  nuDebConTextPos(0,12,23);
  sprintf(conbuf,"magic 0x%x", teapotModel.header.magic);
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0,12,24);
  sprintf(conbuf,"vertex count %u", teapotModel.header.vertexCount);
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0,12,25);
  sprintf(conbuf,"mesh count %d", shadedTris);
  nuDebConCPuts(0, conbuf);
    
  /* Draw characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}


/* The game progressing process for stage 0 */
void updateGame00(void)
{  
  static float vel = 1.0;

  /* The game progressing process for stage 0 */
  nuContDataGetEx(contdata,0);

  /* Change the display position by stick data */
  triPos_x = contdata->stick_x / 4;
  triPos_z = contdata->stick_y / 4;

  if(contdata[0].button & A_BUTTON)
  {
      triPos_y += 0.5;
  }
  if(contdata[0].button & B_BUTTON)
  {
      triPos_y -= 0.5;
  }

  updateCamera();

  if(contdata[0].button & U_JPAD)
  {
      addEqual( &camPos, &camForward);
  }
  if(contdata[0].button & D_JPAD)
  {
      subEqual( &camPos, &camForward);
  }
  if(contdata[0].button & R_JPAD)
  {
      addEqual( &camPos, &camRight);
  }
  if(contdata[0].button & L_JPAD)
  {
      subEqual( &camPos, &camRight);
  }

  //theta += vel;
}

int max(int a, int b){
  return a > b ? a : b;
}

int min(int a, int b){
  return a < b ? a : b;
}

/* Drew a square */
int shadetri(Dynamic* dynamicp)
{
  int i;
  int k;
  const int clusterSize = 12;
  const int trisPerCluster = clusterSize / 3;
  int clustersLeft = (teapotModel.header.vertexCount / clusterSize) + 1;
  int trisLeft = (teapotModel.header.vertexCount / 3);
  int clusterTris = min(trisLeft, trisPerCluster);
  int shadedTris = 0;

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)),
    G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_LIGHTING | G_CULL_BACK | G_ZBUFFER);
  gSPLight(glistp++, &sun, 1);
  gSPNumLights(glistp++, NUMLIGHTS_1);
  //set texture
  //gSPTexture(glistp++, 0x8000, 0x8000, 0, 0, G_ON);
  //gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
  //gDPSetTextureFilter(glistp++, G_TF_BILERP);
  //gDPLoadTextureBlock(glistp++, textureData, G_IM_FMT_RGBA, G_IM_SIZ_32b,
  // 32, 32, 0, G_TX_CLAMP | G_TX_NOMIRROR, G_TX_CLAMP | G_TX_NOMIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

  k = 0;
  do
  {
    gSPVertex(glistp++,&(teapotModel.vertices[k]), clusterTris * 3, 0);
    for(i = 0; i < clusterTris; i++){
      gSP1Triangle(glistp++, (i * 3) + 0, (i * 3) + 1, (i * 3) + 2, 0);
      shadedTris++;
    }
    trisLeft -= clusterTris;
    clusterTris = min(trisLeft, trisPerCluster);
    clustersLeft--;
    k += clusterTris * 3;
  } while(clustersLeft);

  return shadedTris;
}
