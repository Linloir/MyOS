/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 23:09:40
 * LastEditTime : 2022-06-08 23:11:49
 * Description  : 
 */

#ifndef _BIT_MAP_H_
#define _BIT_MAP_H_

#include "systemtype.h"

/* WARNING:
 *  BITMAP IS NOT AVAILABLE BEFORE HEAP IS INITIALIZED
 *  SINCE BITMAP RELIES ON THE ALLOC FUNCTION OF HEAP
 */

class BitMap {
    private:
        uint8* map = NULL;
        int size = 0;
        int freeSize = 0;
        /**
         * Find the first available block in the map
         * 
         * @param index the first index to start the search
         * @return the index of first avaible resource
         */
        int firstAvailable(int start);
    public:
        BitMap();
        BitMap(int length);
        /**
         * Fetch the amount of resources available
         * 
         * @return the amount of resources available
         */
        int availableResources();
        /**
         * Fetch the amount of resources unavailable
         * 
         * @return the amount of resources unavailable
         */
        int unavailableResources();
        /**
         * Get the total resources of the map
         * 
         * @return the size of resources
         */
        int totalResources();
        /**
         * Set the status of specified index
         * 
         * @param index the specified resource
         * @param status the status to be set
         */
        void set(int index, bool status);
        /**
         * Allocate one resource from the bit map
         * 
         * @param size the amount of pages to be allocated
         * @return the index of the allocated resource, -1 if failed
         */
        int alloc();
        /**
         * Free the specific resource
         * 
         * @param index the index of the resource to be freed
         */
        void free(int index);
};

#endif