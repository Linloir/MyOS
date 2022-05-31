/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:11
 * LastEditTime : 2022-05-31 16:55:13
 * Description  : Frame manager, including allocate physical frames and deallocate physical frames
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "bitmap.h"

class FrameManager {
    private:
        static BitMap framePool;
    public:
        static void initialize(int memorySize);
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
