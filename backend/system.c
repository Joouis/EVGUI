#include "system.h"

struct timeval tv = {
	.tv_sec = 0,
	.tv_msec = 0,
};

struct tm t = {
	.tm_sec = 30,
	.tm_min = 15,
	.tm_hour = 0,
	.tm_mday = 20,
	.tm_mon = 3,
	.tm_year = 2016,
	.tm_wday = 0,
	.tm_yday = 79,
	.tm_isdst = 0,
};

/* TouchPanel Data Structure*/
static TP_STATE* TP_State;
fb_event *TP_Head;
fb_event *TP_End;
static size_t FirstTouch = 1;

/* GUI structure */
UG_GUI gui;

/* Hardware accelerator for UG_DrawLine (Platform: STM32F4x9) */
UG_RESULT _HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   DMA2D_InitTypeDef DMA2D_InitStruct;

   RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, ENABLE);
   RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, DISABLE);
   DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
   DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
   /* Convert UG_COLOR to RGB565 */
   DMA2D_InitStruct.DMA2D_OutputBlue = (c>>3) & 0x1F;
   DMA2D_InitStruct.DMA2D_OutputGreen = (c>>10) & 0x3F;
   DMA2D_InitStruct.DMA2D_OutputRed = (c>>19) & 0x1F;
   DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;

   /* horizontal line */
   if ( y1 == y2 )
   {
      DMA2D_InitStruct.DMA2D_OutputOffset = 0;
      DMA2D_InitStruct.DMA2D_NumberOfLine = 1;
      DMA2D_InitStruct.DMA2D_PixelPerLine = x2-x1+1;
   }
   /* vertical line */
   else if ( x1 == x2 )
   {
      DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;
      DMA2D_InitStruct.DMA2D_NumberOfLine = y2-y1+1;
      DMA2D_InitStruct.DMA2D_PixelPerLine = 1;
   }
   else
   {
      return UG_RESULT_FAIL;
   }

   if ( ltdc_work_layer == LAYER_1 )
   {
      DMA2D_InitStruct.DMA2D_OutputMemoryAdd = SDRAM_BANK_ADDR + LAYER_1_OFFSET + 2*(LCD_PIXEL_WIDTH * y1 + x1);
   }
   else
   {
      DMA2D_InitStruct.DMA2D_OutputMemoryAdd = SDRAM_BANK_ADDR + LAYER_2_OFFSET + 2*(LCD_PIXEL_WIDTH * y1 + x1);
   }
   DMA2D_Init(&DMA2D_InitStruct);
   DMA2D_StartTransfer();
   while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET){};
   return UG_RESULT_OK;
}

/* Hardware accelerator for UG_FillFrame (Platform: STM32F4x9) */
UG_RESULT _HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   DMA2D_InitTypeDef      DMA2D_InitStruct;

   DMA2D_DeInit();
   DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
   DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
   /* Convert UG_COLOR to RGB565 */
   DMA2D_InitStruct.DMA2D_OutputBlue = (c>>3) & 0x1F;
   DMA2D_InitStruct.DMA2D_OutputGreen = (c>>10) & 0x3F;
   DMA2D_InitStruct.DMA2D_OutputRed = (c>>19) & 0x1F;
   DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
   DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - (x2-x1+1));
   DMA2D_InitStruct.DMA2D_NumberOfLine = y2-y1+1;
   DMA2D_InitStruct.DMA2D_PixelPerLine = x2-x1+1;
   if ( ltdc_work_layer == LAYER_1 )
   {
      DMA2D_InitStruct.DMA2D_OutputMemoryAdd = SDRAM_BANK_ADDR + LAYER_1_OFFSET + 2*(LCD_PIXEL_WIDTH * y1 + x1);
   }
   else
   {
      DMA2D_InitStruct.DMA2D_OutputMemoryAdd = SDRAM_BANK_ADDR + LAYER_2_OFFSET + 2*(LCD_PIXEL_WIDTH * y1 + x1);
   }
   DMA2D_Init(&DMA2D_InitStruct);

   DMA2D_StartTransfer();
   while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET){}
   return UG_RESULT_OK;
}

