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

static TextureHeader* textureHdr;
static u8* textureData;
static u8 TextureHeap[1024 * 8];

static u16 perspNorm;
/* Declaration of the prototype */
void shadetri(Dynamic* dynamicp);

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

  InitHeap(TextureHeap, 1024 * 8);
  //init header
  textureHdr = (TextureHeader*)malloc(8);
  //memcpy(textureHdr, _z_texSegmentRomStart, 8);
  //load data
  //textureData = (u8*)malloc(textureHdr->width * textureHdr->height * 2);
  //memcpy(textureData, _z_texSegmentRomStart + 8, textureHdr->width * textureHdr->height * 2);
}

/* Make the display list and activate the task */
void makeDL00(void)
{
  vector3 tempPoint;
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* The initialization of RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  guPerspective(&dynamicp->projection,& perspNorm, 45.0F, 1.3F, 0.1F, 512.0F, 1.0F);
  //guRotate(&dynamicp->modeling, theta, 0.0F, 1.0F, 0.0F);
  guMtxIdent(&dynamicp->modeling);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, triPos_z);

  tempPoint = add(&camPos, &camForward);
  guLookAt(&dynamicp->camera, camPos.x, camPos.y, camPos.z, tempPoint.x,tempPoint.y,tempPoint.z, 0.0F, 1.0F, 0.0F);

  /*  Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,12,23);
      sprintf(conbuf,"Pos_x=%5.1f",camForward.x);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0,12,24);
      sprintf(conbuf,"Pos_y=%5.1f",camForward.y);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0,12,25);
      sprintf(conbuf,"Pos_z=%5.1f",camForward.z);
      nuDebConCPuts(0, conbuf);

      if(1){
        nuDebConTextPos(0,12,26);
        sprintf(conbuf,"size 0x%x", _seqSegmentRomStart);
        nuDebConCPuts(0, conbuf);
      }

    }
  else
  {
    nuDebConTextPos(0,9,24);
    nuDebConCPuts(0, "Controller1 not connect");
  }
    
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

/* The vertex coordinates for a box */
static Vtx shade_vtx[] =  {
        {        -8,  0, -8, 0, 0, 0, 0, 0xff, 0, 0xff	},   //0
        {         8,  0, -8, 1, 0, 0, 0, 0, 0xff, 0xff	},   //1
        {         8,  0, 8, 1, 1, 0, 0, 0xff, 0, 0xff	},     //2
        {        -8,  0, 8, 0, 1, 0, 0, 0, 0xff, 0xff	},     //3

        {        -8,  16, -8, 0, 0, 0, 0, 0, 0xff, 0xff },   //4
        {         8,  16, -8, 1, 0, 0, 0, 0xff, 0, 0xff },   //5
        {         8,  16, 8, 1, 1, 0, 0, 0, 0xff, 0xff },    //6
        {        -8,  16, 8, 0, 1, 0, 0, 0xff, 0, 0xff },    //7
};

/* Drew a square */
void shadetri(Dynamic* dynamicp)
{
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)),
    G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  //if(*((u16*)texturePtr) == 0xF00D){
    //shade_vtx[0].;
    //shade_vtx[0].cn[1] = 0xff;
    //shade_vtx[0].cn[2] = 0xff;
  //}

  
  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK | G_ZBUFFER);
  //set texture
  //gSPTexture(glistp++, 0x8000, 0x8000, 0, 0, G_ON);
  //gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
  //gDPSetTextureFilter(glistp++, G_TF_BILERP);
  //gDPLoadTextureBlock(glistp++, texturePtr, G_IM_FMT_RGBA, G_IM_SIZ_16b,
  // 32, 32, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  //set vertex
  gSPVertex(glistp++,&(shade_vtx[0]),8, 0);
  gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);//bottom
  gSP2Triangles(glistp++,4,6,5,0,4,7,6,0);//top

  gSP2Triangles(glistp++,0,5,1,0,0,4,5,0);//front
  gSP2Triangles(glistp++,3,2,6,0,3,6,7,0);//back

  gSP2Triangles(glistp++,0,3,7,0,0,7,4,0);//right
  gSP2Triangles(glistp++,1,6,2,0,1,5,6,0);//left
}
