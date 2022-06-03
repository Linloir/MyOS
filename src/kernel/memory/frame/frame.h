/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:11
 * LastEditTime : 2022-06-03 14:46:38
 * Description  : Frame manager, including allocate physical frames and deallocate physical frames
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "bitmap.h"

#define MEMORY_INFO_ADDR 0x0

class FrameManager {
    private:
        static BitMap framePool;
    public:
        static void initialize();
        /**
         * Allocate one frame from memory
         * 
         * @return the physic address of the allocated memory
         */
        static uint32 allocateFrame();
        /**
         * Get the amount of available frames
         * 
         * @return the amount of available frames
         */
        static int availableFrames();
        /**
         * Get the amount of occupiedFrames
         * 
         * @return the amount of occupied frames
         */
        static int occupiedFrames();
        /**
         * Get the size of frame pool
         * 
         * @return the size of frame pool
         */
        static int totalFrames();
};

#endif