static void _twin_fbdev_put_span (twin_coord_t left,
					twin_coord_t top,
					twin_coord_t right,
					twin_argb32_t *pixels,
					void 		*closure)
{
	twin_coord_t width = right - left;
	while(width--)
		UG_DrawPixel((left++), top, *(pixels++));
}

/* Systick interrupt */
void SysTick_Handler(void)
{
   tv.tv_msec++;
   /* TODO: mon, year, ...  */
   if ( tv.tv_msec > 1000  ) {
	   tv.tv_msec = 0;
	   tv.tv_sec++;
	   t.tm_sec++;
	   if ( t.tm_sec == 60 ) {
		   t.tm_sec = 0;
		   t.tm_min++;
		   if ( t.tm_min == 60 ) {
			   t.tm_min = 0;
			   t.tm_hour++;
		   }
	   }
   }

   // Maybe too many works in ISR.
   // Another solution is adding every TP_State into list.
   // But it will consume more and more memory as time goes by!
   /* char HavePressed = 0; */
   /* TP_State = IOE_TP_GetState(); */
   /* if ( TP_State->TouchDetected ) { */
	/*    if ( (TP_State->X > 0) && (TP_State->X < 239 ) )  */
	/*    {   */
	/* 	   if ( (TP_State->Y > 0) && (TP_State->Y < 319 ) )  */
	/* 	   {    */
	/* 		   if ( FirstTouch ) { */
	/* 			   // FirstTouch, so move the pointer. */
	/* 			   if ( !TP_End ) { */
	/* 				   TP_End = sram_malloc(sizeof(fb_event)); */
	/* 				   TP_End->event->kind = TwinEventMotion; */
	/* 				   TP_End->event->u.pointer.x = TP_End->next->event->u.pointer.screen_x = TP_State->X; */
	/* 				   TP_End->event->u.pointer.y = TP_End->next->event->u.pointer.screen_y = TP_State->Y; */
	/* 			   }else { */
	/* 				   TP_End->next = sram_malloc(sizeof(fb_event)); */
	/* 				   TP_End->next->event->kind = TwinEventMotion; */
	/* 				   TP_End->next->event->u.pointer.x = TP_End->next->event->u.pointer.screen_x = TP_State->X; */
	/* 				   TP_End->next->event->u.pointer.y = TP_End->next->event->u.pointer.screen_y = TP_State->Y; */
	/* 				   TP_End = TP_End->next; */
	/* 			   } */
	/* 			   // Then add button down node. */
	/* 			   TP_End->next = sram_malloc(sizeof(fb_event)); */
	/* 			   TP_End->next->event->kind = TwinEventButtonDown; */
	/* 			   TP_End->next->event->u.pointer.x = TP_End->next->event->u.pointer.screen_x = TP_State->X; */
	/* 			   TP_End->next->event->u.pointer.y = TP_End->next->event->u.pointer.screen_y = TP_State->Y; */
	/* 			   TP_End = TP_End->next; */
   /*  */
	/* 			   // Clear the flag. */
	/* 			   FirstTouch = 0; */
	/* 		   }else { */
	/* 			   // TODO: Concurrency issue? */
	/* 			   TP_End->next = sram_malloc(sizeof(fb_event)); */
	/* 			   TP_End->next->event->kind = TwinEventButtonDown; */
	/* 			   TP_End->next->event->u.pointer.x = TP_End->next->event->u.pointer.screen_x = TP_State->X; */
	/* 			   TP_End->next->event->u.pointer.y = TP_End->next->event->u.pointer.screen_y = TP_State->Y; */
	/* 			   TP_End = TP_End->next; */
   /*  */
	/* 			   HavePressed = 1; */
	/* 		   } */
	/* 	   } */
	/*    } */
   /* }else{ */
	/*    FirstTouch = 1; */
	/*    if ( HavePressed ) { */
	/* 	   TP_End->next = sram_malloc(sizeof(fb_event)); */
	/* 	   TP_End->next->event->kind = TwinEventButtonUp; */
	/* 	   TP_End->next->event->u.pointer.x = TP_End->next->event->u.pointer.screen_x = TP_State->X; */
	/* 	   TP_End->next->event->u.pointer.y = TP_End->next->event->u.pointer.screen_y = TP_State->Y; */
	/* 	   TP_End = TP_End->next; */
	/* 	   HavePressed = 0; */
	/*    } */
   /* } */
}

