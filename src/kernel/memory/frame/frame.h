/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:11
 * LastEditTime : 2022-06-04 14:46:28
 * Description  : Frame manager, including allocate physical frames and deallocate physical frames
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "bitmap.h"

#define MEMORY_INFO_ADDR 0x0

class FrameManager {
    private:
        BitMap framePool;
    public:
        static FrameManager physicalFrames;
        static void initialize(int totalFrames, int mappedFrames);

        FrameManager(int size);
        /**
         * Allocate one frame from memory
         * 
         * @return the physic address of the allocated memory
         */
        uint32 allocateFrame();
        /**
         * Allocate more than one frames from memory
         * 
         * @param count the count of frames to be allocated
         * @return an array of physical addresses of the allocated frames
         */
        uint32* allocateFrames(int count);
        /**
         * Allocate sequencial frames from memory
         * 
         * @param count the count of frames to be allocated
         * @return the start physical address of the allocated sequence
         */
        uint32 allocateSequencialFrames(int count);
        /**
         * Free the specified frame
         * 
         * @param physicalAddr the physical address of the frame
         */
        void freeFrame(uint32 physicalAddr);
        /**
         * Get the amount of available frames
         * 
         * @return the amount of available frames
         */
        int availableFrames();
        /**
         * Get the amount of occupiedFrames
         * 
         * @return the amount of occupied frames
         */
        int occupiedFrames();
        /**
         * Get the size of frame pool
         * 
         * @return the size of frame pool
         */
        int totalFrames();
};

#endif