void systick_init( void )
{
   /* Init SysTick (1000Hz) */
   SystemCoreClockUpdate();
   if (SysTick_Config(SystemCoreClock / 1000))
   {
      /* Capture error */
      while (1);
   }
}

void backend_init(void)
{
	SystemInit();
	delay_init();
	sdram_init();
	ltdc_init();
	ili9341_init();

   /* Init µGUI */
   UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))pset,240,320);

   /* Init Touch */
   IOE_Config();

   /* Init SysTick (100Hz) */
   systick_init();
   
   /* Register hardware acceleration */
   UG_DriverRegister( DRIVER_DRAW_LINE, (void*)_HW_DrawLine );
   UG_DriverRegister( DRIVER_FILL_FRAME, (void*)_HW_FillFrame );
   UG_DriverEnable( DRIVER_DRAW_LINE );
   UG_DriverEnable( DRIVER_FILL_FRAME );

   /* Clear screen */
   ltdc_draw_layer(LAYER_1);
   ltdc_show_layer(LAYER_1);
   UG_FillScreen( C_BLUE );
}

static twin_bool_t twin_fbdev_init_screen(twin_fbdev_t *tf)
{
	twin_put_span_t span;

	span = _twin_fbdev_put_span;

	tf->xres = WIDTH;
	tf->yres = HEIGHT;
	tf->screen = twin_screen_create(tf->xres,
					tf->yres,
					NULL, span, tf);
	if ( tf->screen == NULL  ) {
		return 0;
	}
	return 1;
}

static twin_bool_t twin_fbdev_work(void *closure)
{
	twin_fbdev_t *tf = closure;
	
#ifndef _IMMEDIATE_REFRESH
	if ( tf->screen && twin_screen_damaged(tf->screen)) {
		twin_screen_update(tf->screen);
	}
#endif

	return TWIN_TRUE;
}

#ifdef _IMMEDIATE_REFRESH
static void twin_fbdev_damaged(void *closure)
{
    twin_fbdev_t *tf = closure;

#if 0
    DEBUG("fbdev damaged %d,%d,%d,%d, active=%d\n",
			tf->screen->damage.left, tf->screen->damage.top,
			tf->screen->damage.right, tf->screen->damage.bottom,
			tf->active);
#endif

    if (tf->active && twin_screen_damaged (tf->screen))
		twin_screen_update(tf->screen);
}
#endif /* _IMMEDIATE_REFRESH */

static twin_bool_t
twin_fbdev_read_events (void *closure)
{
	twin_fbdev_t *tf = closure;
	fb_event *tmp;
	while ( TP_Head->next ) {
		twin_screen_dispatch(tf->screen, TP_Head->next->event);
		tmp = TP_Head->next;
		TP_Head->next = TP_Head->next->next;
		sram_free(tmp);
	}
	return TWIN_TRUE;
}

twin_fbdev_t *twin_fbdev_create(void){
	backend_init();
	
	twin_fbdev_t *tf;
	tf = calloc(1, sizeof(twin_fbdev_t));
	if ( tf == NULL ) {
		return NULL;
	}
	if ( !twin_fbdev_init_screen(tf) ) {
		return NULL;
	}

	twin_set_work(twin_fbdev_work, TWIN_WORK_REDISPLAY, tf);
	
	char handle_events = 1;
	if ( handle_events ) {
		TP_Head = sram_malloc(sizeof(fb_event));
		if ( TP_Head  ) {
			TP_End = TP_Head->next;

			twin_set_file(twin_fbdev_read_events, tf);
		}
	} 

#ifdef _IMMEDIATE_REFRESH
	twin_screen_register_damaged(tf->screen, twin_fbdev_damaged, tf);
#endif

	return tf;
}

twin_bool_t twin_fbdev_activate(twin_fbdev_t *tf)
{
	/* Run work to process the VT switch */
	twin_fbdev_work(tf);

	/* If the screen is not active, then we failed
	 * the fbdev configuration
	 */
	return 1;
}






